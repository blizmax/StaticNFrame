
JingSaiModel = {}

JingSaiModel.redis_index = "redis_jingsai"
JingSaiModel.jingsai_info = "jingsai_info_"
JingSaiModel.jingsai_starttime = "jingsai_starttime"
JingSaiModel.jingsai_longtime = "jingsai_longtimetime"  --竞赛开始的时间
JingSaiModel.jingsai_julebu = "js_jlb_"  --每个俱乐部有自己单独的列表
JingSaiModel.jingsai_jifen = "jingsai_jf_"  --每个俱乐部有自己单独的列表，竞赛的积分列表
JingSaiModel.jingsai_nick = "jingsai_nick_"  --每个俱乐部有自己单独的列表，竞赛的积分列表
JingSaiModel.jingsai_faceid = "jingsai_faceid_"  --每个俱乐部有自己单独的列表，竞赛的积分列表
JingSaiModel.game_info = "jingsai_gi_"  --这个是保存玩家的信息的
JingSaiModel.game_list = "jingsai_gl_"  --这个是保存时间的先后顺序的
JingSaiModel.jingsai_bmf = "jingsai_bmf_"  --报名费

JingSaiModel.jingsai_history = "jingsai_hty_"  --这个是每次结算后保存每次竞赛的战绩列表,是跟俱乐部ID有关的

function JingSaiModel.CreateJingSaiID()   --通过俱乐部的ID，获取到竞赛的ID
	
	local randomNum = math.myrandom(1000000,9000000)  --四位的竞赛ID

	while true do
		if false == redisItem:hexists(JingSaiModel.jingsai_starttime, randomNum, JingSaiModel.redis_index) then
			--这里设置时间
			--
			redisItem:hset(JingSaiModel.jingsai_starttime, randomNum, JingSaiModel.redis_index)
			return randomNum
		end
		randomNum = randomNum + 1
	end
end

function JingSaiModel.IsJingSai(jingSaiID)
	
	return tonumber(jingSaiID) >= 1000000 and tonumber(jingSaiID) < 9100000
	
	--return redisItem:hexists(JingSaiModel.jingsai_starttime, jingSaiID, JingSaiModel.redis_index)
end

function JingSaiModel.IsExist(jingSaiID)
	--因为有可能存在竞赛已经删除了，但是牌桌还存在的情况
	--
	return redisItem:hexists(JingSaiModel.jingsai_starttime, jingSaiID, JingSaiModel.redis_index)
end

function JingSaiModel.SetStartTime(jsInfo)
	redisItem:hset(JingSaiModel.jingsai_starttime, jsInfo.jingsaiid, jsInfo.starttime, JingSaiModel.redis_index)
end

function JingSaiModel.GetAllStart()
	return redisItem:hgetall(JingSaiModel.jingsai_starttime,JingSaiModel.redis_index)
end

function JingSaiModel.SetLongTime(jsInfo)
	redisItem:hset(JingSaiModel.jingsai_longtime, jsInfo.jingsaiid, jsInfo.longtime, JingSaiModel.redis_index)
end

function JingSaiModel.GetAllLong()
	return redisItem:hgetall(JingSaiModel.jingsai_longtime,JingSaiModel.redis_index)
end

function JingSaiModel.SetJulebuList(jingSaiID, julebuID, startTime)
	redisItem:hset(JingSaiModel.jingsai_julebu..julebuID, jingSaiID, startTime, JingSaiModel.redis_index)
end

function JingSaiModel.GetJulebuAllList(julebuID)
	return redisItem:hgetall(JingSaiModel.jingsai_julebu..julebuID, JingSaiModel.redis_index)
end

function JingSaiModel.AddJingSai(jsInfo)
	--在创建的时候调用
	JingSaiModel.SetStartTime(jsInfo)
	JingSaiModel.SetLongTime(jsInfo)
	JingSaiModel.SetJingSaiInfo(jsInfo)
	JingSaiModel.SetJulebuList(jsInfo.jingsaiid, jsInfo.julebuid, jsInfo.starttime)
end

function JingSaiModel.SetJingSaiInfo(jsInfo)
	redisItem:set(JingSaiModel.jingsai_info..jsInfo.jingsaiid, jsInfo:SerializeToString(), JingSaiModel.redis_index)
end

function JingSaiModel.GetJingSaiInfo(jingSaiID, jsInfo)
	if jsInfo == nil then
		jsInfo = st_jingsai_pb.jingsaiinfo()
	end
	local strGet = redisItem:get(JingSaiModel.jingsai_info..jingSaiID, JingSaiModel.redis_index)
	if strGet == nil then
		return nil
	end
	jsInfo:ParseFromString(strGet)
	return jsInfo
end

function JingSaiModel.DelJingSai(jingSaiID, julebuID)
	--竞赛已经结束了，开始结算
	--删除开始的ID
	redisItem:hdel(JingSaiModel.jingsai_starttime, jingSaiID, JingSaiModel.redis_index)
	redisItem:hdel(JingSaiModel.jingsai_longtime, jingSaiID, JingSaiModel.redis_index)
	redisItem:del(JingSaiModel.jingsai_jifen..jingSaiID, JingSaiModel.redis_index)
	redisItem:del(JingSaiModel.jingsai_nick..jingSaiID, JingSaiModel.redis_index)
	redisItem:del(JingSaiModel.jingsai_faceid..jingSaiID, JingSaiModel.redis_index)
	redisItem:del(JingSaiModel.jingsai_bmf..jingSaiID, JingSaiModel.redis_index)
	redisItem:hdel(JingSaiModel.jingsai_julebu..julebuID, jingSaiID, JingSaiModel.redis_index)
	local len = redisItem:hlen(JingSaiModel.jingsai_julebu..julebuID, JingSaiModel.redis_index)
	if 0 == tonumber(len) then
		--长度为0的情况下，就把真个类别给清空
		redisItem:hdel(JingSaiModel.jingsai_julebu..julebuID, JingSaiModel.redis_index)
	end
	redisItem:del(JingSaiModel.jingsai_info..jingSaiID, JingSaiModel.redis_index)
end

function JingSaiModel.AddUserJiFen(jingSaiID, userID, jiFen, nickName, faceID)
	
	if false == redisItem:hexists(JingSaiModel.jingsai_jifen..jingSaiID, userID, JingSaiModel.redis_index) then
		redisItem:hset(JingSaiModel.jingsai_jifen..jingSaiID, userID, jiFen, JingSaiModel.redis_index)
		redisItem:hset(JingSaiModel.jingsai_nick..jingSaiID, userID, nickName, JingSaiModel.redis_index)
		redisItem:hset(JingSaiModel.jingsai_faceid..jingSaiID, userID, faceID, JingSaiModel.redis_index)
	else
		redisItem:hincrby(JingSaiModel.jingsai_jifen..jingSaiID, userID, jiFen, JingSaiModel.redis_index)
	end
	
	
end

function JingSaiModel.GetAllUserJiFen(jingSaiID)
	return redisItem:hgetall(JingSaiModel.jingsai_jifen..jingSaiID, JingSaiModel.redis_index)
end

function JingSaiModel.GetMemberNum(jingSaiID)
	local len = redisItem:hlen(JingSaiModel.jingsai_jifen..jingSaiID, JingSaiModel.redis_index)
	return len == nil and 0 or tonumber(len)
end

function JingSaiModel.GetAllUserNick(jingSaiID)
	return redisItem:hgetall(JingSaiModel.jingsai_nick..jingSaiID, JingSaiModel.redis_index)
end

function JingSaiModel.GetAllUserFaceID(jingSaiID)
	return redisItem:hgetall(JingSaiModel.jingsai_faceid..jingSaiID, JingSaiModel.redis_index)
end


function JingSaiModel.SetGameInfo(gInfo)
	redisItem:set(JingSaiModel.game_info..gInfo.tableid, gInfo:SerializeToString(), JingSaiModel.redis_index)
end

function JingSaiModel.GetGameInfo(jingSaiID, tableID)
	return redisItem:get(JingSaiModel.game_info..tableID, JingSaiModel.redis_index)
end

function JingSaiModel.DelGameInfo(jingSaiID, tableID)
	redisItem:del(JingSaiModel.game_info..tableID, JingSaiModel.redis_index)
	JingSaiModel.RemoveGame(jingSaiID,tableID)  --同时也要在这里删除
end

function JingSaiModel.PushGame(jingSaiID, tableID)
	redisItem:lpush(JingSaiModel.game_list..jingSaiID, tableID, JingSaiModel.redis_index)
end

function JingSaiModel.RemoveGame(jingSaiID, tableID)
	redisItem:lrem(JingSaiModel.game_list..jingSaiID, 1, tableID, JingSaiModel.redis_index)
end

function JingSaiModel.GetGameList(jingSaiID, pageNum)
	pageNum = pageNum == 0 and 1 or pageNum
	return redisItem:lrange(JingSaiModel.game_list..jingSaiID, (pageNum-1)*10, pageNum*10-1, JingSaiModel.redis_index)
end

function JingSaiModel.PushHistory(julebuID, htyInfo)
	local len = redisItem:lpush(JingSaiModel.jingsai_history..julebuID, htyInfo:SerializeToString(), JingSaiModel.redis_index)
	if len > 20 then
		--最多只保存20个
		redisItem:rpop(JingSaiModel.jingsai_history..julebuID, JingSaiModel.redis_index)
	end	
end

function JingSaiModel.GetHistoryList(julebuID, pageNum)
	return redisItem:lrange(JingSaiModel.jingsai_history..julebuID, 0, -1, JingSaiModel.redis_index)
end

function JingSaiModel.SetBaoMingFei(jingSaiID, userID, payNum)
	redisItem:hset(JingSaiModel.jingsai_bmf..jingSaiID, userID, payNum, JingSaiModel.redis_index)
end

function JingSaiModel.IsBaoMing(jingSaiID, userID)
	return redisItem:hexists(JingSaiModel.jingsai_bmf..jingSaiID, userID)
end
