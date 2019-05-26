
TcpClient = TcpClient or { }

--网络相关

--添加网络客户端
function TcpClient.AddServer(serverType, ip, port, websocket)
	return LuaNFrame:AddServerForClient(serverType, ip, port, websocket)
end

function TcpClient:AddWebServer(serverType, url)
	return LuaNFrame:AddWebServerForClient(serverType, url)
end

--添加网络协议回调函数
--luaFunc比如：
-- function(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
-- end
function TcpClient.AddRecvCallBack(serverType, nMsgId, luaFunc)
	LuaNFrame:AddRecvCallBackForClient(serverType, nMsgId, luaFunc)
end

--添加网络事件接受回调
--luaFunc比如：nMsgType serverdefine.lua
-- function(nMsgType, unLinkId)
-- end
function TcpClient.AddEventCallBack(serverType, luaFunc)
	LuaNFrame:AddEventCallBackForClient(serverType, luaFunc)
end

function TcpClient.SendMsgByServerId(unLinkId, nMsgId, strData, nPlayerId)
	if type(unLinkId) == "number" and type(nMsgId) == "number" and type(strData) == "string" and type(nPlayerId) == "number" then
		CPPNFrame:SendByServerIDForClient(unLinkId, nMsgId, strData, nPlayerId)
	else
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, __G__TRACKBACK__("TcpServer.sendByServerID Para Error"))
	end
end