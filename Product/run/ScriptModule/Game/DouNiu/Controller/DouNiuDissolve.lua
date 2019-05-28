module("DouNiuDissolve", package.seeall)

--该操作是解散房间的操作
--如果是牌局还没开始，就只有房主能够解散。房间直接删除
--如果牌局已经开始，坐下玩的四个人能够申请解散。其他三个人同意了，就能够解散

function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_douniu_pb.cgdouniudissolve()
	local gcmsg = msg_douniu_pb.gcdouniudissolve()
	
	cgmsg:ParseFromString(buffer)    --艹，差点忘了这个
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "SDouNiuDissolve", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuCreate", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid) 
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	--在申请解散房间状态不能重复申请解散
	if tInfo.state == g_douNiuDefine.state_dissolve then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_opt_error"]
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	if tInfo.viptable == 0 then
		
		--如果是非VIP房间,不存在解散的概念，直接返回
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_dissolve_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	if tInfo.usevipnum == 0 then
		--如果牌局还没有开始，这个时候，只有房主才能解散房间	
		if cgmsg.userid ~= tInfo.ownerid then
			--如果这个情况下，不是房主，不能够解散房间
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			gcmsg.result = ReturnCode["douniu_dissolve_fail2"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
			
		else
			--如果是房主
			--
			--首先需要给其他人下发离开的协议,不管是在牌桌中的还是在观看列表中的
			--在这里是的需要返回钻石的。
			
			local gcLeave = msg_douniu_pb.gcdouniuleave()
			gcLeave.result = 0
			gcLeave.leavemsg = "房主已经解散房间!!!"
			
			for k,v in ipairs(tInfo.standuser) do   --这个是一个int list
				if v ~= cgmsg.userid then  
					--房主的另外发
					gcLeave.userid = v
					SendMessage(v, PacketCode[3012].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				end
			end
			
			for k,v in ipairs(tInfo.situser) do
				if v.userid ~= 0 and v.userid ~= cgmsg.userid then
					--这个是坐在座位上的
					gcLeave.userid = v.userid
					gcLeave.chairid = v.chairid
					SendMessage(v.userid, PacketCode[3012].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				end
			end
				
			tInfo.state = g_douNiuDefine.state_delete
			--房主付费：游戏还没开始，解散房间，退还钻石
			if tInfo.payway == 1 then 
				DouNiuService.FeeReturns(pInfo,tInfo.paynum, tInfo.paytype)
			end
			
			DouNiuModel.DelUserTableList(cgmsg.userid, tInfo.tableid)
			--把所有人提出房间
			local userIDList = DouNiuService.GetUserIDList(tInfo)
			for k,v in ipairs(userIDList) do
				DouNiuModel.DelUserTableID(v)	--房间解散时把自己所在房间的记录
			end 
			gcLeave.leavemsg = ""   --对于房主来说，就是解散了
			gcLeave.userid = cgmsg.userid
			SendMessage(cgmsg.userid, PacketCode[3012].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
		end
	else --如果牌局已经开始了，就是申请解散房间了
		--首先检查，是不是在牌桌中坐下的玩家
		local numIndex = 0
		for k,v in ipairs(tInfo.situser) do
			if cgmsg.userid == v.userid then
				numIndex = k
			end
		end
		
		if numIndex == 0 then
			
			ThreadManager.DouNiuUnLock(cgmsg.tableid)
			gcmsg.result = ReturnCode["douniu_dissolve_fail3"] 
			return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		else
			--如果是坐在上面的玩家
			local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
			
			gcmsg.msg = "玩家["..pInfo.nickname.."]申请解散房间，请问是否同意？(超过1分钟未做选择，则默认同意)"
			tInfo.straction = "玩家["..pInfo.nickname.."]申请解散房间，请问是否同意？(超过1分钟未做选择，则默认同意)"
			--print("tInfo.dissolveinfo1111 = "..tInfo.dissolveinfo)
			--print("tInfo.usermaxnum = "..tInfo.usermaxnum)
			--print("tInfo.situser[numIndex].chairid = "..tInfo.situser[numIndex].chairid)
			--print("numIndex = "..numIndex)
			--print("math.pow(10,tInfo.usermaxnum) = "..math.pow(10,tInfo.usermaxnum))
			--print("math.pow(10, tInfo.usermaxnum - tInfo.situser[numIndex].chairid ) = "..math.pow(10, tInfo.usermaxnum - tInfo.situser[numIndex].chairid ))
			
			local num = math.pow(10,tInfo.usermaxnum) + math.pow(10, tInfo.usermaxnum - tInfo.situser[numIndex].chairid )  --设置对应的标识 ，这里就需要注意了。
			gcmsg.dissolveinfo = tostring(num)
			--print("gcmsg.dissolveinfo = "..gcmsg.dissolveinfo)
			--print("numIndex = "..numIndex)
			--print("tInfo.situser[numIndex].chairid = "..tInfo.situser[numIndex].chairid)
			tInfo.tablemark = tInfo.state   --暂时保存状态
			tInfo.tabletime = tInfo.timemark
			tInfo.dissolveinfo = gcmsg.dissolveinfo
			tInfo.timemark = g_douNiuTime.dissolve_time
			tInfo.state =g_douNiuDefine.state_dissolve
			gcmsg.timemark = g_douNiuTime.dissolve_time
			gcmsg.disolvetype = 1
			local userIDList =  DouNiuService.GetUserIDList(tInfo) 
			SendMessage(userIDList, PacketCode[3006].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
			
			if #tInfo.situser == 1 then
				tInfo.state = g_douNiuDefine.state_dissolve
				tInfo.timemark = 0
			end
			
		end
	end
	--DouNiuModel.DelUserTableList(cgmsg.userid,tInfo.tableid)
	DouNiuModel.SetTableInfo(tInfo,1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)  --解锁，搞定
	--print(gcmsg)
	return cgmsg.userid,0,0,gcmsg:SerializeToString()
end