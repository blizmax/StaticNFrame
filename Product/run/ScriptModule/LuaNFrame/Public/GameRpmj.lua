GameRpmj = GameRpmj or {}

function GameRpmj.Load()
	package.path = package.path .. ";../ScriptModule/Game/?.lua;"
    package.path = package.path .. ";../ScriptModule/Game/common/?.lua;"
    LoadLuaFile("../ScriptModule/Public/GameRpmj", true)
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

function GameRpmj.Init()
    GameRpmj.Load()
    breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
    mysqlItem = mysqlConnect.new()
    redisItem = redisConnect.new()

    BroadCastModel.Init()

    MailModel.LoadSysMail()
    JettonModel.Init()
    PopularModel.Init()

    OnlineModel.Init()    --把非在线的全部清空

    if g_servername == "run_gdmj" then
        GdmjService.Init()
        DouNiuService.Init()
        PszService.Init()	
    end

    LuaNFrame.AddTimer("LoopRpmjSec", 1)
end

LoopRpmjSec = LoopRpmjSec or {}
function LoopRpmjSec.execute(timerId)
    NoticeModel.SeverLoop()
    if g_servername == "run_gdmj" then
        GdmjService.ServerLoop()
        PszService.ServerLoop()
        DouNiuService.ServerLoop()		
    end
end