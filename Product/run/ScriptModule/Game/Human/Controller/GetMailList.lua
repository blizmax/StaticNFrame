module("GetMailList", package.seeall)

--获取邮件列表
function execute(packetID, operateID, buffer)
	
	
	
	local cgmsg = msg_human_pb.cggetmaillist()
	local gcmsg = msg_human_pb.gcgetmaillist()

	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "mail", "GetMailList", 0, "重复获取邮件列表:optid="..operateID)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	cgmsg.pagenum = cgmsg.pagenum == 0 and 1 or cgmsg.pagenum
	
	local myList = redisItem:hkeys(MailModel.mail_time..cgmsg.userid,MailModel.redis_index)
	

	table.sort(myList, function(a,b) return tonumber(a) > tonumber(b) end)
	
	for i = 1,#myList do
		
		if i > (cgmsg.pagenum - 1)*10 then			
			local strGet = redisItem:hget(MailModel.mail_list..cgmsg.userid, myList[i], MailModel.redis_index)
			if strGet ~= nil then
				local addItem = gcmsg.mlist:add()
				addItem:ParseFromString(strGet)
			end
		end
		if i > cgmsg.pagenum*10 then
			break
		end
	end
	gcmsg.result = 0
	LogBehavior.Info(cgmsg.userid, "mail", "GetMailList", 0 , "获取邮件列表")
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end