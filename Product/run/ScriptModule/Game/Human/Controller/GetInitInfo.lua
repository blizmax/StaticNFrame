
module("GetInitInfo", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cggetinitinfo()
	local gcmsg = msg_human_pb.gcgetinitinfo()
	
	cgmsg:ParseFromString(buffer)
	
	
	ShopService.Init(cgmsg,gcmsg)
	

	if g_servername == "run_gdmj" or g_servername == "run_test" or g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		gcmsg.wechatlist:append("gdmj918")
	end
	
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end