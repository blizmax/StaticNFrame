TimerManager = TimerManager or {}
NFTimer = NFTimer or {}

function TimerManager.execute(luaFunc, timerId)
    TimerManager[luaFunc](timerId)
end


function TimerManager.InitRpmj()
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

function TimerManager.LoopRpmjSec()
    NoticeModel.SeverLoop()
    if g_servername == "run_gdmj" then
        GdmjService.ServerLoop()
        PszService.ServerLoop()
        DouNiuService.ServerLoop()		
    end
end

function TimerManager.InitWuxiandai()

end

function TimerManager.LoopWuxianDaiSec()

end

function TimerManager.update_debugsocket()
	if breakSocketHandle ~= nil then
		breakSocketHandle()
	end
end