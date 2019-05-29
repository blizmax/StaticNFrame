
--广东麻将创建桌子

module("GdmjSitDown", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjsitdown()
	local gcmsg = msg_gdmj_pb.gcgdmjsitdown()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjStandUp", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjStandUp", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if cgmsg.tableid == 0 then
		--如果tableID 是0，表示进入的是金币场，这边需要随机分配
		--暂时先返回
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if cgmsg.chairid <= 0 or cgmsg.chairid > 4 then
		gcmsg.result = ReturnCode["gdmj_sitdown_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	ThreadManager.GdmjLock(cgmsg.tableid)
	
	local tInfo = GdmjModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		--牌桌不存在
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_not_exist"]
		LogFile("error","GdmjEnter table not exist tableid="..cgmsg.tableid..",userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end


	--在这里需要判断钱够不够
	if tInfo.payway == g_gamePayWay.pay_AA then
		--房费均摊
		--
		if tInfo.paytype == g_gdmjDefine.pay_gold then
			--这个是钻石支付的
			--先看看掌上币够不够
			if pInfo.gold < tInfo.paynum and  pInfo.money < math.floor(tInfo.paynum/10) then
				--如果掌上币不够，在判断钻石够不够，钻石和掌上币对比的是1:10
				ThreadManager.GdmjUnLock(cgmsg.tableid)
				gcmsg.result = ReturnCode["gdmj_sit_fail"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		else
			if pInfo.money < tInfo.paynum  and pInfo.gold < tInfo.paynum*10 then
				--如果连钻石也不够了
				ThreadManager.GdmjUnLock(cgmsg.tableid)
				gcmsg.result = ReturnCode["gdmj_sit_fail"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end			
		end
		
	end

	--这里是行动的开始
	if tInfo.maxplayernum == 3 and cgmsg.chairid == 4 then
		--如果是三人玩法，但是玩家坐下的是第四个座位的时候
		cgmsg.chairid = 3
	end
	
	--防止出错
	if tInfo.situser[cgmsg.chairid] == nil then
		ThreadManager.GdmjUnLock(cgmsg.tableid)
		gcmsg.result = ReturnCode["gdmj_sitdown_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	if tInfo.situser[cgmsg.chairid] ~= 0 then
		if tInfo.situser[cgmsg.chairid] == cgmsg.userid then
			--已经坐下来了，重复发送的
			local mjUser = GdmjModel.GetChairUser(cgmsg.tableid, cgmsg.userid)
			if mjUser ~= nil then
				gcmsg.mjuser:ParseFromString(mjUser:SerializeToString())
			end
			gcmsg.result = 0
			ThreadManager.GdmjUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		else
			--该座位已经有别的玩家了
			--gcmsg.result = ReturnCode["gdmj_sitdown_error"]
			--ThreadManager.GdmjUnLock(cgmsg.tableid)
			--return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			--get他分配另外一个座位
			local isExist = false
			for i = 1,#tInfo.situser do
				if tInfo.situser[i] == 0 then
					cgmsg.chairid = i
					isExist = true
					break
				end
			end
			if isExist == false then
				--已经满人了
				ThreadManager.GdmjUnLock(cgmsg.tableid)
				gcmsg.result = ReturnCode["gdmj_sitdown_fail"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()			
			end
		end
	end
	
	PlayerModel.ChangeSimpleInfo(pInfo, gcmsg.mjuser.psinfo)
	gcmsg.mjuser.userid = cgmsg.userid
	GdmjModel.SitDown(tInfo, gcmsg.mjuser, cgmsg.chairid)  --玩家坐下来了
		
	local standLen = #tInfo.standuser
	for i = standLen,1,-1 do
		if cgmsg.userid == tInfo.standuser[i] then
			--确保把全部都要删除
			tInfo.standuser:remove(i)
		end
	end
	
	--在这里，同时也要把消息广播给其他人

	local userIDList = GdmjWork.GetUserList(tInfo)
	gcmsg.result = 0
	SendMessage(userIDList,PacketCode[2216].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
	
	--
	GdmjModel.SetTableInfo(tInfo,1)
	ThreadManager.GdmjUnLock(cgmsg.tableid)
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end