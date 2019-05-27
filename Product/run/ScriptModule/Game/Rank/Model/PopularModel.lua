--玩家的魅力榜，目前只统计总数


PopularModel = {}
PopularModel.redis_index = "rank_popular"
PopularModel.popular_all = "popular_all"  --魅力榜的总榜

function PopularModel.Init()
	
	if true == redisItem:exists(PopularModel.popular_all, PopularModel.redis_index) then
		return true
	end
	--下面开始加载魅力
	local sqlCase = "select userid,popularity from dy_player where userid>900000 and popularity>=0 order by popularity desc"
	mysqlItem:executeQuery(sqlCase)
	
	for i = 1,2000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		PopularModel.AddRank(sqlData[1],sqlData[2])
	end
end

function PopularModel.AddRank(userID,popular)
	redisItem:zadd(PopularModel.popular_all, popular, userID,PopularModel.redis_index)
end


function PopularModel.GetMyRank(userID)
	local rank = redisItem:zrank(PopularModel.popular_all,userID, PopularModel.redis_index)
	local score = redisItem:zscore(PopularModel.popular_all,userID, PopularModel.redis_index)
	
	rank = rank == nil and 0 or rank + 1
	score = score == nil and 0 or tonumber(score)
	return rank, score
end

function PopularModel.GetRankList(pageNum)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = -((pageNum - 1)*10 + 1)
	local endPos = startPos - 9

	local list = redisItem:zrange(PopularModel.popular_all, endPos, startPos, PopularModel.redis_index, 'withscores')	
	return list == nil and {} or list
	
end
