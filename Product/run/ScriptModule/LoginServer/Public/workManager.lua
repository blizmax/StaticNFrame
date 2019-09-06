
module('workManager', package.seeall)



function workManager:createWork(strIndex)
	--return PacketCode[packetid]["client"],PacketCode[packetid].func
	
	--if PacketCode[packetid] == nil then
	--	return nil
	--end
	
	return _G[strIndex]
end



