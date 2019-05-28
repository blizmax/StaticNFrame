PszService = {}

PszService.tableList = {}   --牌桌
PszService.IDList = {}      --这个是用于单独存放ID的
PszService.modifyList = {}  --这个是用于检查是否被修改的

PszService.dismissList = {}
PszService.pourCountList = {}

function PszService.Init()
	--初始化牌桌的机制，每页显示8张桌子，每次初始化8张
		--根据牌桌数量，当空余牌桌少于4张牌桌时，就补充8张，当空余牌桌数量大于12张时，释放其中的8张
	local IDList = PszModel.GetTableList()
	for k,v in ipairs(IDList) do
		local tInfo = PszModel.GetTableInfo(v)
		if tInfo ~= nil then
			PszService.tableList[tonumber(v)] = tInfo
			PszService.modifyList[tonumber(v)] = 0
			table.insert(PszService.IDList,tonumber(v))
		end
	end
end

function PszService.ServerLoop()
	
	--在一开始的时候对庄家进行处理
	for k,v in ipairs(PszService.IDList) do
		ThreadManager.PszLock(v)
		if true == PszModel.isModify(v) then
			local tInfo = PszModel.GetTableInfo(v)
			PszService.tableList[v] = tInfo
		end
		PszService.DoWork(PszService.tableList[v])
		
		if PszService.modifyList[v] ~= 0 then
			PszService.modifyList[v] = 0
			PszModel.SetTableInfo(PszService.tableList[v])
		end
		
		ThreadManager.PszUnLock(v)
	end
	
	local newList = PszModel.GetNewTableList()
	
	for k,v in ipairs(newList) do   --对于刚刚创建的房间，需要在这里加入到table的列表中
		ThreadManager.PszLock(v)
		local tInfo = PszModel.GetTableInfo(v)
		if tInfo ~= nil then
			if PszService.tableList[tInfo.tableid] == nil then
				PszService.DoWork(tInfo)
				PszService.tableList[tInfo.tableid] = tInfo
				PszService.modifyList[tInfo.tableid] = 0
				table.insert(PszService.IDList,tInfo.tableid)
				PszModel.SetTableInfo(tInfo)
				PszModel.DelModify(tInfo.tableid)
			else
				PszService.tableList[tInfo.tableid] = tInfo
			end
		else
			table.insert(PszService.dismissList,v)
		end
		ThreadManager.PszUnLock(v)
	end
	
	--检查是否需要删除的
	for i = #PszService.dismissList,1,-1 do
		PszModel.DelTableInfo(PszService.dismissList[i])
		table.remove(PszService.tableList,PszService.dismissList[i])  --同时在这里也删除
		PszService.tableList[ PszService.dismissList[i] ] = nil
		PszService.modifyList[ PszService.dismissList[i] ] = nil
		for j = 1,#PszService.IDList do
			if PszService.IDList[j] == PszService.dismissList[i] then
				table.remove(PszService.IDList,j)
				break
			end
		end
		table.remove(PszService.dismissList,i)
	end

end

function PszService.DoWork(tInfo)
	if tInfo.state == g_PszDefine.state_ready then
		PszService.PszReady(tInfo)
		PszService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_PszDefine.state_start then
		PszService.PszStart(tInfo)
		PszService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_PszDefine.state_play then
		PszService.PszPlay(tInfo)  
		PszService.modifyList[tInfo.tableid] = 1
	
	elseif tInfo.state == g_PszDefine.state_count then
		PszService.PszCount(tInfo)
		PszService.modifyList[tInfo.tableid] = 1
	
	elseif tInfo.state == g_PszDefine.state_next then
		PszService.PszNext(tInfo)
		PszService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_PszDefine.state_dissolve then
		tInfo.timemark = tInfo.timemark - 1
		if tInfo.timemark <= 0 then
			--需要删除了
			--历史记录
			PszModel.SetHistory(tInfo, 1)
		
			local is = false 
			for i = 1, #tInfo.situser do 
				if tInfo.situser[i].winnum ~= 0 then
					is = true
				end
			end
			if is == false then 
				for i = 1, #tInfo.situser do 
					tInfo.situser[i].winnum = 0
					tInfo.situser[i].maxdntype = 0
					tInfo.situser[i].maxscore = 0
				end
			end
		
			local gcCount = msg_psz_pb.gcpszcount()
			gcCount.hinfo:ParseFromString(tInfo:SerializeToString())
			gcCount.result = 0
		
			--说明牌局需要结束了
			gcCount.counttype = 1
			gcCount.timemark = TimeUtils.GetTime()
			for k,v in ipairs(tInfo.situser) do
				local pInfo = PlayerModel.GetPlayerInfo(v.userid)
				local addItem = gcCount.pslist:add()
				PlayerModel.ChangeSimpleInfo(pInfo, addItem)
				addItem.jifen = v.jifen
			
				gcCount.jifenlist:append(v.countjetton)
				gcCount.winnumlist:append(v.winnum)
				gcCount.maxtypelist:append(v.maxdntype)
			
			end
			PszModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)
			--把所有人提出房间
			local userIDList = PszService.GetUserIDList(tInfo)
			for k,v in ipairs(userIDList) do
				PszModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
			end
			
			tInfo.state = g_PszDefine.state_delete  --如果是在这里，就直接删除了
			local userIDList = PszService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3322].client, gcCount:ByteSize(), gcCount:SerializeToString())
		end	
		PszService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_PszDefine.state_delete then
		PszEvent.JulebuGameEnd(tInfo.julebuid, tInfo.tableid, tInfo.julebutype,g_PszType.type_psz)
		table.insert(PszService.dismissList, tInfo.tableid) --直接删除了
	end
	
end

--等待游戏开始
function PszService.PszReady(tInfo)
	--如果满人了就自动开始
	local start = false
	local startmun = tInfo.maxuser
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		startmun = 2
	end
	--金币场2个人就可以开始了
	if #tInfo.situser >= startmun then
	
	--[[local isok = true
	for k,v in ipairs(tInfo.situser)do
		if v.state ~= g_PszPlayStatus.status_next then
			--isok = false
		end
	end
	if isok == true then]]
		tInfo.state = g_PszDefine.state_start
		tInfo.timemark = g_PszTime.start_time + 1
		start = true
		
		local gcmsgtmp = msg_psz_pb.gcpszleave()
		while #tInfo.standuser > 0 do
			gcmsgtmp.result = 0
			gcmsgtmp.userid = tInfo.standuser[1]
			gcmsgtmp.leavemsg = "游戏已开始，请无座玩家离开房间"
			PszModel.DelUserTableID(tInfo.standuser[1])
			SendMessage(tInfo.standuser[1], PacketCode[3312].client,gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
			tInfo.standuser:remove(1)
		end
		--end
	end
	
	--如果半个小时还没开始游戏，自动解散房间
	tInfo.timemark = tInfo.timemark - 1
	
	if tInfo.timemark > 0 then
		return
	else
		if start == true then
			return
		end
	end
	
	local gcLeave = msg_psz_pb.gcpszleave()
	gcLeave.result = 0
	gcLeave.leavemsg = "游戏已超过半个小时没开始，系统默认解散房间!!!"
	
	for k,v in ipairs(tInfo.standuser) do   --这个是一个int list
		gcLeave.userid = v
		SendMessage(v, PacketCode[3312].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
	end
	
	for k,v in ipairs(tInfo.situser) do
		--这个是坐在座位上的
		gcLeave.userid = v.userid
		gcLeave.chairid = v.chairid
		SendMessage(v.userid, PacketCode[3312].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
	end
	
	tInfo.state = g_PszDefine.state_delete
	--游戏还没开始退还钻石
	if tInfo.payway == 1 then 
		local pInfo = PlayerModel.GetPlayerInfo(tInfo.ownerid)
		PszService.FeeReturns(pInfo,tInfo.paynum,tInfo.paytype)
	end
	
	PszModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)
	--把所有人提出房间
	local userIDList = PszService.GetUserIDList(tInfo)
	for k,v in ipairs(userIDList) do
		PszModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
	end
	--从在金币空闲列表删除
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		PszModel.DelJettonTableList(tInfo.tabletype,tInfo.tableid)
	end
	
end

--开始游戏，及做动画时间
function PszService.PszStart(tInfo)
	if tInfo.timemark == g_PszTime.start_time then
		tInfo.usevipnum = tInfo.usevipnum + 1
		--金币场游戏局数不增加
		if tInfo.tabletype ~= g_PszDefine.table_srf then
			tInfo.usevipnum = 1
		end
		--确定庄家，庄家先行动(第一局随机)
		tInfo.bankerID = PszService.CheckBanker(tInfo)
		
		--没把重新开始把玩家的手牌列表、手牌类型和已下注的量初始化为 0
		for i = 1,#tInfo.situser do
			while #tInfo.situser[i].handpoker > 0 do
				tInfo.situser[i].handpoker:remove(1)
			end
			tInfo.situser[i].psztype = 0
			tInfo.situser[i].jetton = 0
			--取消跟到底
			tInfo.situser[i].audocingl = g_PszDefine.autoCingl_NO
			
		end
		
		--初始化玩家的各种状态
		for i = 1, #tInfo.situser do
			tInfo.situser[i].pokerstate = 0
			tInfo.situser[i].pokerstate_discard = 1
			tInfo.situser[i].compare = 0
			tInfo.situser[i].jetton = 0
			tInfo.situser[i].endshow = 0
			tInfo.situser[i].thanuserid = 0
			--第一局初始化托管状态
			if tInfo.usevipnum == 1 then
				tInfo.situser[i].trusteeship = 0
				tInfo.situser[i].followbet = 0
				tInfo.situser[i].addbet = 0
				tInfo.situser[i].addbetpoint = 2
				tInfo.situser[i].lookcards = 3
				tInfo.situser[i].comparecards = 7
				tInfo.situser[i].discard = 0
			end
			
			while #tInfo.situser[i].jettonlist > 0 do
				tInfo.situser[i].jettonlist:remove(1)
			end
			while #tInfo.situser[i].filList > 0 do
				tInfo.situser[i].filList:remove(1)
			end
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.situser[i].jifen >= tInfo.difen then 
					local difenTemp = tInfo.difen
					tInfo.situser[i].callNum = g_PszDefine_difen[tInfo.difen]
					tInfo.situser[i].thanNum = tInfo.situser[i].callNum
					if tInfo.doublecomp == 1 then
						tInfo.situser[i].thanNum = tInfo.situser[i].thanNum * 2
					end
					for k, v in ipairs(g_PszDefine_difenlist[tInfo.jifen]) do
						tInfo.situser[i].filList:append(v)
					end
				end
			else
				local difenTemp = tInfo.difen
				tInfo.situser[i].callNum = g_PszDefine_difen[tInfo.difen]
				tInfo.situser[i].thanNum = tInfo.situser[i].callNum
				if tInfo.doublecomp == 1 then
					tInfo.situser[i].thanNum = tInfo.situser[i].thanNum * 2
				end
				for k, v in ipairs(g_PszDefine_difenlist[tInfo.jifen]) do
					tInfo.situser[i].filList:append(v)
				end
			end
		end
		
		local guodi = 0
		for i = 1, #tInfo.situser do
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.situser[i].jifen >= tInfo.difen then 
					tInfo.situser[i].jetton = g_PszDefine_difen[tInfo.difen]
					tInfo.situser[i].jettonlist:append(g_PszDefine_difen[tInfo.difen])
					guodi = guodi + g_PszDefine_difen[tInfo.difen]
				end
			else
				tInfo.situser[i].jetton = g_PszDefine_difen[tInfo.difen]
				tInfo.situser[i].jettonlist:append(g_PszDefine_difen[tInfo.difen])
				guodi = guodi + g_PszDefine_difen[tInfo.difen]
			end
		end
		--洗锅（把锅底初始化为低分 * 坐下玩家数量）
		tInfo.guodi = guodi
		tInfo.deluser = 0
		--把单注设置为低分
		tInfo.oncepour = g_PszDefine_difen[tInfo.difen]
		--把庄家设为当前行动者
		tInfo.actionuser = tInfo.bankerID
		--把当前下注回合设置为1
		tInfo.presentbout = 1
		tInfo.addtime = 0
		--初始化行动列表
		while #tInfo.actionlist > 0 do
			tInfo.actionlist:remove(1)
		end
		--按座位号把玩家的下表加到行动列表
		local tmp = {} 
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then 
			for k,v in ipairs(tInfo.situser)do 
				if v.jifen >= tInfo.difen then 
					tmp[tInfo.situser[k].chairid] = v.userid
				end
			end
		else
			for k,v in ipairs(tInfo.situser)do 
				tmp[tInfo.situser[k].chairid] = v.userid
			end
		end

		local flag = 1
		for z = 1, 2 do
			for k,v in pairs(tmp) do
				if v == tInfo.bankerID then
					--这里为了取反
					flag = 0 - flag
					if flag == 1 then
						break
					end
				end
				if flag ~= 1 then
					tInfo.actionlist:append(v)
				end	
			end
		end
		--发牌
		PszService.fapai(tInfo)
		
		--把行动玩家状态设置为：行动，其他玩家状态设置为：等待
		for j = 1, #tInfo.situser do
			if tInfo.situser[j].userid == tInfo.actionuser then
				tInfo.situser[j].state = g_PszPlayStatus.status_play
			else
				if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then 
					if tInfo.situser[j].jifen >= tInfo.difen then
						tInfo.situser[j].state = g_PszPlayStatus.status_wait
					else
						tInfo.situser[j].state = g_PszPlayStatus.status_watch
					end
				else
					tInfo.situser[j].state = g_PszPlayStatus.status_wait
				end
			end
		end
		
		local userIDList = PszService.GetUserIDList(tInfo)
		local gcStart = msg_psz_pb.gcpszstart()
		gcStart.hinfo:ParseFromString(tInfo:SerializeToString())
		for i = 1, #gcStart.hinfo.situser do 
			gcStart.hinfo.situser[i].psztype = 0
			while #gcStart.hinfo.situser[i].handpoker > 0 do
				gcStart.hinfo.situser[i].handpoker:remove(1) 
			end
		end
		gcStart.result  = 0
		SendMessage(userIDList, PacketCode[3316].client, gcStart:ByteSize(), gcStart:SerializeToString())		
		--扣个费
		if tInfo.usevipnum == 1 then
			--如果是比赛场这里就不扣钱了
			if false == PszEvent.IsBiSai(tInfo.julebuid) then
				PszService.PszPay(tInfo, 0)
			else 
				for k, v in ipairs(tInfo.situser)do
					local pInfo = PlayerModel.GetPlayerInfo(v.userid)
					PlayerModel.SendPlayerInfo(pInfo,{"money"})
				end
			end
			PszEvent.JulebuGameStart(tInfo)
		end
		
	elseif tInfo.timemark <= 0 then
		tInfo.state = g_PszDefine.state_play   --进人等待玩家操作状态
		tInfo.timemark = tInfo.qipai + 1  --好了，开始倒计时了
	end
	tInfo.timemark = tInfo.timemark - 1
	
end

--等待玩家操作
function PszService.PszPlay(tInfo)
	if tInfo.timemark == tInfo.qipai then
		--给所有玩家发送当前行动者
		local gcPlay = msg_psz_pb.gcpszinformaction()
		gcPlay.result = 0
		gcPlay.userid = tInfo.actionuser
		gcPlay.oncepour = tInfo.oncepour
		gcPlay.presentbout = tInfo.presentbout
		local tmpflag = true 
		for k, v in ipairs(tInfo.situser) do
			if v.userid == tInfo.actionuser then
				gcPlay.chairid = v.chairid
				gcPlay.abandon = v.pokerstate_discard
				if  v.audocingl == g_PszDefine.autoCingl_OK then
					gcPlay.audocingl = 1
				end
				gcPlay.callNum = v.callNum 	--跟注的值
				gcPlay.thanNum = v.thanNum 	--比牌的值
				
				if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then 
					if (v.jifen - v.jetton) < v.callNum then
						if tInfo.addtime == 0 then
							tInfo.timemark = tInfo.timemark + 120
							tInfo.addtime = 1
						elseif tInfo.addtime == 1 then
							tInfo.addtime = 0
							tmpflag = false
						end
					end	
				end
				
				for i = 1, #v.filList do
					gcPlay.filList:append(v.filList[i])
				end	
				gcPlay.actiontime = tInfo.timemark 
				break
			end
		end
		local userIDList = PszService.GetUserIDList(tInfo)
		if tmpflag == true then
			SendMessage(userIDList, PacketCode[3318].client, gcPlay:ByteSize(), gcPlay:SerializeToString())
		end
	end
	if tInfo.timemark <= tInfo.qipai - 2 then
		PszService.PszTrusteeship(tInfo)
	end
	--如果是自动跟注
	for k,v in ipairs(tInfo.situser) do
		if v.userid == tInfo.actionuser then
			if v.audocingl == g_PszDefine.autoCingl_OK then
				tInfo.timemark = -1
			end
		end
	end
	--提前通知客户端隐藏看牌加注跟注等按钮
	if tInfo.timemark == 0 then
		local isTrue = false
		for k,v in ipairs(tInfo.situser) do
			if tInfo.actionuser == v.userid then
				if v.state == g_PszPlayStatus.status_play then
					if v.audocingl == g_PszDefine.autoCingl_OK then
						isTrue = true
					end
					if isTrue == true then
						local gcmsgtmp = msg_psz_pb.gcpszconcealbutton()
						gcmsgtmp.result = 0
						gcmsgtmp.flag = 0
						SendMessage(v.userid, PacketCode[3336].client, gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
					end
				end
				--如果是自动跟注直接将时间设置为-1
				if v.audocingl == 1 then
					tInfo.timemark = -1
				end
				break
			end
		end
	end
	
	if tInfo.timemark == - 1 then
		local gcmsg = msg_psz_pb.gcpszaction()
		for i = 1, #tInfo.situser do
			if tInfo.actionuser == tInfo.situser[i].userid then
				if tInfo.situser[i].state == g_PszPlayStatus.status_play and #tInfo.actionlist > 1 then
					if tInfo.situser[i].audocingl == g_PszDefine.autoCingl_OK  then
						--自动跟注
						local jetton = PszService.followjetton(tInfo)
						gcmsg.actiontype = g_PszDefine.followjetton
						gcmsg.addjetton = jetton
						gcmsg.oncepour = jetton
					else
						--玩家超时未操作视为：弃牌
						PszService.abandonpoker(tInfo)
						gcmsg.actiontype = g_PszDefine.abandonpoker
						tInfo.deluser = tInfo.actionuser
					end
					
					tInfo.situser[i].state = g_PszPlayStatus.status_wait
					for k, v in ipairs(tInfo.situser) do
						if v.userid == tInfo.actionuser then
							gcmsg.callNum = v.callNum 	--跟注的值
							gcmsg.thanNum = v.thanNum 	--比牌的值
							for i = 1, #v.filList do
								gcmsg.filList:append(v.filList[i])
							end
						end
					end
					gcmsg.guodi = tInfo.guodi
					gcmsg.result = 0
					gcmsg.userid = tInfo.actionuser
					local userIDList = PszService.GetUserIDList(tInfo)
					SendMessage(userIDList, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
				end
				if tInfo.situser[i].state == g_PszPlayStatus.status_wait and tInfo.timemark <= 0 then
					--轮到下一给玩家行动
					PszService.NextAction(tInfo)
					tInfo.timemark = tInfo.timemark + 1
					break
				end
			end
		end
	end
	
	--如果行动列表只剩一个玩家或下注回合等于最大下注回合，说明要进入结算状态
	if #tInfo.actionlist <= 1 or tInfo.presentbout > tInfo.boutcapp then
		--准备进入结算状态把所有玩家的按钮全部隐藏起来
		local gcmsgb = msg_psz_pb.gcpszconcealbutton()
		gcmsgb.result = 0
		gcmsgb.flag = 1
		local userIDList = PszService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3336].client, gcmsgb:ByteSize(), gcmsgb:SerializeToString())
		
		tInfo.state = g_PszDefine.state_count
		tInfo.timemark = g_PszTime.count_time + 1
		if tInfo.flag == 1 then
			tInfo.timemark = tInfo.timemark + 2
			tInfo.flag = 0
		end
		
		if tInfo.presentbout > tInfo.boutcapp then
			if #tInfo.actionlist > 1 then
				local gcA = msg_psz_pb.gcpszaction()
				local max = PszService.returnSub(tInfo, tInfo.actionlist[1])
				for i = 2, #tInfo.actionlist do
					local ord = PszService.returnSub(tInfo, tInfo.actionlist[i])
					if false == PszService.whoWin(max,ord, tInfo) then
						if tInfo.presentbout > tInfo.boutcapp then
							tmp = gcA.bipailist:add()
							tmp.winnerid = tInfo.situser[ord].userid
							tmp.loserid = tInfo.situser[max].userid
							tInfo.situser[max].compare = 1
						end
						max = ord
					else
						if tInfo.presentbout > tInfo.boutcapp then
							tmp = gcA.bipailist:add()
							tmp.winnerid = tInfo.situser[max].userid
							tmp.loserid = tInfo.situser[ord].userid
							tInfo.situser[ord].compare = 1
						end
					end
				end			
				--用来记录下局谁先开始
				tInfo.flag = 1
				tInfo.actionuser = tInfo.situser[max].userid
				tInfo.timemark = ArtoonTime(tInfo,#tInfo.actionlist) + 1
				gcA.result = 0
				gcA.guodi = tInfo.guodi
				gcA.actiontype = g_PszDefine.comparepoker
				gcA.winnerid = tInfo.actionuser
				local userIDList = PszService.GetUserIDList(tInfo)
				SendMessage(userIDList, PacketCode[3320].client, gcA:ByteSize(), gcA:SerializeToString())
			end
			tInfo.presentbout = tInfo.boutcapp
		end
	end
	tInfo.timemark = tInfo.timemark - 1
end

--托管处理
function PszService.PszTrusteeship(tInfo)
	local cgmsg = msg_psz_pb.cgpszaction() 
	cgmsg.tableid = tInfo.tableid
	local flag = 0
	for k,v in ipairs(tInfo.situser) do
		if v.trusteeship == g_PszDefine.trusteeship_yes then
			cgmsg.userid = v.userid
			if v.userid == tInfo.actionuser then
				--如果已经看牌则检测是否弃牌
				if v.pokerstate_discard ~= 2 and v.pokerstate == g_PszPokerStatus.poker_looked and v.discard > 0 and v.discard >= v.psztype then
					--弃牌
					cgmsg.actiontype = g_PszDefine.abandonpoker
					cgmsg.addjetton = 0
					cgmsg.rivalID = 0
					PszService.Action(cgmsg,tInfo)
				elseif v.comparecards > 0 and v.comparecards < tInfo.presentbout then
					--比牌
					cgmsg.actiontype = g_PszDefine.comparepoker
					for z,j in ipairs(tInfo.situser)do 
						if j.pokerstate_discard ~= 2 and z ~= k then
							cgmsg.rivalID = j.userid
							cgmsg.addjetton = j.userid
						end
					end
					local ret = PszService.Action(cgmsg,tInfo)
					if ret and ret == 1 then
						flag = flag + 1
					end
				else
					if v.pokerstate == g_PszPokerStatus.poker_looked then
						if tInfo.presentbout >= tInfo.boutcapp then
							--比牌
							cgmsg.actiontype = g_PszDefine.comparepoker
							for z,j in ipairs(tInfo.situser)do 
								if j.pokerstate_discard ~= 2 and z ~= k then
									cgmsg.rivalID = j.userid
									cgmsg.addjetton = j.userid
								end
							end
							local ret = PszService.Action(cgmsg,tInfo)
							if ret and ret == 1 then
								flag = flag + 1
							end
						end
						if flag == 0 then
							--已经看牌了
							local tmp = false --查看是否加过注 加过一次不再加注
							for z, j in ipairs(v.filList)do
								--print("***filList***"..j..v.callNum)
								if j == v.addbetpoint*2 and v.addbetpoint*2 > v.callNum then
									tmp = true
								end
							end
							if  tmp and v.addbetpoint > 0 and v.addbet <= v.psztype then	--加注
								cgmsg.actiontype = g_PszDefine.upjetton
								cgmsg.addjetton = v.addbetpoint*2
								cgmsg.rivalID = 0
								local ret = PszService.Action(cgmsg,tInfo)
								if ret and ret == 1 then
									flag = flag + 1
								end
							else
								if v.followbet == 0 or (v.followbet > 0 and v.followbet <= v.psztype) then
									--跟注
									cgmsg.actiontype = g_PszDefine.followjetton
									cgmsg.addjetton = 0
									cgmsg.rivalID = 0
									local ret = PszService.Action(cgmsg,tInfo)
									if ret and ret == 1 then
										flag = flag + 1
									end
								else
									--弃牌
									cgmsg.actiontype = g_PszDefine.abandonpoker
									cgmsg.addjetton = 0
									cgmsg.rivalID = 0
									PszService.Action(cgmsg,tInfo)
								end
							end
						end
						--未比牌则先看是否需要看牌
					elseif v.pokerstate == g_PszPokerStatus.poker_lookno then
						if tInfo.presentbout >= tInfo.boutcapp then
							--比牌
							cgmsg.actiontype = g_PszDefine.comparepoker
							for z,j in ipairs(tInfo.situser)do 
								if j.pokerstate_discard ~= 2 and z ~= k then
									cgmsg.rivalID = j.userid
									cgmsg.addjetton = j.userid
								end
							end
							local ret = PszService.Action(cgmsg,tInfo)
							if ret and ret == 1 then
								flag = flag + 1
							end
						end
						if flag == 0 then
							--如果闷牌的话,默认自动跟到底 不弃牌 不加注 不比牌
							if (v.lookcards >= tInfo.presentbout or tInfo.menpai > tInfo.presentbout or v.lookcards == 0) then
								local tmp = false --查看是否加过注 加过一次不再加注
								for z, j in ipairs(v.filList)do 
									if j == v.addbetpoint and v.addbetpoint > v.callNum then
										tmp = true
									end
								end
								if tmp and v.addbetpoint > 0 and v.addbet <= v.psztype then	--加注
									cgmsg.actiontype = g_PszDefine.upjetton
									cgmsg.addjetton = v.addbetpoint
									cgmsg.rivalID = 0
									local ret = PszService.Action(cgmsg,tInfo)
									if ret and ret == 1 then
										flag = flag + 1
									end
								end
								if flag == 0 then
									--跟到底
									cgmsg.actiontype = g_PszDefine.followjetton
									cgmsg.addjetton = 0
									cgmsg.rivalID = 0
									local ret = PszService.Action(cgmsg,tInfo)
									if ret and ret == 1 then
										flag = flag + 1
									end
								end
							else
								if v.pokerstate == g_PszPokerStatus.poker_lookno then
									if (v.lookcards < tInfo.presentbout and tInfo.menpai <= tInfo.presentbout and v.lookcards ~= 0) or v.lookcards == 1 then
										--看牌
										cgmsg.actiontype = g_PszDefine.lookpoker
										cgmsg.addjetton = 0
										cgmsg.rivalID = 0
										PszService.Action(cgmsg,tInfo)
									end
								end
								--看完牌后可以比牌了
								if tInfo.situser[k].pokerstate == g_PszPokerStatus.poker_looked then
									if v.discard > 0 and v.discard >= v.psztype then
										--弃牌
										cgmsg.actiontype = g_PszDefine.abandonpoker
										cgmsg.addjetton = 0
										cgmsg.rivalID = 0
										PszService.Action(cgmsg,tInfo)
									else
										local tmp = false --查看是否加过注 加过一次不再加注
										for z, j in ipairs(v.filList)do 
											if j == v.addbetpoint*2 and v.addbetpoint*2 > v.callNum then
												tmp = true
											end
										end
										if tmp and v.addbetpoint > 0 and v.addbet <= v.psztype then	--加注
											cgmsg.actiontype = g_PszDefine.upjetton
											cgmsg.addjetton = v.addbetpoint*2
											cgmsg.rivalID = 0
											local ret = PszService.Action(cgmsg,tInfo)
											if ret and ret == 1 then
												flag = flag + 1
											end
										else
											if v.followbet == 0 or (v.followbet > 0 and v.followbet <= v.psztype) then
												--跟注
												cgmsg.actiontype = g_PszDefine.followjetton
												cgmsg.addjetton = 0
												cgmsg.rivalID = 0
												local ret = PszService.Action(cgmsg,tInfo)
												if ret and ret == 1 then
													flag = flag + 1
												end
											else
												--弃牌
												cgmsg.actiontype = g_PszDefine.abandonpoker
												cgmsg.addjetton = 0
												cgmsg.rivalID = 0
												PszService.Action(cgmsg,tInfo)
											end
										end
									end
								end
							end
						end
					end
				end
				if flag > 0 then
					tInfo.timemark = tInfo.timemark + 1
				end
				--print("****tInfo.timemark*"..tInfo.timemark)
				break
			else
				--未比牌则先看是否需要看牌
				--[[if v.pokerstate == g_PszPokerStatus.poker_lookno then
					if (v.lookcards < tInfo.presentbout and tInfo.menpai <= tInfo.presentbout and v.lookcards ~= 0) or v.lookcards == 1 then
						--看牌
						cgmsg.actiontype = g_PszDefine.lookpoker
						cgmsg.addjetton = 0
						cgmsg.rivalID = 0
						Action(cgmsg)
					end
				end]]
			end
		end
	end

end

--托管的时候使用的接口
function PszService.Action(cgmsg,tInfo)
	local gcmsg = msg_psz_pb.gcpszaction()
	
	--除了看牌操作和弃牌，其他操作必须在轮到轮到玩家才能
	if cgmsg.userid ~= tInfo.actionuser  then
		if cgmsg.actiontype ~= g_PszDefine.abandonpoker and cgmsg.actiontype ~= g_PszDefine.lookpoker then
			return 
		end
	end
	
	if cgmsg.actiontype ~= g_PszDefine.lookpoker then
		for k, v in ipairs(tInfo.situser) do
			if cgmsg.userid == v.userid then
				if v.pokerstate_discard == 2 or v.compare == 1 or #tInfo.actionlist < 2  then
					return 
				end
			end
		end
	end
	
	if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
		local indexPos = 0
		for k, v in ipairs(tInfo.situser)do
			if v.userid == cgmsg.userid then
				indexPos = k
			end
		end
		if cgmsg.actiontype == g_PszDefine.lookpoker then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].callNum * 2 then
				return 
			end
		end
		if cgmsg.actiontype == g_PszDefine.upjetton then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < cgmsg.addjetton then
				return 
			end
		end
		
		if cgmsg.actiontype == g_PszDefine.comparepoker then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].thanNum then
				return 
			end
		end
		
		if cgmsg.actiontype == g_PszDefine.followjetton then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].callNum then
				return 
			end
		end
		
	end
	
	local deluaction = 0
	--弃牌操作
	if cgmsg.actiontype == g_PszDefine.abandonpoker then
		for k,v in ipairs(tInfo.situser)do
			if v.userid == cgmsg.userid and v.pokerstate_discard == 2 then
				return
			end
		end
		local userid = PszService.abandonpoker(tInfo,cgmsg.userid)
		gcmsg.actiontype = g_PszDefine.abandonpoker
		deluaction = userid
	--看牌操作
	elseif cgmsg.actiontype == g_PszDefine.lookpoker then
		--print("tInfo.presentbout"..tInfo.presentbout.."********".."tInfo.menpai"..tInfo.menpai)
		if tInfo.presentbout <  tInfo.menpai then
			return 
		end
		for k,v in ipairs(tInfo.situser)do
			if v.userid == cgmsg.userid and v.pokerstate == 1 then
				return
			end
		end
		local pokerlist, psztype = PszService.lookpoker(tInfo,cgmsg.userid)
		gcmsg.ptype = psztype
		gcmsg.actiontype = g_PszDefine.lookpoker
		for k,v in ipairs(pokerlist) do
			gcmsg.pokerlist:append(v)
		end

	--比牌操作
	elseif cgmsg.actiontype == g_PszDefine.comparepoker then
		if tInfo.presentbout <  tInfo.mincomp then
			return 
		end
		local isok = false
		for k,v in ipairs(tInfo.actionlist)do 
			if v == cgmsg.rivalID then
				isok = true
			end
		end
		
		if isok == false then
			--gcmsg.status = 1 proto里面没有定义
			gcmsg.result = 0
			gcmsg.actiontype = cgmsg.actiontype
			for k, v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					gcmsg.callNum = v.callNum 	--跟注的值
					gcmsg.thanNum = v.thanNum 	--比牌的值
					for i = 1, #v.filList do
						gcmsg.filList:append(v.filList[i])
					end
				end
			end
			return 
		end
		
		local winner, loser,jetton = PszService.comparepoker(tInfo, cgmsg.rivalID)
		gcmsg.actiontype = g_PszDefine.comparepoker
		gcmsg.winnerid = winner
		tmp = gcmsg.bipailist:add()
		tmp.winnerid = winner
		tmp.loserid = loser
		deluaction = loser
		gcmsg.addjetton = jetton
		
	--加注操作
	elseif cgmsg.actiontype == g_PszDefine.upjetton then
		local isEx = false 
		for k,v in ipairs(tInfo.situser)do
			if cgmsg.userid == v.userid then
				for i = 1, #g_PszDefine_difenlist[tInfo.jifen] do
					local tmp = g_PszDefine_difenlist[tInfo.jifen][i]
					if v.pokerstate == 1 then
						tmp = tmp * 2
					end
					if tmp == cgmsg.addjetton then
						if cgmsg.addjetton > v.callNum then
							isEx = true
						end
					end
				end
				break
			end
		end	 
		if isEx == false then
			--查看这个错误的是否是存在玩家的列表中
			local tmp = false
			for k,v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					for z, j in ipairs(v.filList)do 
						if z == cgmsg.addjetton then
							tmp = true
						end
					end
				end
			end
			--如果是true就说明服务器出错了， 重新刷新一下玩家的下注列表
			if tmp == true then
				LogFile("error","tableID:"..tInfo.tableid.."usevipnum:"..tInfo.usevipnum.."addjetton: "..cgmsg.addjetton )
				for k,v in ipairs(tInfo.situser) do
					if v.userid == cgmsg.userid then
						while #tInfo.situser[k].filList > 0 do
							tInfo.situser[k].filList:remove(1)
						end
						for z, y in ipairs(g_PszDefine_difenlist[tInfo.difen]) do
							local tmp = y 
							if v.pokerstate == g_PszPokerStatus.poker_looked then
								tmp = tmp * 2
							end
							tInfo.situser[k].filList:append(tmp)
						end
					end
				end
			end
			--gcmsg.status = 1   landy：190109 先注释掉。搞不懂这里干嘛的。
			gcmsg.result = 0
			gcmsg.actiontype = cgmsg.actiontype
			for k, v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					gcmsg.callNum = v.callNum 	--跟注的值
					gcmsg.thanNum = v.thanNum 	--比牌的值
					for i = 1, #v.filList do
						gcmsg.filList:append(v.filList[i])
					end
				end
			end
			return 
		end
	
		local jetton = PszService.upjetton(tInfo, cgmsg.addjetton)
		gcmsg.actiontype = g_PszDefine.upjetton
		gcmsg.addjetton = jetton
		
	--跟注操作
	elseif cgmsg.actiontype == g_PszDefine.followjetton then
		local jetton = PszService.followjetton(tInfo)
		gcmsg.actiontype = g_PszDefine.followjetton
		gcmsg.addjetton = jetton
	end
		
	--金币场：扣除下注的金币
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		if false == PszService.FeeDeductions(pInfo, gcmsg.addjetton, g_PszDefine.jetton_pay, "PszPourJetton") then
			return 
		end
	end
	
	--只有在弃牌和比牌才会产生淘汰的玩家
	if cgmsg.actiontype == g_PszDefine.abandonpoker or cgmsg.actiontype == g_PszDefine.comparepoker then
		tInfo.deluser =  deluaction
	end
	--如果弃牌的不是当前行动，就不把时间置为0	
	if cgmsg.actiontype == g_PszDefine.abandonpoker and tInfo.actionuser == cgmsg.userid then
		tInfo.timemark = -1
	elseif  cgmsg.actiontype == g_PszDefine.abandonpoker then
		PszService.NextAction(tInfo, 1)
	end
	
	--看牌不结束该玩家的行动时间 
	if cgmsg.actiontype ~= g_PszDefine.lookpoker and cgmsg.actiontype ~= g_PszDefine.abandonpoker then
		tInfo.timemark = -1
	end
	--
	if cgmsg.actiontype ~= g_PszDefine.lookpoker then
		for i = 1, #tInfo.situser do 
			if cgmsg.userid ==  tInfo.situser[i].userid then
				tInfo.situser[i].state = g_PszPlayStatus.status_wait
				gcmsg.jetton = tInfo.situser[i].jetton
				gcmsg.oncepour = tInfo.oncepour
				if tInfo.situser[i].pokerstate == 1 then
					gcmsg.oncepour = gcmsg.oncepour * 2
				end
			end
		end
	end
	
	for k, v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			gcmsg.callNum = v.callNum 	--跟注的值
			gcmsg.thanNum = v.thanNum 	--比牌的值
			for i = 1, #v.filList do
				gcmsg.filList:append(v.filList[i])
			end
		end
	end
	gcmsg.guodi = tInfo.guodi
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	local userIDList = PszService.GetUserIDList(tInfo)
	if cgmsg.actiontype == g_PszDefine.lookpoker then
		userIDList = PszService.GetUserIDList(tInfo, cgmsg.userid)
		SendMessage(cgmsg.userid, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		gcmsg.ptype = 0
		while #gcmsg.pokerlist > 0 do
			gcmsg.pokerlist:remove(1)
		end
	end
	
	SendMessage(userIDList, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	if  cgmsg.actiontype == g_PszDefine.abandonpoker then
		if #tInfo.actionlist == 1 or  (#tInfo.actionlist == 2 and tInfo.deluser == tInfo.actionuser)then
			local gcmsgb = msg_psz_pb.gcpszconcealbutton()
			gcmsgb.result = 0
			gcmsgb.flag = 1
			local userIDList = PszService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3336].client, gcmsgb:ByteSize(), gcmsgb:SerializeToString())
		end
	end
	
	if tInfo.timemark == - 1 then
		if cgmsg.actiontype ~= g_PszDefine.lookpoker then
			PszService.NextAction(tInfo)
			if #tInfo.actionlist == 1 or tInfo.presentbout > tInfo.boutcapp then
				tInfo.state = g_PszDefine.state_play
				tInfo.timemark = -2
				if cgmsg.actiontype == g_PszDefine.comparepoker then
					tInfo.flag = 1
				end
			else
				if cgmsg.actiontype == g_PszDefine.comparepoker then
					tInfo.timemark = tInfo.timemark +  4
				end
			end
		end
	end
	PszModel.SetTableInfo(tInfo, 1)   
	SendMessage(cgmsg.userid, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	return 1
end

--结算
function PszService.PszCount(tInfo)

	if tInfo.timemark == g_PszTime.count_time then
		-- 把所以玩家在的状态都置为等行动
		for i = 1, #tInfo.situser do
			tInfo.situser[i].state = g_PszPlayStatus.status_wait
		end

		--单局结算传0
		PszService.Count(tInfo, 0)
		
		--最后多人比牌把赢家的牌亮出来
		if tInfo.flag == 1 then
			tInfo.flag = 0
			local gcmsgs = msg_psz_2_pb.gcpszendshow()
			gcmsgs.result = 0
			gcmsgs.userid = tInfo.actionuser
			for k,v in ipairs(tInfo.situser)do	
				if v.userid == tInfo.actionuser then
					gcmsgs.chairid = v.chairid
				end
			end
			local userIDList = PszService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3340].client, gcmsgs:ByteSize(), gcmsgs:SerializeToString())
		else
			for k,v in ipairs(tInfo.situser)do	
				if v.thanuserid ~= 0 then
					local gcEndShow = msg_psz_2_pb.gcpszendshow()
					gcEndShow.result = 0
					gcEndShow.userid = tInfo.situser[v.thanuserid].userid
					gcEndShow.chairid =  tInfo.situser[v.thanuserid].chairid
					SendMessage(v.userid, PacketCode[3340].client, gcEndShow:ByteSize(), gcEndShow:SerializeToString())
				end
			end
		end
	elseif tInfo.timemark == g_PszTime.count_time - 2 then
		--全部牌局的结算
		local is = false
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
			local tmpNum = 0
			for k,v in ipairs(tInfo.situser)do
				if v.jifen >= tInfo.difen then
					tmpNum = tmpNum + 1
				end
			end
			if tmpNum <= 1 then
				is = true
				PszModel.SetHistory(tInfo, 1)
			end
		end
		
		if tInfo.usevipnum >= tInfo.maxplaynum or is == true then
			--总结算传1
			PszService.Count(tInfo, 1)
		end
		
	elseif tInfo.timemark <= 0 and tInfo.usevipnum < tInfo.maxplaynum  then
		
		local gcmsgb = msg_psz_pb.gcpszconcealbutton()
		gcmsgb.result = 0
		gcmsgb.flag = 2
		local userIDList = PszService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3336].client, gcmsgb:ByteSize(), gcmsgb:SerializeToString())
		
		--金币场不进入等待下局游戏状态
		if tInfo.tabletype == g_PszDefine.table_srf then
			--PszService.CheckJiFen(tInfo, LeaveUserID)
			tInfo.state = g_PszDefine.state_next
			tInfo.timemark = g_PszTime.next_time + 1
		elseif tInfo.tabletype ~= g_PszDefine.table_srf then
			tInfo.state = g_PszDefine.state_start
			tInfo.timemark = g_PszTime.start_time + 1
			--金币场不足2个，房间进入准备状态
			if #tInfo.standuser + #tInfo.situser < 2	then
				tInfo.state = g_PszDefine.state_ready
				tInfo.timemark = g_PszTime.ready_time + 1
			end
		end
	end
	
	tInfo.timemark = tInfo.timemark - 1
end

function PszService.Count(tInfo, flag)
	local gcmsg = msg_psz_pb.gcpszcount()
	
	if flag == 0 then
		for i = 1, #tInfo.situser do
			--把锅底加给到胜利的玩家
			if tInfo.situser[i].userid == tInfo.actionuser then
				tInfo.situser[i].jifen = tInfo.situser[i].jifen + tInfo.guodi - tInfo.situser[i].jetton
				tInfo.situser[i].countjetton = tInfo.situser[i].countjetton + tInfo.guodi - tInfo.situser[i].jetton
				--统计玩家的胜利次数
				tInfo.situser[i].winnum = tInfo.situser[i].winnum + 1
			else
				tInfo.situser[i].jifen = tInfo.situser[i].jifen - tInfo.situser[i].jetton
				tInfo.situser[i].countjetton = tInfo.situser[i].countjetton - tInfo.situser[i].jetton
			end
			--统计玩家的最大牌型
			if tInfo.situser[i].psztype > tInfo.situser[i].maxdntype then
				tInfo.situser[i].maxdntype = tInfo.situser[i].psztype
			end
		end
		--这里是给同花或者同花顺收喜分
		if tInfo.tonghua ~= 0  then
			for i = 1, #tInfo.situser do
				if tInfo.situser[i].psztype == g_PszCard_type.tonghua or tInfo.situser[i].psztype == g_PszCard_type.tonghuashun then
					for k = 1, #tInfo.situser do
						if i ~= k then
							tInfo.situser[i].jifen = tInfo.situser[i].jifen + tInfo.tonghua
							tInfo.situser[k].jifen = tInfo.situser[k].jifen - tInfo.tonghua
							
							
							tInfo.situser[i].countjetton = tInfo.situser[i].countjetton + tInfo.tonghua
							tInfo.situser[k].countjetton = tInfo.situser[k].countjetton - tInfo.tonghua							
						end
					end
				end
			end
		end
		--这里是给豹子收喜分
		if tInfo.baozi ~= 0  then
			for i = 1, #tInfo.situser do
				if tInfo.situser[i].psztype == g_PszCard_type.baozi  then
					for k = 1, #tInfo.situser do
						if i ~= k then
							tInfo.situser[i].jifen = tInfo.situser[i].jifen + tInfo.baozi
							tInfo.situser[k].jifen = tInfo.situser[k].jifen - tInfo.baozi
							
							
							tInfo.situser[i].countjetton = tInfo.situser[i].countjetton + tInfo.baozi
							tInfo.situser[k].countjetton = tInfo.situser[k].countjetton - tInfo.baozi							
						end
					end
				end
			end
		end
		gcmsg.winerID = tInfo.actionuser
		--历史记录(金币场不记录)
		if tInfo.tabletype == g_PszDefine.table_srf then
			PszModel.SetHistory(tInfo, 0)
			--金币场(赢家获得金币)
		elseif tInfo.tabletype ~= g_PszDefine.table_srf then
			local pInfo = PlayerModel.GetPlayerInfo(gcmsg.winerID)
			PszService.FeeReturns(pInfo,tInfo.guodi,g_PszDefine.jetton_pay,"Pszcount")
		end
		
		
		
	elseif flag == 1 then
		for k,v in ipairs(tInfo.situser) do
			gcmsg.jifenlist:append(v.countjetton)
			gcmsg.winnumlist:append(v.winnum)
			gcmsg.maxtypelist:append(v.maxdntype)
		end
		PszModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)
		--把所有人提出房间
		local userIDList = PszService.GetUserIDList(tInfo)
		for k,v in ipairs(userIDList) do
			PszModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
		end
		tInfo.state = g_PszDefine.state_delete --如果是在这里，就直接删除了
	end
	--把玩家的pInfo加到协议里面去
	for k,v in ipairs(tInfo.situser) do
		local pInfo = PlayerModel.GetPlayerInfo(v.userid)
		local addItem = gcmsg.pslist:add()
		PlayerModel.ChangeSimpleInfo(pInfo, addItem)
		if flag == 1 then
			PlayerModel.TenGames(pInfo)
		end
		addItem.jifen = v.jifen
	end
	gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
	gcmsg.counttype = flag
	gcmsg.timemark = TimeUtils.GetTime()
	gcmsg.result = 0
	local userIDList = PszService.GetUserIDList(tInfo)
	SendMessage(userIDList, PacketCode[3322].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		
end

--等待下一局开始
function PszService.PszNext(tInfo)
	local gnext = msg_psz_pb.gcpsznext()
	local gcmsg = msg_psz_pb.gcpsznextout()
	
	if tInfo.timemark == g_PszTime.next_time then
		gcmsg.tableid = tInfo.tableid
		gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
		gcmsg.hinfo.guodi = 0
		gcmsg.hinfo.oncepour = 0
		gcmsg.hinfo.presentbout = 0
		for i = 1, #gcmsg.hinfo.situser do 
			gcmsg.hinfo.situser[i].psztype = 0
			while #gcmsg.hinfo.situser[i].handpoker > 0 do
				gcmsg.hinfo.situser[i].handpoker:remove(1) 
			end
		end
		local userList = PszService.GetUserIDList(tInfo)
		gcmsg.result = 0
		SendMessage(userList, PacketCode[3328].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	elseif tInfo.timemark <= 0 and #tInfo.situser > 1 then
		for i = 1, #tInfo.situser do
			if tInfo.situser[i].state ~= g_PszPlayStatus.status_next then
				gnext.chairid = tInfo.situser[i].chairid
				gnext.userid = tInfo.situser[i].userid
				local userIDList = PszService.GetUserIDList(tInfo)
				gnext.result = 0
				SendMessage(userIDList, PacketCode[3326].client, gnext:ByteSize(), gnext:SerializeToString())
			end
		end
		tInfo.state =  g_PszDefine.state_start
		tInfo.timemark = g_PszTime.start_time + 1
	end
	
	
	if #tInfo.situser == 0 then
		tInfo.state = g_PszDefine.state_dissolve
		tInfo.timemark = 0
	end
	--把后面加入的玩家坐下
	PszService.dnsit(tInfo)
	tInfo.timemark = tInfo.timemark - 1
end

--查看AA够不够扣费
function PszService.Pszcheck(tInfo, pInfo)
	
	if tInfo.payway == g_PszDefine.payment_owner then
		return
	end
	
	local needMoney = 0
	local needGold = 0
	if tInfo.paytype ==	g_douNiuDefine.diamond_pay then
		needMoney = tInfo.paynum
		needGold = tInfo.paynum * 10
	elseif tInfo.paytype ==	g_douNiuDefine.jade_pay then
		needMoney = tInfo.paynum / 10
		needGold = tInfo.paynum
	end
	if pInfo.money < needMoney and pInfo.gold < needGold then
		return false
	end
	return true
	
end

--AA付费
--payt : 0 游戏刚开始的付费
--payt : 1 游戏开始后的付费
function PszService.PszPay(tInfo, payt)
	--房主付费返回，房主不再这里付费
	if tInfo.payway == g_PszDefine.payment_owner then
		return
	end
	--AA付费
	--房主优先扣他所选择的类型
	--其他玩家优先扣除钻石
	--游戏刚开始扣费
	if payt == 0 then
		local needgold = tInfo.paynum
		if tInfo.paytype == g_PszDefine.diamond_pay then
			needgold = needgold * 10
		end
		
		for i = 1, #tInfo.situser do
			local pInfo = PlayerModel.GetPlayerInfo(tInfo.situser[i].userid)
			local needNum = needgold
			local needType = g_PszDefine.jade_pay
			if tInfo.situser[i].userid == tInfo.ownerid then
				needNum = tInfo.paynum
				needType = tInfo.paytype
			end
			PszService.FeeDeductions(pInfo,needNum,needType)
		end
		--游戏开始后扣费
	elseif payt == 1 then
		
		local needmoney = math.ceil((tInfo.maxplaynum - tInfo.usevipnum) / 10 ) --需要扣钻的数量
		local needgold = needmoney * 10
		
		for i = 1, #tInfo.standuser do
			local pInfo = PlayerModel.GetPlayerInfo(tInfo.standuser[i])
			local needNum = needgold
			local needType = g_PszDefine.jade_pay
			if tInfo.standuser[i] == tInfo.ownerid then
				needNum = needgold
				if tInfo.paytype == g_PszDefine.diamond_pay then
					needNum = needmoney
				end
				needType = tInfo.paytype
			end
			PszService.FeeDeductions(pInfo,needNum,needType)
		end
	end
	
	return true
end

--扣费
function PszService.FeeDeductions(pInfo,needMoney,paytype,cause)
	
	if cause == nil then
		cause = "create room"
	end
	
	--扣钻石的情况
	if paytype == g_PszDefine.diamond_pay then
		--钻石足够
		if pInfo.money >= needMoney then
			PlayerModel.DecMoney(pInfo, needMoney, "psz",cause)  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendPlayerInfo(pInfo,{"money"})
			--钻石不够
		else
			--扣玉
			needMoney = needMoney * 10
			if pInfo.gold >= needMoney then
				PlayerModel.DecGold(pInfo, needMoney, "psz",cause)  --创建房间
				PlayerModel.SetPlayerInfo(pInfo)
				PlayerModel.SendPlayerInfo(pInfo,{"gold"})
			else
				return false
			end
		end
		--扣玉的情况
	elseif paytype == g_PszDefine.jade_pay then
		--玉足够的情况
		if pInfo.gold >= needMoney then
			PlayerModel.DecGold(pInfo, needMoney, "psz",cause)  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendPlayerInfo(pInfo,{"gold"})
			--玉不够
		else
			--钻石够
			needMoney = needMoney / 10
			if pInfo.money >= needMoney then
				PlayerModel.DecMoney(pInfo, needMoney, "psz",cause)  --创建房间
				PlayerModel.SetPlayerInfo(pInfo)
				PlayerModel.SendPlayerInfo(pInfo,{"money"})
				--钻石不够
			else
				return false
			end
		end
	elseif paytype == g_PszDefine.jetton_pay  then
		--金币够扣
		if pInfo.jetton >= needMoney then
			PlayerModel.DecJetton(pInfo,needMoney,"psz",cause)
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendJetton(pInfo)
			--金币不够扣
		else
			return false
		end
		
	end
	return true
end

--退费
function PszService.FeeReturns(pInfo,needMoney,paytype,cause)
	
	if pInfo == 0 or pInfo == nil then
		return
	end
	
	if cause == nil then
		cause = "dissolve room"
	end
	
	if paytype == g_PszDefine.diamond_pay then
		PlayerModel.AddMoney(pInfo, needMoney, "psz", cause)
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo,{"money"})
	elseif paytype == g_PszDefine.jade_pay then
		PlayerModel.AddGold(pInfo, needMoney, "psz", cause)
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo,{"gold"})
	elseif paytype == g_PszDefine.jetton_pay  then
		PlayerModel.AddJetton(pInfo, needMoney,"psz", cause)
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendJetton(pInfo)
	end
	
end

--获得房间内的玩家列表(坐下+站立)
function PszService.GetUserIDList(tInfo,exceptID)
	local userIDList = {}
	
	for k,v in ipairs(tInfo.situser) do
		if v.userid ~= 0 and v.userid ~= exceptID then
			table.insert(userIDList, v.userid)
		end
	end
	for k,v in ipairs(tInfo.standuser) do
		if v ~= exceptID then
			table.insert(userIDList, v)
		end
	end
	return userIDList
end

--确定庄家(第一句随机，后面都庄家都是前面赢得玩家)
function PszService.CheckBanker(tInfo)
	
	local bankerID = 0
	
	if tInfo.usevipnum == 1 then
		local ran = math.myrandom(1,#tInfo.situser)
		bankerID = tInfo.situser[ran].userid
	else
		bankerID = tInfo.actionuser
	end
	local isEx = false
	for k, v in ipairs(tInfo.situser)do
		if bankerID == v.userid then
			isEx = true
		end
	end
	if isEx == false then
		local ran = math.myrandom(1,#tInfo.situser)
		bankerID = tInfo.situser[ran].userid
	end
	
	return bankerID
	
end

--发牌(给每位玩家发3张牌)
function PszService.fapai(tInfo, n)
	
	if n == 0 or n == nil then
		n = 3
	end
		
	local cardInit = {}
	for i = 1,4 do
		for j = 1,13 do
			table.insert(cardInit, i*100 + j)
		end
	end
	
	
	local cardList = {}
	
	--在这里先做一次随机
	for i = 1,100 do
		local index = math.myrandom(1,#cardInit)
		table.insert(cardList, cardInit[index])
		table.remove(cardInit, index)
		if #cardInit == 0 then
			break
		end
	end


	local cardArrList = {}
	for i = 1,#tInfo.situser do
		cardArrList[i] = {}
		for j = 1,n do
			local pokerID = PszService.GetRankCard(cardList)
			table.insert(cardArrList[i], pokerID)			
		end
	end
	--取到了对应的牌型。
	

	PszService.CheckSpecialList(tInfo, cardArrList)   

	--luaDump(cardArrList)
	
	--给每个坐下的玩家发牌
	for i = 1,#tInfo.situser do
		local inList = {}
		
		if nil == PszService.GetDebugCardList(tInfo.tableid,tInfo.situser[i].userid, inList, cardList) then
			
			--for j = 1, n do
				--local pokerID = PszService.GetRankCard(cardList)
				--table.insert(inList, pokerID)
			--	table.insert( inList,cardArrList[i][j] )
			--end
			inList = cardArrList[i]
		end
		--计算牌型
		if n == #inList then	
			local psztype = PszService.GetCardType(inList)
			if tInfo.chibaozi == 0 and psztype == g_PszCard_type.teshu then
				psztype = g_PszCard_type.danzhang
			end
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.situser[i].jifen >= tInfo.difen then
					tInfo.situser[i].psztype = psztype
					for j = 1, #inList do
						tInfo.situser[i].handpoker:append(inList[j])
					end
				end
			else
				tInfo.situser[i].psztype = psztype
				for j = 1, #inList do
					tInfo.situser[i].handpoker:append(inList[j])
				end	
			end
		end
	end
end

--获得单张牌
function PszService.GetRankCard(cList,cardNum)
	if cardNum == nil then
		if #cList == 0 then
			return 
		end
		local index = math.myrandom(1,#cList)
		local temp = cList[index]
		table.remove(cList, index)
		return temp
	else
		local temp = 0
		for i = 1,#cList do
			if cList[i] == cardNum then
				temp = cList[i]
				table.remove(cList, i)
				break
			end
		end
		return temp
	end
end

--获得牌型
function PszService.GetCardType(inList)
	
 	local dianList = {}			--点数列表
	local dianListLen = 0		--点数列表长度
	local huaList = {}			--花色列表
	local huaListLen = 0		--花色列表长度
	local pszType = g_PszCard_type.danzhang
	table.sort(inList,function(a,b)    --按点数升序排序后的列表
		local dianshu_a = math.mod(a, 100)
		if dianshu_a == 1 then
			dianshu_a = 14
		end
		local dianshu_b = math.mod(b, 100)
		if dianshu_b == 1 then
			dianshu_b = 14
		end
		if dianshu_a == dianshu_b then 
			local hua_a = math.floor(a/100)
			local hua_b = math.floor(b/100)
			return hua_a < hua_b
		else
			return dianshu_a < dianshu_b
		end
	end)
	
	for i = 1,#inList do
		local dianshu = math.mod(inList[i], 100)
		if dianList[dianshu] == nil then
			dianList[dianshu] = 1
			dianListLen = dianListLen + 1
		else
			dianList[dianshu] = dianList[dianshu] + 1
		end
		
		local huase = math.floor(inList[i]/100)
		if huaList[huase] == nil then
			huaList[huase] = 1
			huaListLen = huaListLen + 1
		else
			huaList[huase] = huaList[huase] + 1
		end
	end
	
	--同花
	if huaListLen == 1 then
		--如果长度只有一个个
		for k,v in pairs(huaList) do
			if v == 3 then
				pszType = g_PszCard_type.tonghua
			end
		end
	end
	--这是豹子
	if dianListLen == 1 then
		--如果长度只有一个个
		for k,v in pairs(dianList) do
			if v == 3 then
				pszType = g_PszCard_type.baozi
			end
		end
	--这个是对子
	elseif dianListLen == 2 then
		--如果长度只有两个
		for k,v in pairs(dianList) do
			if v == 1 or v == 2 then
				pszType = g_PszCard_type.duizi
			end
		end
	
	--看下是不是顺子
	elseif dianListLen == 3 then
		local Ace = math.mod(inList[3], 100)
		if Ace == 1 then
			Ace = 14
		end
		if  (Ace - math.mod(inList[1], 100)) == 2 then
			local tmp = math.mod(inList[3], 100) + math.mod(inList[2], 100) + math.mod(inList[1], 100)
			if pszType ~=  g_PszCard_type.tonghua then
				--顺子
				pszType = g_PszCard_type.shunzi
			else
				--同花顺
				pszType = g_PszCard_type.tonghuashun		
			end
		end
		if Ace == 14 then
			Ace = 1
			if (math.mod(inList[2], 100) - Ace) == 2 then
				local tmp = inList[1]
				inList[1] = inList[3]
				inList[3] = inList[2]
				inList[2] = tmp
				
				if pszType ~=  g_PszCard_type.tonghua then
					pszType = g_PszCard_type.shunzi
				else
					--同花顺
					pszType = g_PszCard_type.tonghuashun
				end
			end
		end
		
		--不同花色的235是特殊牌
		if huaListLen == 3 then
			if math.mod(inList[1], 100) == 2 then
				if math.mod(inList[2], 100) == 3 then
					if math.mod(inList[3], 100) == 5 then
						pszType = g_PszCard_type.teshu
					end
				end
			end
		end
	end
	
	return pszType
	
end

--更新当前行动玩家
function PszService.NextAction(tInfo, n)
	
	if #tInfo.actionlist < 2 then
		return
	end
	
	if n == 0 or n == nil then
		for k,v in ipairs(tInfo.actionlist)do
			if v == tInfo.actionuser then
				--一轮轮完了
				if k >= #tInfo.actionlist then
					if tInfo.actionlist[1] == tInfo.deluser then
						tInfo.actionuser = tInfo.actionlist[2]
					else
						tInfo.actionuser = tInfo.actionlist[1]
					end
					tInfo.presentbout = tInfo.presentbout + 1
					--没轮完一轮
				else
					if k >= #tInfo.actionlist - 1 then
						if tInfo.actionlist[k + 1] == tInfo.deluser then
							tInfo.actionuser = tInfo.actionlist[1]
						else
							tInfo.actionuser = tInfo.actionlist[k + 1]
						end
						
					else
						if tInfo.actionlist[k + 1] == tInfo.deluser then
							tInfo.actionuser = tInfo.actionlist[k + 2]
						else
							tInfo.actionuser = tInfo.actionlist[k + 1]
						end
					end
				end
				break
			end
		end
		--把时间重置
		tInfo.timemark = tInfo.qipai
	end
	if tInfo.deluser ~= 0 then
		for k,v in ipairs(tInfo.actionlist)do
			if v == tInfo.deluser then
				tInfo.actionlist:remove(k)
				break
			end
		end
	end
	
	--把等待删除玩家只为0
	tInfo.deluser = 0
	
	--把上个行动玩家的状态：等待g_PszPlayStatus.status_wait 当前行动玩家设置：行动g_PszPlayStatus.status_play
	for i = 1, #tInfo.situser do
		if tInfo.actionuser == tInfo.situser[i].userid then
			tInfo.situser[i].state = g_PszPlayStatus.status_play
		else
			tInfo.situser[i].state = g_PszPlayStatus.status_wait
		end
	end
end

--弃牌操作
function PszService.abandonpoker(tInfo, userid)
	if userid == 0 or userid == nil then
		userid = tInfo.actionuser
	end
	--把该玩家的状态设置为：qipai， 牌的状态设置为：弃牌
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == userid then
			tInfo.situser[i].pokerstate_discard = g_PszPokerStatus.poker_abandon
			tInfo.situser[i].state = g_PszDefine.abandonpoker
		end
	end
	
	return userid
end

--看牌操作
function PszService.lookpoker(tInfo, userid)
	if userid == 0 or userid == nil then
		return {},0
	end
	local pokerlist = {}
	local psztype = 0
	--牌的状态设置为：看牌
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == userid then
			if tInfo.situser[i].pokerstate ~= g_PszPokerStatus.poker_looked then
				tInfo.situser[i].callNum = tInfo.situser[i].callNum * 2 
				tInfo.situser[i].thanNum = tInfo.situser[i].thanNum * 2 
				for j = 1, #tInfo.situser[i].filList do
					tInfo.situser[i].filList[j] = tInfo.situser[i].filList[j] * 2
				end
			end
			tInfo.situser[i].pokerstate = g_PszPokerStatus.poker_looked
			psztype = tInfo.situser[i].psztype
			for i,j in ipairs(tInfo.situser[i].handpoker) do
				table.insert(pokerlist, j)
			end
			
		end
	end
	
	return pokerlist, psztype
end

--比牌操作
function PszService.comparepoker(tInfo, rivaler, userid)
	if userid == 0 or userid == nil then
		userid = tInfo.actionuser
	end
	--比牌也需要下注
	local sponsor 
	local receiver
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == userid then
			tInfo.situser[i].jetton = tInfo.situser[i].jetton + tInfo.situser[i].thanNum
			tInfo.situser[i].jettonlist:append(tInfo.situser[i].thanNum)
			tInfo.situser[i].state = g_PszDefine.comparepoker
			sponsor = i
		elseif  tInfo.situser[i].userid == rivaler then
			receiver = i
		end
	end
	--比牌
	local  winner = sponsor
	local loser = receiver
	if false == PszService.whoWin(sponsor,receiver, tInfo, 0) then
		winner = receiver
		loser = sponsor
	end
	
	--比牌输玩家比牌状态置为 1
	tInfo.situser[loser].compare = 1
	tInfo.situser[loser].thanuserid = winner
	
	--刷新锅底
	tInfo.guodi = tInfo.guodi + tInfo.situser[sponsor].thanNum
	return tInfo.situser[winner].userid, tInfo.situser[loser].userid, tInfo.situser[sponsor].thanNum
	
end

--加注操作
function PszService.upjetton(tInfo, addjetton, userid)
	if userid == 0 or userid == nil then
		userid = tInfo.actionuser
	end

	local tmpjetton = addjetton
	local tmpPos = 0
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == userid then
			tmpPos = i
			--刷新单注
			tInfo.oncepour = addjetton
			if tInfo.situser[i].pokerstate == g_PszPokerStatus.poker_looked then
				tInfo.oncepour  = addjetton / 2
			end
			tInfo.situser[i].jetton = tInfo.situser[i].jetton + addjetton
			tInfo.situser[i].jettonlist:append(addjetton)
			tInfo.situser[i].state = g_PszDefine.upjetton
			
			
			tInfo.situser[i].callNum = addjetton
			tInfo.situser[i].thanNum = tInfo.doublecomp == 1 and (tmpjetton * 2) or tmpjetton
			break
		end
	end
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid ~= userid then
			local tmp = tmpjetton
			if tInfo.situser[tmpPos].pokerstate == g_PszPokerStatus.poker_looked then
				if tInfo.situser[i].pokerstate == g_PszPokerStatus.poker_lookno then
					tmp = tmpjetton / 2 
				end
			else
				if tInfo.situser[i].pokerstate == g_PszPokerStatus.poker_looked then
					tmp = tmpjetton * 2 
				end
			end
			tInfo.situser[i].callNum = tmp
			tInfo.situser[i].thanNum = tInfo.doublecomp == 1 and (tmp * 2) or tmp
		end
	end
	--刷新锅底
	tInfo.guodi = tInfo.guodi + addjetton
	return addjetton
end

--跟注操作
function PszService.followjetton(tInfo, userid)
	if userid == 0 or userid == nil then
		userid = tInfo.actionuser
	end
	local tmp = 0
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == userid then
			tmp = i
			tInfo.situser[i].jetton = tInfo.situser[i].jetton + tInfo.situser[i].callNum
			tInfo.situser[i].jettonlist:append(tInfo.situser[i].callNum)
			tInfo.situser[i].state = g_PszDefine.followjetton
		end
	end
	--刷新锅底
	tInfo.guodi = tInfo.guodi + tInfo.situser[tmp].callNum
	return tInfo.situser[tmp].callNum
end

--判断比牌结果(第一个参数大返回：true)
function PszService.whoWin(sponsor, receiver, tInfo, flag)
	if tInfo.situser[sponsor].psztype > tInfo.situser[receiver].psztype then
		if tInfo.chibaozi == 1 and (tInfo.situser[sponsor].psztype == g_PszCard_type.baozi) and (tInfo.situser[receiver].psztype == g_PszCard_type.teshu) then	
            return false
		elseif tInfo.shunzi == 1 and (tInfo.situser[sponsor].psztype == g_PszCard_type.tonghua) and (tInfo.situser[receiver].psztype == g_PszCard_type.shunzi) then
			return false
		else 
			return true
		end
	elseif tInfo.situser[sponsor].psztype < tInfo.situser[receiver].psztype then
		if tInfo.chibao == 1 and (tInfo.situser[sponsor].psztype == g_PszCard_type.teshu) and (tInfo.situser[receiver].psztype == g_PszCard_type.baozi) then
			return true 
		elseif tInfo.shunzi == 1 and (tInfo.situser[sponsor].psztype == g_PszCard_type.shunzi) and (tInfo.situser[receiver].psztype == g_PszCard_type.tonghua) then
			return true
		else 
			return false
		end
	else
		local sponsor_handpoker = {}
		local receiver_handpoker = {}
		for k, v in ipairs(tInfo.situser[sponsor].handpoker)do 
			if k == #tInfo.situser[sponsor].handpoker then
				if math.mod(v, 100) == 1 then
					v = v + 13
				end
			end
			table.insert(sponsor_handpoker, v)
		end
		for k, v in ipairs(tInfo.situser[receiver].handpoker) do
			if k == #tInfo.situser[sponsor].handpoker then
				if math.mod(v, 100) == 1 then
					v = v + 13
				end
			end
			table.insert(receiver_handpoker, v)
		end
		--如果是按花色比牌  --如果主动比牌并且选择先比为输 则flag为0 否则flag为nil 或者1
		if flag and flag == 0 and tInfo.tongpai == 1 then
			flag = 1
		end
		--单张比单张的大小，先比最大的牌张，如最大牌张相同则比第二大的张，以此类推。
		--若三张牌大小相同，再比较最大单张牌的花色
		if tInfo.situser[sponsor].psztype == g_PszCard_type.danzhang or tInfo.situser[sponsor].psztype == g_PszCard_type.tonghua then
			local num = #sponsor_handpoker
			for i = num , 1, -1 do
				local dian_sponsor = math.mod(sponsor_handpoker[i], 100)
				local dian_receiver = math.mod(receiver_handpoker[i], 100)
				if dian_sponsor > dian_receiver then
					return true
				elseif dian_sponsor < dian_receiver then
					return false
				end	
				
				if i == 1 then
					--flag 为1表示玩家主动比牌
					if flag and flag == 0 then
						return false
					end
					
					local hua_sponsor = math.floor(sponsor_handpoker[num]/100)
					local hua_receiver = math.floor(receiver_handpoker[num]/100)
					if hua_sponsor > hua_receiver then
						return true
					else 
						return false
					end
				end
			end 		
		--对子比对子的大小，对子大小一样则比较剩余单张的大小，单张大小相同，再比单张花色；
		elseif tInfo.situser[sponsor].psztype == g_PszCard_type.duizi then
			local Pair_sponsor, Single_sponsor = PszService.FindPair(sponsor_handpoker)
			local Pair_receiver, Single_receiver =  PszService.FindPair(receiver_handpoker)
			local dian_sponsor = math.mod(Pair_sponsor, 100)
			local dian_receiver = math.mod(Pair_receiver, 100)
			if dian_sponsor > dian_receiver then
				return true
			elseif dian_sponsor < dian_receiver then
				return false
			else
				if true == PszService.Compare(Single_sponsor, Single_receiver,flag) then
					return true
				else
					return false
				end
			end	
		--顺子、顺金和豹子，先比最大单张大小，若单张大小相同，再比花色
		elseif tInfo.situser[sponsor].psztype == g_PszCard_type.shunzi then
			local num1 = #sponsor_handpoker
			local num2 = #receiver_handpoker
			if tInfo.shunzi == 1 then
				if math.mod(sponsor_handpoker[num1], 100) == 13 and math.mod(receiver_handpoker[num2], 100) == 3 then
					return false
				elseif math.mod(sponsor_handpoker[num1], 100) == 3 and math.mod(receiver_handpoker[num2], 100) == 13 then
					return true
				end
			end
			if true == PszService.Compare(sponsor_handpoker[num1], receiver_handpoker[num2],flag) then
				return true
			else
				return false
			end
		else
			local num1 = #sponsor_handpoker
			local num2 = #receiver_handpoker
			if true == PszService.Compare(sponsor_handpoker[num1], receiver_handpoker[num2],flag) then
				return true
			else
				return false
			end
		end
	end
	return false
end

--找到对子
function PszService.FindPair(handpokerList)
	
	if 0 > #handpokerList then
		return 
	end

	local Pair = handpokerList[1]
	local Single = 0
	local PairList = {}
	for i = 2, #handpokerList do
		if math.mod(handpokerList[i], 100) == math.mod(handpokerList[1], 100) then	--点数
			Pair = handpokerList[i] 
			table.insert(PairList, handpokerList[1])
			table.insert(PairList, handpokerList[i])
			if i == 2 then
				Single =  handpokerList[i]
			else
				Single =  handpokerList[3]
			end
			
			break
		end
	end
	if Pair == handpokerList[1] then
		table.insert(PairList, handpokerList[2])
		table.insert(PairList,handpokerList[3])
		Single = handpokerList[1]
	end
	
	if math.floor(PairList[1]/100) > math.floor(PairList[1]/100) then
			max = PairList[1] 
		else 
			max = PairList[2] 
		end
	return max, Single
end

--比较两张牌的大小
function PszService.Compare(a,b,flag)
	local dian_a = math.mod(a, 100)		--点数
		if dian_a == 1 then
			dian_a = 14
		end
	local dian_b = math.mod(b,100)
	if dian_b == 1 then
		dian_b = 14
	end
	if dian_a > dian_b then
		return true
	elseif dian_a < dian_b then
		return false
	else
		--如果主动比牌并且选择先比为输 则flag为0 否则flag为nil 或者1
		if flag and flag == 0 then
			return false
		end
		local hua_a = math.floor(a/100)
		local hua_b = math.floor(b/100)
		if hua_a > hua_b then
			return true
		else
			return false
		end
	end
end

--通过玩家ID获得玩家信息下标
function PszService.returnSub(tInfo, userid)
	local pos
	for k,v in ipairs(tInfo.situser) do
		if v.userid == userid then
			pos = k
		end
	end
	return pos
end

--游戏开始后，把进入玩家坐下
function PszService.dnsit(tInfo)
	
	if #tInfo.standuser == 0 then
		return
	end
	--扣个费
	PszService.PszPay(tInfo,1)
	while #tInfo.standuser > 0 do
		local pInfo = PlayerModel.GetPlayerInfo(tInfo.standuser[1])
		PszSitdown.UserSitdown(tInfo, pInfo)
	end
	
end

--跟注到最后比牌动画效果的时间
function ArtoonTime(tInfo, n)
	
	if n == 2 then
		return tInfo.timemark + 3
	elseif n == 3 then
		return tInfo.timemark + 6
	elseif n == 4 then
		return tInfo.timemark + 9
	elseif n == 5 then
		return tInfo.timemark + 12
	elseif n == 6 then
		return tInfo.timemark + 16
	elseif n == 7 then
		return tInfo.timemark + 19
	elseif n == 8 then
		return tInfo.timemark + 23
	else 
		return tInfo.timemark + 3
	end
end


function PszService.GetDebugCardList(tableID, userID, getList, pubList)

	if g_isDebug ~= 1 then
		return nil
	end
	
	local cardList = PszModel.GetDebugNextCard(tableID, userID)
	
	if cardList == nil or #cardList == 0 then
		return nil
	end
	cardList = luajson.decode(cardList)
	for k1,v1 in ipairs(cardList) do
		if #getList >= 3 then
			break
		end
		local tempNum = tonumber(v1)
		
		for k2,v2 in ipairs(pubList) do
			if tempNum == v2 then
				table.remove(pubList, tempNum)
				table.insert(getList, tempNum)
				break
			end
		end
		
	end
	
	for i = (#getList + 1),3 do
		table.insert(getList,PszService.GetRankCard(pubList))
	end
	return getList
end

function PszService.CheckJiFen(tInfo, LeaveUserID)
	
	if tInfo.julebuid == 0 or tInfo.julebutype < 2 then
		return	
	end
	
	--检查每个玩家的积分是否足够
	for i = #tInfo.situser,1,-1 do
		if false == PszEvent.CheckJiFen(tInfo.julebuid, tInfo.situser[i].userid, tInfo.situser[i].jifen) then
			--需要把人清出房间
			local gcLeave = msg_psz_pb.gcpszleave()
			gcLeave.userid = tInfo.situser[i].userid
			gcLeave.chairid = tInfo.situser[i].chairid
			
			gcLeave.leavemsg = "你积分不足，已被移出房间！"
			SendMessage(tInfo.situser[i].userid, PacketCode[3311].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
			local gcmsgup = msg_psz_pb.gcpszstandup()
			gcmsgup.chairid = tInfo.situser[i].chairid
			gcmsgup.result = 0
			local userIDList =  DouNiuService.GetUserIDList(tInfo, tInfo.situser[i].userid) 
			SendMessage(userIDList, PacketCode[3334].client, gcmsgup:ByteSize(), gcmsgup:SerializeToString())
			--下面开始统计结果。
			PszService.PszddJiFen_History(tInfo, tInfo.situser[i].userid)
			PszModel.DelUserTableID(tInfo.situser[i].userid)
			--在这里需要把积分同步到俱乐部
			tInfo.usernum = tInfo.usernum - 1	
			tInfo.situser:remove(i)
			--更新玩家数量		
			PszEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser,tInfo.julebutype,tInfo.situser[i].userid)
			if i == LeaveChair then
				--离开的玩家，就一个就够了。
				break
			end
		end
	end
end

--统计积分结果
function PszService.PszddJiFen_History(tInfo, userID)
	--下面开始统计结果。
	local gcHistory = nil 
	if gcHistory  == nil then
		gcHistory = st_psz_pb.pszhistory()
		PszModel.GetHistroy(tInfo.tableid, gcHistory)				
		for i=1, #tInfo.situser do
			if userID == tInfo.situser[i].userid then
				local tempInfo = PlayerModel.GetPlayerInfo(tInfo.situser[i].userid)
				local isEX = false 
				local pos = 0
				for k = 1, #gcHistory.useridlist do
					if userID == gcHistory.useridlist[k] then
						pos = k
						isEX = true
					end
				end
				if isEX == false then
					gcHistory.useridlist:append(tInfo.situser[i].userid)
					gcHistory.nicklist:append(tempInfo.nickname)
					gcHistory.winlist:append(tInfo.situser[i].jifen)
				else
					gcHistory.winlist[pos] = gcHistory.winlist[pos] + tInfo.situser[i].jifen
				end
				local redisKey = g_pszRedis.psz_tbl_hty..tInfo.tableid
				redisItem:set(redisKey, gcHistory:SerializeToString(), g_pszRedis.redis_index)
				
			end
		end
	end
end


function PszService.CheckSpecialList(tInfo, cardArrList)
	
	local winUserID = SpecialModel.PszGetWinID(tInfo.tableid)
	if winUserID == 0 then
		return
	end

	local userIndex = 0
	for k,v in ipairs(tInfo.situser) do
		if winUserID == v.userid then
			userIndex = k   --说明这个才是需要转换位置的地方
		end
	end
	if userIndex == 0 then
		return
	end
	
	local cardArrTemp = table.clone(cardArrList)
	local cardTypeList = {}
	local secIndex = 0
	local thrIndex = 0
	local maxType = -111
	local maxIndex = 0
	for k,v in ipairs(cardArrTemp) do
		cardTypeList[k] = PszService.GetCardType(v)
		
		if cardTypeList[k] > maxType then
			--保存倒数第一和第二的牌型
			thrIndex = secIndex
			secIndex = maxIndex

			maxIndex = k
		end
	end
	
	
	
	if secIndex ~= 0  and cardTypeList[maxIndex] == cardTypeList[secIndex] then
		--如果他们两个相等，那么就需要比较了。
		
		if thrIndex ~= 0 and cardTypeList[thrIndex] == cardTypeList[secIndex] then
			--如果他们两个也相等,就说明有三个；排列第一的了
			
			if false == PszService.SpecialCompare(cardArrTemp[secIndex], cardArrTemp[thrIndex], maxType ) then
				secIndex = thrIndex
			end
			
		end
		
		if false == PszService.SpecialCompare(cardArrTemp[maxIndex], cardArrTemp[secIndex], maxType ) then
			maxIndex = secIndex
		end
	end
	
	--渠道了最大的牌型类别，那么开始比较牌型
	

	
	if maxIndex == userIndex then
		return
	end
	
	local temp = table.clone(cardArrList[userIndex])
	cardArrList[userIndex] = cardArrList[maxIndex]
	cardArrList[maxIndex] = temp
	
end

--改函数是在已知牌型的情况下，比较两个牌型相等的大小
function PszService.SpecialCompare(cardArr_1, cardArr_2,cardType)
	--拼三张，只发三张牌
	if cardType == 0 then
		cardType = PszService.GetCardType(cardArr_1)
	end

	local comCard_1 = 0
	local comCard_2 = 0
	if cardType == g_PszCard_type.duizi then
		--取到一堆的牌型
		if cardArr_1[1] == cardArr_1[2] or cardArr_1[1] == cardArr_1[3] then
			comCard_1 = cardArr_1[1]
		else
			comCard_1 = cardArr_1[2]
		end
		if cardArr_2[1] == cardArr_2[2] or cardArr_2[1] == cardArr_2[3] then
			comCard_2 = cardArr_2[1]
		else
			comCard_2 = cardArr_2[2]
		end		
	else
		
		for k,v in ipairs(cardArr_1) do
			if math.mod(v,100) > math.mod(comCard_1,100) then
				comCard_1 = v
			end
		end
		
		for k,v in ipairs(cardArr_2) do
			if math.mod(v,100) > math.mod(comCard_2,100) then
				comCard_2 = v
			end
		end		
	end
	
	return PszService.Compare(comCard_1, comCard_2, false)
end
