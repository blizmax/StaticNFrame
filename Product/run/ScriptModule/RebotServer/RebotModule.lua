
RebotModel = {}
RebotModel.redis_index = "redis_agent"

RebotModel.rebot_max_id = "rebot_max_id"

RebotModel.rebot_player_id = "rebot_player_id"
RebotModel.rebot_player_info = "rebot_player_info"

RebotModel.rebot_table_id = "rebot_table_id"

RebotModel.rebot_operate_id = "rebot_operate_id"

function RebotModel.GetRebotOperateId(unLinkId)
    return redisItem:incrby(RebotModel.rebot_operate_id..unLinkId, 1, RebotModel.redis_index)
end

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

function RebotModel.GetPlayerTableId(userid)
    return redisItem:hget(RebotModel.rebot_table_id, userid, RebotModel.redis_index)
end

function RebotModel.SetPlayerTableId(userid, tableId)
    return redisItem:hset(RebotModel.rebot_table_id, userid, tableId, RebotModel.redis_index)
end

function RebotModel.DelPlayerTableId(userid)
    return redisItem:hdel(RebotModel.rebot_table_id, userid, RebotModel.redis_index)
end

function RebotModel.GetAllPlayerTableId()
    return redisItem:hgetall(RebotModel.rebot_table_id, RebotModel.redis_index)
end

function RebotModel.RecordPlayerSendPour(userid)
    redisItem:set("rebot_send_pour_"..userid, "1", RebotModel.redis_index)
    redisItem:expire("rebot_send_pour_"..userid, 300, RebotModel.redis_index)
end

function RebotModel.GetPlayerSendPour(userid)
    local value = redisItem:get("rebot_send_pour_"..userid, RebotModel.redis_index)
    return value
end

function RebotModel.AccountConnnect(clientId)
    redisItem:rpush("rebot_connect", clientId, RebotModel.redis_index)
end

function RebotModel.GetAccountConnnect()
    return redisItem:lrange("rebot_connect", 0, -1, RebotModel.redis_index)
end

function RebotModel.DelAccountConnnect(clientId)
    return redisItem:lrem("rebot_connect", 0, clientId, RebotModel.redis_index)
end
