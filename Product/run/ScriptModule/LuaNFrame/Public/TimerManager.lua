TimerManager = TimerManager or {}
NFTimer = NFTimer or {}

function TimerManager.execute(luaFunc, timerId)
    NFTimer[luaFunc](timerId)
end


function TimerManager.Init()
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
end

function NFTimer.LoopSec()
    NoticeModel.SeverLoop()
    if g_servername == "run_gdmj" then
        GdmjService.ServerLoop()
        PszService.ServerLoop()
        DouNiuService.ServerLoop()		
    end
end

function NFTimer.update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end