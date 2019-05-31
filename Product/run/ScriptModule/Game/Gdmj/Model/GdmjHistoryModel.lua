
--这个是设置广东麻将历史记录的地方

GdmjHistoryModel = {}
GdmjHistoryModel.redis_index = "gdmj_hty"
g_gdmjHistory = {}
g_gdmjHistory.redis_daycount = "gdmj_daycount"


function GdmjHistoryModel.GetDayCount()
	return redisItem:lpush(GdmjHistoryModel.redis_daycount,1, GdmjHistoryModel.redis_index)
end

--传入的参数包括，每个牌局的frameid,协议ID，协议字符串内容
function GdmjHistoryModel.SetTableAction(tablePlayID,packetID,pkgInfo)
	
	local gcHistory = st_gdmj_pb.gdmjhistorypair()
	gcHistory.packetcode = packetID
	gcHistory.pkginfo = pkgInfo
	
	local redisKey = "gdmj_action_"..tablePlayID
	
	local len = redisItem:hlen(redisKey,GdmjHistoryModel.redis_index)
	len = len + 1
	redisItem:hset(redisKey, len, gcHistory:SerializeToString(),GdmjHistoryModel.redis_index)
	redisItem:expire(redisKey, g_daySeconds, GdmjHistoryModel.redis_index)
end

function GdmjHistoryModel.GetAction(tablePlayID, optID, htyPair)
	if nil == htyPair then
		htyPair = st_gdmj_pb.gdmjhistorypair()
	end
	local redisKey = "gdmj_action_"..tablePlayID
	local strGet = redisItem:hget(redisKey, optID, GdmjHistoryModel.redis_index)
	if strGet == nil or #strGet == 0 then
		return false
	end
	htyPair:ParseFromString(strGet)
end

function GdmjHistoryModel.GetActionLen(tablePlayID)
	return redisItem:hlen("gdmj_action_"..tablePlayID, GdmjHistoryModel.redis_index)
end

function GdmjHistoryModel.SetHistory(tItem, isDissolve, gcAcount)
	--每次在麻将结算的时候调用该函数
	local redisKey = "gdmjhtyinfo_"..math.floor(tItem.m_tInfo.frameid/100)
	
	local mjHistory = st_gdmj_pb.gdmjhistory()
	
	
	
	if true == redisItem:exists(redisKey, GdmjHistoryModel.redis_index) then
		local strGet = redisItem:get(redisKey, GdmjHistoryModel.redis_index)
		mjHistory:ParseFromString(strGet)
	end
	
	
	if #mjHistory.hitemlist == 0 then
		--这是第一局
		mjHistory.tableid = tItem.m_tInfo.tableid
		mjHistory.tablemark = math.floor(tItem.m_tInfo.frameid/100)   --等于第一场的ID
		mjHistory.gametime = TimeUtils.GetTime()
		
		local bankerUser = false
		
		for k,v in ipairs(tItem.m_userList) do
			mjHistory.useridlist:append(v.psinfo.userid)
			mjHistory.nicklist:append(v.psinfo.nickname)
			mjHistory.winlist:append(0)
			
			GdmjHistoryModel.SetPlayerHistory(v.psinfo.userid,mjHistory.tablemark)  --在这里就设置好了，为什么在这里设置呢，自己想
			
			if v.psinfo.userid == tItem.m_tInfo.tableuserid then
				bankerUser = true
			end
			
		end
		
		if bankerUser == false then
			GdmjHistoryModel.SetPlayerHistory(tItem.m_tInfo.tableuserid, mjHistory.tablemark)  --在这里就设置好了，为什么在这里设置呢，自己想
		end
		
	end
	
	local addItem = mjHistory.hitemlist:add()
	addItem.tableplayid = tItem.m_tInfo.frameid
	addItem.tableid = tItem.m_tInfo.tableid
	addItem.playtime = TimeUtils.GetTime()

	
	if isDissolve == 1 then
		
		addItem.playstate = 1
		addItem.quitdes = "房间解散"
		for i = 1,tItem.m_maxUser do
			addItem.userdes:append("同意解散")
			--mjHistory.winlist[i] = tItem.m_userList[i].psinfo.jetton
		end
	else
		for k,v in ipairs(tItem.m_userList) do
			addItem.useridlist:append(v.psinfo.userid)
			addItem.nicklist:append(v.psinfo.nickname)
			addItem.winlist:append(0)
			mjHistory.winlist[k] = v.psinfo.jetton
			addItem.winlist[k] = gcAcount.countlist[k].winjetton
		end	
	end
	

	
	
	mjHistory.playnum = #mjHistory.hitemlist
	
	redisItem:set(redisKey,mjHistory:SerializeToString(), GdmjHistoryModel.redis_index)
	redisItem:expire(redisKey, g_daySeconds*7, GdmjHistoryModel.redis_index) --设置三天的时间
	--保存一天
	
	if tItem.m_tInfo.viptable == 2 then
		--只有这种情况下，下发送战报
		local userArr = {}
		for i = 1,tItem.m_maxUser do
			if tItem.m_tInfo.situser[i] ~= 0 then
				local addTemp = {}
				addTemp['userid'] = tItem.m_userList[i].userid
				addTemp['faceid'] = tItem.m_userList[i].psinfo.face_1
				addTemp['nick'] = tItem.m_userList[i].psinfo.nickname
				addTemp['jifen'] = tItem.m_userList[i].psinfo.jetton
				table.insert(userArr,addTemp)
				--把同步的积分放到这里来同步，在GameCount中不做同步了
				GdmjEvent.AddJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid, tItem.m_userList[i].psinfo.jetton,tItem.m_tInfo.tableid,tItem.m_tInfo.julebutype)
			end
		end		
		GdmjEvent.JulebuGameCount(tItem.m_tInfo.julebuid, tItem.m_tInfo.tableid, userArr)
	end
end


function GdmjHistoryModel.GetHistroy(tableMark,htyItem)
	
	local redisKey = "gdmjhtyinfo_"..tableMark
	
	if htyItem == nil then
		htyItem = st_gdmj_pb.gdmjhistory()
	end
	
	local strGet = redisItem:get(redisKey, GdmjHistoryModel.redis_index )
	
	if strGet == nil or #strGet == 0 then
		return nil
	end
	
	htyItem:ParseFromString(strGet)

	return htyItem
	
end

function GdmjHistoryModel.HistoryExist(tableMark)
	
	return redisItem:exists("gdmjhtyinfo_"..tableMark,GdmjHistoryModel.redis_index )
end

function GdmjHistoryModel.SetPlayerHistory(userID,tableMark)

	local redisKey = "gdmjhtyuserid_"..userID
	redisItem:lpush(redisKey,tableMark, GdmjHistoryModel.redis_index)
end

function GdmjHistoryModel.GetPlayerHistory(userID, pageNum)
	return redisItem:lrange("gdmjhtyuserid_"..userID, (pageNum - 1)*10, pageNum*10 - 1, GdmjHistoryModel.redis_index)
end




