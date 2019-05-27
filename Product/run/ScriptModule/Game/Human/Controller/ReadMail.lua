module("ReadMail", package.seeall)

--读取邮件
function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_human_pb.cgreadmail()
	local gcmsg = msg_human_pb.gcreadmail()
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid,"mail","ReadMail",0,"重复读取邮件，mailid="..cgmsg.mailid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	local mItem = MailModel.GetMailItem(cgmsg.userid, cgmsg.mailid)
	if mItem ~= nil then
			if mItem.mailstate == g_mailState.state_unreadadd  then
				mItem.mailstate = g_mailState.state_readadd
			elseif mItem.mailstate == g_mailState.state_unread then
				mItem.mailstate = g_mailState.state_read
			end
			gcmsg.mailstate = mItem.mailstate

			local sqlCase = "update dy_mail set mailstate="..mItem.mailstate.." where id="..mItem.mailid
			SqlServer.rpush(sqlCase)		
			MailModel.SetMailItem(cgmsg.userid, mItem)
	end

	gcmsg.result = 0
	gcmsg.mailid = cgmsg.mailid
	
	
	--MailModel.CheckNotice(cgmsg.userid)
	LogBehavior.Info(cgmsg.userid, "mail", "ReadMail", 0 , "读取邮件")
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end