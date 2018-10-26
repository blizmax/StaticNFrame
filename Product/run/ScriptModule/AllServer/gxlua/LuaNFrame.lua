require "gxlua/class"

CreateClass("LuaNFrame")

function LuaNFrame:init(pluginManager)
    self.pluginManager = pluginManager
    if self.pluginManager == nil then
        print("初始化失败。。。。。。。。。")
    end
    self.logModule = self.pluginManager:GetLogModule()
    self.luaModule = self.pluginManager:GetLuaModule()
    
    --用来存放加载的module
    self.ScriptList = { }
    --用来存放加载函数
    self.LoadScriptList = { }
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
