
DouNiuService = {}

--在欢乐赢豆中使用新的引擎技术
--在定时器的线程保存一份每个牌桌的情况，每次定时器线程循环的时候
--检查缓存中有没有需要更新的，当需要更新的时候就用缓存中更新，
--如果不需要更新，就不用从缓存只能够load牌桌的数据
--每次服务器启动的时候，不清空缓存，接着上次的数据进行

DouNiuService.tableList = {}   --牌桌
DouNiuService.IDList = {}      --这个是用于单独存放ID的
DouNiuService.modifyList = {}  --这个是用于检查是否被修改的

DouNiuService.dismissList = {}
DouNiuService.pourCountList = {}

function DouNiuService.Init()
	--初始化牌桌的机制，每页显示8张桌子，每次初始化8张
	--根据牌桌数量，当空余牌桌少于4张牌桌时，就补充8张，当空余牌桌数量大于12张时，释放其中的8张
	local IDList = DouNiuModel.GetTableList()
	for k,v in ipairs(IDList) do
		local tInfo = DouNiuModel.GetTableInfo(v)
		if tInfo ~= nil then
			DouNiuService.tableList[tonumber(v)] = tInfo
			DouNiuService.modifyList[tonumber(v)] = 0
			table.insert(DouNiuService.IDList,tonumber(v))
		end
	end
end

function DouNiuService.ServerLoop()
	
	--在一开始的时候对庄家进行处理
	for k,v in ipairs(DouNiuService.IDList) do
		
		if DouNiuService.tableList[v] ~= nil then
		
			ThreadManager.DouNiuLock(v)
			if true == DouNiuModel.isModify(v) then
				local tInfo = DouNiuModel.GetTableInfo(v)
				DouNiuService.tableList[v] = tInfo
			end
			DouNiuService.DoWork(DouNiuService.tableList[v])			
			if DouNiuService.modifyList[v] ~= 0 then
				DouNiuService.modifyList[v] = 0
				DouNiuModel.SetTableInfo(DouNiuService.tableList[v])
				
			end
			ThreadManager.DouNiuUnLock(v)
		else
			table.insert(DouNiuService.dismissList,v)
		end
		
	end
	
	local newList = DouNiuModel.GetNewTableList()
	
	for k,v in ipairs(newList) do   --对于刚刚创建的房间，需要在这里加入到table的列表中
		ThreadManager.DouNiuLock(v)
		local tInfo = DouNiuModel.GetTableInfo(v)
		if tInfo ~= nil then
			if DouNiuService.tableList[tInfo.tableid] == nil then
				DouNiuService.DoWork(tInfo)
				DouNiuService.tableList[tInfo.tableid] = tInfo
				DouNiuService.modifyList[tInfo.tableid] = 0
				table.insert(DouNiuService.IDList,tInfo.tableid)
				DouNiuModel.SetTableInfo(tInfo)
				DouNiuModel.DelModify(tInfo.tableid)
			else
				DouNiuService.tableList[tInfo.tableid] = tInfo
			end
		else
			table.insert(DouNiuService.dismissList,v)
		end
		ThreadManager.DouNiuUnLock(v)
	end
	
	--检查是否需要删除的
	for i = #DouNiuService.dismissList,1,-1 do
		DouNiuModel.DelTableInfo(DouNiuService.dismissList[i])
		table.remove(DouNiuService.tableList,DouNiuService.dismissList[i])  --同时在这里也删除
		DouNiuService.tableList[ DouNiuService.dismissList[i] ] = nil
		DouNiuService.modifyList[ DouNiuService.dismissList[i] ] = nil
		for j = 1,#DouNiuService.IDList do
			if DouNiuService.IDList[j] == DouNiuService.dismissList[i] then
				table.remove(DouNiuService.IDList,j)
				break
			end
		end
		table.remove(DouNiuService.dismissList,i)
	end

end

function DouNiuService.DoWork(tInfo)
	
	if tInfo == nil then
		LogBehavior.Warning(12355654456, "douniu", "DouNiuDoWork", 0,"牌桌出错了")
		return 
	end
	
	--print("tableID = "..tInfo.tableid .."   tInfo.state = "..tInfo.state.."   tInfo.timemark = "..tInfo.timemark )
	if tInfo.state == g_douNiuDefine.state_ready then
		DouNiuService.DouNiuReady(tInfo)
		DouNiuService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_douNiuDefine.state_start then
		DouNiuService.DouNiuStart(tInfo)
		DouNiuService.modifyList[tInfo.tableid] = 1
		
	elseif tInfo.state == g_douNiuDefine.state_banker then
		DouNiuService.DouNiuBanker(tInfo)
		DouNiuService.modifyList[tInfo.tableid] = 1
		
	elseif tInfo.state == g_douNiuDefine.state_wait then
		DouNiuService.DouNiuWait(tInfo)
		DouNiuService.modifyList[tInfo.tableid] = 1

	elseif tInfo.state == g_douNiuDefine.state_play then
		DouNiuService.DouNiuPlay(tInfo)  
		DouNiuService.modifyList[tInfo.tableid] = 1
		
	elseif tInfo.state == g_douNiuDefine.state_count then
		DouNiuService.DouNiuCount(tInfo) 
		DouNiuService.modifyList[tInfo.tableid] = 1
		
	elseif tInfo.state == g_douNiuDefine.state_next then
		DouNiuService.DouNiuNext(tInfo)
		DouNiuService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_douNiuDefine.state_end then
	elseif tInfo.state == g_douNiuDefine.state_dissolve then
		tInfo.timemark = tInfo.timemark - 1
		--print(tInfo.timemark)
		if tInfo.timemark <= 0 then
			--需要删除了
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
					tInfo.situser[i].bankernum = 0
				end
			end
			
			local gcCount = msg_douniu_pb.gcdouniucount()
			gcCount.hinfo:ParseFromString(tInfo:SerializeToString())
			gcCount.result = 0
			--说明牌局需要结束了
			gcCount.counttype = 1
			gcCount.timemark = TimeUtils.GetTime()
			for k,v in ipairs(tInfo.situser) do
				local pInfo = PlayerModel.GetPlayerInfo(v.userid)
				local addItem = gcCount.pslist:add()
				PlayerModel.ChangeSimpleInfo(pInfo, addItem)
				addItem.jifen = v.countjetton
			end
			DouNiuHistoryModel.SetHistory(tInfo, 1, 0)    	--把牌局信息记录到历史战绩
			DouNiuModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)
			--把所有人提出房间
			local userIDList = DouNiuService.GetUserIDList(tInfo)
			for k,v in ipairs(userIDList) do
				DouNiuModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
			end
			--table.insert(DouNiuService.dismissList, tInfo.tableid)
			tInfo.state = g_douNiuDefine.state_delete  --如果是在这里，就直接删除了
			local userIDList = DouNiuService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3026].client, gcCount:ByteSize(), gcCount:SerializeToString())	
		end		
		DouNiuService.modifyList[tInfo.tableid] = 1
	elseif tInfo.state == g_douNiuDefine.state_delete then
		--游戏结束通知俱乐部
		DouNiuEvent.JulebuGameEnd(tInfo.julebuid, tInfo.tableid, tInfo.julebutype, tInfo.dntype)
		table.insert(DouNiuService.dismissList, tInfo.tableid) --直接删除了
	end
	
end

function DouNiuService.CheckBanker(tInfo)
	--每次start中调用，用于选定庄家
	--print(tInfo.dntype)
	if tInfo.dntype == g_douNiuType.type_gdzj then--如果是固定庄家的
		if tInfo.usevipnum == 1 then
			local is = false
			for k,v in ipairs(tInfo.situser) do
				if tInfo.ownerid == v.userid then
					is = true
				end
			end
			if is == true then
				tInfo.bankerID = tInfo.ownerid   --房主就是庄家
			else 
				local banker= DouNiuService.WhoBanker(tInfo.situser,buqiangList,tInfo)
				tInfo.bankerID = banker.userid
			end
		else
			local is = false
			for k,v in ipairs(tInfo.situser) do
				if tInfo.bankerID == v.userid and v.state == g_douniuPlayStatus.status_next then
					is = true
				end
			end
			if is == false then
				local qList = {} 
				for k,v in ipairs(tInfo.situser)do
					if v.state == g_douniuPlayStatus.status_next then
						table.insert(qList, v)
					end
				end
				local banker= DouNiuService.WhoBanker(qList,buqiangList,tInfo)
				tInfo.bankerID = banker.userid
			end
			
		end
	elseif tInfo.dntype == g_douNiuType.type_nnsz then --牛牛上庄
		if tInfo.usevipnum == 1 then 
			local banker= DouNiuService.WhoBanker(tInfo.situser,buqiangList,tInfo)
			tInfo.bankerID = banker.userid
		else
	
			local nnlist = {}
			local tmp = 0
			for k,v in ipairs(tInfo.situser) do
				if v.nntype >= 10 and v.state == g_douniuPlayStatus.status_next then
					if tmp == v.nntype then
						table.insert(nnlist,v.userid)
					elseif tmp < v.nntype then
						tmp = v.nntype 
						nnlist = {}
						table.insert(nnlist,v.userid)
					end
				end
			end
			
			local is = false
			for k,v in ipairs(tInfo.situser) do
				if tInfo.bankerID == v.userid and v.state == g_douniuPlayStatus.status_next then
					is = true
				end
			end
			
			if is == false and #nnlist == 0 then
				for k,v in ipairs(tInfo.situser) do
					if v.state == g_douniuPlayStatus.status_next then
						if tmp == v.nntype then
							table.insert(nnlist,v.userid)
						elseif tmp < v.nntype then
							tmp = v.nntype 
							nnlist = {}
							table.insert(nnlist,v.userid)
						end
					end
				end
			end
			
			if #nnlist > 1 then
				local mun = math.myrandom(1,#nnlist)
				tInfo.bankerID = nnlist[mun]
			elseif #nnlist == 1 then
				tInfo.bankerID = nnlist[1]
			end
		end	
	else  --自由抢庄 ，通比牛牛 ，明牌抢庄
		tInfo.bankerID = 0
	end	
end

function DouNiuService.DouNiuReady(tInfo)
	--如果满人了就自动开始
	local start = false
	if #tInfo.situser == tInfo.autoStart then
		
		tInfo.state = g_douNiuDefine.state_start  
		tInfo.timemark = 0  
		start = true
		--[[local gcmsgtmp = msg_douniu_pb.gcdouniuleave()
		while #tInfo.standuser > 0 do
			gcmsgtmp.result = 0
			gcmsgtmp.userid = tInfo.standuser[1]
			gcmsgtmp.leavemsg = "游戏已开始，请无座玩家离开房间"
			DouNiuModel.DelUserTableID(tInfo.standuser[1])
			SendMessage(tInfo.standuser[1], PacketCode[3012].client,gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
			tInfo.standuser:remove(1)
			--break
		end]]
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
	
	local gcLeave = msg_douniu_pb.gcdouniuleave()
	gcLeave.result = 0
	gcLeave.leavemsg = "游戏已超过半个小时没开始，系统默认解散房间!!!"
		
	for k,v in ipairs(tInfo.standuser) do   --这个是一个int list
		gcLeave.userid = v
		SendMessage(v, PacketCode[3012].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
	end
			
	for k,v in ipairs(tInfo.situser) do
		--这个是坐在座位上的
		gcLeave.userid = v.userid
		gcLeave.chairid = v.chairid
		SendMessage(v.userid, PacketCode[3012].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
	end
				
	tInfo.state = g_douNiuDefine.state_delete
	--游戏还没开始, 房主付费退还钻石
	if tInfo.payway == 1 then 
		local pInfo = PlayerModel.GetPlayerInfo(tInfo.ownerid)
		DouNiuService.FeeReturns(pInfo,tInfo.paynum, tInfo.paytype)
	end
			
	DouNiuModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)
	--把所有人提出房间
	local userIDList = DouNiuService.GetUserIDList(tInfo)
	for k,v in ipairs(userIDList) do
		DouNiuModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
	end


end

function DouNiuService.DouNiuStart(tInfo)
	--向所有玩家发送start协议
	--在开始的时候，首先需要把玩家手上的牌给清空。
	tInfo.timemark = tInfo.timemark - 1
	
	if tInfo.timemark > 0 then 
		return 
	end
	local count = DouNiuHistoryModel.GetDayCount()  --一定要弄清楚这里为什么要这么做，是为了取得一个唯一的ID啊
	if tInfo.frameid == 0 then    --在私人房中，这个是用于保存历史记录的
		local tm = TimeUtils.GetTableTime()
		tInfo.frameid = tm.day*10000000 + count*100 + 1  --这里就是ID的妙用了
	else		
		tInfo.frameid = tInfo.frameid + 1  --取得了唯一的ID
	end
	
	tInfo.usevipnum = tInfo.usevipnum + 1 
	
	DouNiuService.CheckBanker(tInfo) --牛牛上庄 需要上局牌的信息
	
	for i = 1,#tInfo.situser do
		while #tInfo.situser[i].handpoker > 0 do
			tInfo.situser[i].handpoker:remove(1)
		end
		while #tInfo.situser[i].pourList > 0 do
			tInfo.situser[i].pourList:remove(1)
		end
		tInfo.situser[i].buyjetton.userid = 0
		tInfo.situser[i].buyjetton.pour = 0
		tInfo.situser[i].double = 0
		tInfo.situser[i].isbuy = 0
		tInfo.situser[i].buyInfo = 0
		tInfo.situser[i].jetton = 1
		tInfo.situser[i].endcard = 0
		if  tInfo.usevipnum == 1 then
			tInfo.situser[i].tgbet = 1
			tInfo.situser[i].tgbanker = 0
			tInfo.situser[i].tgbolus = 0
		end
		if tInfo.dntype == g_douNiuType.type_tbnn  then
			tInfo.situser[i].jetton = g_douNiuPour_fenshu[tInfo.difen][1]
		end
		--牛牛轮庄就不先不清楚玩家的手牌类型
		if tInfo.dntype ~= g_douNiuType.type_nnsz then
			tInfo.situser[i].nntype = 0
		end
	end
	--改这里统一发牌
	DouNiuHuUtils.DouNiufapai(tInfo)

	if tInfo.dntype == g_douNiuType.type_zyqz or tInfo.dntype == g_douNiuType.type_mpqz then
		tInfo.state = g_douNiuDefine.state_banker   --进人抢庄状态
		tInfo.timemark = g_douNiuTime.banker_time + 2    --多加2秒做动画的时间
	elseif tInfo.dntype == g_douNiuType.type_nnsz then
		tInfo.state = g_douNiuDefine.state_banker   --进人抢庄状态
		tInfo.timemark = 0
	else
		tInfo.state = g_douNiuDefine.state_wait   --进人下注状态
		if tInfo.dntype == g_douNiuType.type_tbnn then
			tInfo.timemark = 0
			
		else
			tInfo.timemark = g_douNiuTime.wait_time - 1 --好了，开始倒计时了。
		end	
	end
	
	--把玩家状态设置为下一局
	for j = 1, #tInfo.situser do 
		if tInfo.situser[j].state == g_douniuPlayStatus.status_next then
			tInfo.situser[j].state = (tInfo.dntype ~= g_douNiuType.type_tbnn) and g_douniuPlayStatus.status_next or g_douniuPlayStatus.status_wait
		end
		tInfo.situser[j].double = 0
		tInfo.situser[j].buyjetton.userid = 0
		tInfo.situser[j].buyjetton.pour = 0
	end
	
	local gcStart = msg_douniu_pb.gcdouniustart()
	gcStart.hinfo:ParseFromString(tInfo:SerializeToString())
	gcStart.result  = 0
	for i = 1, #gcStart.hinfo.situser do
		gcStart.hinfo.situser[i].nntype = 0
		while #gcStart.hinfo.situser[i].handpoker > 0 do
			gcStart.hinfo.situser[i].handpoker:remove(1)
		end
	end
	
	local userIDList = DouNiuService.GetUserIDList(tInfo)
	SendMessage(userIDList, PacketCode[3018].client, gcStart:ByteSize(), gcStart:SerializeToString())
	--扣个费
	if tInfo.usevipnum == 1 then
		if false == DouNiuEvent.IsBiSai(tInfo.julebuid) then
			DouNiuService.DouNiuPay(tInfo, 0)
		else
			for k, v in ipairs(tInfo.situser)do
				local pInfo = PlayerModel.GetPlayerInfo(v.userid)
				PlayerModel.SendPlayerInfo(pInfo,{"money"})
			end
		end
		DouNiuEvent.JulebuGameStart(tInfo)
	end
	
	--if true == DouNiuEvent.IsBiSai(tInfo.julebuid) then
		--如果有玩家一个回合都没有操作就把他设置为托管
		if tInfo.dntype ~= g_douNiuType.type_tbnn then
			local num = g_douNiuDefine.maxovertime
			if tInfo.dntype == g_douNiuType.type_gdzj or  tInfo.dntype == g_douNiuType.type_nnsz then
				num = g_douNiuDefine.maxovertime / 2
			end
			for	i = 1, #tInfo.situser do
				if tInfo.situser[i].overtimenum >=  num then
					if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_no then
						tInfo.situser[i].trusteeship = g_douNiuDefine.trusteeship_yes
						tInfo.situser[i].tgbet = tInfo.situser[i].tgbet or 1
						tInfo.situser[i].tgbanker = tInfo.situser[i].tgbanker or 0
						tInfo.situser[i].tgbolus = tInfo.situser[i].tgbolus or 0
						local gcmsgT = msg_douniu_2_pb.gcdouniutrusteeship()
						gcmsgT.result = 0
						gcmsgT.userid = tInfo.situser[i].userid
						gcmsgT.trusteeship = g_douNiuDefine.trusteeship_yes
						gcmsgT.tgbet = tInfo.situser[i].tgbet 
						gcmsgT.tgbanker = tInfo.situser[i].tgbanker
						gcmsgT.tgbolus = tInfo.situser[i].tgbolus
						local userIDList = DouNiuService.GetUserIDList(tInfo)
						SendMessage(userIDList, PacketCode[3054].client, gcmsgT:ByteSize(), gcmsgT:SerializeToString())
					end
				end
			end
		end
	--end
end

function DouNiuService.DouNiuBanker(tInfo)
	local timetmp = 1 
	local timetmp1 = 1
	local gcBankerOpt = msg_douniu_pb.gcdouniubanketopt()
	if tInfo.dntype == g_douNiuType.type_mpqz then 
		if tInfo.timemark == (g_douNiuTime.banker_time + 2) then
			for k,v in ipairs(tInfo.situser)do
				local gcDeal = msg_douniu_pb.gcdouniudeal()
				gcDeal.result = 0
				for i,j in ipairs(tInfo.situser)do 
					if j.state == g_douniuPlayStatus.status_next then
						gcDeal.useridlist:append(j.userid)
						local addPoker = gcDeal.handpokerlist:add()
						local tempList = {}
						for z,y in ipairs(v.handpoker) do
							if j.userid ~= v.userid then
								y = 0
							end
							addPoker.handpoker:append(y)
							if j.userid == v.userid then
								table.insert(tempList,y)
							end
							if z == 4 then
								if #tempList == 4 then
									tInfo.situser[i].fourcardtype = DouNiuHuUtils.GetFourCardType(tempList, tInfo) --检测发给玩家的四张牌的牛牛类型
									--print("**fourcardtype****"..tInfo.situser[i].fourcardtype)
								end
								break
							end
						end
					end
				end
				SendMessage(v.userid, PacketCode[3022].client, gcDeal:ByteSize(), gcDeal:SerializeToString())
			end		
			--给无座玩家也发一下
			for k, v in ipairs(tInfo.standuser)do 
				local gcDeal = msg_douniu_pb.gcdouniudeal()
				gcDeal.result = 0
				for k1,v1 in ipairs(tInfo.situser)do
					if v1.state == g_douniuPlayStatus.status_next then
						gcDeal.useridlist:append(v1.userid)
						local addPoker = gcDeal.handpokerlist:add()
						for z,y in ipairs(v1.handpoker) do
							y = 0
							addPoker.handpoker:append(y)
							if z == 4 then
								break
							end
						end
					end
				end
				SendMessage(v, PacketCode[3022].client, gcDeal:ByteSize(), gcDeal:SerializeToString())
			end
		end
	end
	
	if tInfo.timemark == (g_douNiuTime.banker_time - timetmp + 2) then
		gcmsg = msg_douniu_pb.gcdouniubanketgo()
		gcmsg.result = 0	
		gcmsg.tableid = tInfo.tableid
		local userIDList = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3032].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	--如果等于托管状态的玩家自动帮他选择不抢
	elseif	tInfo.timemark < (g_douNiuTime.banker_time) and tInfo.timemark > timetmp1 then
		for i = 1, #tInfo.situser do
			if tInfo.situser[i].state == g_douniuPlayStatus.status_next then
				if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes then 
					--如果选择了智能托管,并且牌型是牛八以上则抢庄一倍
					if tInfo.situser[i].tgbanker >= 8 and tInfo.situser[i].fourcardtype >= tInfo.situser[i].tgbanker then
						tInfo.situser[i].basicsmul = 1
						tInfo.situser[i].state = g_douniuPlayStatus.status_banker 
						local gcmsgQ = msg_douniu_pb.gcdouniubanket()
						gcmsgQ.userid = tInfo.situser[i].userid
						gcmsgQ.chairid = tInfo.situser[i].chairid
						gcmsgQ.bankermul = tInfo.situser[i].basicsmul
						gcmsgQ.bankerstate = tInfo.situser[i].state
						gcmsgQ.result = 0
						local userIDList = DouNiuService.GetUserIDList(tInfo)
						SendMessage(userIDList, PacketCode[3030].client, gcmsgQ:ByteSize(), gcmsgQ:SerializeToString())
					else
						tInfo.situser[i].basicsmul = 0
						tInfo.situser[i].state = g_douniuPlayStatus.status_buqiang 
						local gcmsgQ = msg_douniu_pb.gcdouniubanket()
						gcmsgQ.userid = tInfo.situser[i].userid
						gcmsgQ.chairid = tInfo.situser[i].chairid
						gcmsgQ.bankermul = tInfo.situser[i].basicsmul
						gcmsgQ.bankerstate = tInfo.situser[i].state
						gcmsgQ.result = 0
						local userIDList = DouNiuService.GetUserIDList(tInfo)
						SendMessage(userIDList, PacketCode[3030].client, gcmsgQ:ByteSize(), gcmsgQ:SerializeToString())
					end
				end
			end
		end
	--提前通知客户端关闭抢庄按钮
	elseif tInfo.timemark == timetmp1 then
		local gcButton = msg_douniu_2_pb.gcdouniuconcealbutton()
		gcButton.hideType = 1
		gcButton.result  = 0
		local userIDList = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3056].client, gcButton:ByteSize(), gcButton:SerializeToString())
	end
	
	local isOverTime = false
	for k,v in ipairs(tInfo.situser) do
		if v.state == g_douniuPlayStatus.status_next then
			isOverTime = true
		end
	end
	
	if isOverTime == false then
		if tInfo.timemark >= 2 then
			tInfo.timemark = 2
		end
	end
	tInfo.timemark = tInfo.timemark - 1
		
	if tInfo.timemark >= 0 then
		return 
	end
	
	DouNiuService.sendBanker(tInfo)
end

function DouNiuService.DouNiuWait(tInfo)
	
	local gcmsg = msg_douniu_pb.gcdouniupouropt()
	if tInfo.timemark == (g_douNiuTime.wait_time - 1)  and tInfo.dntype ~= g_douNiuType.type_tbnn then
		gcPourGo = msg_douniu_pb.gcdouniupourgo()
		gcPourGo.result = 0	
		gcPourGo.tableid = tInfo.tableid
		gcPourGo.bankerID = tInfo.bankerID
		local ban = 0
		for k, v in ipairs(tInfo.situser)do
			if tInfo.bankerID == v.userid then
				ban = k
			end
		end	
		for k, v in ipairs(tInfo.situser)do
			local isTZ = true
			if v.userid ~= tInfo.situser[ban].userid then
				while #gcPourGo.pourlist > 0 do
					gcPourGo.pourlist:remove(1)
				end
				for i = 1, #g_douNiuPour_fenshu[tInfo.difen] do
					gcPourGo.pourlist:append(g_douNiuPour_fenshu[tInfo.difen][i])
				end
				--买码
				tInfo.situser[k].isbuy = 2
				if tInfo.playerbuycode == 1 then
					tInfo.situser[k].isbuy = 1
					gcPourGo.maima = 1
				end
				--下注限制
				if tInfo.pourrestrict == 1 then
					if v.basicsmul >=  tInfo.bankermultiple and v.basicsmul ~= 99 then
						gcPourGo.pourlist:remove(1)
					end
					if v.basicsmul < tInfo.situser[ban].basicsmul or v.basicsmul == 99 then
						isTZ = false
					end
				end
				--下注加倍
				gcPourGo.double = 0
				if tInfo.pourdouble == 1 then
					if v.basicsmul >= tInfo.bankermultiple and v.basicsmul ~= 99 then
						tInfo.situser[k].double = 1
						gcPourGo.double = 1
					end
				end 
				--推注 
				if tInfo.TZmax == g_douNiuTZ_no then
					isTZ = false
				end
				if isTZ == true and v.TZnum > 0 then
					gcPourGo.pourlist:append(v.TZnum)
				end
			end

			while #gcPourGo.pourlist < 4 do
				gcPourGo.pourlist:append(0)
			end
			for i = 1, #gcPourGo.pourlist do

				tInfo.situser[k].pourList:append(gcPourGo.pourlist[i]) 
			end
			--print(gcPourGo)
			SendMessage(v.userid, PacketCode[3042].client, gcPourGo:ByteSize(), gcPourGo:SerializeToString())
		end
		
		for i = 1,#tInfo.situser do  --庄家不用下注，到了下注，状态默认已下注
			if tInfo.bankerID == tInfo.situser[i].userid then
				tInfo.situser[i].state = g_douniuPlayStatus.status_wait
				tInfo.situser[i].jetton = 0  
				
				if tInfo.dntype == g_douNiuType.type_mpqz or  tInfo.dntype == g_douNiuType.type_zyqz then
					--记录超时次数
					tInfo.situser[i].overtimenum = tInfo.situser[i].overtimenum + 1
				end
			else
				if tInfo.dntype == g_douNiuType.type_gdzj then
					if tInfo.situser[i].state == g_douniuPlayStatus.status_next then
						tInfo.situser[i].state = g_douniuPlayStatus.status_banker
					end
				end
			end
		end
	elseif	tInfo.timemark < (g_douNiuTime.wait_time - 2) and tInfo.timemark > 2 then
		if tInfo.dntype ~= g_douNiuType.type_tbnn then
			for i = 1, #tInfo.situser do
				if tInfo.situser[i].state == g_douniuPlayStatus.status_banker or tInfo.situser[i].state == g_douniuPlayStatus.status_buqiang then
					if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes then
						--智能下注,下注固定两分
						--存在推注情况的下注
						if tInfo.situser[i].TZnum > 0 and tInfo.situser[i].tgbet >= 8 and tInfo.situser[i].fourcardtype >= tInfo.situser[i].tgbet then
							tInfo.situser[i].jetton = tInfo.situser[i].TZnum
							tInfo.situser[i].state = g_douniuPlayStatus.status_wait
							local gcmsgQ = msg_douniu_pb.gcdouniupour()
							gcmsgQ.userid = tInfo.situser[i].userid
							gcmsgQ.pourbei = tInfo.situser[i].TZnum
							gcmsgQ.result = 0
							gcmsg.isbolus = 1
							local userIDList = DouNiuService.GetUserIDList(tInfo)
							SendMessage(userIDList, PacketCode[3020].client, gcmsgQ:ByteSize(), gcmsgQ:SerializeToString())
						--不存在推注情况的下注	-- 设置为2和智能下注都是固定2分
						elseif  tInfo.situser[i].tgbet == 2 or (tInfo.situser[i].tgbet >= 8 and tInfo.situser[i].fourcardtype >= tInfo.situser[i].tgbet )then
							tInfo.situser[i].jetton = g_douNiuPour_fenshu[tInfo.difen][2]
							tInfo.situser[i].state = g_douniuPlayStatus.status_wait
							local gcmsgQ = msg_douniu_pb.gcdouniupour()
							gcmsgQ.userid = tInfo.situser[i].userid
							gcmsgQ.pourbei = g_douNiuPour_fenshu[tInfo.difen][2]
							gcmsgQ.result = 0
							local userIDList = DouNiuService.GetUserIDList(tInfo)
							SendMessage(userIDList, PacketCode[3020].client, gcmsgQ:ByteSize(), gcmsgQ:SerializeToString())
						else
							--非智能下注,按托管设置下注
							tInfo.situser[i].jetton = g_douNiuPour_fenshu[tInfo.difen][1] 
							tInfo.situser[i].state = g_douniuPlayStatus.status_wait
							local gcmsgQ = msg_douniu_pb.gcdouniupour()
							gcmsgQ.userid = tInfo.situser[i].userid
							gcmsgQ.pourbei = g_douNiuPour_fenshu[tInfo.difen][1]
							gcmsgQ.result = 0
							local userIDList = DouNiuService.GetUserIDList(tInfo)
							SendMessage(userIDList, PacketCode[3020].client, gcmsgQ:ByteSize(), gcmsgQ:SerializeToString())
						end
					end
				end
			end
		end
	--提前通知客户端关闭下注按钮
	elseif tInfo.timemark == 2  and tInfo.dntype ~= g_douNiuType.type_tbnn then
		local gcButton = msg_douniu_2_pb.gcdouniuconcealbutton()
		gcButton.hideType  = 2
		gcButton.result  = 0
		local userIDList = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3056].client, gcButton:ByteSize(), gcButton:SerializeToString())
	end
	
	local isOverTime = false
	for k,v in ipairs(tInfo.situser) do
		if v.state == g_douniuPlayStatus.status_banker or v.state == g_douniuPlayStatus.status_buqiang then
			isOverTime = true
		end
	end
	
	if isOverTime == false then 
		if tInfo.timemark >= 2 then
			tInfo.timemark = 2
		end
	end
	tInfo.timemark = tInfo.timemark - 1
		
	if tInfo.timemark > 0 then 
		return 
	end
	
	DouNiuService.sendPour(tInfo)
end
	
function DouNiuService.DouNiuPlay(tInfo) 
	local gcmsg = msg_douniu_pb.gcdouniushowpoker()
	local isOverTime = false
	for k,v in ipairs(tInfo.situser) do
		if v.state == g_douniuPlayStatus.status_wait then
			isOverTime = true
		end
	end
	
	if isOverTime == false then 
		tInfo.timemark = 0
	end
	if tInfo.timemark <= g_douNiuTime.play_time - 1 and tInfo.timemark > 0 then
		
		if tInfo.banrubcard == 1 then
			tInfo.timemark = -1
			return
		end
		for i = 1, #tInfo.situser do
			if tInfo.situser[i].state == g_douniuPlayStatus.status_wait then
				if tInfo.situser[i].volun == g_douNiuDefine.volun_yes or tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes then
					tInfo.situser[i].state = g_douniuPlayStatus.status_play
			
					gcmsg.chairid:append(tInfo.situser[i].chairid)
					gcmsg.nntype:append(tInfo.situser[i].nntype) 
					gcmsg.userid:append(tInfo.situser[i].userid)
					local addItem = gcmsg.handpoker:add()
					for j = 1, #tInfo.situser[i].handpoker do
						addItem.handpoker:append(tInfo.situser[i].handpoker[j])
					end
					gcmsg.complete = tInfo.situser[i].userid
					gcmsg.result = 0
					local userIDList = DouNiuService.GetUserIDList(tInfo) 
					SendMessage(userIDList, PacketCode[3024].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
				end
			end
		end
		
	end
	
	tInfo.timemark = tInfo.timemark - 1
	
	if tInfo.timemark > 0 then 
		return 
	end
	
	local unfinishedNum = 0
	for i = 1, #tInfo.situser do
		
		if tInfo.situser[i].state == g_douniuPlayStatus.status_wait then
			unfinishedNum = unfinishedNum + 1
		end
		
		if tInfo.situser[i].state == g_douniuPlayStatus.status_wait or tInfo.situser[i].state == g_douniuPlayStatus.status_play then
			local addItem = gcmsg.handpoker:add()
			for j = 1, #tInfo.situser[i].handpoker do
				addItem.handpoker:append(tInfo.situser[i].handpoker[j])
				--print(j.." = " .. tInfo.situser[i].handpoker[j])
			end
			--print("-----------------------------------------"..i)
			gcmsg.nntype:append( tInfo.situser[i].nntype)
			gcmsg.userid:append(tInfo.situser[i].userid)
			gcmsg.chairid:append(tInfo.situser[i].chairid)
			if tInfo.situser[i].endcard ~= nil then
				gcmsg.endcardlist:append(tInfo.situser[i].endcard)
			end
			tInfo.situser[i].state = g_douniuPlayStatus.status_play	
		end
	end

	local userIDList = DouNiuService.GetUserIDList(tInfo) 
	gcmsg.result = 0
	gcmsg.complete = 0
	SendMessage(userIDList, PacketCode[3024].client, gcmsg:ByteSize(), gcmsg:SerializeToString())	
	
	local addTime = unfinishedNum * 0.85 
	addTime = math.ceil(addTime)
	tInfo.state = g_douNiuDefine.state_count
	tInfo.timemark = g_douNiuTime.count_time + addTime
	
end

function DouNiuService.DouNiuCount(tInfo)
	if tInfo.timemark == g_douNiuTime.count_time then 
		if tInfo.dntype == g_douNiuType.type_tbnn then
			local winner = DouNiuHuUtils.WhoWinner(tInfo)
			tInfo.bankerID = tInfo.situser[winner].userid
		end
		DouNiuService.DouNiucountOut(tInfo, 0)
		
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
			if tInfo.ruleset == 1 then
				for i = 1, #tInfo.situser do 
					if tInfo.situser[i].carryjetton <= 0 then
						tInfo.situser[i].state = g_douniuPlayStatus.status_ready
					end					
				end
			end
		end

	elseif tInfo.timemark == g_douNiuTime.count_time - 2 then 
		--在固定庄家的是有一个最大上庄上限，庄家输完了游戏就结束了
		local isover = false
		if tInfo.dntype == g_douNiuType.type_gdzj and tInfo.integralmax ~= 0  then
			for i = 1, #tInfo.situser do 
				if tInfo.situser[i].userid == tInfo.bankerID then
					if tInfo.situser[i].carryjetton <= 0 then
						isover = true
						DouNiuHistoryModel.SetHistory(tInfo, 1, 0)    	--把牌局信息记录到历史战绩
					end
				end
			end
		end
		if tInfo.usevipnum == tInfo.maxplaynum or isover == true then
			DouNiuService.DouNiucountOut(tInfo, 1)
		end
	end
	
	tInfo.timemark = tInfo.timemark - 1
	
	
		if tInfo.timemark <= 0  then
		--只剩一个人能玩就直接结束
		local readNum = 0
		for k,v in ipairs(tInfo.situser) do
			local isT = false 
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 and tInfo.ruleset == 1 then
				if v.carryjetton <= 0 and v.state == g_douniuPlayStatus.status_ready then
					readNum = readNum + 1
				end					
			end
		end
		if readNum == #tInfo.situser - 1 then
			tInfo.state = g_douNiuDefine.state_dissolve
			tInfo.timemark = 0
			return 
		end
	end
	
	
	if tInfo.timemark <= 0 and tInfo.usevipnum < tInfo.maxplaynum and isover ~= true then
		tInfo.state = g_douNiuDefine.state_next
		tInfo.timemark = g_douNiuTime.next_time
	end
	
end

function DouNiuService.DouNiucountOut(tInfo, flas)
	local gcCount =  msg_douniu_pb.gcdouniucount()
	if flas == 0 then
		local multipleList = {}
		DouNiuHuUtils.getDN_MultipleList(tInfo, multipleList)
		
		local bankerCount = 0		--庄家的得分
		local ban = 0				--庄家的下表
		for k ,v in ipairs(tInfo.situser) do 
			if v.userid == tInfo.bankerID then
				--统计当庄的次数
				ban = k
				tInfo.situser[ban].bankernum = tInfo.situser[ban].bankernum + 1
				break
			end
		end
		local DNcountlist = {}
		
		for i = 1, #tInfo.situser do 
			DNcountlist[i] = 0
			gcCount.wincount:append(0)
			if tInfo.situser[i].state == g_douniuPlayStatus.status_play	then
				--统计最大的牛牛类型
				if tInfo.situser[i].nntype > tInfo.situser[i].maxdntype then
					tInfo.situser[i].maxdntype = tInfo.situser[i].nntype
				end
				--先算闲家
				if tInfo.situser[i].userid ~= tInfo.bankerID then
					local playerCount = 0    --闲家的得分
					local banBasicsmul = 1
					if tInfo.situser[ban].basicsmul ~= 99 and tInfo.situser[ban].basicsmul ~= 0 then
						banBasicsmul = tInfo.situser[ban].basicsmul
					end
					--自己牌的下注
					if false == DouNiuHuUtils.DouNiuWin(ban,i,tInfo) then
						--闲家赢
						playerCount = playerCount + (banBasicsmul * tInfo.situser[i].jetton * multipleList[tInfo.situser[i].nntype])
					else
						playerCount = playerCount - (banBasicsmul * tInfo.situser[i].jetton * multipleList[tInfo.situser[ban].nntype])
					end
					--买码的下注
					if tInfo.situser[i].buyjetton.userid ~= 0 then
						local tmp = 0  --买的玩家的下表
						for j = 1, #tInfo.situser do
							if tInfo.situser[i].buyjetton.userid == tInfo.situser[j].userid then
								tmp = j 
								break
							end
						end
						if false == DouNiuHuUtils.DouNiuWin(ban,tmp,tInfo) then
							--闲家赢
							playerCount = playerCount + (banBasicsmul * tInfo.situser[i].buyjetton.pour * multipleList[tInfo.situser[tmp].nntype])
						else
							playerCount = playerCount - (banBasicsmul * tInfo.situser[i].buyjetton.pour * multipleList[tInfo.situser[ban].nntype])
						end
					end
					DNcountlist[i] = playerCount
					--tInfo.situser[i].carryjetton = tInfo.situser[i].carryjetton + playerCount
					--tInfo.situser[i].countjetton = tInfo.situser[i].countjetton + playerCount
					bankerCount =  bankerCount - playerCount
						--统计庄家胜利次数
					if playerCount > 0 then
						tInfo.situser[i].winnum = tInfo.situser[i].winnum + 1
					end
					--统计庄家单局最高得分
					if playerCount > tInfo.situser[i].maxscore and playerCount > 0  then
						tInfo.situser[i].maxscore = playerCount
					end
					
					--推注
					if tInfo.TZmax ~= g_douNiuTZ_no then
						if playerCount > 0 then
							local isTZ = false  --检查是否有推注
							if tInfo.situser[i].double == 3 then
								if g_gamename == "xnds" then
									if tInfo.situser[i].jetton == tInfo.TZmax * 2 then
										if tInfo.situser[i].TZnum ~= 0 then
											isTZ = true
										end
									end									
								else
									if tInfo.situser[i].jetton == tInfo.situser[i].TZnum * 2 then
										isTZ = true
									elseif tInfo.situser[i].jetton == g_douNiuPour_fenshu[tInfo.difen][1] * 3 then
										isTZ = true
									elseif tInfo.situser[i].jetton == g_douNiuPour_fenshu[tInfo.difen][1] * 4 then
										isTZ = true
									elseif tInfo.situser[i].jetton == g_douNiuPour_fenshu[tInfo.difen][1] * 8 then
										isTZ = true
									elseif tInfo.situser[i].jetton == g_douNiuPour_fenshu[tInfo.difen][1] * 10 then
										isTZ = true
									end
								end
							else
								if g_gamename == "xnds" then
									if tInfo.situser[i].jetton == tInfo.TZmax then
										if tInfo.situser[i].TZnum ~= 0 then
											isTZ = true
										end
									end									
								else
									if tInfo.situser[i].jetton == tInfo.situser[i].TZnum then
										isTZ = true
									end
								end
							end
							if isTZ == true then    --   -1 表示本局推注了
								--不能连续推注
								tInfo.situser[i].TZnum = 0
							else
								if g_gamename == "xnds" then
									--惠州运营的版本，推注安装原来的来
									tInfo.situser[i].TZnum = tInfo.TZmax
								else
									--tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][#g_douNiuPour_fenshu[tInfo.difen]] + playerCount
									--[[if tInfo.situser[i].TZnum > g_douNiuPour_fenshu[tInfo.difen][1] * tInfo.TZmax  then
										tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][1] * tInfo.TZmax
									end]]
									if tInfo.situser[i].nntype >= 0 and tInfo.situser[i].nntype <= 3 then
											tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][1] * 3
									elseif tInfo.situser[i].nntype >= 4 and tInfo.situser[i].nntype <= 6 then
										tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][1] * 4
									elseif tInfo.situser[i].nntype >= 7 and tInfo.situser[i].nntype <= 9 then
										tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][1] * 8
									elseif tInfo.situser[i].nntype >= 10 then
										tInfo.situser[i].TZnum = g_douNiuPour_fenshu[tInfo.difen][1] * 10
									end
									if tInfo.TZmax == 0 then
										tInfo.situser[i].TZnum = 0
									end
								end
							end
						else
							--输了下一局就不能推注
							tInfo.situser[i].TZnum = 0
						end
					end
				else
					--当庄下一局就不能推注
					tInfo.situser[i].TZnum = 0
				end	
			end
		end
		
		--再算庄家的
		--tInfo.situser[ban].carryjetton = tInfo.situser[ban].carryjetton  + bankerCount
		--tInfo.situser[ban].countjetton = tInfo.situser[ban].countjetton + bankerCount
		DNcountlist[ban] = bankerCount
		
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 --[[and tInfo.ruleset == 1 ]] then
			local maxBanWin = 0
			local maxBanLose = 0
			local tmpDNcountlist = {}
			local tmpBanWin = 0
			local tmpBanLose = 0
			local tempLostList = {}
			local tempLostAll = 0
			local tempWinList = {}
			local tempWinAll = 0
			for k, v in pairs(DNcountlist) do
				if k ~= ban then
					if v < 0 then
						local tempValue = 0 - v
						tmpBanWin = tmpBanWin + tempValue
						local tmpPalyLose =  tempValue < tInfo.situser[k].carryjetton and tempValue or tInfo.situser[k].carryjetton
						maxBanWin = maxBanWin + tmpPalyLose
						tempLostAll = tempLostAll + tmpPalyLose
						tempLostList[k] = {}
						tempLostList[k][1] = tmpPalyLose
						tempLostList[k][2] = tInfo.situser[k].carryjetton
					elseif v > 0 then
						tmpBanLose = tmpBanLose + v
						local tmpPalyWin =  v < tInfo.situser[k].carryjetton and v or tInfo.situser[k].carryjetton
						maxBanLose = maxBanLose + v
						tempWinList[k] = {}
						tempWinList[k][1] = tmpPalyWin
						tempWinAll = tempWinAll + tmpPalyWin
						tempWinList[k][2] = tInfo.situser[k].carryjetton
					end	
				end			
			end
			
			local tempRealLost = 0  --全部玩家真正输的钱
			local loseAllMax = tInfo.situser[ban].carryjetton > tempLostAll and tempLostAll or tInfo.situser[ban].carryjetton
			
			if tempLostAll == 0 then
				local fffff = 0
			end
			
			for k, v_lose in pairs(tempLostList) do
				tempLostList[k][1] = math.floor(loseAllMax*v_lose[1]/tempLostAll)
				tempRealLost = tempRealLost + tempLostList[k][1]
			end
			
			--需要补全的余额
			local tempLostBu = loseAllMax - tempRealLost
			for j = 1,5 do
				if tempLostBu <= 0 then
					break
				end					
				for k,v_lose in pairs(tempLostList) do
					if v_lose[2] > v_lose[1] then
						tempLostList[k][1] = tempLostList[k][1] + 1
						tempLostBu = tempLostBu - 1
						tempRealLost = tempRealLost + 1
					end
					if tempLostBu <= 0 then
						break
					end
				end				
			end
			
			
			local bankerAllJetton = tInfo.situser[ban].carryjetton + tempRealLost  --得到了真正全部的钱
			
			local tempWinMax = tempWinAll > bankerAllJetton and bankerAllJetton or tempWinAll
			local tempWinReal = 0
			if tempWinAll == 0 then
				local ssss  = 0
			end
			
			for k,v_win in pairs(tempWinList) do
				tempWinList[k][1] = math.floor(tempWinMax*v_win[1]/tempWinAll)
				tempWinReal = tempWinReal + tempWinList[k][1]
			end
			
			local tempWinBu = tempWinMax - tempWinReal
			for i = 1, 5 do
				if tempWinBu <= 0 then
					break
				end					
				for k,v_win in pairs(tempWinList) do
					if v_win[2] > v_win[1] then
						tempWinList[k][1] = tempWinList[k][1] + 1
						tempWinBu = tempWinBu - 1
						tempWinReal = tempWinReal + 1
						if tempWinBu <= 0 then
							break
						end
					end
				end
			
			end
			for k,v_lose in pairs(tempLostList) do
				DNcountlist[k] = 0 - v_lose[1]
			end
			for k,v_win in pairs(tempWinList) do
				DNcountlist[k] = v_win[1]
			end
			DNcountlist[ban] = tempRealLost - tempWinReal	
		end
		
		
		for k, v in pairs(DNcountlist) do
			tInfo.situser[k].carryjetton = tInfo.situser[k].carryjetton  + v
			tInfo.situser[k].countjetton = tInfo.situser[k].countjetton + v
			gcCount.wincount[k] = v
		end
		
		
		--统计庄家胜利次数
		if bankerCount > 0 then
			tInfo.situser[ban].winnum = tInfo.situser[ban].winnum + 1
		end
			--统计庄家单局最高得分
		if bankerCount > tInfo.situser[ban].maxscore and bankerCount > 0  then
			tInfo.situser[ban].maxscore = bankerCount
		end
		DouNiuHistoryModel.SetHistory(tInfo, 0, DNcountlist)
		
	else
		DouNiuModel.DelUserTableList(tInfo.ownerid, tInfo.tableid)	
		for k,v in ipairs(tInfo.situser) do
			DouNiuModel.DelUserTableID(v.userid)	--房间解散时把自己所在房间的记录
		end
		tInfo.state = g_douNiuDefine.state_delete  --如果是在这里，就直接删除了
	end
	
	gcCount.result = 0
	gcCount.counttype = flas
	gcCount.timemark = TimeUtils.GetTime()
	gcCount.hinfo:ParseFromString(tInfo:SerializeToString())
	for k,v in ipairs(tInfo.situser) do
		local pInfo = PlayerModel.GetPlayerInfo(v.userid)
		local addItem = gcCount.pslist:add()
		PlayerModel.ChangeSimpleInfo(pInfo, addItem)
		if gcCount.counttype == 1 then
			PlayerModel.TenGames(pInfo)
		end
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 
			and  flas == 1 then
			addItem.jifen = v.countjetton
		else
			addItem.jifen = v.carryjetton
		end
		
	end
	local userIDList = DouNiuService.GetUserIDList(tInfo)
	SendMessage(userIDList, PacketCode[3026].client, gcCount:ByteSize(), gcCount:SerializeToString())
end

function DouNiuService.DouNiuNext(tInfo)
	local gnext = msg_douniu_pb.gcdouniunext()
	local gcmsg = msg_douniu_pb.gcdouniunextout()
	
	if tInfo.timemark == g_douNiuTime.next_time then 
		gcmsg.tableid = tInfo.tableid
		local userList = DouNiuService.GetUserIDList(tInfo)
		gcmsg.result = 0
		SendMessage(userList, PacketCode[3036].client, gcmsg:ByteSize(), gcmsg:SerializeToString())		
	end
	--DouNiuService.dnsit(tInfo)
	for i = 1, #tInfo.situser do 
		if tInfo.situser[i].state ~= g_douniuPlayStatus.status_next then
			if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes or tInfo.situser[i].volun == g_douNiuDefine.volun_yes then
				tInfo.situser[i].state = g_douniuPlayStatus.status_next
				gnext.chairid = tInfo.situser[i].chairid
				local userIDList = DouNiuService.GetUserIDList(tInfo)
				gnext.result = 0
				SendMessage(userIDList, PacketCode[3034].client, gnext:ByteSize(), gnext:SerializeToString())
			end
		end
	end

	
	local isOverTime = false
	local readNum = 0
	for k,v in ipairs(tInfo.situser) do
		if v.state ~= g_douniuPlayStatus.status_next then
			local isT = false 
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.ruleset == 1 then
					if v.carryjetton <= 0 and v.state == g_douniuPlayStatus.status_ready then
						isT = true 
					end					
				end
			end
			if isT == false then 
				isOverTime = true
				readNum = readNum + 1
			end
		else
			readNum = readNum + 1
		end
	end
	if isOverTime == false then
		tInfo.timemark = 0
	end	
	tInfo.timemark = tInfo.timemark - 1
	
	if #tInfo.situser == 0 then
		tInfo.state = g_douNiuDefine.state_dissolve
		tInfo.timemark = 0
		return
	end
	if tInfo.timemark > 0 or readNum < 2 then 
		return 
	end
	
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].state ~= g_douniuPlayStatus.status_next  then
			local isT = false 
			if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
				if tInfo.ruleset == 1 then
					if tInfo.situser[i].carryjetton <= 0 and tInfo.situser[i].state == g_douniuPlayStatus.status_ready then
						isT = true 
					end					
				end
			end
			if isT == false then 
				tInfo.situser[i].state = g_douniuPlayStatus.status_next
			end
			
			gnext.chairid = tInfo.situser[i].chairid
			local userIDList = DouNiuService.GetUserIDList(tInfo)
			gnext.result = 0
			SendMessage(userIDList, PacketCode[3034].client, gnext:ByteSize(), gnext:SerializeToString())
		end		
	end	
	
	tInfo.state =  g_douNiuDefine.state_start
	tInfo.timemark = 0
end

function DouNiuService.GetUserIDList(tInfo,exceptID)
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

function DouNiuService.WhoBanker(qiangzhuangList,buqiangList,tInfo)
	if #qiangzhuangList == 1 then
		return qiangzhuangList[1]
	end
	local tmp = {}
	if #qiangzhuangList == 0 then 			--如果没人抢庄就在所以玩家中随机选一个
		tmp = buqiangList	
	elseif tInfo.dntype == g_douNiuType.type_mpqz then	--只有明牌抢庄的抢庄玩家有倍数
		local max = qiangzhuangList[1]
		table.insert(tmp, max)
		for i = 2, #qiangzhuangList do
			if max.basicsmul == qiangzhuangList[i].basicsmul then
				table.insert(tmp, qiangzhuangList[i])
			elseif max.basicsmul < qiangzhuangList[i].basicsmul then
				max = qiangzhuangList[i]
				tmp = {}
				table.insert(tmp, qiangzhuangList[i])
			end
		end
 	else 
		tmp = qiangzhuangList
	end
	
	local mun = math.myrandom(1,#tmp)
	return tmp[mun]
end

--查看AA够不够扣费
function DouNiuService.DouNiucheck(tInfo, pInfo)
	
	if tInfo.payway ~= g_douNiuDefine.payment_A then
		return true
	end
	
	local needMoney 
	local needGold
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
function DouNiuService.DouNiuPay(tInfo, payt)
	--房主付费返回，房主不再这里付费
	if tInfo.payway ~= g_douNiuDefine.payment_A then
		return   --如果不是AA付费的情况，就返回了
	end
	--AA付费
	--房主优先扣他所选择的类型
	--其他玩家优先扣除钻石
	--游戏刚开始扣费
	if payt == 0 then
		local needgold = tInfo.paynum 
		if tInfo.paytype == g_douNiuDefine.diamond_pay then
			needgold = needgold * 10
		end
			
		for i = 1, #tInfo.situser do
			local pInfo = PlayerModel.GetPlayerInfo(tInfo.situser[i].userid)
			local needNum = needgold 
			local needType = g_douNiuDefine.jade_pay
			if tInfo.situser[i].userid == tInfo.ownerid then
				needNum = tInfo.paynum
				needType = tInfo.paytype
			end
			DouNiuService.FeeDeductions(pInfo,needNum,needType)
		end
	--游戏开始后扣费
	elseif payt == 1 then
		
		local needmoney = math.ceil((tInfo.maxplaynum - tInfo.usevipnum) / 10 ) --需要扣钻的数量
		local needgold = needmoney * 10
		
		for i = 1, #tInfo.standuser do	
			local pInfo = PlayerModel.GetPlayerInfo(tInfo.standuser[i])	
			local needNum = needgold 
			local needType = g_douNiuDefine.jade_pay
			if tInfo.standuser[i] == tInfo.ownerid then
				needNum = needgold
				if tInfo.paytype == g_douNiuDefine.diamond_pay then
					needNum = needmoney
				end
				needType = tInfo.paytype
			end
			DouNiuService.FeeDeductions(pInfo,needNum,needType)
		end
	end
	
	return true
end

--扣费
function DouNiuService.FeeDeductions(pInfo,needMoney,paytype,cause)
	
	if cause == nil then
		cause = "create room"
	end
	--扣钻石的情况
	if paytype == g_douNiuDefine.diamond_pay then
		--钻石足够
		if pInfo.money >= needMoney then
			PlayerModel.DecMoney(pInfo, needMoney, "douniu",cause)  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)	
			PlayerModel.SendPlayerInfo(pInfo,{"money"})
		--钻石不够
		else 
			--扣玉
			needMoney = needMoney * 10
			if pInfo.gold >= needMoney then
				PlayerModel.DecGold(pInfo, needMoney, "douniu",cause)  --创建房间
				PlayerModel.SetPlayerInfo(pInfo)
				PlayerModel.SendPlayerInfo(pInfo,{"gold"})
			else
				return false	
			end
		end
	--扣玉的情况
	elseif paytype == g_douNiuDefine.jade_pay then
		--玉足够的情况
		if pInfo.gold >= needMoney then
			PlayerModel.DecGold(pInfo, needMoney, "douniu",cause)  --创建房间
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendPlayerInfo(pInfo,{"gold"})
		--玉不够
		else
			--钻石够
			needMoney = needMoney / 10
			if pInfo.money >= needMoney then
				PlayerModel.DecMoney(pInfo, needMoney, "douniu",cause)  --创建房间
				PlayerModel.SetPlayerInfo(pInfo)	
				PlayerModel.SendPlayerInfo(pInfo,{"money"})
			--钻石不够
			else 
				return false
			end
		end		
	end
	return true
end 

--退费
function DouNiuService.FeeReturns(pInfo,needMoney,paytype,cause)
	
	if cause == nil then
		cause = "dissolve room"
	end
	
	if paytype == g_douNiuDefine.diamond_pay then
		PlayerModel.AddMoney(pInfo, needMoney, "douniu", cause)
		PlayerModel.SetPlayerInfo(pInfo)	
		PlayerModel.SendPlayerInfo(pInfo,{"money"})
	elseif paytype == g_douNiuDefine.jade_pay then
		PlayerModel.AddGold(pInfo, needMoney, "douniu", cause)
		PlayerModel.SetPlayerInfo(pInfo)	
		PlayerModel.SendPlayerInfo(pInfo,{"gold"})
	end

end

function DouNiuService.dnsit(tInfo)

	if #tInfo.standuser == 0 then
		return
	end
	--扣个费
	DouNiuService.DouNiuPay(tInfo,1)
	
	for i = 1, #tInfo.standuser do	
		local pInfo = PlayerModel.GetPlayerInfo(tInfo.standuser[i])				
		--坐下
		--取得chairid
		local chairidList = {0,0,0,0,0,0,0,0,0,0}
		for k,v in ipairs(tInfo.situser) do
			chairidList[v.chairid] = 1
		end
		local sit = 0
		local addItem = tInfo.situser:add()
		for k,v in ipairs(chairidList) do
			if v == 0 then
				addItem.chairid = k
				sit = k
				break
			end
		end
					
		addItem.userid = tInfo.standuser[i]
		addItem.state = g_douniuPlayStatus.status_next
		addItem.carryjetton = 0
		addItem.volun = g_douNiuDefine.volun_yes

		local gcmsg = msg_douniu_pb.gcdouniusitdown()
		
		--更新玩家数量		
		DouNiuEvent.JulebuGameUpdate(tInfo.julebuid, tInfo.tableid, #tInfo.situser, tInfo.julebutype, cgmsg.userid)  
						
		PlayerModel.ChangeSimpleInfo(pInfo,gcmsg.psinfo)
		gcmsg.chairid = sit
		gcmsg.result = 0
		local userIDList = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3014].client,gcmsg:ByteSize(), gcmsg:SerializeToString()) --发送给其他玩家
	end
	while #tInfo.standuser > 0 do
		tInfo.standuser:remove(1)
	end
	
end


function DouNiuService.sendBanker(tInfo)
	local gcBankerOpt = msg_douniu_pb.gcdouniubanketopt()
	local qiangzhuangList = {}
	local buqiangList = {}
	for j = 1, #tInfo.situser do 
		if tInfo.situser[j].state == g_douniuPlayStatus.status_banker then
			table.insert(qiangzhuangList, tInfo.situser[j]) 
		elseif tInfo.situser[j].state == g_douniuPlayStatus.status_buqiang then
			tInfo.situser[j].basicsmul = 99
			table.insert(buqiangList, tInfo.situser[j]) 
		elseif  tInfo.situser[j].state == g_douniuPlayStatus.status_next then
			tInfo.situser[j].basicsmul = 99
			tInfo.situser[j].state = g_douniuPlayStatus.status_buqiang 
			table.insert(buqiangList, tInfo.situser[j]) 
			if tInfo.dntype == g_douNiuType.type_nnsz then 
				if   tInfo.bankerID ~=	tInfo.situser[j].userid  then
					--记录超时次数
					tInfo.situser[j].overtimenum = tInfo.situser[j].overtimenum + 1
				end
			else
				--记录超时次数
				tInfo.situser[j].overtimenum = tInfo.situser[j].overtimenum + 1
			end
			
		end
		
		if tInfo.dntype == g_douNiuType.type_nnsz then
			if tInfo.bankerID == tInfo.situser[j].userid or tInfo.usevipnum == 1 then 
				tInfo.situser[j].state = g_douniuPlayStatus.status_banker
			end
		end
		if tInfo.situser[j].state == g_douniuPlayStatus.status_banker or tInfo.situser[j].state == g_douniuPlayStatus.status_buqiang then
			gcBankerOpt.userid:append(tInfo.situser[j].userid)
			gcBankerOpt.chairid:append(tInfo.situser[j].chairid)
			gcBankerOpt.bankermul:append(tInfo.situser[j].basicsmul)
		end
	end
	if tInfo.dntype ~= g_douNiuType.type_nnsz then
		local ban = DouNiuService.WhoBanker(qiangzhuangList,buqiangList,tInfo)
		tInfo.bankerID = ban.userid
	end
	
	if tInfo.dntype ~= g_douNiuType.type_nnsz then
 		for j = 1, #tInfo.situser do 
 			if tInfo.situser[j].state == g_douniuPlayStatus.status_banker or tInfo.situser[j].state == g_douniuPlayStatus.status_buqiang then
				if tInfo.situser[j].userid == tInfo.bankerID or #buqiangList == #tInfo.situser then
					gcBankerOpt.bankerstate:append(g_douniuPlayStatus.status_banker)
				else
					gcBankerOpt.bankerstate:append(tInfo.situser[j].state)
				end
			end
		end
	elseif tInfo.dntype == g_douNiuType.type_nnsz then
		local nnlist = {}
		local tmp = 0
		for k,v in ipairs(tInfo.situser) do
			if v.nntype >= 10 then
				if tmp == v.nntype then
					table.insert(nnlist,v.userid)
				elseif tmp < v.nntype then
					tmp = v.nntype 
					nnlist = {}
					table.insert(nnlist,v.userid)
				end
			end
		end
		local is = false
		for k,v in ipairs(tInfo.situser) do 
			if v.state == g_douniuPlayStatus.status_banker or v.state == g_douniuPlayStatus.status_buqiang then
				for z,y in ipairs(nnlist) do
					if v.userid == y then
						gcBankerOpt.bankerstate:append(g_douniuPlayStatus.status_banker)
						is = true
					end
				end
				if is == false then
					gcBankerOpt.bankerstate:append(v.state)
				else 
					is = false
				end
			end
		end
	end
	--这里是没人抢庄直接确定庄家的代码
	gcBankerOpt.bankeropt =  tInfo.bankerID 
	tInfo.state = g_douNiuDefine.state_wait   --进人下注状态
	tInfo.timemark = g_douNiuTime.wait_time + 1    --多加1秒做动画的时间
	
	local userIDList = DouNiuService.GetUserIDList(tInfo)
	gcBankerOpt.result  = 0
	SendMessage(userIDList, PacketCode[3040].client, gcBankerOpt:ByteSize(), gcBankerOpt:SerializeToString())


end


function DouNiuService.sendPour(tInfo)
	
	local gcmsg = msg_douniu_pb.gcdouniupouropt()
	
	if tInfo.dntype ~= g_douNiuType.type_tbnn then
		for i = 1, #tInfo.situser do
			gcmsg.userid:append(tInfo.situser[i].userid)
			addgcmsg = gcmsg.playerpoker:add()
			for k,v in ipairs(tInfo.situser[i].handpoker) do
				addgcmsg.handpoker:append(v)
			end
			if tInfo.situser[i].state == g_douniuPlayStatus.status_banker or tInfo.situser[i].state == g_douniuPlayStatus.status_buqiang then
				local min = 0
				for k,v in ipairs(tInfo.situser[i].pourList) do
					if v > 0 then
						if min == 0 then
							min = v
						end
						if min > v then
							min = v
						end
					end
				end
				tInfo.situser[i].jetton = min
				tInfo.situser[i].state = g_douniuPlayStatus.status_wait
				--记录超时次数
				tInfo.situser[i].overtimenum = tInfo.situser[i].overtimenum + 1
			end
			local pourbei = tInfo.situser[i].jetton
			if tInfo.situser[i].state == g_douniuPlayStatus.status_ready then
				pourbei = 0
			end
			gcmsg.pourbei:append(pourbei)
		end
		gcmsg.result = 0
		local userIDList =  DouNiuService.GetUserIDList(tInfo) 
		SendMessage(userIDList, PacketCode[3044].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	end

	tInfo.state = g_douNiuDefine.state_play 
	tInfo.timemark = g_douNiuTime.play_time
	if tInfo.dntype ~= g_douNiuType.type_mpqz then
		local gcDeal = msg_douniu_pb.gcdouniudeal()
		gcDeal.result = 0
		for i,j in ipairs(tInfo.situser)do 
			if j.state == g_douniuPlayStatus.status_wait then
				gcDeal.useridlist:append(j.userid)
				local addPoker = gcDeal.handpokerlist:add()
				for z,y in ipairs(j.handpoker) do
					addPoker.handpoker:append(y)
				end
			end
		end		
		local userIDList = DouNiuService.GetUserIDList(tInfo)
		SendMessage(userIDList, PacketCode[3022].client, gcDeal:ByteSize(), gcDeal:SerializeToString())
	end
	--给手牌排序
	DouNiuHuUtils.CradSort(tInfo)
end



