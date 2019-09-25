
ActivityServices = {}

function ActivityServices.ServerLoop()

	--这里不需要初始化
	local currTm = TimeUtils.GetTableTime()
	if math.mod(currTm.min, 1)  == 0 and currTm.sec == 0 then
		processWork("ActivityPlayernum","tenSec")
	end
	
end








