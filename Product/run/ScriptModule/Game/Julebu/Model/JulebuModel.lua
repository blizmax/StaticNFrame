
JulebuModel = {}
JulebuModel.redis_index = "redis_julebu"
JulebuModel.julebu_info = "julebu_info_"
JulebuModel.user_julebu_list = "u_jlb_l_"

JulebuModel.apply_list = "jlb_apply_"
JulebuModel.member_list = "jlb_member_"
JulebuModel.online_list = "jlb_online_"
JulebuModel.game_info = "jlb_gameinfo_"  --这个是保存玩家的信息的
JulebuModel.game_list = "jlb_gamelist_"  --这个是保存时间的先后顺序的

JulebuModel.chat_id = "jlb_chatid_"  --这个是保存时间的先后顺序的
JulebuModel.chat_list = "jlb_chatlist_"  --这个是保存时间的先后顺序的
JulebuModel.jifen_today = "jlb_jf_day_"  --每天实时保存积分的信息
JulebuModel.history_list= "jlb_hty_list_"  --每天实时保存积分的信息
JulebuModel.day_history= "jlb_hty_day_"  --每天实时保存积分的信息
JulebuModel.zhanji_list= "jlb_user_zhanji_"  --这个是玩家按照天算的战绩，先保存第一天，每天结算的时候，把key改成对应结算时间,同时保存20天

JulebuModel.member_day_list= "jlb_member_day_"  --保存玩家的记录信息

JulebuModel.count_time = "jlb_count_time"   --记录俱乐部结算的时间 ,这是一个哈希列表


JulebuModel.playingmethod_info = "playingmethod_info_"   --记录俱乐部玩法设置
JulebuModel.limitjifenhistory = "limitjifenhistory_"   --记录俱乐部上下分

JulebuModel.aututable = "jlb_aututable_"   --记录俱乐部玩法设置

JulebuModel.partnerinfo = "jlb_partnerinfo_"   --记录俱乐部合伙人信息
JulebuModel.JulebuRecord = "jlb_JulebuRecord_"   --记录俱乐部合伙人信息

JulebuModel.JulebuDelPartner = "jlb_JulebuDelPartner_"   --要被删除数据的俱乐部合伙人

JulebuModel.JulebuGetJifen = "jlb_get_jifen_"

JulebuModel.JulebuJifenHistory = "jlb_jifen_hty_"


function JulebuModel.GetRedisKey(userID)
	return "chat_"..userID
end

function JulebuModel.CreateJulebu(jInfo)
	local rsDate = TimeUtils.GetTimeString()
	
	local modulesList = {}
	local adminlist = {}
	local partnerlist = {}
	
	for i = 1,#jInfo.gamemodules do
		table.insert(modulesList, jInfo.gamemodules[i])
	end
	for i = 1, #jInfo.adminlist do
		table.insert(adminlist, jInfo.adminlist[i])
	end
	
	for i = 1, #jInfo.partnerlist do
		table.insert(partnerlist, jInfo.partnerlist[i])
	end
	
	local sqlCase = "insert into dy_julebu(name,ownerid,rsdate,maxmembernum,membernum,gamemodules,adminlist,channel,partnerlist) values('"..
	jInfo.name.."',"..jInfo.ownerid..",'"..rsDate.."',"..jInfo.maxmembernum..","..jInfo.membernum..",'"..luajson.encode(modulesList)..
	"','"..luajson.encode(adminlist).."','"..g_servername.."','"..luajson.encode(partnerlist).."')"   --加入渠道统计在里面，方便区分不同渠道
	mysqlItem:execute(sqlCase)
	sqlCase = "select id from dy_julebu where ownerid="..jInfo.ownerid.." and rsdate='"..rsDate.."'"
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch()
	if sqlData == nil then
		return nil
	end
	jInfo.julebuid = tonumber(sqlData)
	--把房主也插入到房间信息里面
	local sqlCase="insert into dy_julebu_member(userid,julebuid,status) values("..jInfo.ownerid..","..jInfo.julebuid..","..g_JulebuDefine.mstatus_normal..")"
	mysqlItem:execute(sqlCase)
	return jInfo
end

function JulebuModel.GetJulebuInfo(julebuID, jInfo)
	
	if jInfo == nil then
		jInfo = st_julebu_pb.julebuinfo()
	end
	
	local strGet = redisItem:get(JulebuModel.julebu_info..julebuID, JulebuModel.redis_index)
	if strGet ~= nil then
		jInfo:ParseFromString(strGet)
		return jInfo
	end
	return JulebuModel.LoadJulebuInfo(julebuID, jInfo)
end



function JulebuModel.LoadJulebuInfo(julebuID, jInfo)
	if jInfo == nil then
		jInfo = st_julebu_pb.julebuinfo()
	end
	local sqlCase = "select * from dy_julebu where status=99 and id="..julebuID  
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		return nil
	end
	JulebuModel.LoadMemberList(julebuID)
	jInfo.julebuid = tonumber(sqlData[1])
	jInfo.name = sqlData[2]
	jInfo.ownerid = tonumber(sqlData[3])
	jInfo.maxmembernum = tonumber(sqlData[5])
	jInfo.membernum = tonumber(sqlData[6])
	jInfo.notice = sqlData[7]
	local jsonArr = luajson.decode(sqlData[8])
	for k,v in ipairs(jsonArr) do
		jInfo.gamemodules:append(tonumber(v))
	end
	jsonArr = luajson.decode(sqlData[9])
	for k,v in ipairs(jsonArr) do
		jInfo.adminlist:append(tonumber(v))
	end
	if #sqlData >= 15 and #sqlData[15] > 0 then
		jsonArr = luajson.decode(sqlData[15])
		for k,v in ipairs(jsonArr) do
			jInfo.partnerlist:append(tonumber(v))
		end
	end
	jInfo.setroom = 0
	jInfo.autoroom = 0
	jInfo.gamecount = 2000
	jInfo.bankercount = 1000
	jInfo.ruleset = 1
	jInfo.faceaward_figure = 1
	jInfo.faceaward_num = 1
	jInfo.faceaward_count = 1
	JulebuModel.SetJulebuInfo(jInfo)
	return jInfo
end

function JulebuModel.SetJulebuInfo(jInfo)   --设置俱乐部的信息
	redisItem:set(JulebuModel.julebu_info..jInfo.julebuid,jInfo:SerializeToString(),JulebuModel.redis_index)
end

function JulebuModel.DelJulebuInfo(julebuID)
	redisItem:del(JulebuModel.julebu_info..julebuID, JulebuModel.redis_index)
	redisItem:del(JulebuModel.apply_list..julebuID, JulebuModel.redis_index)
	redisItem:del(JulebuModel.member_list..julebuID, JulebuModel.redis_index)
	redisItem:del(JulebuModel.online_list..julebuID, JulebuModel.redis_index)
end

function JulebuModel.SetUserJulebuList(userID, julebuID)
	--设置玩家的俱乐部列表
	redisItem:hset(JulebuModel.user_julebu_list..userID, julebuID, julebuID, JulebuModel.redis_index)
end

function JulebuModel.GetUserJulebuList(userID)
	if true == redisItem:exists(JulebuModel.user_julebu_list..userID, JulebuModel.redis_index) then
		return redisItem:hvals(JulebuModel.user_julebu_list..userID, JulebuModel.redis_index)
	end
	local tList = {}
	local sqlCase = "select julebuid from dy_julebu_member where userid="..userID.." and status="..g_JulebuDefine.mstatus_normal
	mysqlItem:executeQuery(sqlCase)
	
	while true do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		table.insert(tList, tonumber(sqlData[1]))
		JulebuModel.SetUserJulebuList(userID, tonumber(sqlData[1]))
	end
	
	return tList
end





function JulebuModel.IsJoinIn(userID, julebuID)   --检查是否已经加入这个俱乐部
	
	if julebuID == 100 then
		return true
	end
	
	return redisItem:hexists(JulebuModel.user_julebu_list..userID,julebuID, JulebuModel.redis_index)
end

function JulebuModel.DelFromUserJulebuList(userID, julebuID)   --检查是否已经加入这个俱乐部
	return redisItem:hdel(JulebuModel.user_julebu_list..userID,julebuID, JulebuModel.redis_index)
end

function JulebuModel.GetApplyList(julebuID)
	if true == redisItem:exists(JulebuModel.apply_list..julebuID, JulebuModel.redis_index) then
		return redisItem:hgetall(JulebuModel.apply_list..julebuID, JulebuModel.redis_index)
	end
	return JulebuModel.LoadApplyList(julebuID)
end

function JulebuModel.LoadApplyList(julebuID)
	local sqlCase = "select id,userid from dy_julebu_member where julebuid="..julebuID.." and status="..g_JulebuDefine.mstatus_apply
	mysqlItem:executeQuery(sqlCase)
	local itemList = {}
	while true do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		--先把这里的保存下来，因为有可能需要操作数据库的
		itemList[tonumber(sqlData[1])] = tonumber(sqlData[2])
	end
	local item = st_julebu_pb.julebumember()
	for k,v in pairs(itemList) do
		local pInfo = PlayerModel.GetPlayerInfo(v)
		if pInfo ~= nil then 
			item.userid = v
			item.nickname = pInfo.nickname
			item.state = 0
			item.id = k
			item.faceid = pInfo.face_1	
			redisItem:hset(JulebuModel.apply_list..julebuID, k, item:SerializeToString(), JulebuModel.redis_index)
			--这里通过ID来当key，然后通过ID来进行排序
		end
	end
	return redisItem:hgetall(JulebuModel.apply_list..julebuID, JulebuModel.redis_index)
end


function JulebuModel.LoadUserMemberInfo(julebuID, userID, mInfo)
	--这个函数是在结算的时候，因为有些玩家退出，无法加载在这个列表中的成员数据，而从数据库中加载的
	local sqlCase = "select * from dy_julebu_member where julebuid="..julebuID.." and userid="..userID
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		return 
	end
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	mInfo.userid = tonumber(userID)
	mInfo.nickname = pInfo.nickname
	mInfo.id = tonumber(sqlData[1])
	mInfo.faceid = pInfo.face_1
	mInfo.jifen = tonumber(sqlData[6])
	mInfo.limitjifen = tonumber(sqlData[7])
	mInfo.higherid = tonumber(sqlData[9])
	JulebuModel.AddToMemberList(julebuID, mInfo)
	return mInfo
end


function JulebuModel.AddToApplyList(julebuID, mItem)
	redisItem:hset(JulebuModel.apply_list..julebuID, mItem.id, mItem:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.DelFromApplyList(julebuID, memberID)
	redisItem:hdel(JulebuModel.apply_list..julebuID, memberID, JulebuModel.redis_index)
end


function JulebuModel.GetMemberList(julebuID)
	if true == redisItem:exists(JulebuModel.member_list..julebuID, JulebuModel.redis_index) then
		return redisItem:hgetall(JulebuModel.member_list..julebuID, JulebuModel.redis_index)
	end
	return JulebuModel.LoadMemberList(julebuID)
end

function JulebuModel.LoadMemberList(julebuID)
	local sqlCase = "select id,userid,jifen,limitjifen,higherid from dy_julebu_member where julebuid="..julebuID.." and status="..g_JulebuDefine.mstatus_normal
	mysqlItem:executeQuery(sqlCase)
	local itemList = {}
	while true do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		--先把这里的保存下来，因为有可能需要操作数据库的

		
		if false == redisItem:hexists(JulebuModel.member_list..julebuID, sqlData[2], JulebuModel.redis_index) then
			--检查到在缓存中不存在，再加载
			local temp = {}
			temp['id'] = tonumber(sqlData[1])
			temp['userid'] = tonumber(sqlData[2])
			temp['jifen'] = tonumber(sqlData[3])
			temp['limitjifen'] = tonumber(sqlData[4])
			temp['higherid'] = tonumber(sqlData[5])
			table.insert(itemList, temp)
		end
	end
	local mItem = st_julebu_pb.julebumember()
	for i = 1,#itemList do
		local pInfo = PlayerModel.GetPlayerInfo(itemList[i]['userid'])
		mItem.userid = itemList[i]['userid']
		mItem.nickname = pInfo.nickname
		mItem.state = 0
		mItem.id = itemList[i]['id']
		mItem.faceid = pInfo.face_1
		mItem.jifen = itemList[i]['jifen']
		mItem.limitjifen = itemList[i]['limitjifen']
		mItem.higherid = itemList[i]['higherid']
		redisItem:hset(JulebuModel.member_list..julebuID, itemList[i]['userid'], mItem:SerializeToString(), JulebuModel.redis_index)
		JulebuModel.SetUserJulebuList(itemList[i]['userid'], julebuID)   --检查是否已经加入这个俱乐部
		--这里通过ID来当key，然后通过ID来进行排序
	end
	return redisItem:hgetall(JulebuModel.member_list..julebuID, JulebuModel.redis_index)
end

function JulebuModel.AddToMemberList(julebuID, mItem)
	--redisItem:hset(JulebuModel.member_list..julebuID, mItem.id, mItem:SerializeToString(), JulebuModel.redis_index)
	--暂时没有发现用userid来当key有任何问题
	redisItem:hset(JulebuModel.member_list..julebuID, mItem.userid, mItem:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.GetUserMemberInfo(julebuID, userID, mItem)
	--redisItem:hset(JulebuModel.member_list..julebuID, mItem.id, mItem:SerializeToString(), JulebuModel.redis_index)
	--暂时没有发现用userid来当key有任何问题
	
	if mItem == nil then
		mItem = st_julebu_pb.julebumember()
	end
	if false == redisItem:hexists(JulebuModel.member_list..julebuID, userID, JulebuModel.redis_index) then
		JulebuModel.LoadMemberList(julebuID)		
	end
	if true == redisItem:hexists(JulebuModel.member_list..julebuID, userID, JulebuModel.redis_index) then
		local strGet = redisItem:hget(JulebuModel.member_list..julebuID, userID, JulebuModel.redis_index)
		mItem:ParseFromString(strGet)
		return mItem
	else
		--return nil
		return JulebuModel.LoadUserMemberInfo(julebuID, userID, mItem)
	end

end

function JulebuModel.DelFromMemberList(julebuID, userID)
	redisItem:hdel(JulebuModel.member_list..julebuID, userID, JulebuModel.redis_index)
end

function JulebuModel.AddOnline(julebuID, userID)
	redisItem:hset(JulebuModel.online_list..julebuID,userID,1,JulebuModel.redis_index)
end

function JulebuModel.IsOnline(julebuID, userID)
	redisItem:hexists(JulebuModel.online_list..julebuID,userID,JulebuModel.redis_index)
end

function JulebuModel.DelFromOnline(julebuID, userID)
	redisItem:hdel(JulebuModel.online_list..julebuID,userID,JulebuModel.redis_index)
end

function JulebuModel.GetOnlineList(julebuID)
	return redisItem:hkeys(JulebuModel.online_list..julebuID, JulebuModel.redis_index)
end


function JulebuModel.SetGameInfo(gInfo)
	redisItem:set(JulebuModel.game_info..gInfo.tableid, gInfo:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.GetGameInfo(julebuID, tableID)
	return redisItem:get(JulebuModel.game_info..tableID, JulebuModel.redis_index)
end

function JulebuModel.DelGameInfo(julebuID, tableID, julebuType)
	redisItem:del(JulebuModel.game_info..tableID, JulebuModel.redis_index)
	JulebuModel.RemoveGame(julebuID,tableID, julebuType)  --同时也要在这里删除
end

function JulebuModel.PushGame(julebuID, tableID, julebuType)
	redisItem:lpush(JulebuModel.game_list..julebuID.."_"..julebuType, tableID, JulebuModel.redis_index)
end

function JulebuModel.RemoveGame(julebuID, tableID, julebuType)
	redisItem:lrem(JulebuModel.game_list..julebuID.."_"..julebuType, 1, tableID, JulebuModel.redis_index)
end

function JulebuModel.GetGameList(julebuID, pageNum, julebuType)
	local endCount = redisItem:llen(JulebuModel.game_list..julebuID.."_"..julebuType, JulebuModel.redis_index)
	return redisItem:lrange(JulebuModel.game_list..julebuID.."_"..julebuType, 0, endCount, JulebuModel.redis_index)
end

function JulebuModel.GetIncrChatID(julebuID)
	
	if false == redisItem:exists(JulebuModel.chat_id..julebuID, JulebuModel.redis_index) then
		JulebuModel.LoadChatID(julebuID)
	end
	local chatID = redisItem:incr(JulebuModel.chat_id..julebuID, JulebuModel.redis_index)
	local sqlCase = "update dy_julebu chatid=chatid+1 where id="..julebuID
	SqlServer.rpush(sqlCase)
	return chatID
end
function JulebuModel.GetChatID(julebuID)
	if true == redisItem:exists(JulebuModel.chat_id..julebuID, JulebuModel.redis_index) then
		return redisItem:get(JulebuModel.chat_id..julebuID,JulebuModel.redis_index)
	end
	return JulebuModel.LoadChatID(julebuID)
end
function JulebuModel.LoadChatID(julebuID)
	local sqlCase = "select chatid from dy_julebu where id="..julebuID
	mysqlItem:executeQuery(sqlCase)
	local chatID = mysqlItem:fetch()
	if chatID == nil then
		LogFile("error","load chatid is nil julebuID="..julebuID)
		return 1
	end
	redisItem:set(JulebuModel.chat_id..julebuID, chatID, JulebuModel.redis_index)
	return chatID
end
function JulebuModel.SetChatInfo(julebuID, cInfo)
	redisItem:hset(JulebuModel.chat_list..julebuID, cInfo.id,cInfo:SerializeToString(), JulebuModel.redis_index)
	local delID = cInfo.id - 1000
	if delID > 0 then
		redisItem:hdel(JulebuModel.chat_list..julebuID, delID, JulebuModel.redis_index)
	end
end

function JulebuModel.GetChatList(julebuID,startPos)
	--每次获取20条
	if julebuID == nil or julebuID == 0 then
		return {}
	end
	if startPos == 0 then
		return {}
	end
	local getList = {}
	for i = startPos,startPos-5,-1 do
		if i <=0 then
			break
		end
		table.insert(getList, i)
	end
	return redisItem:hmget(JulebuModel.chat_list..julebuID, getList, JulebuModel.redis_index)
end

function JulebuModel.LoadTodayJifen(julebuID)
	
	if true == redisItem:exists(JulebuModel.jifen_today..julebuID, JulebuModel.redis_index) then
		--如果已经存在这个列表了，就说明不用从新加载
		return 
	end
	
	local sqlCase = "select userid,jifen from dy_julebu_member where julebuid="..julebuID
	mysqlItem:executeQuery(sqlCase)
	while true do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		if tonumber(sqlData[2]) ~= 0 then
			JulebuModel.SetUserJiFen(julebuID, sqlData[1], sqlData[2])   --非0的情况，才加入到内存类别中
		end
	end
end

function JulebuModel.LoadTodayAll()
	local sqlCase = "SELECT count(id) FROM dy_julebu where status=99"
	mysqlItem:executeQuery(sqlCase)
	local count = mysqlItem:fetch()
	count = tonumber(count)
	local cycleNUm = math.floor(count/1000) + 1
	local startNum = 0
	for i = 1,cycleNUm do
		--这样做是为了保证一定会被取完
		sqlCase = "select id from dy_julebu where status="..g_JulebuDefine.mstatus_normal.." limit "..startNum..",1000" --每次取1000个
		mysqlItem:executeQuery(sqlCase)
		local idList = {}
		while true do
			local sqlData = mysqlItem:fetch()
			if sqlData == nil then
				break
			end
			table.insert(idList, tonumber(sqlData))
			--一定要放到一个列表中，才不会影响mysql缓存里面的 数据
		end
		for k,v in ipairs(idList) do
			JulebuModel.LoadTodayJifen(v)
		end
		--
	end
end

function JulebuModel.GetUserJiFen(julebuID, userID)
	--local retNum = redisItem:hget(JulebuModel.jifen_today..julebuID, userID, JulebuModel.redis_index)
	--return 0 -- retNum == nil and 0 or tonumber(retNum)
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if member == nil then
		return 0
	end
	return member.limitjifen
end

function JulebuModel.SetUserJiFen(julebuID, userID, jiFen)
	redisItem:hset(JulebuModel.jifen_today..julebuID, userID, jiFen, JulebuModel.redis_index)
end

function JulebuModel.AddUserJiFen(julebuID,userID,jiFen, tableID)
	--增加了多少积分
	--不用管积分的问题了，全部换成魅力值
	--redisItem:hincrby(JulebuModel.jifen_today..julebuID, userID, jiFen, JulebuModel.redis_index)
	--至二级添加到魅力值中
	
	if julebuID == 0 or true == JingSaiModel.IsJingSai(julebuID) then
		return
	end
	
	local addItem = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if addItem == nil then
		return
	end
	addItem.limitjifen = addItem.limitjifen + jiFen
	JulebuModel.AddToMemberList(julebuID, addItem)
	local sqlCase = "update dy_julebu_member set limitjifen="..addItem.limitjifen.." where julebuid="..julebuID.." and userid="..userID
	--mysqlItem:execute(sqlCase)
	SqlServer.rpush(sqlCase)
	
	LogBehavior.JulebuInfo( userID, "julebu="..julebuID, "add jifen" , jiFen, "tableID="..tableID, addItem.limitjifen )
	
end

function JulebuModel.GetTodayJiFen(julebuID)
	--对于群主来说，需要取到全部人的列表信息
	return redisItem:hgetall(JulebuModel.jifen_today..julebuID, JulebuModel.redis_index)
end

function JulebuModel.DelTodayJifen(julebuID)
	redisItem:del(JulebuModel.jifen_today..julebuID, JulebuModel.redis_index)
end

function JulebuModel.GetHistory(julebuID)
	return redisItem:lrange(JulebuModel.history_list..julebuID, 0, 20, JulebuModel.redis_index)
end

function JulebuModel.SetHistory(julebuID, zjInfo)
	
	local len = redisItem:lpush( JulebuModel.history_list..julebuID,zjInfo:SerializeToString(), JulebuModel.redis_index)
	
	if len > 20 then
		--最低只是保存50个
		redisItem:rpop(JulebuModel.history_list..julebuID, JulebuModel.redis_index)
	end		
end
--TMD ,一定要搞清楚这里面的关系。
function JulebuModel.GetdayUserZhanJi(julebuID, userID,pageNum)
	return redisItem:lrange(JulebuModel.zhanji_list..julebuID.."_"..userID, (pageNum-1)*10, pageNum*10-1, JulebuModel.redis_index)
end

function JulebuModel.GetdayUserZhanJi2(julebuID, userID, timeMark, pageNum)
	return redisItem:lrange(JulebuModel.zhanji_list..julebuID.."_"..userID.."_"..timeMark, (pageNum-1)*10, pageNum*10-1, JulebuModel.redis_index)
end

function JulebuModel.SetTodayUserZhanJi(julebuID, userID, zjInfo) --先设置
	redisItem:lpush( JulebuModel.zhanji_list..julebuID.."_"..userID,zjInfo:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.ReNameDayUserZhanji(julebuID, userID, timeMark)
	--每天结算的时候，修改
	
	if false == redisItem:exists(JulebuModel.zhanji_list..julebuID.."_"..userID, JulebuModel.redis_index) then
		--如果不存在，就不能rename
		return
	end
	
	redisItem:rename(JulebuModel.zhanji_list..julebuID.."_"..userID, JulebuModel.zhanji_list..julebuID.."_"..userID.."_"..timeMark, JulebuModel.redis_index)
	redisItem:expire(JulebuModel.zhanji_list..julebuID.."_"..userID.."_"..timeMark, g_daySeconds*25, JulebuModel.redis_index)
end


function JulebuModel.SetDayHistory(julebuID, timeStr, dayInfo)
	--这里保存15天的记录
	redisItem:setex(JulebuModel.day_history..julebuID.."_"..timeStr, g_daySeconds*15, dayInfo:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.GetDayHistory(julebuID,timeStr)
	return redisItem:get(JulebuModel.day_history..julebuID.."_"..timeStr, JulebuModel.redis_index)
end

function JulebuModel.SetMemberDay(julebuID, userID, mDayInfo)
	local len = redisItem:lpush( JulebuModel.member_day_list..julebuID.."_"..userID,mDayInfo:SerializeToString(), JulebuModel.redis_index)
	if len > 20 then
		--最低只是保存20个
		redisItem:rpop(JulebuModel.member_day_list..julebuID.."_"..userID, JulebuModel.redis_index)  --删除掉一个
	end
end

function JulebuModel.GetMemberDay(julebuID, userID)
	return redisItem:lrange(JulebuModel.member_day_list..julebuID.."_"..userID, 0, 20, JulebuModel.redis_index)
end

function JulebuModel.GetCountTime(julebuID)
	
	local strGet = redisItem:hget(JulebuModel.count_time, julebuID, JulebuModel.redis_index)
	if strGet ~= nil then
		return strGet
	else
		local sqlCase = "select counttime from dy_julebu where id="..julebuID 
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch()
		if sqlData == nil or string.len(sqlData) < 5 then
			sqlData = "2001-01-01"  --默认一个初始值
		end
		redisItem:hset(JulebuModel.count_time, julebuID, sqlData, JulebuModel.redis_index)
		return sqlData		
	end
	
end

function JulebuModel.SetCountTime(julebuID, timeMark)
	local sqlCase = "update dy_julebu set counttime='"..timeMark.."' where id="..julebuID
	SqlServer.rpush(sqlCase)
	redisItem:hset(JulebuModel.count_time, julebuID, timeMark, JulebuModel.redis_index)
end

function JulebuModel.SetJiFenCheck(checkStr)
	redisItem:setex(checkStr, 30, 1, JulebuModel.redis_index)
end

function JulebuModel.IsCount(checkStr)
	return redisItem:exists(checkStr, JulebuModel.redis_index)
end


function JulebuModel.SetPlayingMethod(julebuid, julebutype, info, gametype)
	local gInfo = st_julebu_pb.julebuplayingmethodinfo()
	gInfo.gametype = gametype
	gInfo.playingmethod = info
	local redisKey = JulebuModel.playingmethod_info..julebutype.."_"..julebuid
	redisItem:set(redisKey, gInfo:SerializeToString(), JulebuModel.redis_index)
end

function JulebuModel.GetPlayingMethod(julebuid, julebutype)
	local redisKey = JulebuModel.playingmethod_info..julebutype.."_"..julebuid
	local tmsg = redisItem:get(redisKey, JulebuModel.redis_index)
	if tmsg == nil then
		local gInfo = st_julebu_pb.julebuplayingmethodinfo()
		local cgCreateTable = msg_douniu_2_pb.cgdouniucreate() 
		cgCreateTable.maxuser = 6
		cgCreateTable.julebutype = julebutype
		cgCreateTable.dntype = 104
		cgCreateTable.difen = 24
		cgCreateTable.maxplaynum = 10
		cgCreateTable.payway = 1
		cgCreateTable.paynum = 3
		cgCreateTable.typeodds = 1
		cgCreateTable.restrict = 0
		cgCreateTable.integralmax = 0
		cgCreateTable.bankermultiple = 4
		cgCreateTable.autoready = 1
		cgCreateTable.paytype = 0
		cgCreateTable.julebuid = julebuid
		cgCreateTable.autoStart = 99
		cgCreateTable.TZmax = 20
		cgCreateTable.banrubcard = 0
		cgCreateTable.playerbuycode = 0
		cgCreateTable.pourrestrict	= 0
		cgCreateTable.kongwildcard	= 0
		cgCreateTable.pourdouble = 0
		
		local Cmsg = cgCreateTable:SerializeToString()
		gInfo.gametype = g_JulebuDefine.modules_douniu
		gInfo.playingmethod = Cmsg
		tmsg = gInfo:SerializeToString()
		JulebuModel.SetPlayingMethod(julebuid, julebutype, Cmsg, g_JulebuDefine.modules_douniu)
	end
	return tmsg
end

function JulebuModel.DelPlayingMethod(julebuid, julebutype)
	local redisKey = JulebuModel.playingmethod_info..julebutype.."_"..julebuid
	redisItem:del(redisKey, JulebuModel.redis_index)
end

function JulebuModel.SetLimitJiFenHistory(julebuid, hInfo)
	
	local redisKey = JulebuModel.limitjifenhistory.."_"..julebuid
	local operationType = 1
	if hInfo.count > 0 then
		redisKey = redisKey.."_1"
		local len = redisItem:lpush(redisKey,hInfo:SerializeToString(), JulebuModel.redis_index)
		if len > 100 then
			--最低只是保存50个
			redisItem:rpop(redisKey, JulebuModel.redis_index)
		end	
		operationType = 1
	elseif hInfo.count < 0 then
		redisKey = redisKey.."_2"
		local len = redisItem:lpush(redisKey,hInfo:SerializeToString(), JulebuModel.redis_index)
		if len > 100 then
			--最低只是保存50个
			redisItem:rpop(redisKey, JulebuModel.redis_index)
		end	
		operationType = 2
	end
	local sqlCase="insert into log_julebuljimitjifenhistory(julebuid,executorid,targetid,operationtype, timenum, countnum) values("
				..julebuid..","..hInfo.executorid..","..hInfo.targetid..","..operationType..","..hInfo.timemark..","..hInfo.count..")"
	mysqlItem:execute(sqlCase)
	
end

function JulebuModel.GetPlayerHistory(julebuid, flag)
	local redisKey = JulebuModel.limitjifenhistory.."_"..julebuid.."_"..flag
	local hList = redisItem:lrange(redisKey, 0, 100, JulebuModel.redis_index)
	if #hList == 0 then
		local sqlCase = "select * from log_julebuljimitjifenhistory where julebuid="..julebuid.." and operationtype="..flag
		.." and DATEDIFF(Ctime,NOW())<=0 and DATEDIFF(Ctime,NOW())>-3"
		mysqlItem:executeQuery(sqlCase)
		for i = 1, 1000 do
			local sqlData = mysqlItem:fetch({})
			if sqlData == nil then
				break
			end
			
			local tmpInfo = st_julebu_pb.julebujimitjifeninfo()
			tmpInfo.executorid =  tonumber(sqlData[3])
			tmpInfo.targetid =  tonumber(sqlData[4])
			tmpInfo.count =  tonumber(sqlData[6])
			tmpInfo.timemark =  tonumber(sqlData[7])
			local msg = tmpInfo:SerializeToString()
			if tmpInfo.count > 0 then
				local len = redisItem:lpush(redisKey,msg, JulebuModel.redis_index)
				if len > 100 then
					--最低只是保存50个
					redisItem:rpop(redisKey, JulebuModel.redis_index)
				end	
			elseif tmpInfo.count < 0 then
				local len = redisItem:lpush(redisKey,msg, JulebuModel.redis_index)
				if len > 100 then
					--最低只是保存50个
					redisItem:rpop(redisKey, JulebuModel.redis_index)
				end	
			end
		end
		hList = redisItem:lrange(redisKey, 0, 100, JulebuModel.redis_index)
		
	end
	return hList
end

function JulebuModel.DelPlayerHistory(julebuid)
	local redisKey = JulebuModel.limitjifenhistory.."_1"..julebuid
	redisItem:del(redisKey, JulebuModel.redis_index)
	local redisKey1 = JulebuModel.limitjifenhistory.."_2"..julebuid
	redisItem:del(redisKey1, JulebuModel.redis_index)
	
end

function JulebuModel.GetPartnerInfo(julebuid, userid)
	local redisKey = JulebuModel.partnerinfo.."_"..julebuid.."_".. userid
	local tmp = redisItem:get(redisKey, JulebuModel.redis_index)
	if tmp == nil then
		local PartnerInfo = st_julebu_pb.julebupartnerinfo()
		PartnerInfo.userid = userid
		local tpInfo = PlayerModel.GetPlayerInfo(userid)
		if tpInfo ~= nil then
			PartnerInfo.nickname = tpInfo.nickname
			PartnerInfo.yesterdayscore = 0
			PartnerInfo.yesterdaydayingjia = 0
			PartnerInfo.yesterdaygamenum = 0
			PartnerInfo.yesterdayconsume = 0
			PartnerInfo.gamenum = 0
			PartnerInfo.todayscore = 0
			PartnerInfo.todaydayingjia = 0
			PartnerInfo.todaygamenum = 0
			PartnerInfo.todayconsume = 0
		end
		JulebuModel.SetPartnerInfo(julebuid, userid, PartnerInfo)
		tmp = PartnerInfo:SerializeToString()
	end
	
	return tmp
end


function JulebuModel.SetPartnerInfo(julebuid, userid, msg)
	local redisKey = JulebuModel.partnerinfo.."_"..julebuid.."_".. userid
	redisItem:set(redisKey, msg:SerializeToString()	, JulebuModel.redis_index)
end

function JulebuModel.DelPartnerInfo(julebuid, userid)
	local redisKey = JulebuModel.partnerinfo.."_"..julebuid.."_".. userid
	redisItem:del(redisKey, JulebuModel.redis_index)
end

function JulebuModel.DelJulebuPartnerInfo(julebuid)
	local sqlCase = "select userid from dy_julebu_member  where superiorid>0 and julebuid="..julebuid
	mysqlItem:executeQuery(sqlCase)
	while true do
		sqlData = mysqlItem:fetch()
		if sqlData == nil then
			break
		end
		JulebuModel.DelPartnerInfo(julebuid, sqlCase)
	end
end



-- 设置俱乐部战绩
function JulebuModel.SetJulebuRecord(redisField, julebuid, userid)
	local redisKey = JulebuModel.JulebuRecord..julebuid.."_"..userid
	--从左边插入数据
	redisItem:lpush(redisKey, redisField, JulebuModel.redis_index)
	redisItem:expire(redisKey, 1296000, JulebuModel.redis_index)
	
	--超过10条记录，弹出最右一个记录
	if redisItem:llen(redisKey, JulebuModel.redis_index) > 20 then
		redisItem:rpop(redisKey, JulebuModel.redis_index)
	end
end

-- 获取俱乐部战绩列表
function JulebuModel.GetJulebuModelRecordList(julebuid, userid)
	local redisKey = JulebuModel.JulebuRecord..julebuid.."_"..userid
	local keysList = {}
	if true == redisItem:exists(redisKey, JulebuModel.redis_index)  then
		keysList = redisItem:lrange(redisKey, 0, -1, JulebuModel.redis_index)	--获取所有的记录
		--过期的不要
		local realKeysList = {}
		local delCnt = 0	--需要删除的数量，由于是排序的，过期的全部在后面
		for k,v in ipairs(keysList) do
			if true == redisItem:exists(v, g_zzphzRedis.redis_index) then
				table.insert(realKeysList, v)
			else
				delCnt = delCnt + 1
			end
		end
		
		--删除过期的
		for i=1,delCnt do
			redisItem:rpop(redisKey, JulebuModel.redis_index)
		end
		
		return realKeysList
	end
	
	return keysList
end

--设置要被删除数据的合伙人ID
function JulebuModel.SetJulebuDelPartnerid(julebuID, userid)   --设置俱乐部的信息
	local redisKey = JulebuModel.JulebuDelPartner..julebuID.."_"..userid
	redisItem:set(redisKey,1,JulebuModel.redis_index)
end

--查看要被删除数据的合伙人ID是否存在
function JulebuModel.IsJulebuDelPartnerid(julebuID, userid)
	local redisKey = JulebuModel.JulebuDelPartner..julebuID.."_"..userid
	return redisItem:exists(redisKey, JulebuModel.redis_index)
end

--删除要被删除数据的合伙人ID
function JulebuModel.DelJulebuDelPartnerid(julebuID, userid)
	local redisKey = JulebuModel.JulebuDelPartner..julebuID.."_"..userid
	redisItem:del(redisKey, JulebuModel.redis_index)
end


--在结算的时候，如果确定有邀请积分的，那么
function JulebuModel.AddGetJiFen(julebuID, userID, fromID, getCount)
	--把这两个积分加到对应的列表中
	redisItem:hincrby( JulebuModel.JulebuGetJifen..julebuID, userID, getCount,  JulebuModel.redis_index)
	redisItem:hincrby(JulebuModel.JulebuJifenHistory..julebuID.."_"..userID, fromID, getCount, JulebuModel.redis_index)

end

function JulebuModel.GetAwardJiFen(julebuID, userID)
	local getData = redisItem:hget( JulebuModel.JulebuGetJifen..julebuID, userID,  JulebuModel.redis_index )
	return getData == nil and 0 or tonumber(getData)
end

