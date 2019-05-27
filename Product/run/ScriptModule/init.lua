


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

function init_script_system(pluginManager, luaModule)
	package.path = package.path .. ";../ScriptModule/?.lua;"
	package.path = package.path .. ";../ScriptModule/LuaNFrame/?.lua;"
	package.path = package.path..";../ScriptModule/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
	package.path = package.path..";../ScriptModule/trdlib/lua/?.lua"
	package.path = package.path .. ";../ScriptModule/Game/?.lua;"
	package.path = package.path .. ";../ScriptModule/Game/common/?.lua;"

	LoadLuaFile("../ScriptModule/trdlib")
	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/LuaNFrame", true)
	LoadLuaFile("../ScriptModule/conf", true)
	LoadLuaFile("../ScriptModule/Game/common", true)
	LoadLuaFile("../ScriptModule/Game/Human")
	LoadLuaFile("../ScriptModule/Game/Julebu")
	LoadLuaFile("../ScriptModule/Game/LogServer")

	breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
	--初始化LuaNFrame
	LuaNFrame.init(pluginManager, luaModule)

	LuaNFrame.AddTimer("update_debugsocket", 1)
end

NFTimer = NFTimer or {}

function NFTimer.update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end