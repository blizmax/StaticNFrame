unilight = unilight or {}

unilight.debug = function(...)
	LuaNFrame:debug(...)
end

unilight.info = function(...)
	LuaNFrame:info(...)
end

unilight.warn = function(...)
	LuaNFrame:warn(...)
end

unilight.error = function(...)
    LuaNFrame:error(...)
end

--网络相关

--添加网络服务器
function unilight.addServer(server_type, server_id, max_client, port)
	return LuaNFrame:addServer(server_type, server_id, max_client, port)
end

--添加网络协议回调函数
--luaFunc比如：
-- function(const uint32_t unLinkId, const uint64_t playerId, const uint32_t nMsgId, const char* msg, const uint32_t nLen)
-- end
function unilight.addRecvCallBack(serverType, nMsgId, luaFunc)
	LuaNFrame:addRecvCallBack(serverType, nMsgId, luaFunc)
end

--添加网络协议回调函数
function unilight.addRecvCallBackToOthers(serverType, luaFunc)
	LuaNFrame:addRecvCallBackToOthers(serverType, luaFunc)
end

--添加网络事件接受回调
--luaFunc比如：nMsgType serverdefine.lua
-- function(nMsgType, unLinkId)
-- end
function unilight.addEventCallBack(serverType, luaFunc)
	LuaNFrame:addEventCallBack(serverType, luaFunc)
end