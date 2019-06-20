GameWuxiandai = GameWuxiandai or {}

function GameWuxiandai.Load()
	package.path = package.path .. ";../ScriptModule/wuxiandai/?.lua;"
	package.path = package.path .. ";../ScriptModule/wuxiandai/common/?.lua;"
	LoadLuaFile("../ScriptModule/LuaNFrame/Public/GameWuxiandai", true)
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
	LoadLuaFile("../ScriptModule/wuxiandai/Sicbo")
	LoadLuaFile("../ScriptModule/wuxiandai/Sscai")
	LoadLuaFile("../ScriptModule/wuxiandai/Texas")
	LoadLuaFile("../ScriptModule/wuxiandai/Tgpd")
	LoadLuaFile("../ScriptModule/wuxiandai/Yqs")
end

function GameWuxiandai.Init()
    GameWuxiandai.Load()
    breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
    mysqlItem = mysqlConnect.new()
    redisItem = redisConnect.new()
	LuaNFrame.AddTimer("LoopWuxianDaiSec", 1)
	
	LogService.Init()
	RobotService.Init()   --先在这里加载机器人,后面再分开每个游戏单独管理
	MailModel.LoadSysMail()
	JettonModel.Init()
	PopularModel.Init()
	OnlineModel.Init()    --把非在线的全部清空

	
	BrnnNewService.Init()
	HongHeiNewService.Init()
	BarccatatNewService.Init()
	BcbmNewService.Init()
	LhdNewService.Init()
	
	DdzNewService.Init()
	PszNewService.Init()
	DouNiuNewService.Init()
	SicboNewService.Init()
	FqzsNewService.Init()
	YqsNewService.Init()
	TexasNewService.Init()
	if g_gamename == "hhgj" then
		CjmpNewService.Init()
	end
	LogServer.Init()
end

LoopWuxianDaiSec = LoopWuxianDaiSec or {}
function LoopWuxianDaiSec.execute(dataStr, timerId)
	g_markTime.curr = TimeUtils.GetTableTime()
	
	BrnnNewService.ServerLoop()
			
	HongHeiNewService.ServerLoop()
	BarccatatNewService.ServerLoop()
	BcbmNewService.ServerLoop()
	LhdNewService.ServerLoop()
	DdzNewService.ServerLoop()
	PszNewService.ServerLoop()
	
	DouNiuNewService.ServerLoop()
	
	SicboNewService.ServerLoop()
	FqzsNewService.ServerLoop()
	YqsNewService.ServerLoop()
	TexasNewService.ServerLoop()
	AgentService.ServiceLoop()
	CjmpNewService.ServerLoop()

	LegalService.UtilsLoop()
	LogService.UtilsLoop()
	AgentService.UtilsLoop()
end