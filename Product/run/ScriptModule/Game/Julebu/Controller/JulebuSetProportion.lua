module("JulebuSetProportion",package.seeall)


function execute(packetID, operateID, buffer)
	local cgmsg = msg_julebu2_pb.cgsetjulebuproportion()
	local gcmsg = msg_julebu2_pb.gcsetjulebuproportion()
	
	cgmsg:ParseFromString(buffer)
	print(cgmsg)
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	
	--检查这个ID是否是管理员
	local isAdmin = false
	for k,v in ipairs(jInfo.adminlist) do
		if v == cgmsg.userid then  --只有管理员才有权限修改公告
			isAdmin = true
			break
		end
	end
	
	if isAdmin == false then
		gcmsg.result = ReturnCode["jlb_set_error"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
		
	if cgmsg.optype == 0 then
		--这里是查询的
		gcmsg.proportion = jInfo.proportion or 0
		if not jInfo.proportion then
			jInfo.proportion = 0
			JulebuModel.SetJulebuInfo(jInfo)
		end
	elseif cgmsg.optype == 1 then
		--这里是修改比例
		gcmsg.proportion = cgmsg.proportion
		jInfo.proportion = cgmsg.proportion
		JulebuModel.SetJulebuInfo(jInfo)
	end
	print(jInfo.proportion)
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.optype = cgmsg.optype
	print(gcmsg)
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end