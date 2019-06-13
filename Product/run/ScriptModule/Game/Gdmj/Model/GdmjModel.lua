

GdmjModel = {}

g_gdmjRedis = {}
g_gdmjRedis.redis_index = "gdmj"
g_gdmjRedis.gdmj_userlist = "gdmj_userlist"
g_gdmjRedis.gdmj_info = "gdmj_info"
g_gdmjRedis.modify_list = "gdmj_modify"
g_gdmjRedis.table_list = "gdmj_list"
g_gdmjRedis.gdmj_room = "gdmj_room"
g_gdmjRedis.user_tablelist = "gdmj_utlist_" --没当有玩家创建房间的时候，需要在这个列表中把tableid设置好,表示是玩家自己的列表
g_gdmjRedis.chair_user = "gdmj_chair_"
g_gdmjRedis.chair_detail = "gdmj_play_"
g_gdmjRedis.next_pokerid = "next_"
g_gdmjRedis.jbc_list = "hbj_jbc_" --金币场空余的列表

function GdmjModel.GetTableInfo(tableID,tInfo)
	if tInfo == nil then
		tInfo = st_gdmj_pb.gdmjinfo()
	end
	local strGet = redisItem:get(g_gdmjRedis.gdmj_info..tableID, g_gdmjRedis.redis_index)
	if strGet == nil then
		return nil
	end
	tInfo:ParseFromString(strGet)
	return tInfo
end


function GdmjModel.SetTableInfo(tInfo,isModify)   --设置信息，同时设置修改方案
	redisItem:hset(g_gdmjRedis.table_list,tInfo.tableid,tInfo.playernum, g_gdmjRedis.redis_index)  --首先设置人数
	redisItem:set(g_gdmjRedis.gdmj_info..tInfo.tableid,tInfo:SerializeToString(),g_gdmjRedis.redis_index)  --接着设置信息
	if isModify ~= nil and isModify ~= 0 then
		redisItem:hset(g_gdmjRedis.modify_list,tInfo.tableid, isModify, g_gdmjRedis.redis_index)  --如果是修改的，则设置需求修改的信息
	end		
end

function GdmjModel.DelTableInfo(tableID)
	redisItem:hdel(g_gdmjRedis.table_list, tableID, g_gdmjRedis.redis_index)  --首先要在全部的列表中删除
	redisItem:del(g_gdmjRedis.gdmj_info..tableID, g_gdmjRedis.redis_index)
end


function GdmjModel.isModify(tableID)
	if true == redisItem:hexists(g_gdmjRedis.modify_list, tableID,g_gdmjRedis.redis_index) then
		redisItem:hdel(g_gdmjRedis.modify_list, tableID, g_gdmjRedis.redis_index)  --取完后就需要把是否修改的变量删除
		return true
	end
	return false
end

function GdmjModel.GetNewTableList()
	return redisItem:hkeys(g_gdmjRedis.modify_list, g_gdmjRedis.redis_index)
end

function GdmjModel.DelModify(tableID)
	redisItem:hdel(g_gdmjRedis.modify_list, tableID, g_gdmjRedis.redis_index)
end

function GdmjModel.GetUserTableID(userID)
	return redisItem:hget(g_gdmjRedis.gdmj_userlist, userID, g_gdmjRedis.redis_index)
end

function GdmjModel.GetRandomID()
	while true do
		local tableID = math.myrandom(100001,119999)
		
		if false == redisItem:hexists(g_gdmjRedis.table_list, tableID, g_gdmjRedis.redis_index) then   --要检查是否存在了
			redisItem:hset(g_gdmjRedis.table_list, tableID, 0, g_gdmjRedis.redis_index)               --取到的时候立刻就设置
			return tableID
		end
	end	
end

function GdmjModel.GetJettonRandomID()
	--取得一个金币场的ID,在这里配置下发
	while true do
		local tableID = math.myrandom(100001,119999)
		
		if false == redisItem:hexists(g_gdmjRedis.table_list, tableID, g_gdmjRedis.redis_index) then   --要检查是否存在了
			redisItem:hset(g_gdmjRedis.table_list, tableID, 0, g_gdmjRedis.redis_index)               --取到的时候立刻就设置
			return tableID
		end
	end	
end

function GdmjModel.SetUserTableID(userID, tableID) --进入的时候设置
	redisItem:hset(g_gdmjRedis.gdmj_userlist,userID,tableID, g_gdmjRedis.redis_index)
end

function GdmjModel.DelUserTableID(userID) --离开的时候删除

	if userID == 0 then
		return 
	end
	redisItem:hdel(g_gdmjRedis.gdmj_userlist,userID, g_gdmjRedis.redis_index)
end

function GdmjModel.GetTableList()
	return redisItem:hkeys(g_gdmjRedis.table_list, g_gdmjRedis.redis_index)
end

function GdmjModel.SetVipRoomByStr(tableID,strInfo)
	--该函数传过来的是字符串
	redisItem:set(g_gdmjRedis.gdmj_room..tableID,strInfo, g_gdmjRedis.redis_index)
end

function GdmjModel.SetRoomInfo(tableID, roomInfo)
	redisItem:set(g_gdmjRedis.gdmj_room..tableID, roomInfo:SerializeToString(), g_gdmjRedis.redis_index)
end

function GdmjModel.GetRoomInfoStr(tableID)

	local strGet = redisItem:get(g_gdmjRedis.gdmj_room..tableID,g_gdmjRedis.redis_index)
	
	if strGet == nil then
		return nil
	end
	return strGet
end

function GdmjModel.DelRoomInfo(tableID)
	--把牌桌的个性化信息删除
	redisItem:del(g_gdmjRedis.gdmj_room..tableID, g_gdmjRedis.redis_index)
end
--每当有玩家创建房间的时候，需要把每个房间的人数
function GdmjModel.SetUserTableList(userID, tableID)
	
	local redisKey = g_gdmjRedis.user_tablelist..userID
	
	--在这里设置
	redisItem:hset(redisKey,tableID, TimeUtils.GetTime(), g_gdmjRedis.redis_index)
	
end

function GdmjModel.GetUserTableList(userID)
	--获取玩家所有麻将牌桌的列表
	local redisKey = g_gdmjRedis.user_tablelist..userID
	return redisItem:hgetall(redisKey, g_gdmjRedis.redis_index)
end

function GdmjModel.DelUserTableList(userID,tableID)
	--把这个牌桌从房主的牌桌列表中删除，这样就就从房主的棋牌类别中删除了
	local redisKey = g_gdmjRedis.user_tablelist..userID
	redisItem:hdel(redisKey, tableID, g_gdmjRedis.redis_index)
end

function GdmjModel.SitDown(tInfo,mjUser,pos)
	
	if pos < 0 or pos > 4 then
		return 0
	end
	
	if tInfo.situser[pos] ~= 0 then   --如果不为0，表示已经有人坐下来了
		return 0
	end
	tInfo.situser[pos] = mjUser.userid
	
	mjUser.playstate = g_gdmjPlayerState.play_ready --g_gdmjPlayerState.play_waiting  --进入就是等待的状态
	mjUser.chairid = pos
	
	if tInfo.viptable == 0 then
		mjUser.carryjetton = mjUser.psinfo.jetton  --如果不是私人房，那就是
	else
		mjUser.carryjetton = 0 --JulebuService.GetJiFen(tInfo.julebuid, mjUser.userid)     --如果是私人房
		mjUser.psinfo.jetton = 0 --JulebuService.GetJiFen(tInfo.julebuid, mjUser.userid)
		mjUser.julebu_jetton = JulebuService.GetJiFen(tInfo.julebuid, mjUser.userid)
	end
	tInfo.playernum = tInfo.playernum + 1   --只有坐下来的时候，才算一个玩家
	GdmjModel.SetChairUser(tInfo.tableid,mjUser)
	GdmjEvent.JulebuGameUpdate(tInfo,mjUser.userid)
	return pos
end

function GdmjModel.SetChairUser(tableID,mjUser)
	--把每个玩家做下去的信息保存到缓存中
	local redisKey = g_gdmjRedis.chair_user..tableID
	redisItem:hset(redisKey, mjUser.userid, mjUser:SerializeToString(),g_gdmjRedis.redis_index)
end

function GdmjModel.GetChairUser(tableID, userID, mjUser)
	if mjUser == nil then
		mjUser = st_gdmj_pb.gdmjuser()
	end
	
	local redisKey = g_gdmjRedis.chair_user..tableID
	
	local strGet = redisItem:hget(redisKey, userID,g_gdmjRedis.redis_index)
	
	if strGet == nil then
		return nil
	end
	
	mjUser:ParseFromString(strGet)
	return mjUser
end

function GdmjModel.DelChairUserByUserID(tableID, userID)
	local redisKey = g_gdmjRedis.chair_user..tableID
	redisItem:hdel(redisKey, userID,g_gdmjRedis.redis_index )

end

function GdmjModel.DelChairUser(tableID)
	--把玩家在牌子的信息删除
	local redisKey = g_gdmjRedis.chair_user..tableID
	redisItem:del(redisKey, g_gdmjRedis.redis_index)

end

function GdmjModel.SetPlayDetail(tableID, userID, detailInfo)
	local redisKey = g_gdmjRedis.chair_detail..tableID
	redisItem:hset(redisKey, userID, detailInfo:SerializeToString(), g_gdmjRedis.redis_index)
end

function GdmjModel.GetPlayDetail(tableID, userID, detailInfo)
	if detailInfo == nil then
		detailInfo = st_gdmj_pb.gdmjplaydetail()
	end
	local redisKey = g_gdmjRedis.chair_detail..tableID
	
	local strGet = redisItem:hget(redisKey, userID, g_gdmjRedis.redis_index)
	
	if strGet ~= nil then
		detailInfo:ParseFromString(strGet)
	end
	
	return detailInfo
end

function GdmjModel.DelPlayDetail(tableID)
	redisItem:del(g_gdmjRedis.chair_detail..tableID, g_gdmjRedis.redis_index)
end
--这个是调试使用的
function GdmjModel.SetNextCard(tableID,nextCard)
	--这个接口是留给网页端给服务器发送
	local redisKey = g_gdmjRedis.next_pokerid..tableID
	redisItem:set(redisKey, nextCard, g_gdmjRedis.redis_index)
end

function GdmjModel.GetNextCard(tableID)
	local redisKey = g_gdmjRedis.next_pokerid..tableID
	local ret = redisItem:get(redisKey, g_gdmjRedis.redis_index)
	
	if ret == nil then
		return 0 
	else
		redisItem:del(redisKey,g_gdmjRedis.redis_index)
		return tonumber(ret)
	end
end


function GdmjModel.GetJBCTableID(mjType, tableType)
	--在玩家进入金币场的时候，获取的金币场的tableid
	--传入的是麻将类型（晃晃，一脚癞油..）和牌桌的类型：初级，中级，高级
	--在这里用哈希列表
	--首先，要先看看有没有空的牌桌
	--如果没有空的牌桌，就取一个新的牌桌给玩家
	local redisKey = g_gdmjRedis.jbc_list..mjType.."_"..tableType
	local redList = redisItem:hgetall(redisKey, g_gdmjRedis.redis_index)
	for k,v in pairs(redList) do
		if tonumber(v) < 4 then
			return tonumber(k),false  --只要碰到一个不是空牌桌的
		end
	end
	local tableID = math.myrandom(20200000,20300000)
	return tableID,true
end

function GdmjModel.GetJBCTableList(mjType, tableType)
	local redisKey = g_gdmjRedis.jbc_list..mjType.."_"..tableType
	return redisItem:hgetall(redisKey, g_gdmjRedis.redis_index)	
end

function GdmjModel.SetJBCTableID(mjType, tableType, tableID, playNum)
	local redisKey = g_gdmjRedis.jbc_list..mjType.."_"..tableType
	redisItem:hset(redisKey, tableID, playNum, g_gdmjRedis.redis_index)
end

function GdmjModel.DelJCBTableID(mjType, tableType, tableID)
	local redisKey = g_gdmjRedis.jbc_list..mjType.."_"..tableType
	redisItem:hdel(redisKey, tableID,g_gdmjRedis.redis_index )
end