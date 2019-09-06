
LuaNFrame = LuaNFrame or {}

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

function LuaNFrame.InitLoad()
	require("LuaDebug")
	require("LuaNFrame/CPPNFrame")
	require("LuaNFrame/LuaNFrame")
	require("LuaNFrame/NFExtrend")
	require("LuaNFrame/NFLogDefine")
	require("LuaNFrame/ServerDefine")
	require("LuaNFrame/NFExecutor")
	require("LuaNFrame/NFReload")
	--require("LuaNFrame/NFTimeUtils")
end

function LuaNFrame.InitScript(luaModule)
	package.path = package.path .. ";../ScriptModule/?.lua;"
	package.path = package.path .. ";../ScriptModule/LuaNFrame/?.lua;"

	LuaNFrame.InitLoad()

	--初始化LuaNFrame
	LuaNFrame.init(luaModule)
	--初始化热更
	NFLuaReload.Init()

	if type(LuaNFrame.Platfrom) == 'function' then
		g_platfrom = LuaNFrame.Platfrom()    --from C++ always is 'win32' or 'linux'
	end

	if g_platfrom == "win32" and LuaNFrame.IsThreadModule() == false then
		breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
		LuaNFrame.AddTimer("update_debugsocket", 1)
	end

	local function timerExecute()
		if LuaNFrame.GetAppName() == "AllServer" then
			package.path = package.path .. ";../ScriptModule/GameServer/?.lua;"
			package.path = package.path..";../ScriptModule/GameServer/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
			package.path = package.path..";../ScriptModule/GameServer/trdlib/lua/?.lua"

			require("GameServer/LoadHelper")
		elseif LuaNFrame.GetAppName() == "GameServer" then
			package.path = package.path .. ";../ScriptModule/GameServer/?.lua;"
			package.path = package.path..";../ScriptModule/GameServer/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
			package.path = package.path..";../ScriptModule/GameServer/trdlib/lua/?.lua"

			require("GameServer/LoadHelper")
		elseif LuaNFrame.GetAppName() == "LoginServer" then
			package.path = package.path .. ";../ScriptModule/LoginServer/?.lua;"
			package.path = package.path..";../ScriptModule/LoginServer/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
			package.path = package.path..";../ScriptModule/LoginServer/trdlib/lua/?.lua"

			require("LoginServer/LoadHelper")
		end

		require("LuaNFrame/NFTimeUtils")

		--记录所有文件的当前修改时间，为以后热更新做准备, 时间大概300ms
		NFLuaReload.RecordAllFilesTimes()

		math.newrandomseed()
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