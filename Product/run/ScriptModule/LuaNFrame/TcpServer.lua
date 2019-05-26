
TcpServer = TcpServer or { }

--网络相关

--添加网络服务器
function TcpServer.addServer(server_type, server_id, max_client, port, websocket)
	return LuaNFrame:addServer(server_type, server_id, max_client, port, websocket)
end

--添加网络协议回调函数
--luaFunc比如：
-- function(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
-- end
function TcpServer.addRecvCallBack(serverType, nMsgId, luaFunc)
	LuaNFrame:addRecvCallBack(serverType, nMsgId, luaFunc)
end

--添加网络事件接受回调
--luaFunc比如：nMsgType serverdefine.lua
-- function(nMsgType, unLinkId)
-- end
function TcpServer.addEventCallBack(serverType, luaFunc)
	LuaNFrame:addEventCallBack(serverType, luaFunc)
end

function TcpServer.sendByServerID(unlinkId, cmd)
	if type(cmd) == "table" then
		cmd = table2json(cmd)
	end

	if type(cmd) == "string" then
		LuaNFrame:sendByServerID(unlinkId, 0, cmd, 0)
	end
end

function TcpServer.sendJsonMsg(cmd, laccount)
	if type(cmd) == "table" then
		cmd = table2json(cmd)
	end

	if type(cmd) == "string" then
		LuaNFrame:sendByServerID(laccount.unLinkId, 0, cmd, laccount.Id)
	end
end