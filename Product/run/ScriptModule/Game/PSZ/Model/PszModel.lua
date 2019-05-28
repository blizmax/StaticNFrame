PszModel = {}

g_pszRedis = {}
g_pszRedis.redis_index = "psz"
g_pszRedis.psz_userlist = "psz_userlist"
g_pszRedis.psz_info = "psz_info"
g_pszRedis.modify_list = "psz_modify"
g_pszRedis.table_list = "psz_list"
g_pszRedis.douniu_room = "psz_room"
g_pszRedis.user_tablelist = "psz_utlist_" --没当有玩家创建房间的时候，需要在这个列表中把tableid设置好,表示是玩家自己的列表
g_pszRedis.chair_user = "psz_chair_"
g_pszRedis.chair_detail = "psz_play_"
g_pszRedis.next_pokerid = "next_"
g_pszRedis.pour_index = "pour_"
g_pszRedis.user_index = "user_"
g_pszRedis.new_situser = "new_situser_"
g_pszRedis.newsit_list ="psz_newsit"

g_pszRedis.elementary_list = "psz_elementary_list" 	--初级场
g_pszRedis.middle_list = "psz_middle_list"			--中级场
g_pszRedis.jadvanced_list = "psz_jadvanced_list"	--高级场

g_pszRedis.psz_tbl_hty = "psz_tbl_hty_"    --牌桌的历史记录
g_pszRedis.psz_user_hty = "psz_user_hty_"  --玩家个人的历史记录

g_pszRedis.debug_card = "psz_debug"

--把自己加入房间
function PszModel.SetUserTableID(userID, tableID) --进入的时候设置
	redisItem:hset(g_pszRedis.psz_userlist,userID,tableID, g_pszRedis.redis_index)
end
--获得自己在那个房间
function PszModel.GetUserTableID(userID)
	return redisItem:hget(g_pszRedis.psz_userlist, userID, g_pszRedis.redis_index)
end
--退出所在的房间
function PszModel.DelUserTableID(userID) --离开的时候删除
	redisItem:hdel(g_pszRedis.psz_userlist,userID, g_pszRedis.redis_index)
end



--把房间号加入玩家的房间列表
function PszModel.SetUserTableList(userID, tableID)
	
	local redisKey = g_pszRedis.user_tablelist..userID
	
	--在这里设置
	redisItem:hset(redisKey,tableID, TimeUtils.GetTime(), g_pszRedis.redis_index)
	
end
--获取玩家的房间列表
function PszModel.GetUserTableList(userID)
	local redisKey = g_pszRedis.user_tablelist..userID
	return redisItem:hgetall(redisKey, g_pszRedis.redis_index)
end
--把房间号从玩家的房间列表中删除
function PszModel.DelUserTableList(userID,tableID)
	local redisKey = g_pszRedis.user_tablelist..userID
	redisItem:hdel(redisKey, tableID, g_pszRedis.redis_index)
end



--保存房间信息
function PszModel.SetTableInfo(tInfo,isModify)   
	redisItem:hset(g_pszRedis.table_list,tInfo.tableid,tInfo.usernum, g_pszRedis.redis_index)  --首先设置人数
	redisItem:set(g_pszRedis.psz_info..tInfo.tableid,tInfo:SerializeToString(),g_pszRedis.redis_index)  --接着设置信息
	if isModify ~= nil and isModify ~= 0 then
		redisItem:hset(g_pszRedis.modify_list,tInfo.tableid, isModify, g_pszRedis.redis_index)  --如果是修改的，则设置需求修改的信息
	end		
end
--获取房间信息
function PszModel.GetTableInfo(tableID,tInfo)
	if tInfo == nil then
		tInfo = st_psz_pb.pszinfo()
	end
	local strGet = redisItem:get(g_pszRedis.psz_info..tableID, g_pszRedis.redis_index)
	if strGet == nil then
		return nil
	end
	tInfo:ParseFromString(strGet)
	return tInfo
end
--删除房间信息
function PszModel.DelTableInfo(tableID)
	redisItem:hdel(g_pszRedis.table_list, tableID, g_pszRedis.redis_index)  --首先要在全部的列表中删除
	redisItem:del(g_pszRedis.psz_info..tableID, g_pszRedis.redis_index)
end


--获得随机的房号
function PszModel.GetRandomID()
	while true do
		local tableID = math.myrandom(210000,219000)
		if false == redisItem:hexists(g_pszRedis.table_list, tableID, g_pszRedis.redis_index) then   --要检查是否存在了
			redisItem:hset(g_pszRedis.table_list, tableID, 0, g_pszRedis.redis_index)               --取到的时候立刻就设置
			return tableID
		end
	end	
end
--获得所有的房间列表
function PszModel.GetTableList()
	return redisItem:hkeys(g_pszRedis.table_list, g_pszRedis.redis_index)
end




--检查房间是否有更新操作
function PszModel.isModify(tableID)
	if true == redisItem:hexists(g_pszRedis.modify_list, tableID,g_pszRedis.redis_index) then
		redisItem:hdel(g_pszRedis.modify_list, tableID, g_pszRedis.redis_index)  --取完后就需要把是否修改的变量删除
		return true
	end
	return false
end
--获得有更新操作的房间列表
function PszModel.GetNewTableList()
	return redisItem:hkeys(g_pszRedis.modify_list, g_pszRedis.redis_index)
end
--从有更新操作的列表中删除房间号
function PszModel.DelModify(tableID)
	redisItem:hdel(g_pszRedis.modify_list, tableID, g_pszRedis.redis_index)
end



--暂存当局数据
function PszModel.SetHistory(tInfo, isDissolve)
	--每次在麻将结算的时候调用该函数
	local redisKey = g_pszRedis.psz_tbl_hty..tInfo.tableid
	
	local pszHistory = st_psz_pb.pszhistory()
	
	if true == redisItem:exists(redisKey, g_pszRedis.redis_index) then
		local strGet = redisItem:get(redisKey, g_pszRedis.redis_index)
		pszHistory:ParseFromString(strGet)
	end
	
	if #pszHistory.hitemlist == 0 then
		--这是第一局		
		pszHistory.tableid = tInfo.tableid
		pszHistory.playnummax = tInfo.maxplaynum
		pszHistory.difen = tInfo.difen
		pszHistory.ownerid = tInfo.ownerid		
	end
	
		
	if isDissolve == 0 then
		local addItem = pszHistory.hitemlist:add()
		addItem.leg = tInfo.usevipnum
		addItem.guodi = tInfo.guodi
		addItem.presentbout = tInfo.presentbout
		addItem.oncepour = tInfo.oncepour
		addItem.winerid = tInfo.actionuser

		for k,v in ipairs(tInfo.situser) do
			local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
			addItem.useridlist:append(v.userid)
			addItem.nicklist:append(tempInfo.nickname)
			addItem.jetton:append(v.jetton)
			addItem.psztypelist:append(v.psztype)
			if v.pokerstate_discard == 2 then
				addItem.resultlist:append(v.pokerstate_discard)
			else
				if v.compare == 1 then
					addItem.resultlist:append(v.compare)
				else
					addItem.resultlist:append(0)
				end
			end
			
			if tInfo.actionuser == v.userid then
				addItem.winlist:append(tInfo.guodi - v.jetton)
			else 
				addItem.winlist:append( 0 - v.jetton)
			end
			
			if v.basicsmul == 0 then
				v.basicsmul = 1
			end

			local poker = addItem.pokerlist:add()
			for k,t in ipairs(v.handpoker) do
				poker.handpoker:append(t)
			end
		end
		
	end
	
	if tInfo.usevipnum == tInfo.maxplaynum or isDissolve == 1  then
		--向俱乐部发送战绩
		pszHistory.gametime = TimeUtils.GetTime()
		for k,v in ipairs(tInfo.situser) do
			local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
			local isEX = false 
			local pos = 0
			for k = 1, #pszHistory.useridlist do
				if v.userid == pszHistory.useridlist[k] then
					pos = k
					isEX = true
				end
			end
			if isEX == false then
				pszHistory.useridlist:append(v.userid)
				pszHistory.nicklist:append(tempInfo.nickname)
				pszHistory.winlist:append(v.countjetton)
			else
				pszHistory.winlist[pos] = pszHistory.winlist[pos] + v.countjetton
			end
			--结算魅力值
			
		end
		local userArrtTemp = {}
		for z ,y in ipairs(pszHistory.useridlist) do 
			pszHistory.playnum = #pszHistory.hitemlist
			PszModel.SetPlayerHistory(y,pszHistory)  --在这里就设置好了，为什么在这里设置呢，自己想 
		
			pInfo = PlayerModel.GetPlayerInfo(y)
			local tmp = {}
			tmp["userid"] = y
			tmp["nick"] = pInfo.nickname
			tmp["faceid"] = pInfo.face_1
			tmp["jifen"] = pszHistory.winlist[z]
			table.insert(userArrtTemp,tmp)		
		end
		PszEvent.JulebuGameCount(tInfo, userArrtTemp)
		PszModel.DelHistroy(tInfo.tableid)
		return
	end
	redisItem:set(redisKey, pszHistory:SerializeToString(), g_pszRedis.redis_index)
end
--获得暂存当局数据
function PszModel.GetHistroy(tableid,htyItem)
	
	local redisKey = g_pszRedis.psz_tbl_hty..tableid
	
	if htyItem == nil then
		htyItem = st_psz_pb.pszhistory()
	end
	
	local strGet = redisItem:get(redisKey, g_pszRedis.redis_index )
	
	if strGet == nil or #strGet == 0 then
		return nil
	end
	
	htyItem:ParseFromString(strGet)

	return htyItem
	
end
--删除暂存当局数据
function PszModel.DelHistroy(tableid)
	local redisKey = g_pszRedis.psz_tbl_hty..tableid
	redisItem:del(redisKey, g_pszRedis.redis_index)
end
--把数据加载到玩家的个人战绩
function PszModel.SetPlayerHistory(userID,htyItem)

	local redisKey = g_pszRedis.psz_user_hty..userID
	local len = redisItem:lpush(redisKey,htyItem:SerializeToString(), g_pszRedis.redis_index)
	if len > 50 then
		--最低只是保存50个
		redisItem:rpop(redisKey, g_pszRedis.redis_index)
	end	
end
--获得玩家的个人战绩
function PszModel.GetPlayerHistory(userID, pageNum)
	local tmp = redisItem:lrange(g_pszRedis.psz_user_hty..userID, (pageNum - 1)*5, pageNum*5 - 1, g_pszRedis.redis_index)
	return tmp
end



--保存金币房列表
function PszModel.SetJettonTableList(classType,tableID)
	
	local redisKey 
	if classType == g_PszDefine.table_elementary then
		redisKey = g_pszRedis.elementary_list
	elseif classType == g_PszDefine.table_middle then
		redisKey = g_pszRedis.middle_list
	elseif classType == g_PszDefine.table_jadvanced then
		redisKey = g_pszRedis.jadvanced_list
	end
	 
	--在这里设置
	redisItem:hset(redisKey,tableID, g_pszRedis.redis_index)
	
end
--获取金币房列表
function PszModel.GetJettonTableList(classType)
	local redisKey 
	if classType == g_PszDefine.table_elementary then
		redisKey = g_pszRedis.elementary_list
	elseif classType == g_PszDefine.table_middle then
		redisKey = g_pszRedis.middle_list
	elseif classType == g_PszDefine.table_jadvanced then
		redisKey = g_pszRedis.jadvanced_list
	end

	
	
	return redisItem:hkeys(redisKey, g_pszRedis.redis_index)
end
--把房间号从金币房列表中删除
function PszModel.DelJettonTableList(classType,tableID)
	local redisKey 
	if classType == g_PszDefine.table_elementary then
		redisKey = g_pszRedis.elementary_list
		dizhu =  100
	elseif classType == g_PszDefine.table_middle then
		redisKey = g_pszRedis.middle_list
		dizhu =  500
	elseif classType == g_PszDefine.table_jadvanced then
		redisKey = g_pszRedis.jadvanced_list
		dizhu =  1000
	end
	redisItem:hdel(redisKey, tableID, g_pszRedis.redis_index)
end



function PszModel.SetDebugNextCard(tableID, userID, cardList)
	redisItem:hset(g_pszRedis.debug_card, tableID..userID,cardList, g_pszRedis.redis_index)
end

function PszModel.GetDebugNextCard(tableID, userID)
	local cardList = redisItem:hget(g_pszRedis.debug_card, tableID..userID, g_pszRedis.redis_index)
	redisItem:hdel(g_pszRedis.debug_card, tableID..userID, g_pszRedis.redis_index)
	return cardList
end