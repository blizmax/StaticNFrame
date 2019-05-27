
module("JulebuLeave", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuleave()
	local gcmsg = msg_julebu_pb.gcjulebuleave()
	
	cgmsg:ParseFromString(buffer)
	
	JulebuModel.DelFromOnline(cgmsg.julebuid, cgmsg.userid)
	gcmsg.result = 0
	
	
	
	
	
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end