
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
	unilight.addtimer("update_snapshot", 10)

	unilight.SetLogLevel(1)


	local mri = require("MemoryReferenceInfo")

	-- Set config.
	mri.m_cConfig.m_bAllMemoryRefFileAddTime = false
	--mri.m_cConfig.m_bSingleMemoryRefFileAddTime = false
	--mri.m_cConfig.m_bComparedMemoryRefFileAddTime = false
	
	-- 打印当前 Lua 虚拟机的所有内存引用快照到文件(或者某个对象的所有引用信息快照)到本地文件。
	-- strSavePath - 快照保存路径，不包括文件名。
	-- strExtraFileName - 添加额外的信息到文件名，可以为 "" 或者 nil。
	-- nMaxRescords - 最多打印多少条记录，-1 打印所有记录。
	-- strRootObjectName - 遍历的根节点对象名称，"" 或者 nil 时使用 tostring(cRootObject)
	-- cRootObject - 遍历的根节点对象，默认为 nil 时使用 debug.getregistry()。
	-- MemoryReferenceInfo.m_cMethods.DumpMemorySnapshot(strSavePath, strExtraFileName, nMaxRescords, strRootObjectName, cRootObject)
	collectgarbage("collect")
	mri.m_cMethods.DumpMemorySnapshot("./logs/"..pluginManager:GetAppName()..tostring(pluginManager:GetAppID()), "1-Before", -1)
end

function update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end

function update_snapshot()
	collectgarbage("collect")
end
