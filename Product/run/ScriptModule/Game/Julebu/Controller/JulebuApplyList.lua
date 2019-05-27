
module("JulebuApplyList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuapplylist()
	local gcmsg = msg_julebu_pb.gcjulebuapplylist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuApplyList", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local appList = JulebuModel.GetApplyList(cgmsg.julebuid)
	if appList ~= nil then
		local count = 1
		for k,v in pairs(appList) do
			if count > (cgmsg.pagenum - 1)*10 and count <= cgmsg.pagenum*10 then
				local addItem = gcmsg.jalist:add()
				addItem:ParseFromString(v)
			end
			count = count + 1
			if count > cgmsg.pagenum*10 then
				break
			end
		end
	end
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end