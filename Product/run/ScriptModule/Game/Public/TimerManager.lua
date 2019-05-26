TimerManager = TimerManager or {}

function TimerManager.execute(luaFunc, timerId)
    NFTimer[luaFunc](timerId)
end