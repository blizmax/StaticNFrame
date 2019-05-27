
module("GetAgencyInfo", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cggetagencyinfo()
	local gcmsg = msg_human_pb.gcgetagencyinfo()
	
	cgmsg:ParseFromString(buffer)
	
	if nil == AgencyModel.GetAgencyInfo(cgmsg.userid, gcmsg.agyinfo) then
		gcmsg.agyinfo.userid = 0
	end
	--AgencyModel.ZsmjDivide(cgmsg.userid, 100, "test")
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end