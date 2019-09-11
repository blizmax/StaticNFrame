
RebotModel = {}
RebotModel.redis_index = "redis_agent"

RebotModel.rebot_max_id = "rebot_max_id"

RebotModel.rebot_player_id = "rebot_player_id"
RebotModel.rebot_player_info = "rebot_player_info"

function RebotModel.GetRebotMaxId()
    return redisItem:incrby(RebotModel.rebot_max_id, 1, RebotModel.redis_index)
end

function RebotModel.SetRebotPlayerIdByClientId(clientId, playerId)
    return redisItem:hset(RebotModel.rebot_player_id, clientId, playerId, RebotModel.redis_index)
end

function RebotModel.GetRebotPlayerIdByClient(clientId)
    return redisItem:hget(RebotModel.rebot_player_id, clientId, RebotModel.redis_index)
end

function RebotModel.DelRebotPlayerIdByClient(clientId)
    return redisItem:hdel(RebotModel.rebot_player_id, clientId, RebotModel.redis_index)
end

function RebotModel.GetAllRebotPlayerIdByClient()
    return redisItem:hgetall(RebotModel.rebot_player_id, RebotModel.redis_index)
end

function RebotModel.GetPlayerJetton(userid)
    return redisItem:hget(RebotModel.rebot_player_info..userid, "jetton", RebotModel.redis_index)
end

function RebotModel.SetPlayerJetton(userid, jetton)
    return redisItem:hset(RebotModel.rebot_player_info..userid, "jetton", jetton, RebotModel.redis_index)
end

function RebotModel.GetPlayerGameType(userid)
    return redisItem:hget(RebotModel.rebot_player_info..userid, "gametype", RebotModel.redis_index)
end

function RebotModel.SetPlayerGameType(userid, gametype)
    return redisItem:hset(RebotModel.rebot_player_info..userid, "gametype", gametype, RebotModel.redis_index)
end

function RebotModel.GetPlayerTableType(userid)
    return redisItem:hget(RebotModel.rebot_player_info..userid, "tabletype", RebotModel.redis_index)
end

function RebotModel.SetPlayerTableType(userid, tabletype)
    return redisItem:hset(RebotModel.rebot_player_info..userid, "tabletype", tabletype, RebotModel.redis_index)
end

function RebotModel.GetPlayerTableId(userid)
    return redisItem:hget(RebotModel.rebot_player_info..userid, "tableId", RebotModel.redis_index)
end

function RebotModel.SetPlayerTableId(userid, tableId)
    return redisItem:hset(RebotModel.rebot_player_info..userid, "tableId", tableId, RebotModel.redis_index)
end

function RebotModel.GetPlayerTableState(userid, tableId)
    return redisItem:hget(RebotModel.rebot_player_info..userid..tableId, "table_state", RebotModel.redis_index)
end

function RebotModel.SetPlayerTableState(userid, tableId, state)
    return redisItem:hset(RebotModel.rebot_player_info..userid..tableId, "table_state", state, RebotModel.redis_index)
end

function RebotModel.GetPlayerTableTime(userid, tableId)
    return redisItem:hget(RebotModel.rebot_player_info..userid..tableId, "table_time", RebotModel.redis_index)
end

function RebotModel.SetPlayerTableState(userid, tableId, time)
    return redisItem:hset(RebotModel.rebot_player_info..userid..tableId, "table_time", time, RebotModel.redis_index)
end
