GameWuxiandai = GameWuxiandai or {}

function GameWuxiandai.Load()
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

function GameWuxiandai.Init()
    GameWuxiandai.Load()
    
    mysqlItem = mysqlConnect.new()
    redisItem = redisConnect.new()
    LuaNFrame.AddTimer("LoopWuxianDaiSec", 1)
end

LoopWuxianDaiSec = LoopWuxianDaiSec or {}
function LoopWuxianDaiSec.execute(timerId)

end