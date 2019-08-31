LuaNFrame = LuaNFrame or {}

--这个函数了填写需要Reload的文件
function LuaNFrame.GetNeedReloadFiles()
    local reloadfiles = {
        --"Public/httpManager",
    }

    return reloadfiles
end

--重新加载所有LUA文件
function LuaNFrame.ReloadAll()
    local function ReloadExecute()
        for file_path, value in pairs(package.loaded) do
            if type(file_path) == "string" and type(value) == "boolean" and  value == true then
                package.loaded[file_path] = nil
            end
        end
        require("init")
        initLoad()
        if LuaNFrame.GetAppName() == "AllServer" then
            require("GameServer/LoadHelper")
        elseif LuaNFrame.GetAppName() == "GameServer" then
            require("GameServer/LoadHelper")
        end
        require("LuaNFrame/NFTimeUtils")
	end
	
	local status, msg = xpcall (ReloadExecute, __G__TRACKBACK__)

    if not status then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, msg)
        LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload All Lua File Failed")
        return
    end

    LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload All Lua File Success")
end

function LuaNFrame.ReloadNewFile(newfile)
    if package.loaded[newfile] == true then
        package.loaded[newfile] = nil
        require(newfile)
    else
        require(newfile)
    end
    LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0, "Reload Lua File: "..newfile.." Success")
end

function LuaNFrame.ReloadFile()
    local function ReloadExecute()
        package.loaded["LuaNFrame/NFReload"] = nil
        require("LuaNFrame/NFReload")

        local needreloadfiles = LuaNFrame.GetNeedReloadFiles()
        for index, path_files in ipairs(needreloadfiles) do
            if type(path_files) == "string" then
                LuaNFrame.ReloadNewFile(path_files)
            end
        end
        
	end
	
	local status, msg = xpcall (ReloadExecute, __G__TRACKBACK__)

    if not status then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, msg)
    end
end