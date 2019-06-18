TcpManager = TcpManager or {}
PacketCode = PacketCode or {}
ReturnCode = ReturnCode or {}
g_redisIndex = g_redisIndex or {}

function TcpManager.CreateController(packetid)
	if PacketCode[packetid] == nil then
		return nil
	end
	
	return PacketCode[packetid]["client"],_G[PacketCode[packetid].func]
end


tcpManager = tcpManager or {}
function tcpManager:createController(packetid)
	return TcpManager.CreateController(packetid)
end

function TcpManager.execute(luaFunc, unLinkId, valueId, nMsgId, strMsg)
    _G[luaFunc].execute(unLinkId, valueId, nMsgId, strMsg)
end