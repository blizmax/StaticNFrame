
TcpClient = TcpClient or { }

--网络相关

--添加网络客户端
function TcpClient.addServer(serverType, ip, port, websocket)
	return LuaNFrame:addServerForClient(serverType, ip, port, websocket)
end

function TcpClient:addWebServer(serverType, url)
	return LuaNFrame:addWebServerForClient(serverType, url)
end

--添加网络协议回调函数
--luaFunc比如：
-- function(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
-- end
function TcpClient.addRecvCallBack(serverType, nMsgId, luaFunc)
	LuaNFrame:addRecvCallBackForClient(serverType, nMsgId, luaFunc)
end

--添加网络事件接受回调
--luaFunc比如：nMsgType serverdefine.lua
-- function(nMsgType, unLinkId)
-- end
function TcpClient.addEventCallBack(serverType, luaFunc)
	LuaNFrame:addEventCallBackForClient(serverType, luaFunc)
end

function TcpClient.sendMsgByServerId(cmd, unlinkId, msgId, playerId)
	if type(cmd) == "table" then
		cmd = table2json(cmd)
	end
	
	if type(cmd) == "string" then
		msgId = msgId or 0
		playerId = playerId or 0
		LuaNFrame:sendByServerIDForClient(unlinkId, msgId, cmd, playerId)
	end
end

function TcpClient.sendJsonMsg(cmd, laccount)
	if type(cmd) == "table" then
		cmd = table2json(cmd)
	end
	
	if type(cmd) == "string" then
		LuaNFrame:sendByServerIDForClient(laccount.unLinkId, 0, cmd, laccount.Id)
	end
end

function TcpClient.sendJsonMsgByServerType(serverType, cmd)
	if type(cmd) == "table" then
		cmd = table2json(cmd)
	end
	
	if type(cmd) == "string" then
		LuaNFrame:SendToAllServerByServerTypeForClient(serverType, 0, cmd, 0)
	end
end