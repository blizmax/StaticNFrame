
TimeCheckService = {}

function TimeCheckService.ServerLoop()
	local currMin = g_markTime.curr.min
	local lastMin = g_markTime.last.min

	if lastMin ~= currMin then  --调试，暂时设置一分钟
		--一分钟的判断
		LogServer.ReportInfo(g_markTime.curr)
		
	end
	
	if 0 == math.mod(currMin, 10) and 9==math.mod(lastMin,10) then
		--这里表示是十分钟循环的一个判断
	end
	
	if 0 == math.mod(currMin,30) and 29 == math.mod(lastMin,29) then
	--if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里是半个小时一个循环的判断
	end
	
	if 0 == currMin and 59 == lastMin then
		--这里是一个小时的判断
	end
	
	if g_markTime.last.day ~= g_markTime.curr.day then
	--if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里是每天的判断
		
		if g_servername == "run_test" then
			--跑的是测试服
			redisItem:del("hlyd_share_award")
		end
		
		if g_servername == "run_hlyd" or g_servername == "run_tyyd" then
			redisItem:del("hlyd_share_award")
		end
		
	end	
	if g_markTime.last.wday == 7 and g_markTime.curr.wday == 1 then
		--一周的排行榜
		if g_servername == "run_hlyd" or g_servername == "run_tyyd" then
			redisItem:del(HlydStatsModel.redis_weekwin, HlydStatsModel.redis_index)
		end
	end

end

