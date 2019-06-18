TimerManager = TimerManager or {}

function TimerManager.execute(luaFunc, dataStr, timerId)
    _G[luaFunc].execute(dataStr, timerId)
end

--入参是两个参数，第一个是索引，第一个是参数信息
function processWork(indexStr, dataStr)
	_G[indexStr].work(dataStr)
end

function processTimer(timeSec, indexStr, dataStr)
	LuaNFrame.AddTimer(indexStr, timeSec, 1, dataStr)
end

function processLoopTimer(timeSec, indexStr, dataStr)
	--该函数设置的定时器，是在主线程serverloop的线程中执行
	LuaNFrame.AddTimer(indexStr, timeSec, 1, dataStr)
end