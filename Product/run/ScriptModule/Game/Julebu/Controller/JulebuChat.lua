
module("JulebuChat", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuchat()
	local gcmsg = msg_julebu_pb.gcjulebuchat()
	
	cgmsg:ParseFromString(buffer)
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.senderid)
	
	gcmsg.jchat.id = tonumber(JulebuModel.GetIncrChatID(cgmsg.julebuid))
	
	gcmsg.jchat.chatdate=TimeUtils.GetTime()
	gcmsg.jchat.senderid=cgmsg.senderid
	gcmsg.jchat.sender=cgmsg.sender
	gcmsg.jchat.faceid=pInfo.face_1
	gcmsg.jchat.chattype=cgmsg.chattype
	gcmsg.jchat.msg=cgmsg.msg
	
	
	
	ChatFile(cgmsg.julebuid, cgmsg.senderid.."-"..cgmsg.sender..":"..cgmsg.msg)
	JulebuModel.SetChatInfo(cgmsg.julebuid, gcmsg.jchat)
	gcmsg.result = 0
	local userIDList =  JulebuModel.GetOnlineList(cgmsg.julebuid)
	SendMessage(userIDList, PacketCode[3224].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end