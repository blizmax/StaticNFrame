
module("JulebuSetLimitJiFen", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebusetlimitjifen()
	local gcmsg = msg_julebu_pb.gcjulebusetlimitjifen()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuSetLimitJiFen", 0, "重复设置，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jPinfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jPinfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	local isExist = false
	for k,v in ipairs(jPinfo.adminlist) do
		if v == cgmsg.userid then
			isExist = true
			break
		end
	end
	if isExist == false then
		--只有管理员才能获取全部人的历史记录
		gcmsg.result = ReturnCode["jlb_is_admin2"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	if nil == JulebuModel.GetUserMemberInfo(cgmsg.julebuid, cgmsg.touserid, gcmsg.minfo) then
		gcmsg.minfo.id = 0
	else
		if cgmsg.limitjifen < 0 then
			if gcmsg.minfo.limitjifen <= 0 then
				gcmsg.result = ReturnCode["jlb__enter_jetton_error"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
			end
			
			--检查玩家是否在房间里
			if false == checkInGame(gcmsg.minfo.userid, cgmsg.julebuid) then
				gcmsg.result = ReturnCode["jlb__set_LimitJiFen_error"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
			end
			
			cgmsg.limitjifen = math.abs(cgmsg.limitjifen) > gcmsg.minfo.limitjifen
			and 0 - gcmsg.minfo.limitjifen or cgmsg.limitjifen
		end
		gcmsg.minfo.limitjifen = gcmsg.minfo.limitjifen + cgmsg.limitjifen 
		JulebuModel.AddToMemberList(cgmsg.julebuid, gcmsg.minfo)
		local sqlCase = "update dy_julebu_member set limitjifen="..gcmsg.minfo.limitjifen.." where julebuid="..cgmsg.julebuid.." and userid="..cgmsg.touserid
		mysqlItem:execute(sqlCase)
		local hInfo = st_julebu_pb.julebujimitjifeninfo()
		hInfo.executorid = cgmsg.userid
		hInfo.targetid = cgmsg.touserid
		hInfo.count = cgmsg.limitjifen
		hInfo.timemark = TimeUtils.GetTime()
		JulebuModel.SetLimitJiFenHistory(cgmsg.julebuid, hInfo)
		
		
		--同步到游戏里面
		SynchronizationToGame(gcmsg.minfo.userid, gcmsg.minfo.limitjifen, cgmsg.julebuid)
		
		LogBehavior.JulebuInfo(cgmsg.touserid, "julebu="..cgmsg.julebuid, "set add jifen", cgmsg.limitjifen, "add jifen="..cgmsg.limitjifen, gcmsg.minfo.limitjifen)
	end
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end


function checkInGame(userid, julebuid)
	
	local tableID = DouNiuModel.GetUserTableID(userid)
	if tableID ~= nil then
		local tInfo = DouNiuModel.GetTableInfo(tableID)
		if tInfo ~= nil then
			if tInfo.julebuid == julebuid and tInfo.julebutype == 2 then
				return false
			end
		end
	end
	
	--拼三张
	local tableID = PszModel.GetUserTableID(userid)
	if tableID ~= nil then
		local tInfo =PszModel.GetTableInfo(tableID)
		if tInfo ~= nil then
			if tInfo.julebuid == julebuid and tInfo.julebutype == 2 then
				return false
			end
		end
	end
	return true
end

function SynchronizationToGame(userid, limitjifen, julebuid)

	--查看在不在牛牛的桌子里面
	local tableID = DouNiuModel.GetUserTableID(userid)
	if tableID ~= 0 and tableID ~= nil then
		ThreadManager.DouNiuLock(tableID) 
		local tInfo = DouNiuModel.GetTableInfo(tableID)
		if tInfo == nil then
			return 
		end
		if tInfo.julebuid == julebuid and tInfo.julebutype == 2 then
			for i = 1, #tInfo.situser do
				if tInfo.situser[i].userid == userid then
					tInfo.situser[i].carryjetton = limitjifen + tInfo.situser[i].countjetton
					break
				end
			end
		end
		DouNiuModel.SetTableInfo(tInfo,1)
		ThreadManager.DouNiuUnLock(tableID)	
		return 
	end
	
	--查看在不在拼三张的桌子里面
	local tableID = PszModel.GetUserTableID(userid)
	if tableID ~= 0 and tableID ~= nil then
		ThreadManager.PszLock(tableID) 
		local tInfo = PszModel.GetTableInfo(tableID)
		if tInfo == nil then
			return 
		end
		if tInfo.julebuid == julebuid and tInfo.julebutype == 2 then
			for i = 1, #tInfo.situser do
				if tInfo.situser[i].userid == userid then
					tInfo.situser[i].jifen = limitjifen + tInfo.situser[i].jifen
					break
				end
			end
		end
		PszModel.SetTableInfo(tInfo,1)
		ThreadManager.PszUnLock(tableID)	
		return
	end
	
end