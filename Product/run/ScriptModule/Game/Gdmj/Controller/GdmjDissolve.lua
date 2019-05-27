module("GdmjDissolve", package.seeall)

--该操作是解散房间的操作
--如果是牌局还没开始，就只有房主能够解散。房间直接删除
--如果牌局已经开始，坐下玩的四个人能够申请解散。其他三个人同意了，就能够解散

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_gdmj_pb.cggdmjdissolve()
	local gcmsg = msg_gdmj_pb.gcgdmjdissolve()
	
	cgmsg:ParseFromString(buffer)    --艹，差点忘了这个
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjDissolve", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	ThreadManager.GdmjLock(cgmsg.tableid)
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		--牌桌不存在
		if cgmsg.tableid ~= 0 then
			local gcLeave = msg_gdmj_pb.gcgdmjleave()
			gcLeave.result = 0
			--房主的另外发
			gcLeave.userid = cgmsg.userid
			gcLeave.tableid = cgmsg.tableid
			SendMessage(cgmsg.userid, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
			GdmjModel.DelUserTableID(cgmsg.userid)
		end
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogFile("error","GdmjDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	--在这里检查牌桌的状态
	--
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjDissolve", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	if tInfo.viptable == 0 then
		
		--如果是非VIP房间,不存在解散的概念，直接返回
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_dissolve_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	if tInfo.usevipnum == 0 then
		--如果牌局还没有开始，这个时候，只有房主才能解散房间
		if cgmsg.userid ~= tInfo.tableuserid then
			--如果这个情况下，不是房主，不能够解散房间
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			gcmsg.result = ReturnCode["gdmj_dissolve_fail2"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
			
		else
			--如果是房主
			--
			--首先需要给其他人下发离开的协议,不管是在牌桌中的还是在观看列表中的
			
			local gcLeave = msg_gdmj_pb.gcgdmjleave()
			gcLeave.result = 0
			gcLeave.leavemsg = "房主已经解散房间!!!"
			gcLeave.tableid = cgmsg.tableid
			for k,v in ipairs(tInfo.standuser) do
				if v ~= cgmsg.userid then  
					--房主的另外发
					gcLeave.userid = v
					SendMessage(v, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				end
			end
			
			for k,v in ipairs(tInfo.situser) do
				if v ~= 0 and v ~= cgmsg.userid then
					--这个是坐在座位上的
					gcLeave.userid = v
					gcLeave.chairid = k
					SendMessage(v, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				end
			end
			tInfo.status = g_gdmjStatus.status_delete
			GdmjModel.SetTableInfo(tInfo,1)
			
			gcLeave.leavemsg = ""   --对于房主来说，就是解散了
			gcLeave.userid = cgmsg.userid
			SendMessage(cgmsg.userid, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
			
			--这里开始返还钻石或者掌上币
			
			if tInfo.payway == 1 then
				--房主支付的
				if tInfo.paytype == g_gdmjDefine.pay_gold then
					PlayerModel.AddGold(pInfo, tInfo.paynum, "gdmj", "dissolve gold")
					PlayerModel.SendPlayerInfo(pInfo, {"gold"})
				else
					PlayerModel.AddMoney(pInfo, tInfo.paynum, "gdmj", "dissolve money")
					PlayerModel.SendPlayerInfo(pInfo, {"money"})
				end
				PlayerModel.SetPlayerInfo(pInfo)
			end
			
		end
	else --如果牌局已经开始了，就是申请解散房间了
		--首先检查，是不是在牌桌中坐下的玩家
		local chairID = 0
		for k,v in ipairs(tInfo.situser) do
			if cgmsg.userid == v then
				chairID = k
			end
		end
		
		if chairID == 0 then
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			gcmsg.result = ReturnCode["gdmj_dissolve_fail3"]
			return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		else
			--如果是坐在上面的玩家
			
			gcmsg.msg = "玩家["..pInfo.nickname.."]申请解散房间，请问是否同意？(超过5分钟未做选择，则默认同意)"
			
			gcmsg.dissolveinfo = 10000 + math.pow(10,4-chairID)  --设置对应的标识
			tInfo.dissolveinfo = gcmsg.dissolveinfo
			tInfo.tablemark = tInfo.status   --暂时保存状态

			tInfo.timemark = g_gdmjTime.dissolve_time
			tInfo.status = g_gdmjStatus.status_dissolve
			gcmsg.timemark = g_gdmjTime.dissolve_time
			local userList = GdmjWork.GetUserList(tInfo)
			
			GdmjModel.SetTableInfo(tInfo,1)
			SendMessage(userList, PacketCode[2224].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		end
	end
	
	
	ThreadManager.GdmjUnLock(cgmsg.tableid)  --解锁，搞定
	return cgmsg.userid,0,0,gcmsg:SerializeToString()
end