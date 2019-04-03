
CPPNFrame = CPPNFrame or {}

function __G__TRACKBACK__(msg)
	local traceback = string.split(debug.traceback("", 2), "\n")
	string.trim(traceback[3])
	
	local trace = debug.traceback()
	local logData = "LUA_ERROR:"..string.trim(traceback[3]).."\n"..tostring(msg).."\n".."trace msg:"..trace.."\n"
	
	return logData
end

function CPPNFrame:init(pluginManager, luaModule)

    self.pluginManager = pluginManager
    if self.pluginManager == nil or luaModule == nil then
        self:error("初始化失败。。。。。。。。。")
    end

    self.kernelModule = self.pluginManager:GetKernelModule()
    self.logModule = self.pluginManager:GetLogModule()
    self.luaModule = luaModule
    self.serverModule = self.pluginManager:GetServerModule()
    self.clientModule = self.pluginManager:GetClientModule()
    self.httpClientModule = self.pluginManager:GetHttpClientModule()
    self.httpServerModule = self.pluginManager:GetHttpServerModule()
    self.mongoModule = self.pluginManager:GetMongoModule()
    self.serverNetEventModule = self.pluginManager:GetServerNetEventModule()
    self.asynMongoModule = self.pluginManager:GetAsynMongoModule()
    self.app_name = pluginManager:GetAppName()
    self.app_id = pluginManager:GetAppID()
    self.app_dir = self.app_name .. tostring(self.app_id)
end

function CPPNFrame:GetPluginManager()
    return self.pluginManager
end

--使用方法，BeginProfiler -- EndProfiler必须成对出现
function CPPNFrame:BeginProfiler(funname)
    self.pluginManager:BeginProfiler(funname)
end

function CPPNFrame:EndProfiler()
    return self.pluginManager:EndProfiler()
end

--添加服务器定时器, 返回定时器ID
function CPPNFrame:AddTimer(luaFunc, nInterVal, nCallCount)
    return self.luaModule:AddTimer(luaFunc, nInterVal, nCallCount)
end

--停止服务器定时器
function CPPNFrame:StopTimer(timerId)
    self.luaModule:StopTimer(timerId)
end

--执行定时函数
function CPPNFrame.RunTimer(luaFunc, timerId)
    CPPNFrame.RunStringFunction(luaFunc, timerId)
end

function CPPNFrame:AddClocker(luaFunc, startSec, intervalSec, param_table)
    return self.luaModule:AddClocker(luaFunc, startSec, intervalSec, param_table)
end

function CPPNFrame:StopClocker(timerId)
    return self.luaModule:StopClocker(timerId)
end

--创建全局唯一的UUID
function CPPNFrame:GetUUID()
    return self.kernelModule:GetUUID()
end

--通过字符串获得MD5, 返回MD5字符串
function CPPNFrame:GetMD5(str)
    return self.kernelModule:GetMD5(str)
end

--通过字符串获得对应的CRC32, 返回数字
function CPPNFrame:GetCRC32(str)
    return self.kernelModule:GetCRC32(str)
end

--通过字符串获得对应的CRC16, 返回数字
function CPPNFrame:GetCRC16(str)
    return self.kernelModule:GetCRC16(str)
end

--通过字符串获得对应的Base64Encode, 返回字符串
function CPPNFrame:Base64Encode(str)
    return self.kernelModule:Base64Encode(str)
end

--通过字符串获得对应的Base64Decode, 返回字符串
function CPPNFrame:Base64Decode(str)
    return self.kernelModule:Base64Decode(str)
end

--获得服务器开启时间，单位s
function CPPNFrame:GetInitTime()
    return self.pluginManager:GetInitTime()
end

--获得服务器当前时间，单位s
function CPPNFrame:GetNowTime()
    return self.pluginManager:GetNowTime()
end

--添加网络服务器
function CPPNFrame:addServer(server_type, server_id, max_client, port, websocket)
    return self.serverModule:AddServer(server_type, server_id, max_client, port, websocket)
end

--添加网络协议回调函数
function CPPNFrame:addRecvCallBack(serverType, nMsgId, luaFunc)
    self.serverModule:AddReceiveLuaCallBackByMsgId(serverType, nMsgId, luaFunc)
end

--添加网络协议回调函数
function CPPNFrame:addRecvCallBackToOthers(serverType, luaFunc)
    self.serverModule:AddReceiveLuaCallBackToOthers(serverType, luaFunc)
end

function CPPNFrame:addEventCallBack(serverType, luaFunc)
    self.serverModule:AddEventLuaCallBack(serverType, luaFunc)
end

function CPPNFrame:sendByServerID(unLinkId, nMsgId, strData, nPlayerId)
    self.serverModule:SendByServerID(unLinkId, nMsgId, strData, nPlayerId)
end

function CPPNFrame:sendToAllServer(nMsgId, strData, nPlayerId)
    self.serverModule:SendToAllServer(nMsgId, strData, nPlayerId)
end

function CPPNFrame:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
    self.serverModule:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
end

function CPPNFrame:sendByServerIDForClient(unLinkId, nMsgId, strData, nPlayerId)
    self.clientModule:SendByServerID(unLinkId, nMsgId, strData, nPlayerId)
end

function CPPNFrame:sendToAllServerForClient(nMsgId, strData, nPlayerId)
    self.clientModule:SendToAllServer(nMsgId, strData, nPlayerId)
end

function CPPNFrame:SendToAllServerByServerTypeForClient(serverType, nMsgId, strData, nPlayerId)
    self.clientModule:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
end

function CPPNFrame:addServerForClient(serverType, ip, port, websocket)
    websocket = websocket or false
    return self.clientModule:AddServer(serverType, ip, port, websocket)
end

function CPPNFrame:addWebServerForClient(serverType, url)
    return self.clientModule:AddWebServer(serverType, url)
end

--添加网络协议回调函数
function CPPNFrame:addRecvCallBackForClient(serverType, nMsgId, luaFunc)
    self.clientModule:AddReceiveLuaCallBackByMsgId(serverType, nMsgId, luaFunc)
end

--添加网络协议回调函数
function CPPNFrame:addRecvCallBackToOthersForClient(serverType, luaFunc)
    self.clientModule:AddReceiveLuaCallBackToOthers(serverType, luaFunc)
end

function CPPNFrame:addEventCallBackForClient(serverType, luaFunc)
    self.clientModule:AddEventLuaCallBack(serverType, luaFunc)
end

-- log --

--设置LOG等级
function CPPNFrame:SetLogLevel(level)
    self.logModule:SetLogLevel(level)
end

--设置LOG立马刷新等级
function CPPNFrame:SetFlushOn(level)
    self.logModule:SetFlushOn(level)
end

function CPPNFrame:Debug(logId, guid, str)
	self.logModule:LuaDebug(logId, guid, str)
end

function CPPNFrame:Info(logId, guid, str)
	self.logModule:LuaInfo(logId, guid, str)
end

function CPPNFrame:Warn(logId, guid, str)
	self.logModule:LuaWarn(logId, guid, str)
end

function CPPNFrame:Error(logId, guid, str)
    self.logModule:LuaError(logId, guid, str)
end

--http client接口

--[[
	向指定url请求GET http服务
	resFunc:http请求回调函数
	url:请求http服务器的url
	para:请求的数据,这时里是一个lua的table
    heads 在这里是一个 map[string]string 选定对应参考与值
    
    return bool
]]

function CPPNFrame:HttpClientRequestGet(url, resFunc, heads, para)
    heads = heads or {}
    para = para or {}
	if type(resFunc) ~= "string" or type(url) ~= "string" or type(heads) ~= "table" then
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "unilight.HttpRequestGet params error" .. resFunc .. url)
		return
    end

    local jsonHeaders = table2json(heads)
	local callbackpara = table2json(para)
    self.httpClientModule:HttpRequestGet(url, resFunc, jsonHeaders, callbackpara)
end

--[[
	向指定url请求POS http服务
	resFunc:http请求回调函数
	url:请求http服务器的url
	msg:请求的数据,这时里是一个lua的table
	heads 在这里是一个 map[string]string 选定对应参考与值
]]
function CPPNFrame:HttpClientRequestPost(url, resFunc, body, heads, para)
    para = para or {}
	heads = heads or {}
	if type(resFunc) ~= "string" or type(url) ~= "string" or type(heads) ~= "table" or type(body) ~= "table" then
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "unilight.HttpClientRequestPost params error" .. resFunc .. url)
		return
	end
    local jsonHeaders = table2json(heads)
    local callbackpara = table2json(para)
    local jsonbody = table2json(body)
    self.httpClientModule:HttpRequestPost(url, jsonbody, resFunc, jsonHeaders, callbackpara)
end

function CPPNFrame:HttpServerAddRequestHandler(serverType, urlPath, requestType, resFunc)
	if type(serverType) ~= "number" or type(resFunc) ~= "string" or type(urlPath) ~= "string" or type(requestType) ~= "number"then
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "HttpServerAddRequestHandler params error" .. resFunc .. urlPath)
		return
    end
    self.httpServerModule:AddRequestHandler(serverType, urlPath, requestType, resFunc)
end

function CPPNFrame:HttpServerInitServer(serverType, port)
    if type(serverType) ~= "number" or type(port) ~= "number" then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "HttpServerInitServer failed, port is not number:"..port)
    end

    self.httpServerModule:InitServer(port)
end

function CPPNFrame:HttpServerResponseMsg(serverType, req, strMsg, code, reason)
    if type(serverType) ~= "number" or type(strMsg) ~= "string" or type(code) ~= "number" or type(reason) ~= "string" then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "HttpServerResponseMsg failed")
    end

    self.httpServerModule:ResponseMsg(req, strMsg, code, reason)
end

--serverNetEventModule 注册服务器与服务器之间的网络回调，主要有连接回调，断线回调
--比如说，luaFuncStr格式：luaFuncStr（eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData）
--
function CPPNFrame:AddServerEventCallBack(eSourceType, eTargetType, luaFuncStr)
    self.serverNetEventModule:AddEventCallBack(eSourceType, eTargetType, luaFuncStr)
end

--serverNetEventModule 注册账号网络回调，主要有玩家连接回调，断线回调，重连回调
--比如说，luaFuncStr格式：luaFuncStr（uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData）
--
function CPPNFrame:AddAccountEventCallBack(eServerType, luaFuncStr)
    self.serverNetEventModule:AddAccountEventCallBack(eServerType, luaFuncStr)
end

--执行函数, 函数被字符串表达出来
--比如说，要执行LoginModule.Init函数，
--CPPNFrame.RunStringFunction("LoginModule.Init")
function CPPNFrame.RunStringFunction(strFunction,...)
    local v = _G;
    for w in string.gmatch(strFunction,"[%[%]%w_\"]+") do
      local index = string.find(w, "%[");
      if index == nil then
          v = v[w]
          if v == nil then
            break
          end
      else
          local key = string.match(w, "([%w_]+)%[")
          if key == nil then
              return;
          else
              v = v[key]
              for val in string.gmatch(w, "%[[\"%w_]+%]") do
                  local value = string.match(val, "%[([\"%w_]+)%]")
                  local value_str = string.match(value,"\"([%w_]+)\"");
                  if value_str ~= nil then
                      v = v[value_str];
                  else
                      local value_num = tonumber(value);
                      if value_num ~= nil then
                          v = v[value_num];
                      else
                        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, "strFunction:", strFunction, " is not a function");
                      end
                  end
              end
          end
      end
    end
    if type(v) == "function" then
      return v(...);
    else
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, strFunction .. " is not function");
    end
  end
