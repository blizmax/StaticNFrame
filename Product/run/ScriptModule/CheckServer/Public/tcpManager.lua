
module('tcpManager', package.seeall)



function tcpManager:createController(packetid)
	--return PacketCode[packetid]["client"],PacketCode[packetid].func
	
	if PacketCode[packetid] == nil then
		return nil
	end
	
	
	return PacketCode[packetid]["client"],_G[PacketCode[packetid].func]
end



