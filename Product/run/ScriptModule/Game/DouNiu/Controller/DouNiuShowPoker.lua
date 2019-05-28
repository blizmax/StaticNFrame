module("DouNiuShowPoker", package.seeall)   

--这为亮牌操作
--只有在亮牌阶段才能才能亮牌
--普通玩家亮牌会马上发给其他我玩家
--庄家在其他玩家没有全部亮牌之前，只显示完成，不亮牌

function execute(packetID, operateID, buffer)


	local cgmsg = msg_douniu_pb.cgdouniushowpoker()
	local gcmsg = msg_douniu_pb.gcdouniushowpoker()
	
	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)

	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNidPour", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuShowPoket", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.tableid == 0 then
		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		gcmsg.result = ReturnCode["douniu_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	ThreadManager.DouNiuLock(cgmsg.tableid)
	
	local tInfo = DouNiuModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_not_exist"]
		LogFile("error","DouNiuPour table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end


	--这里是行动的开始
	if tInfo.state ~= g_douNiuDefine.state_play then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuPour douniu_not_state tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local isexist = false
	local isbanker = false 
	for k, v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			isexist = true
			if v.state == g_douniuPlayStatus.status_wait then
				isbanker = true 
			end
			break
		end
	end
	
	if isexist ==false then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["douniu_opt_error"]
		LogFile("error","DouNiuPour table not_sit tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if isbanker == false then
		ThreadManager.DouNiuUnLock(cgmsg.tableid)
		gcmsg.result = 0
		--LogFile("error","DouNiuDissolve table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	gcmsg.userid:append(cgmsg.userid)
	gcmsg.complete = cgmsg.userid
	
	for i = 1, #tInfo.situser do 
		if cgmsg.userid == tInfo.situser[i].userid then		
			gcmsg.chairid:append(tInfo.situser[i].chairid)
			--if cgmsg.userid ~= tInfo.bankerID then
				gcmsg.nntype:append(tInfo.situser[i].nntype) 
				if tInfo.situser[i].endcard ~= nil then
					gcmsg.endcardlist:append(tInfo.situser[i].endcard) 
				end
				local addItem = gcmsg.handpoker:add()
				for j = 1, 5 do
					addItem.handpoker:append(tInfo.situser[i].handpoker[j])
				end
			--end
			tInfo.situser[i].state = g_douniuPlayStatus.status_play 
			--有操作就把超时次数置 0
			tInfo.situser[i].overtimenum = 0
			--有操作就唤醒不托管
			if tInfo.situser[i].trusteeship == g_douNiuDefine.trusteeship_yes then
				tInfo.situser[i].trusteeship = g_douNiuDefine.trusteeship_no 
				local gcmsg = msg_douniu_2_pb.gcdouniutrusteeship()
				gcmsg.userid = tInfo.situser[i].userid
				gcmsg.trusteeship = g_douNiuDefine.trusteeship_no 
				gcmsg.result = 0
				
				local userIDList =  DouNiuService.GetUserIDList(tInfo)
				SendMessage(userIDList, PacketCode[3054].client, gcmsg:ByteSize(), gcmsg:SerializeToString())				
			end
		end			
	end
		
	local userIDList = DouNiuModel.GetUserList(tInfo)
	gcmsg.result = 0
	SendMessage(userIDList,PacketCode[3024].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
	
	DouNiuModel.SetTableInfo(tInfo, 1)
	ThreadManager.DouNiuUnLock(cgmsg.tableid)
	
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end