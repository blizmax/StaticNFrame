
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
	package.path = package.path .. ";../ScriptModule/gxlua/?.lua;"

	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/trdlib", true)
	LoadLuaFile("../ScriptModule/gxlua", true)
	LoadLuaFile("../ScriptModule/common", true)

	if pluginManager:IsLoadAllServer() then
		--vscode luaide调试工具需要
		breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
	end

	--初始化LuaNFrame
	unilight.init(pluginManager, luaModule)

	unilight.addtimer("update_debugsocket", 1)

	unilight.SetLogLevel(1)
end

function update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end
