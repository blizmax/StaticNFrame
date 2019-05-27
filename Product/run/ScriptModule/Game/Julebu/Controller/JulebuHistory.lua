
module("JulebuHistory", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuhistory()
	local gcmsg = msg_julebu_pb.gcjulebuhistory()

	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuHistory", 0, "重复获取，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	

	local jPinfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	
	--[[local isExist = false
	for k,v in ipairs(jPinfo.adminlist) do
		if v == cgmsg.userid then
			isExist = true
			break
		end
	end
	if isExist == false then
		--只有管理员才能获取全部人的历史记录
		gcmsg.result = ReturnCode["jlb_is_admin"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end]]
	
	local getList = JulebuModel.GetHistory(cgmsg.julebuid)
	
	for k,v in ipairs(getList) do
		local addItem = gcmsg.zjlist:add()
		addItem:ParseFromString(v)
	end
	
	gcmsg.result = 0   --这里就取到了战绩列表
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end