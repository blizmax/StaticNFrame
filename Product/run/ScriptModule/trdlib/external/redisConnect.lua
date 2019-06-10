

redis = require('trdlib.external.redis')

local redisConnect = class("redisConnect")



function redisConnect:ctor()
    self.m_params = {
	host = '127.0.0.1',
	port = 6379,
	}
	--LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  "redis connect start")

	if redis == nil then
		--LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  "redis is nil")
	end
	self.m_client = redis.connect(self.m_params)
	self.m_indexList = {}
	self.m_currIndex = 0
	--LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  "redis connect success")
end


function redisConnect:init(p_id)
    self.m_id = p_id;
end

function redisConnect:select(p_redisId)
	self.m_client:select(p_redisId)
end


function redisConnect:checkIndex(p_index)
    local t_index = redisConf:getRedisIndex(p_index)    
	if tonumber(t_index) ~= tonumber(self.m_currIndex) then
		self:select(t_index)
		self.m_currIndex = t_index
	end
end

function redisConnect:del(p_key,db_index)
    self:checkIndex(db_index)
	
	return self.m_client:del(p_key)
end

function redisConnect:get(p_key,db_index)
    self:checkIndex(db_index)
	
	return self.m_client:get(p_key)
end

function redisConnect:keys(p_key, db_index)
	self:checkIndex(db_index)
	return self.m_client:keys(p_key)
end

function redisConnect:exists(p_key,db_index)
    self:checkIndex(db_index)
	
	return self.m_client:exists(p_key)
end

function redisConnect:set(p_key, p_value, db_index)
    self:checkIndex(db_index)
	
	return self.m_client:set(p_key, p_value)
end

function redisConnect:rename(p_oldkey,p_newKey, db_index)
    self:checkIndex(db_index)
	
	return self.m_client:rename(p_oldkey, p_newKey)
end

function redisConnect:incr(p_key,db_index)
    self:checkIndex(db_index)
	
	return self.m_client:incr(p_key)
end

function redisConnect:incrby( p_key, p_value,db_index)
	self:checkIndex(db_index)
	
	return self.m_client:incrby(p_key, p_value)
end

function redisConnect:decrby(p_key, p_value,db_index)
	self:checkIndex(db_index)
	
	return self.m_client:decrby(p_key, p_value)
end

function redisConnect:setex(p_key, timeout, p_value,db_index)
	self:checkIndex(db_index)

	return self.m_client:setex(p_key, timeout, p_value)
end

function redisConnect:expire(p_key, timeout, db_index)
	self:checkIndex(db_index)
	
	return self.m_client:expire(p_key, timeout)
	
end

function redisConnect:setnx(p_key, p_value, db_index)
	self:checkIndex(db_index)
	
	return self.m_client:setnx(p_key, p_value)
end

function redisConnect:hset(p_key, p_field, p_value, db_index)
	self:checkIndex(db_index)
	return self.m_client:hset(p_key, p_field, p_value)
end

function redisConnect:hdel(p_key, p_field,db_index)
	self:checkIndex(db_index)
	
	return self.m_client:hdel(p_key, p_field)
end

function redisConnect:hget(p_key, p_field, db_index)
	self:checkIndex(db_index)
	return self.m_client:hget(p_key, p_field)
end

function redisConnect:hkeys(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:hkeys(p_key)
end

function redisConnect:hgetall(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:hgetall(p_key)
end

function redisConnect:hexists(p_key, p_field,db_index)
	self:checkIndex(db_index)
	--p_field = redisConf:getRedisKey(p_index, p_field)
	return self.m_client:hexists(p_key, p_field)
end

function redisConnect:hlen(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:hlen(p_key)
end

function redisConnect:hvals(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:hvals(p_key)
end

function redisConnect:hincrby(p_key, p_field, increment,db_index)
	self:checkIndex(db_index)
	return self.m_client:hincrby(p_key, p_field, increment)
end

function redisConnect:hmget(p_key, p_fieldStr, db_index)
	self:checkIndex(db_index)
	return self.m_client:hmget(p_key, p_fieldStr)
end

function redisConnect:sadd(p_key, p_value,db_index)
	self:checkIndex(db_index)
	
	return self.m_client:sadd(p_key, p_value)
end

function redisConnect:srem(p_key, p_value,db_index)
	self:checkIndex(db_index)
	return self.m_client:srem(p_key, p_value)
end

function redisConnect:scard(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:scard(p_key)
end

function redisConnect:smembers(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:smembers(p_key)	
end

function redisConnect:spop(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:spop(p_key)
end

function redisConnect:sismember(p_key, p_value,db_index)
	self:checkIndex(db_index)
	
	return self.m_client:sismember(p_key, p_value)
end

function redisConnect:rpush(p_key, p_value,db_index)
	self:checkIndex(db_index)
	return self.m_client:rpush(p_key, p_value)
end

function redisConnect:lpush(p_key, p_value,db_index)
	self:checkIndex(db_index)
	return self.m_client:lpush(p_key, p_value)
end

function redisConnect:llen(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:llen(p_key)
end

function redisConnect:lindex(p_key,p_index, db_index)
	self:checkIndex(db_index)
	return self.m_client:lindex(p_key, p_index)
end

function redisConnect:lpop(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:lpop(p_key)
end

function redisConnect:rpop(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:rpop(p_key)
end

function redisConnect:release()
	--self.m_client:close()
end

--delete value
function redisConnect:lrem(p_key, count, value, db_index)
	self:checkIndex(db_index)
	return self.m_client:lrem(p_key, count, value)
end

function redisConnect:lrange(p_key, start, stop, db_index)
	self:checkIndex(db_index)
	return self.m_client:lrange(p_key, start, stop)
end

function redisConnect:zadd(p_key, score, member,db_index)
	self:checkIndex(db_index)
	return self.m_client:zadd(p_key, score, member)
end

function redisConnect:zincrby(p_key, score, member,db_index)
	self:checkIndex(db_index)
	return self.m_client:zincrby(p_key, score, member)
end

function redisConnect:zrange(p_key, start, stop, db_index, withscores)
	self:checkIndex(db_index)
	return self.m_client:zrange(p_key, start, stop, withscores)
end

function redisConnect:zrem(p_key, member, db_index)
	self:checkIndex(db_index)
	return self.m_client:zrem(p_key, member)
end

function redisConnect:zrank(p_key, member,db_index)
	self:checkIndex(db_index)
	return self.m_client:zrank(p_key, member)
end

function redisConnect:zcard(p_key,db_index)
	self:checkIndex(db_index)
	return self.m_client:zcard(p_key)
end

function redisConnect:zscore(p_key, member,db_index)
	self:checkIndex(db_index)
	return self.m_client:zscore(p_key, member)
end

function redisConnect:zrangebyscore(p_key, minScore, maxScore, db_index, withscores)
	self:checkIndex(db_index)
	return self.m_client:zrangebyscore(p_key,minScore,maxScore, withscores)
end

function redisConnect:zrevrangebyscore(p_key, maxScore, minScore,db_index, withscores)
	self:checkIndex(db_index)
	return self.m_client:zrevrangebyscore(p_key, maxScore, minScore,withscores)
end

return redisConnect
