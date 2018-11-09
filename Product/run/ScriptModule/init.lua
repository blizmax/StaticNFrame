
--加载LUA文件， path为路径，subdir加载子目录，会一直递归
function LoadLuaFile(path, subdir)
	local sep = string.match (package.config, "[^\n]+")
	for file in lfs.dir(path) do
		if file ~= "." and file ~= ".." then
			local f = path..sep..file
			local attr = lfs.attributes(f)
			if attr.mode == "directory"  then
				if subdir == true then
					attrdir(f)
				end
			else
				if string.find(file, ".lua") ~= nil then
					print(f)
					dofile(f)
				end
			end
		end
	end
end

function init_script_system(pluginManager)
	--vscode luaide调试工具需要
	local breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)

	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/gxlua", true)
	LoadLuaFile("../ScriptModule/table", true)

	if pluginManager:IsLoadAllServer() then
		LoadLuaFile("../ScriptModule/GameServer", true)
		LoadLuaFile("../ScriptModule/GateServer", true)
		LoadLuaFile("../ScriptModule/LoginServer", true)
	else
		LoadLuaFile("../ScriptModule/" .. pluginManager:GetAppName(), true)
	end

	--初始化LuaNFrame
	unilight.init(pluginManager)
end