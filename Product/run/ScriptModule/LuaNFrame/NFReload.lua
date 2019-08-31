NFLuaReload = NFLuaReload or {}

NFLuaReload.hotfix = require("Hotfix/hotfix")

-- global_objects which must not hotfix.
NFLuaReload.global_objects = {
    arg,
    assert,
    bit32,
    collectgarbage,
    coroutine,
    debug,
    dofile,
    error,
    getmetatable,
    io,
    ipairs,
    load,
    loadfile,
    loadstring,
    math,
    module,
    next,
    os,
    package,
    pairs,
    pcall,
    print,
    rawequal,
    rawget,
    rawlen,
    rawset,
    require,
    select,
    setmetatable,
    string,
    table,
    tonumber,
    tostring,
    type,
    unpack,
    utf8,
    xpcall,
}

function hotfix_log_debug(strMsg)
    CPPNFrame:Info(NFLogId.NF_LOG_SYSTEMLOG, 0, tostring(strMsg))
end

function hotfix_log_error(strMsg)
    CPPNFrame:Info(NFLogId.NF_LOG_SYSTEMLOG, 0, tostring(strMsg))
end

function hotfix_log_info(strMsg)
    CPPNFrame:Info(NFLogId.NF_LOG_SYSTEMLOG, 0, tostring(strMsg))
end

function NFLuaReload.Init()
    --hotfix.log_debug = function(s) print(s) end

    NFLuaReload.hotfix.log_debug = hotfix_log_debug
    NFLuaReload.hotfix.log_error = hotfix_log_error
    NFLuaReload.hotfix.log_info = hotfix_log_info
    NFLuaReload.hotfix.add_protect(NFLuaReload.global_objects)
    NFLuaReload.uptick = 0;
    -- Map file path to file time to detect modification.
    NFLuaReload.path_to_time = { };
end

--- Check modules and hotfix.
function NFLuaReload.Update( curr_tick )

    if curr_tick~=nil then
        if curr_tick - self.uptick < 2000 then
            return;
        end
        
        self.uptick = curr_tick;    
    end


    local MOD_NAME = "hotfix_module_names"
    if not package.searchpath(MOD_NAME, package.path) then return end
    package.loaded[MOD_NAME] = nil  -- always reload it
    local module_names = require(MOD_NAME)

    for _, module_name in pairs(module_names) do
        local path, err = package.searchpath(module_name, package.path)
        -- Skip non-exist module.
        if not path then
            nlwarning(string.format("No such module: %s. %s", module_name, err))
            goto continue
        end

        --local file_time = lfs.attributes (path, "modification")
        local file_time = Misc.GetFileModificationDate(path)
        if file_time == self.path_to_time[path] then goto continue end

        nlinfo(string.format("Hot fix module %s (%s)", module_name, path))
        self.path_to_time[path] = file_time
        hotfix.hotfix_module(module_name)
        ::continue::
    end  -- for
end  -- check()

--重新加载所有LUA文件
function NFLuaReload.ReloadAll()
    local function ReloadExecute()
        for file_path, value in pairs(package.loaded) do
            if type(file_path) == "string" and type(value) == "boolean" and  value == true then
                NFLuaReload.ReloadNewFile(file_path)
            end
        end
	end
	
	local status, msg = xpcall (ReloadExecute, __G__TRACKBACK__)

    if not status then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, msg)
        LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload All Lua File Failed")
        return
    end

    LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload All Lua File Success")
end

function NFLuaReload.ReloadNewFile(module_name)
    local path, err = package.searchpath(module_name, package.path)
    -- Skip non-exist module.
    if not path then
        LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload Lua File: "..module_name.." Failed, No such module:"..err)
        return
    end

    local file_time = lfs.attributes (path, "modification")
    --local file_time = Misc.GetFileModificationDate(path)
    if file_time == NFLuaReload.path_to_time[path] then
        return
    end

    LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, string.format("Hot fix module %s (%s)", module_name, path))

    NFLuaReload.path_to_time[path] = file_time
    NFLuaReload.hotfix.hotfix_module(module_name)
    LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload Lua File: "..module_name.." Success")
end

function NFLuaReload.ReloadFile()
    local function ReloadExecute()
        local MOD_NAME = "hotfix_module_names"
        if not package.searchpath(MOD_NAME, package.path) then return end
        package.loaded[MOD_NAME] = nil  -- always reload it
        local module_names = require(MOD_NAME)

        for _, module_name in pairs(module_names) do
            NFLuaReload.ReloadNewFile(module_name)
        end  -- for
	end
	
	local status, msg = xpcall (ReloadExecute, __G__TRACKBACK__)

    if not status then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, msg)
    end
end