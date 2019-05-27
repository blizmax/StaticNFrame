
TcpServer = TcpServer or { }

--网络相关

--添加网络服务器
function TcpServer.AddServer(server_type, server_id, max_client, port, websocket)
	return CPPNFrame:AddServer(server_type, server_id, max_client, port, websocket)
end

--添加网络协议回调函数
--luaFunc比如：
-- function(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
-- end
function TcpServer.AddRecvCallBack(serverType, nMsgId, luaFunc)
	CPPNFrame:AddRecvCallBack(serverType, nMsgId, luaFunc)
end

--添加网络事件接受回调
--luaFunc比如：nMsgType serverdefine.lua
-- function(nMsgType, unLinkId)
-- end
function TcpServer.AddEventCallBack(serverType, luaFunc)
	CPPNFrame:AddEventCallBack(serverType, luaFunc)
end

function TcpServer.SendMsgByServerId(unLinkId, nMsgId, strData, nPlayerId)
	if type(unLinkId) == "number" and type(nMsgId) == "number" and type(strData) == "string" and type(nPlayerId) == "number" then
		CPPNFrame:SendByServerID(unLinkId, nMsgId, strData, nPlayerId)
	else
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, __G__TRACKBACK__("TcpServer.sendByServerID Para Error"))
	end
end