
module("JulebuSelfJiFen", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuselfjifen()
	local gcmsg = msg_julebu_pb.gcjulebuselfjifen()
	
	cgmsg:ParseFromString(buffer)
	
	
	if nil == JulebuModel.GetUserMemberInfo(cgmsg.julebuid, cgmsg.userid, gcmsg.minfo) then
		gcmsg.minfo.id = 0
	else
		gcmsg.minfo.jifen = JulebuModel.GetUserJiFen(cgmsg.julebuid, cgmsg.userid)
		gcmsg.minfo.state = OnlineModel.CheckOnline(gcmsg.minfo.userid) == nil and 0 or 1
		if cgmsg.isself == 1 then
			--
			--先把当天的第一个给加过去
			
			local addFirst = gcmsg.zjdaylist:add()
			addFirst.userid = cgmsg.userid
			addFirst.jifen = gcmsg.minfo.jifen
			addFirst.timemark = TimeUtils.GetTimeString()  --当前的时间
			
			local zjList = JulebuModel.GetMemberDay(cgmsg.julebuid, cgmsg.userid)
			if #zjList > 0 then
				for k,v in ipairs(zjList) do
					local addItem = gcmsg.zjdaylist:add()   --这里是后面每天的
					addItem:ParseFromString(v)
				end
			end
		end
	end
	
	gcmsg.result = 0
	gcmsg.isself = cgmsg.isself
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end