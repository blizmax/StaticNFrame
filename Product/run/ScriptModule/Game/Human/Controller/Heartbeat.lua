--进入小马快跑


module("Heartbeat", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgheartbeat()
	local gcmsg = msg_human_pb.gcheartbeat()
	
	cgmsg:ParseFromString(buffer)
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end