DouNiuModel = {}

g_douniuRedis = {}
g_douniuRedis.redis_index = "douniu"
g_douniuRedis.douniu_userlist = "douniu_userlist"
g_douniuRedis.douniu_info = "douniu_info"
g_douniuRedis.modify_list = "douniu_modify"
g_douniuRedis.table_list = "douniu_list"
g_douniuRedis.douniu_room = "douniu_room"
g_douniuRedis.user_tablelist = "douniu_utlist_" --没当有玩家创建房间的时候，需要在这个列表中把tableid设置好,表示是玩家自己的列表
g_douniuRedis.chair_user = "douniu_chair_"
g_douniuRedis.chair_detail = "douniu_play_"
g_douniuRedis.next_pokerid = "next_"
g_douniuRedis.pour_index = "pour_"
g_douniuRedis.user_index = "user_"
g_douniuRedis.new_situser = "new_situser_"
g_douniuRedis.newsit_list ="douniu_newsit"

g_douniuRedis.douniu_tbl_hty = "douniu_tbl_hty_"    --牌桌的历史记录
g_douniuRedis.douniu_user_hty = "douniu_user_hty_"  --玩家个人的历史记录


g_douniuRedis.debug_card = "douniu_debug"



function DouNiuModel:UpdateFromRedis(tInfo)
	--稍后实现该结构
	if true == DouNiuModel.isModify(tInfo.tableid) then
		--表示需要刷新的
		local Info0 = DouNiuModel.GetTableInfo(tInfo.tableid)
		tInfo = Info0
		
		for k,v in ipairs(tInfo.situser) do
			if v ~= 0 then
				local dnUser = DouNiuModel.GetChairUser(tInfo.tableid,v)
				if dnUser ~= nil then
					tInfo.situser[k] = dnUser     --同时也更新这个数据
				end
				local userDtail = DouNiuModel.GetPlayDetail(tInfo.tableid, v)
				if userDtail ~= nil then
				--	tInfo.m_detailList[k] = userDtail
				end
			end
		end
	end
end

function DouNiuModel.GetUserList(tInfo, execptID)
	local retList = {}
	for k,v in ipairs(tInfo.situser) do
		if v.userid ~= 0 and v.userid ~= execptID then
			table.insert(retList,v.userid)
		end
	end
	for k,v in ipairs(tInfo.standuser) do
		if v ~= execptID then
			table.insert(retList, v)
		end
	end 
	return retList
end

function DouNiuModel.GetTableInfo(tableID,tInfo)
	if tInfo == nil then
		tInfo = st_douniu_pb.douniuinfo()
	end
	local strGet = redisItem:get(g_douniuRedis.douniu_info..tableID, g_douniuRedis.redis_index)
	if strGet == nil then
		return nil
	end
	tInfo:ParseFromString(strGet)
	return tInfo
end

function DouNiuModel.SetTableInfo(tInfo,isModify)   --设置信息，同时设置修改方案
	redisItem:hset(g_douniuRedis.table_list,tInfo.tableid,tInfo.usernum, g_douniuRedis.redis_index)  --首先设置人数
	redisItem:set(g_douniuRedis.douniu_info..tInfo.tableid,tInfo:SerializeToString(),g_douniuRedis.redis_index)  --接着设置信息
	if isModify ~= nil and isModify ~= 0 then
		redisItem:hset(g_douniuRedis.modify_list,tInfo.tableid, isModify, g_douniuRedis.redis_index)  --如果是修改的，则设置需求修改的信息
	end		
end

function DouNiuModel.DelTableInfo(tableID)
	redisItem:hdel(g_douniuRedis.table_list, tableID, g_douniuRedis.redis_index)  --首先要在全部的列表中删除
	redisItem:del(g_douniuRedis.douniu_info..tableID, g_douniuRedis.redis_index)
end

function DouNiuModel.GetTableList()
	return redisItem:hkeys(g_douniuRedis.table_list, g_douniuRedis.redis_index)
end

function DouNiuModel.SetChairUser(tableID,dnUser)
	--把每个玩家做下去的信息保存到缓存中
	local redisKey = g_douniuRedis.chair_user..tableID
	redisItem:hset(redisKey, dnUser.userid, dnUser:SerializeToString(),g_douniuRedis.redis_index)
end

function DouNiuModel.GetChairUser(tableID, userID, dnUser)
	if dnUser == nil then
		dnUser = st_douniu_pb.douniuuser()
	end
	
	local redisKey = g_douniuRedis.chair_user..tableID
	
	local strGet = redisItem:hget(redisKey, userID,g_douniuRedis.redis_index)
	
	if strGet == nil then
		return nil
	end
	
	dnUser:ParseFromString(strGet)
	return dnUser
end

function DouNiuModel.DelChairUser(tableID)
	--把玩家在牌子的信息删除
	local redisKey = g_douniuRedis.chair_user..tableID
	redisItem:del(redisKey, g_douniuRedis.redis_index)
end

function DouNiuModel.GetNewTableList()
	return redisItem:hkeys(g_douniuRedis.modify_list, g_douniuRedis.redis_index)
end

function DouNiuModel.isModify(tableID)
	if true == redisItem:hexists(g_douniuRedis.modify_list, tableID,g_douniuRedis.redis_index) then
		redisItem:hdel(g_douniuRedis.modify_list, tableID, g_douniuRedis.redis_index)  --取完后就需要把是否修改的变量删除
		return true
	end
	return false
end

function DouNiuModel.DelModify(tableID)
	redisItem:hdel(g_douniuRedis.modify_list, tableID, g_douniuRedis.redis_index)
end

function DouNiuModel.SetUserTableID(userID, tableID) --进入的时候设置
	redisItem:hset(g_douniuRedis.douniu_userlist,userID,tableID, g_douniuRedis.redis_index)
end

function DouNiuModel.GetUserTableID(userID)
	return redisItem:hget(g_douniuRedis.douniu_userlist, userID, g_douniuRedis.redis_index)
end

function DouNiuModel.DelUserTableID(userID) --离开的时候删除
	redisItem:hdel(g_douniuRedis.douniu_userlist,userID, g_douniuRedis.redis_index)
end

function DouNiuModel.GetRandomID()
	while true do
		local tableID = math.myrandom(190000,200000)
		if false == redisItem:hexists(g_douniuRedis.table_list, tableID, g_douniuRedis.redis_index) then   --要检查是否存在了
			redisItem:hset(g_douniuRedis.table_list, tableID, 0, g_douniuRedis.redis_index)               --取到的时候立刻就设置
			return tableID
		end
	end	
end

--每当有玩家创建房间的时候，需要把每个房间的人数
function DouNiuModel.SetUserTableList(userID, tableID)
	
	local redisKey = g_douniuRedis.user_tablelist..userID
	
	--在这里设置
	redisItem:hset(redisKey,tableID, TimeUtils.GetTime(), g_douniuRedis.redis_index)
	
end

function DouNiuModel.GetUserTableList(userID)
	--获取玩家所有麻将牌桌的列表
	local redisKey = g_douniuRedis.user_tablelist..userID
	return redisItem:hgetall(redisKey, g_douniuRedis.redis_index)
end

function DouNiuModel.DelUserTableList(userID,tableID)
	--把这个牌桌从房主的牌桌列表中删除，这样就就从房主的棋牌类别中删除了
	local redisKey = g_douniuRedis.user_tablelist..userID
	redisItem:hdel(redisKey, tableID, g_douniuRedis.redis_index)
end

function DouNiuModel.SetDnUserInfo(tableID,userInfo)
	--把玩家下注的底分存进缓存
	local redisKey = g_douniuRedis.user_index..tableID
	redisItem:hset(redisKey,userInfo.userid, userInfo, g_douniuRedis.redis_index)		
end

function DouNiuModel.GetDnUserInfo(tableID,userInfo)
	--把玩家下注的底分存进缓存
	local redisKey = g_douniuRedis.user_index..tableID
	return redisItem:hgetall(redisKey, g_douniuRedis.redis_index)		
end

function DouNiuModel.DelDnUserInfo(tableID,userID)
		
	local redisKey = g_douniuRedis.user_index..tableID
	redisItem:hdel(redisKey, userID, g_douniuRedis.redis_index)  --首先要在全部的列表中删除
end

function DouNiuModel.SetVipRoomByStr(tableID,strInfo)
	--该函数传过来的是字符串
	redisItem:set(g_douniuRedis.douniu_room..tableID,strInfo, g_douniuRedis.redis_index)
end

function DouNiuModel.GetRoomInfoStr(tableID)

	local strGet = redisItem:get(g_douniuRedis.douniu_room..tableID,g_douniuRedis.redis_index)
	
	if strGet == nil then
		return nil
	end
	return strGet
end

function DouNiuModel.SitDown(tInfo,dnUser,pos)
	
	if pos < 0 or pos > 8 then
		return 0
	end
	
	if tInfo.situser[pos] ~= 0 then   --如果不为0，表示已经有人坐下来了
		return 0
	end
	tInfo.situser[pos] = dnUser.userid
	
	--dnUser.playstate = g_douniuPlayerState.play_waiting  --进入就是等待的状态
	dnUser.state = 1
	dnUser.chairid = pos
	
	if tInfo.viptable == 0 then
		dnUser.carryjetton = dnUser.psinfo.jetton  --如果不是私人房，那就是
	else
		dnUser.carryjetton = 0     --如果是私人房
		dnUser.psinfo.jetton = 0
	end
	tInfo.usernum = tInfo.usernum + 1   --只有坐下来的时候，才算一个玩家
	DouNiuModel.SetChairUser(tInfo.tableid,dnUser)
	return pos
end

function DouNiuModel.GetPlayDetail(tableID, userID, detailInfo)

	local redisKey = g_douniuRedis.chair_detail..tableID
	
	local strGet = redisItem:hget(redisKey, userID, g_gdmjRedis.redis_index)
	
	if strGet ~= nil then
		detailInfo:ParseFromString(strGet)
	end
	
	return detailInfo
end

--对于历史记录，在游戏进行的时候，先把结构体存到缓存中。
--在游戏结束的时候，统一放到缓存中
function DouNiuModel.SetHistroy(tableID, htyInfo)
	redisItem:set( g_douniuRedis.douniu_tbl_hty..tableID, htyInfo:SerializeToString(), g_douniuRedis.redis_index  )
	
end

function DouNiuModel.GetHistroy(tableID,htyInfo)
	
	if true == redisItem:exists(g_douniuRedis.douniu_tbl_hty..tableID, g_douniuRedis.redis_index) then
		local strGet = redisItem:get(g_douniuRedis.douniu_tbl_hty..tableID, g_douniuRedis.redis_index )
		htyInfo:ParseFromString(strGet)
	end
	
end

function DouNiuModel.DelHistroy(tableID)
	redisItem:del(g_douniuRedis.douniu_tbl_hty..tableID, SdbModel.redis_index)
end

function DouNiuModel.SetUserHistory(userID,htyInfo)

	local redisKey = g_douniuRedis.douniu_user_hty ..userID
	local len = redisItem:lpush(redisKey,htyInfo:SerializeToString(), SdbModel.redis_index)
	
	if len > 50 then
		--最低只是保存50个
		redisItem:rpop(redisKey, SdbModel.redis_index)
	end	
end

function DouNiuModel.GetUserHistory(userID, pageNum)
	return redisItem:lrange(g_douniuRedis.douniu_user_hty ..userID, (pageNum - 1)*10, pageNum*10 - 1, SdbModel.redis_index)
end


function DouNiuModel.SetDebugNextCard(tableID, userID, cardList)
	redisItem:hset(g_douniuRedis.debug_card, tableID..userID,cardList, g_douniuRedis.redis_index)
end

function DouNiuModel.GetDebugNextCard(tableID, userID)
	local cardList = redisItem:hget(g_douniuRedis.debug_card, tableID..userID, g_douniuRedis.redis_index)
	redisItem:hdel(g_douniuRedis.debug_card, tableID..userID, g_douniuRedis.redis_index)
	return cardList
end
