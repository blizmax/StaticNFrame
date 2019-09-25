
ActivityModel = {}
ActivityModel.redis_index = "redis_activity"

ActivityModel.activity_playernm = "activity_playernum"   --玩家数量排行榜
ActivityModel.activity_playernm_mark = "activity_playernum_mark"   --玩家数量排行榜

function ActivityModel.GetAgentInfo(userID, agyInfo)

end

function ActivityModel.AddPlayerNum(userID)
	
	redisItem:zincrby( ActivityModel.activity_playernm, 1, userID,  ActivityModel.redis_index)
	
end

function ActivityModel.SetPlayerUserID(userID)
	redisItem:set( ActivityModel.activity_playernm_mark, userID, ActivityModel.redis_index )
end

function ActivityModel.GetPlayerUserID()
	return redisItem:get( ActivityModel.activity_playernm_mark, ActivityModel.redis_index )
end

function ActivityModel.GetPlayerList()

	return redisItem:zrevrange( ActivityModel.activity_playernm, 0, 20, ActivityModel.redis_index, "withscore" )
	
end


function ActivityModel.GetSelfPlayerNum(userID)

	local score = redisItem:zscore( ActivityModel.activity_playernm, userID, ActivityModel.redis_index )
	local rank = 0
	if score ~= nil  then
		
		rank = redisItem:zrevrank( ActivityModel.activity_playernm, userID, ActivityModel.redis_index )
	else
		score = 0
	end

	
	return rank,score
end


