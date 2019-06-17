


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

rpmj = false
wuxiandai = true

function init_script_system(pluginManager, luaModule)
	package.path = package.path .. ";../ScriptModule/?.lua;"
	package.path = package.path .. ";../ScriptModule/LuaNFrame/?.lua;"
	package.path = package.path..";../ScriptModule/trdlib/libprotobuf/?.lua"   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
	package.path = package.path..";../ScriptModule/trdlib/lua/?.lua"

	LoadLuaFile("../ScriptModule/trdlib")
	LoadLuaFile("../ScriptModule")
	LoadLuaFile("../ScriptModule/LuaNFrame", true)
	LoadLuaFile("../ScriptModule/conf", true)

if rpmj then
	package.path = package.path .. ";../ScriptModule/Game/?.lua;"
	package.path = package.path .. ";../ScriptModule/Game/common/?.lua;"
	LoadLuaFile("../ScriptModule/Game/common", true)
	LoadLuaFile("../ScriptModule/Game/Human")
	LoadLuaFile("../ScriptModule/Game/Julebu")
	LoadLuaFile("../ScriptModule/Game/LogServer")
	LoadLuaFile("../ScriptModule/Game/Mission")
	LoadLuaFile("../ScriptModule/Game/Rank")
	LoadLuaFile("../ScriptModule/Game/Shop")
	LoadLuaFile("../ScriptModule/Game/Http")
	LoadLuaFile("../ScriptModule/Game/JingSai")
	LoadLuaFile("../ScriptModule/Game/Gdmj")
	LoadLuaFile("../ScriptModule/Game/DouNiu")
	LoadLuaFile("../ScriptModule/Game/PSZ")
end

if wuxiandai then
	package.path = package.path .. ";../ScriptModule/wuxiandai/?.lua;"
	package.path = package.path .. ";../ScriptModule/wuxiandai/common/?.lua;"
	LoadLuaFile("../ScriptModule/wuxiandai/common", true)
	LoadLuaFile("../ScriptModule/wuxiandai/Human")
	LoadLuaFile("../ScriptModule/wuxiandai/Agent")
	LoadLuaFile("../ScriptModule/wuxiandai/Event")
	LoadLuaFile("../ScriptModule/wuxiandai/Mission")
	LoadLuaFile("../ScriptModule/wuxiandai/Robot")
	LoadLuaFile("../ScriptModule/wuxiandai/Rank")
	LoadLuaFile("../ScriptModule/wuxiandai/Shop")
	LoadLuaFile("../ScriptModule/wuxiandai/Http")
	LoadLuaFile("../ScriptModule/wuxiandai/LogServer")
	LoadLuaFile("../ScriptModule/wuxiandai/Legal")

	LoadLuaFile("../ScriptModule/wuxiandai/Barccatat")
	LoadLuaFile("../ScriptModule/wuxiandai/Bcbm")
	LoadLuaFile("../ScriptModule/wuxiandai/Brnn")
	LoadLuaFile("../ScriptModule/wuxiandai/Cjmp")
	LoadLuaFile("../ScriptModule/wuxiandai/Ddz")
	LoadLuaFile("../ScriptModule/wuxiandai/DouNiu")
	LoadLuaFile("../ScriptModule/wuxiandai/Ernn")
	LoadLuaFile("../ScriptModule/wuxiandai/Ersh")
	LoadLuaFile("../ScriptModule/wuxiandai/Event")
	LoadLuaFile("../ScriptModule/wuxiandai/Forest")
	LoadLuaFile("../ScriptModule/wuxiandai/Fqzs")
	LoadLuaFile("../ScriptModule/wuxiandai/Fruit")
	LoadLuaFile("../ScriptModule/wuxiandai/HongHei")
	LoadLuaFile("../ScriptModule/wuxiandai/Ersh")
	LoadLuaFile("../ScriptModule/wuxiandai/Ersh")
	LoadLuaFile("../ScriptModule/wuxiandai/Lhd")
	LoadLuaFile("../ScriptModule/wuxiandai/Lhdb")
	LoadLuaFile("../ScriptModule/wuxiandai/Lkpy")
	LoadLuaFile("../ScriptModule/wuxiandai/PSZ")
	LoadLuaFile("../ScriptModule/wuxiandai/Scibo")
	LoadLuaFile("../ScriptModule/wuxiandai/Sscai")
	LoadLuaFile("../ScriptModule/wuxiandai/Texas")
	LoadLuaFile("../ScriptModule/wuxiandai/Tgpd")
	LoadLuaFile("../ScriptModule/wuxiandai/Yqs")
end

	breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
	--初始化LuaNFrame
	LuaNFrame.init(pluginManager, luaModule)

	LuaNFrame.AddTimer("update_debugsocket", 1)
	LuaNFrame.AddAccountEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "AccountNet")	

	if rpmj then
		mysqlItem = mysqlConnect.new()
		redisItem = redisConnect.new()
	
		TimerManager.InitRpmj()
		LuaNFrame.AddTimer("LoopRpmjSec", 1)
	end
	
	if wuxiandai then
		mysqlItem = mysqlConnect.new()
		redisItem = redisConnect.new()
		
		TimerManager.InitWuxiandai()
		LuaNFrame.AddTimer("LoopWuxianDaiSec", 1)
	end
end