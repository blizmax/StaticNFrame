require "gxlua/class"

CreateClass("LuaNFrame")

--用来存放加载函数
LuaNFrame.LoadScriptList = { }

function LuaNFrame:init(pluginManager)
    --local mongo = require 'mongo'
    self:initmongodb('mongodb://14.17.104.12:28900', "ttr-1")

    self.pluginManager = pluginManager
    if self.pluginManager == nil then
        print("初始化失败。。。。。。。。。")
    end
    self.logModule = self.pluginManager:GetLogModule()
    self.luaModule = self.pluginManager:GetLuaModule()
    self.serverModule = self.pluginManager:GetServerModule()
    self.clientModule = self.pluginManager:GetClientModule()
    self.httpClientModule = self.pluginManager:GetHttpClientModule()
    
    --用来存放加载的module
    self.ScriptList = { }

    --加载应用程序的Lua  Module
    self:load_script_file()
end

--数据库

function LuaNFrame:initmongodb(url, dbname)
    self.client = mongo.Client(url)

    if self.client == nil then
        print("数据库初始化失败。。。。。。。。。")
        return
    end

    self.dataBase = self.client:getDatabase(dbname)

    if self.dataBase == nil then
        print("数据库初始化失败。。。。。。。。。")
    end
end

--[[
    功能：创建表
    参数：
        name   : string, 表名
        primary: string, 主键名
    实例：
        LuaNFrame:createdb("userinfo", "_id") // 创建一个"userinfo"表，主键为"_id"

        returns its Collection handle. On error, returns nil and the error message.
]]
function LuaNFrame:createdb(name, primary)
    return self.dataBase:createCollection(name, mongo.BSON{_id = primary})
end

--[[
    功能: 删除表
    参数：
        name   : string, 表名
    实例：
        LuaNFrame:droptable("userinfo") // 删除表"userinfo"
]]
function LuaNFrame:droptable(name)
    local collection = self.dataBase:getCollection(name)
    if collection ~= nil then
        return collection:drop()
    end
    return false
end

--[[
    功能：获取表中一条记录
    参数：
        name: string           , 表名
        id  : 根据存储类型一致 ，主键值
    实例：
        LuaNFrame:getdata("userinfo", 100000) // 获取表"userinfo"中主键值为100000的那条记录
]]
function LuaNFrame:getdata(name, key)
    local collection = self.dataBase:getCollection(name)
    if collection ~= nil then
        local data = collection:findOne(mongo.BSON{_id = key})
        if data ~= nil then
            return data:value()
        end
    end
    return nil
end

--[[
    功能：保存一条记录
    参数：
            name: string, 表名
            data: table , 需要保存的数据的全部信息，如果和表里已有的记录冲突，替换整条记录,
    实例：
        local userInfo = {
            _id = 100000,
            chips = 200000,
            base = {
                headurl = "http://baidu.com"
            }
        }
        unilight.savedata("userinfo", userInfo)
]]
function LuaNFrame:savedata(name, data)
    local collection = self.dataBase:getCollection(name)
    if collection ~= nil then
        return collection:update(mongo.BSON{_id = data.uid}, encode_repair(data))
    end
    return false
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

function LuaNFrame:sendByServerID(unLinkId, nMsgId, strData, nPlayerId)
    self.serverModule:SendByServerID(unLinkId, nMsgId, strData, nPlayerId)
end

function LuaNFrame:sendToAllServer(nMsgId, strData, nPlayerId)
    self.serverModule:SendToAllServer(nMsgId, strData, nPlayerId)
end

function LuaNFrame:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
    self.serverModule:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
end

function LuaNFrame:sendByServerIDForClient(unLinkId, nMsgId, strData, nPlayerId)
    self.clientModule:SendByServerID(unLinkId, nMsgId, strData, nPlayerId)
end

function LuaNFrame:sendToAllServerForClient(nMsgId, strData, nPlayerId)
    self.clientModule:SendToAllServer(nMsgId, strData, nPlayerId)
end

function LuaNFrame:SendToAllServerByServerTypeForClient(serverType, nMsgId, strData, nPlayerId)
    self.clientModule:SendToAllServerByServerType(serverType, nMsgId, strData, nPlayerId)
end

function LuaNFrame:addServerForClient(serverType, ip, port)
    return self.clientModule:AddServer(serverType, ip, port)
end

--添加网络协议回调函数
function LuaNFrame:addRecvCallBackForClient(serverType, nMsgId, luaFunc)
    self.clientModule:AddReceiveLuaCallBackByMsgId(serverType, nMsgId, luaFunc)
end

--添加网络协议回调函数
function LuaNFrame:addRecvCallBackToOthersForClient(serverType, luaFunc)
    self.clientModule:AddReceiveLuaCallBackToOthers(serverType, luaFunc)
end

function LuaNFrame:addEventCallBackForClient(serverType, luaFunc)
    self.clientModule:AddEventLuaCallBack(serverType, luaFunc)
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

--http client接口

--[[
	向指定url请求GET http服务
	resFunc:http请求回调函数
	url:请求http服务器的url
	para:请求的数据,这时里是一个lua的table
    heads 在这里是一个 map[string]string 选定对应参考与值
    
    return bool
]]

function LuaNFrame:HttpRequestGet(url, resFunc, heads, para)
    heads = heads or {}
    para = para or {}
	if type(resFunc) ~= "string" or type(url) ~= "string" or type(heads) ~= "table" then
		unilight.error("unilight.HttpRequestGet params error" .. resFunc .. url)
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
function LuaNFrame:HttpRequestPost(resFunc, url, body, bodyType, heads, para)
    para = para or {}
	heads = heads or {}
	bodyType = bodyType or "application/x-www-form-urlencoded"
	if type(resFunc) ~= "string" or type(url) ~= "string" or type(body) ~= "table" or type(bodyType) ~= "string"or type(heads) ~= "table" then
		unilight.error("unilight.HttpRequestGet params error" .. resFunc .. url)
		return
	end
    local jsonHeaders = table2json(heads)
	local callbackpara = table2json(para)
    self.httpClientModule:HttpRequestPost(url, body, resFunc, jsonHeaders, callbackpara)
end

--执行函数, 函数被字符串表达出来
--比如说，要执行LoginModule.Init函数，
--LuaNFrame.RunStringFunction("LoginModule.Init")
function LuaNFrame.RunStringFunction(strFunction,...)
    local v = _G;
    for w in string.gmatch(strFunction,"[%[%]%w_\"]+") do
      local index = string.find(w, "%[");
      if index == nil then
          v = v[w]
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
                        LuaNFrame:error("strFunction:", strFunction, " is not a function");
                      end
                  end
              end
          end
      end
    end
    if type(v) == "function" then
      return v(...);
    else
        LuaNFrame:error(strFunction .. " is not function");
    end
  end