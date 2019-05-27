--目前每日赚金榜，只统计德州扑克以及欢乐斗牛中赢牌的金额，不计算投入的成本


RankModel = {}

RankModel.redis_index = "rank_index"

function RankModel.GetRankList(rankType,pageNum)

	if rankType == g_rankType.all_jetton then
		return RankModel.GetJettonList(pageNum)
	elseif rankType == g_rankType.all_level then
		return RankModel.GetLevelList(pageNum)
	else
		return RankModel.GetDayWinList(pageNum)
	end
	
end

function RankModel.GetRankListWithScore(rankType,pageNum)

	if rankType == g_rankType.all_jetton then
		return RankModel.GetJettonList(pageNum, "withscores")
	elseif rankType == g_rankType.all_level then
		return RankModel.GetLevelList(pageNum, "withscores")
	else
		return RankModel.GetDayWinList(pageNum,"withscores")
	end
	
end

function RankModel.GetMyRank(rankType,userID)
	if rankType == g_rankType.all_jetton then
		return RankModel.GetMyJettonRank(userID)
	elseif rankType == g_rankType.all_level then
		return RankModel.GetMyLevelRank(userID)
	else
		return RankModel.GetMyDayWinRank(userID)
	end
end

function RankModel.GetJettonList(pageNum, withScores)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = (pageNum - 1)*10
	local endPos = startPos + 9
	
	if false == redisItem:exists("rank_jetton",RankModel.redis_index) then
		RankModel.LoadJettonList()
	end
	return redisItem:zrange("rank_jetton",startPos, endPos,RankModel.redis_index, withScores)
end

function RankModel.LoadJettonList()
	local sqlCase = "select userid,jetton from dy_player where userid>900000 order by jetton desc limit 2000"
	mysqlItem:executeQuery(sqlCase)
	for i = 1,2000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		RankModel.AddJettonList(sqlData[1],tonumber(sqlData[2]))
	end
end

function RankModel.AddJettonList(userID,jetton)
	--这里设置成负数，是为了方便，上面按照排序来取对应的纸
	redisItem:zadd("rank_jetton", -jetton, userID, RankModel.redis_index)
end

function RankModel.GetMyJettonRank(userID)
	local rank = redisItem:zrank("rank_jetton", userID,RankModel.redis_index)
	local score = redisItem:zscore("rank_jetton", userID,RankModel.redis_index)
	rank = rank == nil and 0 or rank + 1 --这里需要加上1，因为在缓存中的index是从0开始的
	score = score == nil and 0 or score
	return rank,-score
end

function RankModel.GetLevelList(pageNum, withScores)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = (pageNum - 1)*10
	local endPos = startPos + 9	
	if false == redisItem:exists("ranklist", "rank_level") then
		RankModel.LoadLevelList()
	end
	return redisItem:zrange("rank_level",startPos, endPos, RankModel.redis_index, withScores)
end

function RankModel.LoadLevelList()
	local sqlCase = "select userid,level from dy_player where userid>900000 order by jetton desc limit 2000"
	mysqlItem:executeQuery(sqlCase)
	for i = 1,2000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		RankModel.AddLevelList(sqlData[1],tonumber(sqlData[2]))
	end
end

function RankModel.AddLevelList(userID,level)
	redisItem:zadd("rank_level", -level, userID, RankModel.redis_index)
end

function RankModel.GetMyLevelRank(userID)
	local rank = redisItem:zrank("rank_level", userID, RankModel.redis_index)
	local score = redisItem:zscore("rank_level", userID, RankModel.redis_index)
	rank = rank == nil and 0 or rank + 1  --这里需要加上1，因为在缓存中的index是从0开始的
	score = score == nil and 0 or score
	return rank,-score
end


function RankModel.GetDayWinList(pageNum)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = (pageNum - 1)*10
	local endPos = startPos + 9	
	if false == redisItem:exists("rank_daywin", RankModel.redis_index) then
		return {}
	end
	return redisItem:zrange("rank_daywin",startPos, endPos, RankModel.redis_index)
end

function RankModel.GetDayWinList(pageNum, withScores)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = (pageNum - 1)*10
	local endPos = startPos + 9	
	if false == redisItem:exists("rankdaywin", RankModel.redis_index) then
		return {}
	end
	return redisItem:zrange("rank_daywin",startPos, endPos, RankModel.redis_index, withScores)
end

function RankModel.AddDayWinList(userID,jetton)
	redisItem:zadd("rank_daywin", -jetton, userID, RankModel.redis_index)
end

function RankModel.GetMyDayWinRank(userID)
	local rank = redisItem:zrank("rank_daywin", userID, RankModel.redis_index)
	local score = redisItem:zscore("rank_daywin", userID, RankModel.redis_index)
	rank = rank == nil and 0 or rank + 1  --这里需要加上1，因为在缓存中的index是从0开始的
	score = score == nil and 0 or score
	return rank,-score
end

function RankModel.DayCheck()
	--该函数的作用是清空每天赚金榜的数据
	redisItem:del(g_rankType.all_winjetton,RankModel.redis_index)
end