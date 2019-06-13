--广东麻将创建桌子

module("GdmjReady", package.seeall)


function execute(packetID, operateID, buffer)
	local cgmsg = msg_gdmj_pb.cggdmjready()
	local gcmsg = msg_gdmj_pb.gcgdmjready()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "Gdmj", "GdmjReady", 0, "缓存已经存在")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	

	
	ThreadManager.GdmjLock(cgmsg.tableid)
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogBehavior.Warning(cgmsg.userid,"Gdmj", "GdmjReady", 0, "牌桌不存,tableID="..cgmsg.tableid)
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	
	if nil == GdmjModel.GetChairUser(cgmsg.tableid, cgmsg.userid, gcmsg.mjuser) then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.mjuser.userid = 0
		gcmsg.result = 0
		gcmsg.result = ReturnCode["gdmj_player_not"]
		LogBehavior.Warning(cgmsg.userid,"Gdmj", "GdmjReady", 0, "玩家不在牌桌中,tableID="..cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--这里还需要看筹码是否足够
	--如果筹码不足，就自动调整
	
	if tInfo.viptable == 0 then
		local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
		--这个是金币场的，需要检查金币是否足够
		--if 
		local tableType = 0
		local isLeave = false
		if pInfo.jetton < 2000 then
			--让他离开这个牌桌
			GdmjJinBiChang.Leave(tInfo, cgmsg.userid)
			--只有这里需要离开的
			local gcLeave = msg_gdmj_pb.gcgdmjleave()
			gcLeave.userid = cgmsg.userid
			gcLeave.leavetype = 1
			gcLeave.leavemsg = "金币不足！！！"
			gcLeave.result = 0
			gcLeave.tableid = cgmsg.tableid
			SendMessage(cgmsg.userid, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
			--这里就不用返回了
			GdmjModel.SetTableInfo(tInfo, 1)
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			
			ThreadManager.GdmjLock(tInfo.mjtype..tInfo.tabletype)  --金币场的加锁	
			GdmjModel.SetJBCTableID(tInfo.mjtype, tInfo.tabletype, tInfo.tableid, tInfo.playernum)  --在这里设置参数	
			ThreadManager.GdmjUnLock(tInfo.mjtype..tInfo.tabletype)			
			return cgmsg.userid, 0, 0, gcmsg:SerializeToString()	
		elseif pInfo.jetton < 20000 then
			if tInfo.tabletype > g_gdmjTableType["junior"] then
				--2W只能进初级场
				--如果不是初级场，就只能先离开了
				isLeave = true
				tableType = g_gdmjTableType["junior"]
			end
		elseif pInfo.jetton < 100000 then
			--不到10W不能进高级场
			if tInfo.tabletype > g_gdmjTableType["medium"] then
				isLeave = true
				tableType = g_gdmjTableType["medium"]
			end
		end

		if isLeave == true then
			
			--先离开这个房间
			GdmjJinBiChang.Leave(tInfo, pInfo.userid)
			--再进入一个新的房间
			--同时设置这个房间的房间号码
			ThreadManager.GdmjLock(tInfo.mjtype..tInfo.tabletype)  --金币场的加锁	
			GdmjModel.SetJBCTableID(tInfo.mjtype, tInfo.tabletype, tInfo.tableid, tInfo.playernum)  --在这里设置参数	
			ThreadManager.GdmjUnLock(tInfo.mjtype..tInfo.tabletype)
			local gcEnter = msg_gdmj_pb.gcgdmjenter()
			
			
			if true == GdmjJinBiChang.Enter(pInfo, gcEnter.mjinfo, tInfo.mjtype, tableType) then

				for i = 1,#gcEnter.mjinfo.situser do
					if gcEnter.mjinfo.situser[i] ~= 0 then
						local mjUser = gcEnter.userlist:add()
						GdmjModel.GetChairUser(gcEnter.mjinfo.tableid, gcEnter.mjinfo.situser[i], mjUser)
					end
				end			
				gcEnter.result = 0
			end
			GdmjModel.SetTableInfo(tInfo, 1)
			
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			SendMessage(pInfo.userid, PacketCode[2202].client, gcEnter:ByteSize(), gcEnter:SerializeToString())
			return cgmsg.userid, 0, 0, gcmsg:SerializeToString()  --就不用发送准备的协议了
		end
		
	end
	
	gcmsg.mjuser.playstate = g_gdmjPlayerState.play_waiting
	
	GdmjModel.SetChairUser(cgmsg.tableid, gcmsg.mjuser)
	GdmjModel.SetTableInfo(tInfo, 1)

	ThreadManager.GdmjUnLock(cgmsg.tableid)
	gcmsg.result = 0
	local userIDList = GdmjWork.GetUserList(tInfo, cgmsg.userid)
	
	--发送给其他玩家
	SendMessage(userIDList, PacketCode[2214].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()	
end