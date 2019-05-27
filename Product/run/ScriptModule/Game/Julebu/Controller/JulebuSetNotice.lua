
module("JulebuSetNotice", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebusetnotice()
	local gcmsg = msg_julebu_pb.gcjulebusetnotice()
	
	cgmsg:ParseFromString(buffer)
	
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
	
		
	if cgmsg.settype == 2 then
		--这里是修改名字的
		jInfo.name = cgmsg.notice
		JulebuModel.SetJulebuInfo(jInfo)
		local sqlCase = "update dy_julebu set name='"..cgmsg.notice.."' where id="..cgmsg.julebuid
		SqlServer.rpush(sqlCase)
	else
		--这就是让来设置公告就好了。
		jInfo.notice = cgmsg.notice
		JulebuModel.SetJulebuInfo(jInfo)
		local sqlCase = "update dy_julebu set notice='"..cgmsg.notice.."' where id="..cgmsg.julebuid
		SqlServer.rpush(sqlCase)
	end

	gcmsg.result = 0
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.notice = cgmsg.notice
	gcmsg.settype = cgmsg.settype
	
	local onLine = JulebuModel.GetOnlineList(cgmsg.julebuid)
	SendMessage(onLine, PacketCode[3237].client, gcmsg:ByteSize(), gcmsg:SerializeToString())  --统一发送给全部人
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end