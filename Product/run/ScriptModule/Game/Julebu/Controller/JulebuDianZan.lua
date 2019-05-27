
module("JulebuDianZan", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebudianzan()
	local gcmsg = msg_julebu2_pb.gcjulebudianzan()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuDianZan", 0, "点赞，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local isAdmin = false
	
	for k,v in ipairs(jInfo.adminlist) do
		if v == cgmsg.userid then  --只有管理员才有权限修改公告
			isAdmin = true
			break
		end

	end
	
	if isAdmin == false then
		--改玩家已经是管理员了
		gcmsg.result = ReturnCode["jlb_set_error"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	local strGet = JulebuModel.GetDayHistory(cgmsg.julebuid, cgmsg.timemark)
	if strGet == nil then
		--gcmsg.zjday:ParseFromString(strGet)
		--点赞失败
		gcmsg.result = ReturnCode["jlb_dianzan_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end	
	
	local zjDay = st_julebu_pb.julebuzhanjiday()
	zjDay:ParseFromString(strGet)


	--这个只能一个一个找了
	for i = 1,#zjDay.mlist do
		if zjDay.mlist[i].userid == cgmsg.touserid then
			zjDay.mlist[i].dianzan = cgmsg.opttype
			gcmsg.dzinfo:ParseFromString(zjDay.mlist[i]:SerializeToString())
			break
		end
	end

	zjDay.timemark = cgmsg.timemark
	JulebuModel.SetDayHistory(cgmsg.julebuid, cgmsg.timemark, zjDay)
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end