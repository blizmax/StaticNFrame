GameRebot = GameRebot or {}

function GameRebot.Load()
	package.path = package.path .. ";../ScriptModule/Rebot/?.lua;"
	LoadLuaFile("../ScriptModule/Rebot", true)
end

function GameRebot.Init()
    GameRpmj.Load()
    breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)

    LuaNFrame.AddTimer("LoopRebotSec", 1)
end

LoopRebotSec = LoopRebotSec or {}
function LoopRebotSec.execute(timerId)

end