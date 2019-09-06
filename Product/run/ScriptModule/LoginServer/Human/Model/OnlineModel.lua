
OnlineModel = {}
OnlineModel.redisKey = "onlinelist"
OnlineModel.redis_index = "redis_online"
OnlineModel.online_max = "online_max"

function OnlineModel.Init()
	redisItem:del("onlinelist",OnlineModel.redis_index)
end

--获取玩家创建角色时需要初始化的成就列表
function OnlineModel.GetOnlineList(pageNum)
	if pageNum == nil or pageNum == 0 then
		LogBehavior.Error(110,"online","GetOnlineList",-1,"pageNum type="..type(pageNum))
		pageNum = 1
	end
	local startPos = pageNum <= 0 and 0 or (pageNum - 1)*12
	local endPos = pageNum <= 0 and -1 or (startPos + 11)

	return redisItem:zrange(OnlineModel.redisKey,startPos, endPos, OnlineModel.redis_index)
end


function OnlineModel.GetAllOnline()
	return redisItem:zrange(OnlineModel.redisKey,0, -1, OnlineModel.redis_index)
end


function OnlineModel.PlayerLogin(pInfo)
	peiwangService.AddList(pInfo.userid)
	redisItem:zadd(OnlineModel.redisKey, 100-pInfo.viplevel, pInfo.userid, OnlineModel.redis_index)
end

function OnlineModel.PlayerExit(userID)
	peiwangService.RmList(userID)
	--两个列表都删除就不会出错了

   
	if 0 == redisItem:srem(PlayerModel.kit_userList, userID, OnlineModel.redis_index) then
		--如果有玩家被踢下线后，先保存到被踢的列表，如果被踢的列表没有记录，再从在线列表中删除
		redisItem:zrem(OnlineModel.redisKey, userID, OnlineModel.redis_index)
	end
	--RaceModel.PlayExit(userID)
	--这里是整个游戏退出的，对于正在牌桌里面的玩家，不处理退出，然后从新进入牌桌的时候恢复
	--在这里做退出的控制
     CaiPiaoModel.UserExit(userID)
end

function OnlineModel.CheckOnline(userID)
	return redisItem:zrank(OnlineModel.redisKey, userID, OnlineModel.redis_index)
end

function OnlineModel.GetOnlineNum()
	return tonumber(redisItem:zcard(OnlineModel.redisKey, OnlineModel.redis_index))
end

function OnlineModel.UpdateMaxOnline()
	local maxOnline = redisItem:get( OnlineModel.online_max, OnlineModel.redis_index )
	local currNum = OnlineModel.GetOnlineNum()
	
	maxOnline = maxOnline == nil and 0 or tonumber(maxOnline)
	
	if currNum > maxOnline then
		redisItem:set(OnlineModel.online_max, currNum, OnlineModel.redis_index )
		local sqlCase = "update log_sysdaily set maxonline="..currNum.." where dateid='"..TimeUtils.GetDayString().."'"
	
		LogModel.LogSysdailyPush(sqlCase)
	end
end


function OnlineModel.InitMaxOnline()
	redisItem:del(OnlineModel.online_max, OnlineModel.redis_index)
end

