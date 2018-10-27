require "gxlua/class"

CreateClass("LuaNFrame")

function LuaNFrame:init(pluginManager)
    self.pluginManager = pluginManager
    if self.pluginManager == nil then
        print("初始化失败。。。。。。。。。")
    end
    self.logModule = self.pluginManager:GetLogModule()
    self.luaModule = self.pluginManager:GetLuaModule()
    self.serverModule = self.pluginManager:GetServerModule()
    
    --用来存放加载的module
    self.ScriptList = { }
    --用来存放加载函数
    self.LoadScriptList = { }

    self.serverModule:AddServer(NF_SERVER_TYPES.NF_ST_LOGIN, 100, 10, 1782)
end

--添加网络服务器
function LuaNFrame:addServer(server_type, server_id, max_client, port)
    return self.serverModule:AddServer(server_type, server_id, max_client, port)
end

--添加网络协议回调函数
function LuaNFrame:addRecvCallBack(serverType, nMsgId, luaFunc)
    self.serverModule:AddReceiveLuaCallBackByMsgId(serverType, nMsgId, luaFunc)
end

--添加网络协议回调函数
function LuaNFrame:addRecvCallBackToOthers(serverType, luaFunc)
    self.serverModule:AddReceiveLuaCallBackToOthers(serverType, luaFunc)
end

function LuaNFrame:addEventCallBack(serverType, luaFunc)
    self.serverModule:AddEventLuaCallBack(serverType, luaFunc)
end

--执行加载函数
function LuaNFrame:load_script_file(  )
    for i, fun in ipairs(self.LoadScriptList) do
        fun()
    end
end

function LuaNFrame:InsertLoadFunc(func)
    table.insert(self.LoadScriptList, func)
end

-- log --

function LuaNFrame:debug(...)
	self.logModule:LuaDebug(...)
end

function LuaNFrame:info(...)
	self.logModule:LuaInfo(...)
end

function LuaNFrame:warn(...)
	self.logModule:LuaWarn(...)
end

function LuaNFrame:error(...)
    self.logModule:LuaError(...)
end
