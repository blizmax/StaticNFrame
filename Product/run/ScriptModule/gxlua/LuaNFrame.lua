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

    self:addRecvCallBack(NF_SERVER_TYPES.NF_ST_GAME, 1111, "LuaNFrame.NetRecvHandleJson1111")
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

--特殊协议1111
function LuaNFrame.NetRecvHandleJson1111(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " valueId:" .. valueId .. " nMsgId:" .. nMsgId .. " strMsg:" .. strMsg)
    local table_msg = json2table(strMsg)
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["do"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Cmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
                strcmd = "Cmd" .. strcmd
                if type(Net[strcmd]) == "function" then
                    Net[strcmd](table_msg, unLinkId, valueId)
                end
            end
        end
    end
    -- body
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
