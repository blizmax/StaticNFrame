
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
				end
			end
		end
	end
end

function init_script_system(pluginManager, luaModule)
	--vscode luaide调试工具需要
	breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
	package.path = package.path .. ";../ScriptModule/gxlua/?.lua;"

	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/gxlua", true)
	LoadLuaFile("../ScriptModule/table", true)

	if pluginManager:IsLoadAllServer() then
		package.path = package.path .. ";../ScriptModule/GameServer/ttr-server/?.lua;"
		LoadLuaFile("../ScriptModule/GameServer", false)
		LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/do", true)
		LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/event", true)
		LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/gxlua_ttr", true)
		package.path = package.path .. ";../ScriptModule/WorldServer/ttr-server/?.lua;"
		LoadLuaFile("../ScriptModule/WorldServer", false)
		LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/do", true)
		LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/event", true)
		LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/gxlua_ttr", true)
	else
		if pluginManager:GetAppName() == "GameServer" then
			package.path = package.path .. ";../ScriptModule/GameServer/ttr-server/?.lua;"
			LoadLuaFile("../ScriptModule/GameServer", false)
			LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/do", true)
			LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/event", true)
			LoadLuaFile("../ScriptModule/GameServer/ttr-server/script/gxlua_ttr", true)
		elseif pluginManager:GetAppName() == "WorldServer" then
			package.path = package.path .. ";../ScriptModule/WorldServer/ttr-server/?.lua;"
			LoadLuaFile("../ScriptModule/WorldServer", false)
			LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/do", true)
			LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/event", true)
			LoadLuaFile("../ScriptModule/WorldServer/ttr-server/script/gxlua_ttr", true)
		else
			LoadLuaFile("../ScriptModule/" .. pluginManager:GetAppName(), true)
		end
	end

	--初始化LuaNFrame
	unilight.init(pluginManager, luaModule)

	unilight.addtimer("update_debugsocket", 1)
	unilight.addtimer("update_snapshot", 60)

	unilight.SetLogLevel(1)
end

function update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end

function update_snapshot()

	if lastSnapShot == nil then
		lastSnapShot = luaopen_snapshot()
	end

	nowSnapShot = luaopen_snapshot()

	for k,v in pairs(nowSnapShot) do
		if lastSnapShot[k] == nil then
			print(k,v)
		end
	end
end
