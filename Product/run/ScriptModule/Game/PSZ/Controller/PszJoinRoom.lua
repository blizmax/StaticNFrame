module("PszJoinRoom", package.seeall)

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_psz_pb.cgpszjoinroom()
	local gcmsg = msg_psz_pb.gcpszjoinroom()
	
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszJoinRoom", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"psz","PszJoinRoom", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local isJion = false
	local tableID = PszModel.GetUserTableID(cgmsg.userid)
	if tableID == nil then	
		--进入私人场
		cgmsg.tabletype = g_PszDefine.table_srf
		if cgmsg.tabletype == g_PszDefine.table_srf then
				tableID = cgmsg.tableid	
		--进人金币场
		elseif cgmsg.tabletype ~= g_PszDefine.table_srf then
			tableID = jettonfield(cgmsg.tabletype)
		end
		--获取房间号失败
		if tableID == 0 or tableID == nil then
			--tableID = BrnnService.GetTableID(pInfo.jetton)
			gcmsg.result = ReturnCode["psz_not_exist"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
	end
	
	ThreadManager.PszLock(tableID) 
	local tInfo = PszModel.GetTableInfo(tableID)
	
	if tInfo == nil then
		LogBehavior.Error(cgmsg.userid, "psz", "PszJoinRoom", ReturnCode["psz_not_exist"], "牌桌不存在，tableid="..tableID)
		PszModel.DelUserTableID(cgmsg.userid)
		gcmsg.result = ReturnCode["psz_not_exist"]
		ThreadManager.PszUnLock(tableID)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end 
	
	if tInfo.julebuid ~= 0 then
		--这里需要检查俱乐部的成员关系
		if false == JulebuService.IsJoinIn(cgmsg.userid, tInfo.julebuid) then
			gcmsg.result = ReturnCode["jlb_game_enter_error"]
			ThreadManager.PszUnLock(tableID)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
		if tInfo.julebutype == 2 then
			if false == PszEvent.CheckJiFen(tInfo.julebuid, cgmsg.userid) then
				--低于50就不能进入房间了
				gcmsg.result = ReturnCode["jlb_enter_error"]
				ThreadManager.PszUnLock(tableID)
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
			end
		end
	end	

	--查看是否在座位上
	
	local isJion = false
	local isSit = false
	local isStan = false
	--在座位上的玩家
	for k, v in ipairs(tInfo.situser) do
		if cgmsg.userid == v.userid then
			isJion = true
			isSit = true
		end
	end
	--站立的玩家
	for k, v in ipairs(tInfo.standuser) do
		if cgmsg.userid == v then
			isJion = true 
			isStan = true
			if tInfo.julebuid ~= 0 then
				--在自动准备的情况下
				if tInfo.autoready == g_PszDefine.autoready_OK  then
					----把玩家加入坐下玩家列表
					PszSitdown.UserSitdown(tInfo, pInfo)
					tInfo.usernum = tInfo.usernum  + 1
				end		
				--进入房间默认自动准备
				for k,v in ipairs(tInfo.situser) do 
					if cgmsg.userid == v.userid then
						local gcmsgNext = msg_psz_pb.gcpsznext()
						gcmsgNext.chairid = v.chairid
						gcmsgNext.userid = cgmsg.userid
						gcmsgNext.result = 0
						SendMessage(cgmsg.userid, PacketCode[3326].client, gcmsgNext:ByteSize(), gcmsgNext:SerializeToString())
						break
					end
				end
			end
		end
	end
		
	--不在房间的情况
	if isJion == false then
		--aa查看钻石够不够
		if false == PszService.Pszcheck(tInfo, pInfo) then 
			gcmsg.result = ReturnCode["psz_money_short"]
			ThreadManager.PszUnLock(tableID)
			return cgmsg.userid, 0, cgmsg:ByteSize(),gcmsg:SerializeToString()
		end
		
		--房间满人的情况
		if #tInfo.situser  >= tInfo.maxuser then
			ThreadManager.PszUnLock(tableID)
			gcmsg.result = ReturnCode["psz_full"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
		--房间没满人的情况
		elseif #tInfo.situser < tInfo.maxuser then
			--游戏已经开始了的情况
			if tInfo.usevipnum > 0 then
				--游戏开始后不能进入房间
				if tInfo.restrict == g_PszDefine.restrict_NO then
					gcmsg.result = ReturnCode["psz_room_close"]
					ThreadManager.PszUnLock(tableID)
					return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
				--游戏开始后能进入房间
				elseif tInfo.restrict == g_PszDefine.restrict_OK then
					--游戏开始后坐下玩家+站立玩家 满人的情况
					if #tInfo.situser + #tInfo.standuser >= tInfo.maxuser then
						ThreadManager.PszUnLock(tableID)
						gcmsg.result = ReturnCode["psz_full"]
						return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
					end
	
					--没确定进入房间且是AA付费	
					if tInfo.tabletype == g_PszDefine.table_srf then
						if cgmsg.resolute == 0 then	
							local gcmsgtmp = msg_psz_pb.gcpszroominfo()
							gcmsgtmp.tableid = cgmsg.tableid
							local tmp = tInfo.maxplaynum - tInfo.usevipnum
							moneytmp = math.ceil(tmp / 5)
							if tInfo.payway == g_PszDefine.payment_owner then
								gcmsgtmp.msg = "当前房间还剩"..tmp.."局"
							else
								gcmsgtmp.msg = "当前房间还剩"..tmp.."局，需要消耗"..tInfo.paynum.."个钻石"
							end
							gcmsgtmp.result = 0

							ThreadManager.PszUnLock(tableID)
							SendMessage(cgmsg.userid, PacketCode[3332].client, gcmsgtmp:ByteSize(), gcmsgtmp:SerializeToString())
							return cgmsg.userid, 0, 0, gcmsgtmp:SerializeToString()			
						end
					end
					--把玩家加入站立列表
					tInfo.standuser:append(cgmsg.userid)
					PszModel.SetUserTableID(cgmsg.userid, tInfo.tableid)

				end
			--游戏还没开始的情况	
			elseif	tInfo.usevipnum == 0 then
				tInfo.standuser:append(cgmsg.userid)
				PszModel.SetUserTableID(cgmsg.userid, tInfo.tableid)
			end	
		end
	end
	
	
	--把牌桌信息发给玩家
	for k,v in ipairs(tInfo.situser)  do	
		if v ~= 0 then
			local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
			local addItem = gcmsg.pslist:add()
			PlayerModel.ChangeSimpleInfo(tempInfo, addItem)
			addItem.jifen = v.jifen
		end
	end
		
	PszModel.SetUserTableID(cgmsg.userid, tInfo.tableid)
	
	gcmsg.hinfo:ParseFromString(tInfo:SerializeToString())
	if tInfo.usevipnum > 0 then
		if tInfo.state ~= g_PszDefine.state_count and tInfo.state ~= g_PszDefine.state_next then
			for i = 1, #gcmsg.hinfo.situser do	
				if gcmsg.hinfo.situser[i].pokerstate == g_PszPokerStatus.poker_lookno then
					gcmsg.hinfo.situser[i].psztype = 0
					while #gcmsg.hinfo.situser[i].handpoker > 0 do
						gcmsg.hinfo.situser[i].handpoker:remove(1) 
					end
				end
			end
		end
	end
	
	gcmsg.result = 0	
	
	SendMessage(cgmsg.userid, PacketCode[3310].client, gcmsg:ByteSize(), gcmsg:SerializeToString())

	--在自动准备的情况下
	if tInfo.autoready == g_PszDefine.autoready_OK and isJion == false then
		if cgmsg.resolute == 0 then	
			--把玩家加入坐下玩家列表
			tInfo.usernum = tInfo.usernum + 1
			PszSitdown.UserSitdown(tInfo, pInfo)
			
			--进入房间默认自动准备
			local gcmsgNext = msg_psz_pb.gcpsznext()
			--游戏没开始
			if tInfo.usevipnum == 0 then
				for k,v in ipairs(tInfo.situser) do
					gcmsgNext.chairid = v.chairid
					gcmsgNext.userid = v.userid
					gcmsgNext.result = 0
					local userList = PszService.GetUserIDList(tInfo)
					SendMessage(userList, PacketCode[3326].client, gcmsgNext:ByteSize(), gcmsgNext:SerializeToString())
				end
			else
				if isSit == false then
					for k,v in ipairs(tInfo.situser) do 
						if cgmsg.userid == v.userid then
							gcmsgNext.chairid = v.chairid
							gcmsgNext.userid = cgmsg.userid
							gcmsgNext.result = 0
							local userList = PszService.GetUserIDList(tInfo)
							SendMessage(userList, PacketCode[3326].client, gcmsgNext:ByteSize(), gcmsgNext:SerializeToString())
							break
						end
					end	
				end
			end
		elseif cgmsg.resolute == 1 then	
			local gcmsgDn = msg_psz_2_pb.gcpszdynamicjion()
			gcmsgDn.result = 0
			gcmsgDn.userid = cgmsg.userid
			SendMessage(cgmsg.userid, PacketCode[3342].client, gcmsgDn:ByteSize(), gcmsgDn:SerializeToString())
		end
	end	
	PszModel.SetTableInfo(tInfo,1)
	ThreadManager.PszUnLock(tableID)
	return cgmsg.userid, 0, 0, gcmsg:SerializeToString()	
end

--获得金币场房号
function jettonfield(tableType, userid)
	
	local tmpList = {}
	local tableid = 0
	 
	--获得金币场有空位的列表
	tmpList = PszModel.GetJettonTableList(tableType)
	
	--把挑出有空位的房间
	for i = 1, #tmpList do
		ThreadManager.PszLock(tmpList[i]) 
		local tInfo = PszModel.GetTableInfo(tmpList[i])
		--如果房间为nil就删掉
		if tInfo == nil or tInfo == 0 then
			PszModel.DelTableInfo(tmpList[i])
			PszModel.DelJettonTableList(tableType,tmpList[i])
			return
		end
		if #tInfo.situser + #tInfo.standuser < tInfo.maxuser then	
			tableid = tmpList[i]
		end
		ThreadManager.PszUnLock(tmpList[i]) 
	end
	
	--如果，就重新创建一个
	if tableid == 0 then
		tableid = createJetton(tableType)
		PszModel.SetJettonTableList(tableType,tableid)
	end

	return tableid
end

function createJetton(tableType)
	local dizhu = 0
	
	if tableType == g_PszDefine.table_elementary then
		dizhu = 100
	elseif tableType == g_PszDefine.table_middle then
		dizhu = 500
	elseif tableType == g_PszDefine.table_jadvanced then
		dizhu = 1000
	end

	
	local tInfo = st_psz_pb.pszinfo()
	
	tInfo.tableid = PszModel.GetRandomID() 
	ThreadManager.PszLock(tInfo.tableid)
	
	
	tInfo.maxuser = g_PszDefine.max_user
	tInfo.paytype = 0
	tInfo.difen = dizhu
	tInfo.state =  g_PszDefine.state_ready
	tInfo.timemark = g_PszTime.ready_time
	tInfo.maxplaynum = 1000  --玩的局数
	tInfo.payway = g_PszDefine.payment_owner
	tInfo.paynum = 0
	tInfo.doublecomp = g_PszDefine.doublecomp_two
	tInfo.boutcapp = g_PszDefine.boutcapp_15
	tInfo.mincomp = g_PszDefine.mincomp_2
	tInfo.menpai = g_PszDefine.menpai_0
	
	tInfo.usernum = 0
	tInfo.tabletype = tableType   --金币场

	tInfo.restrict = g_PszDefine.restrict_OK     --限制，游戏开始后能不能进入房间
	tInfo.autoready = g_PszDefine.autoready_OK
	
	PszModel.SetTableInfo(tInfo,1)
	ThreadManager.PszUnLock(tInfo.tableid)
	return tInfo.tableid
end
