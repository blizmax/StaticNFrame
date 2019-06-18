TimerManager = TimerManager or {}

function TimerManager.execute(luaFunc, timerId)
    _G[luaFunc].execute(timerId)
end