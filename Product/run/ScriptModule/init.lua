


--加载LUA文件， path为路径，subdir加载子目录，会一直递归
function LoadLuaFile(path, subdir)
	local sep = string.match (package.config, "[^\n]+")
	for file in lfs.dir(path) do
		if file ~= "." and file ~= ".." then
			local f = path..sep..file
			local attr = lfs.attributes(f)
			if attr.mode == "directory"  then
				if subdir == true then
					LoadLuaFile(f, subdir)
				end
			else
				if string.find(file, ".lua") ~= nil then
					dofile(f)
					print("load lua file:"..file)
				end
			end
		end
	end
end

function init_script_system(luaModule)
	package.path = package.path .. ";../ScriptModule/?.lua;"
	package.path = package.path .. ";../ScriptModule/LuaNFrame/?.lua;"
	package.path = package.path .. ";../ScriptModule/wuxiandai/?.lua;"
	package.path = package.path..";../ScriptModule/wuxiandai/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
	package.path = package.path..";../ScriptModule/wuxiandai/trdlib/lua/?.lua"

	breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)

	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/LuaNFrame")

	--初始化LuaNFrame
	LuaNFrame.init(luaModule)
	LuaNFrame.AddTimer("update_debugsocket", 1)

	local function timerExecute()
		require("LoadHelper")
		mysqlItem = mysqlConnect.new(g_dbtype, g_dbUser, g_dbPassword, g_dbHost, g_dbPort, g_dbDatabase)
		redisItem = redisConnect.new()
		mysqlLog = mysqlConnect.new(g_dbtype, g_dbUser, g_dbPassword, g_dbHost, g_dbPort, g_dbDatabase)
	end
	
	local status, msg = xpcall (timerExecute, __G__TRACKBACK__)

    if not status then
        LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, 0, msg)
    end
end

update_debugsocket = update_debugsocket or {}
function update_debugsocket.execute()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end