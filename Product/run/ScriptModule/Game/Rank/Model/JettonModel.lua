--玩家的魅力榜，目前只统计总数


JettonModel = {}
JettonModel.redis_index = "rank_jetton"
JettonModel.jetton_all = "jetton_all"  --魅力榜的总榜

function JettonModel.Init()
	
	if true == redisItem:exists(JettonModel.jetton_all, JettonModel.redis_index) then
		return true
	end
	--下面开始加载魅力
	local sqlCase = "select userid,jetton from dy_player where jetton>0 order by jetton desc"
	mysqlItem:executeQuery(sqlCase)
	
	for i = 1,2000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		JettonModel.AddRank(sqlData[1],sqlData[2])
	end
	
end

function JettonModel.AddRank(userID,score)
	redisItem:zadd(JettonModel.jetton_all, score, userID,JettonModel.redis_index)
end


function JettonModel.GetMyRank(userID)
	local rank = redisItem:zrank(JettonModel.jetton_all,userID,JettonModel.redis_index)
	local score = redisItem:zscore(JettonModel.jetton_all,userID, JettonModel.redis_index)
	
	rank = rank == nil and 0 or rank + 1
	score = score == nil and 0 or tonumber(score)
	return rank, score
end

function JettonModel.GetRankList(pageNum)
	pageNum = pageNum < 1 and 1 or pageNum
	local startPos = -((pageNum - 1)*10 + 1)
	local endPos = startPos - 9

	local list = redisItem:zrange(JettonModel.jetton_all, endPos, startPos, JettonModel.redis_index, 'withscores')	
	return list == nil and {} or list
end
