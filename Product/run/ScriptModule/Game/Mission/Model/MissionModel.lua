MissionModel = {}

MissionModel.redis_index = "redis_mission"

MissionModel.reids_exist = "mission_exist"
MissionModel.reids_max = "msmax"

MissionModel.redis_key = "mission_"   --保存玩家的任务列表: mission_type_useid
MissionModel.redis_got = "missgot_"   --玩家已经取得的每日任务列表:无序的集合，missgot_type_userid
MissionModel.redis_gotlist = "missgotlist_"  --玩家已经取得的成就列表，哈希表，不包括称号
MissionModel.bankrupt_key = "user_bankrupt"  --破产补偿的列表

MissionModel.missList = {}

MissionModel.db_conMission = {}

MissionModel.typeList = {11,12,13,21,22,23}

function MissionModel.Init()
	
	local missionType = 0
	local missionType2 = 0
	local msList = proplayer_pb.missionlist()
	for k,v in ipairs(MissionModel.db_missionSort) do
		if v[db_missionIndex.isexist] == 1 then
			--必须是存在的才加载到内存中
			if v[db_missionIndex.category] ~= missionType then
				if #msList.mslist > 0 then
					MissionModel.missList[missionType] = msList
					msList = proplayer_pb.missionlist()
				end
			end
			missionType = v[db_missionIndex.category]
			
			if missionType == 21 or missionType == 23 then
				local mTemp = v[db_missionIndex.id]
				mTemp = math.modf(mTemp/100)
				
				if mTemp ~= missionType2 then
					local msItem = msList.mslist:add()
					msItem.missionid = v[db_missionIndex.id]
				end
				missionType2 = mTemp
			else
				local msItem = msList.mslist:add()
				msItem.missionid = v[db_missionIndex.id]
			end
		end
	end
	
	if #msList.mslist > 0 then
		MissionModel.missList[missionType] = msList
		msList = proplayer_pb.missionlist()
	end
	
	local tempList = {}
	local conType = 0
	for k,v in ipairs(MissionModel.db_missionSort) do
		
		if conType ~= v[db_missionIndex.contype] then
			--如果对应的类型不相等
			if #tempList > 0 then
				MissionModel.db_conMission[conType] = tempList
			end
			
			conType = v[db_missionIndex.contype]
			tempList = {}
			table.insert(tempList, v)
		else
			table.insert(tempList, v)
		end
		
	end
	
	if #tempList > 0 then
		MissionModel.db_conMission[conType] = tempList
	end
end

function MissionModel.ChipIsExist(userID, missionID)
	--检查时光碎片是否存在缓存中了
	return redisItem:hexists(MissionModel.redis_gotlist..userID, missionID, MissionModel.redis_index)
end

function MissionModel.GetSysMissionList(missionType)
	return MissionModel.missList[missionType]
end

function MissionModel.GetUserAchieveInfo(userID, achInfo)
	
	if achInfo == nil then
		achInfo = proplayer_pb.missioninfo()
	end
	local missionKey = MissionModel.redis_key..g_mission.type_achieve.."_"..userID
	
	if true == redisItem:exists(missionKey, MissionModel.redis_index) then
		local strGet = redisItem:get(missionKey, MissionModel.redis_index)
		achInfo:ParseFromString(strGet)
		return achInfo
	end
	
	local sqlCase = "select * from dy_mission where userid="..userID.." and missionid>=230000 and missionid<240000"
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	
	if sqlData == nil then
		achInfo.userid = tonumber(userID)
		achInfo.missionid = 230001
		achInfo.finish = 0
		achInfo.state = g_mission.state_unfinish
		MissionModel.InsertSql(achInfo)
	else
		achInfo.id = tonumber(sqlData[1])
		achInfo.userid = userID
		achInfo.missionid = tonumber(sqlData[3])
		achInfo.finish = tonumber(sqlData[5])
		achInfo.state = tonumber(sqlData[6])
	end
	
	
	sqlCase = "select * from dy_mission where state=2 and userid="..userID.." and missionid>= 210000 and missionid<230000"
	
	mysqlItem:executeQuery(sqlCase)
	
	while true do

		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		
		local item = proplayer_pb.missioninfo()
		item.id = tonumber(sqlData[1])
		item.userid = userID
		item.missionid = tonumber(sqlData[3])
		item.finish = tonumber(sqlData[5])
		item.state = tonumber(sqlData[6])
		redisItem:hset(MissionModel.redis_gotlist..userID, item.missionid, item:SerializeToString(), MissionModel.redis_index)
	end
	
	
	
	redisItem:set(missionKey,achInfo:SerializeToString(), MissionModel.redis_index)
	return achInfo
end

function MissionModel.AchieveInfoUpDate(achInfo, pInfo)
	
	achInfo.finish = achInfo.finish + 1
	local missionID = achInfo.missionid + 1
	if db_mission[missionID] ~= nil and achInfo.finish >= db_mission[achInfo.missionid][db_missionIndex.conamount]  then
		achInfo.missionid = missionID
		pInfo.title = db_mission[missionID][db_missionIndex.name]
	end
	MissionModel.UpdateSql(achInfo)
	local missionKey = MissionModel.redis_key..g_mission.type_achieve.."_"..pInfo.userid
	redisItem:set(missionKey,achInfo:SerializeToString(), MissionModel.redis_index)
end

function MissionModel.GetAchieveListKey(userID)
	return MissionModel.redis_gotlist..userID
end



function MissionModel.GetUserMissionList(userID, missionType, msList)
	--获取玩家所有的列表
	
	if msList == nil then
		msList = proplayer_pb.missionlist()
	end
	
	local missionKey = MissionModel.redis_key..missionType.."_"..userID
	if true == redisItem:exists(missionKey, MissionModel.redis_index) then
		local strGet = redisItem:get(missionKey, MissionModel.redis_index)
		
		msList:ParseFromString(strGet)
		return msList
	end
	return MissionModel.LoadUserMissionList(userID, missionType, msList)
end

function MissionModel.DelUserMissionList(userID, missionType)
	local missionKey = MissionModel.redis_key..missionType.."_"..userID
	redisItem:del(missionKey, MissionModel.redis_index)
end

function MissionModel.SetUserMissionList(userID, missionType, userMlist)
	local missionKey = MissionModel.redis_key..missionType.."_"..userID
	redisItem:set(missionKey, userMlist:SerializeToString(), MissionModel.redis_index)
end

function MissionModel.LoadUserMissionList(userID, missionType, msList)
	if msList == nil then
		msList = proplayer_pb.missionlist()
	end

	if MissionModel.missList[missionType] == nil then
		return msList
	end
	
	msList:ParseFromString(MissionModel.missList[missionType]:SerializeToString())
	for k,v in ipairs(msList.mslist) do
		v.userid = userID
	end
	if math.floor(missionType/10) == 2 then --第一大类里面的成就，不需要添加到数据库中的
		
		local sqlCase = "select * from dy_mission where userid="..userID.." and missionid>="..(missionType*10000).." and missionid<"..((missionType+1)*10000)
		mysqlItem:executeQuery(sqlCase)
		
		for i = 1, 1000 do
			local sqlData = mysqlItem:fetch({})
			if sqlData == nil then
				break
			end
			if missionType ~= g_mission.type_achieve and tonumber(sqlData[5]) ~= g_mission.state_get then				
				local tempID = tonumber(sqlData[3])
				for k,v in ipairs(msList.mslist) do
					
					if missionType == g_mission.type_grow then
						--如果是成长足迹
						if math.floor(tempID/100) == math.floor(v.missionid/100) then
							v.id = tonumber(sqlData[1])
							v.userid = userID
							v.missionid = tonumber(sqlData[3])
							v.finish = tonumber(sqlData[5])
							v.state = tonumber(sqlData[6])
							break
						end
					elseif missionType == g_mission.type_chip then
						if tempID == v.missionid then
							v.id = tonumber(sqlData[1])
							v.userid = userID
							v.missionid = tonumber(sqlData[3])
							v.finish = tonumber(sqlData[5])
							v.state = tonumber(sqlData[6])
							break
						end
					else
						if math.floor(tempID/10000) == missionType then
							v.id = tonumber(sqlData[1])
							v.userid = userID
							v.missionid = tonumber(sqlData[3])
							v.finish = tonumber(sqlData[5])
							v.state = tonumber(sqlData[6])
							break
						end
					end
				end
				
			end
		end
	end
	
	local redisKey = MissionModel.redis_key..missionType.."_"..userID
	
	redisItem:set(redisKey,msList:SerializeToString(), MissionModel.redis_index)
	return msList
end

function MissionModel.InsertSql(uMInfo)
	--把需要保持到数据库中的任务插入到数据库中
	local sqlCase = "insert into dy_mission(userid,missionid,finish,state) values("..uMInfo.userid..","..uMInfo.missionid..","..uMInfo.finish..","..uMInfo.state..")"
	mysqlItem:execute(sqlCase)
	
	sqlCase = "select id from dy_mission where userid="..uMInfo.userid.." and missionid="..uMInfo.missionid.." and finish="..uMInfo.finish
	
	mysqlItem:executeQuery(sqlCase)
	
	local sqlData = mysqlItem:fetch()
	if sqlData ~= nil then
		uMInfo.id = tonumber(sqlData)
	end
	
	return uMInfo
end

function MissionModel.UpdateSql(uMInfo)
	if uMInfo.id <= 0 then
		return nil
	end
	local sqlCase = "update dy_mission set finish="..uMInfo.finish..",state="..uMInfo.state..",missionid="..uMInfo.missionid.." where id="..uMInfo.id
	SqlServer.rpush(sqlCase)
end

function MissionModel.DayFirstLogin(userID)
	--每天玩家第一次进来的时候，先清空对应的任务列表
	local missionKey = MissionModel.redis_key..MissionManager.type_dayMoney.."_"..userID
	redisItem:del(missionKey,MissionModel.redis_index)
	missionKey = MissionModel.redis_key..MissionManager.type_dayMission.."_"..userID
	redisItem:del(missionKey,MissionModel.redis_index)
	redisItem:hset(MissionModel.bankrupt_key, userID, 0, MissionModel.redis_index)  --每天第一次登陆的时候，重置破产补偿
end

function MissionModel.BankruptCheck(pInfo)
	--对玩家是否破产进行检查。
	--破产的标准是资产少于100筹码

	if (pInfo.jetton + pInfo.bankjetton) >= 100 then
		return nil
	end

	--否则，就是破产了
	local brokeNum = redisItem:hget(MissionModel.bankrupt_key, pInfo.userid, MissionModel.redis_index)
	brokeNum = brokeNum == nil and 0 or tonumber(brokeNum)
	
	if brokeNum < 3 then
		--已经补充过三次了
		local baseItem = db_mission[110501]
		if baseItem ~= nil then
			local mUList = MissionModel.GetUserMissionList(pInfo.userid, g_mission.type_money)
			local addItem = mUList.mslist:add()
			addItem.userid = 0
			addItem.missionid = 110501
			addItem.finish = 1
			addItem.state = g_mission.state_finish
			MissionModel.SetUserMissionList(pInfo.userid, g_mission.type_money, mUList)
			NoticeModel.SendNotice(pInfo.userid, 400 + g_mission.type_money)
		end
		brokeNum = brokeNum + 1
		redisItem:hset(MissionModel.bankrupt_key, pInfo.userid, brokeNum, MissionModel.redis_index)
	end

end

function MissionModel.LoginConAward(userID, conType, amount)
	
	--这里对玩家的连续登录进行奖励

	local conDay = amount

	conDay = conDay > 7 and 7 or conDay
	
	local missionID = conType*100 + conDay
	
	local mUList = MissionModel.GetUserMissionList(userID, g_mission.type_money)
	
	local baseItem = db_mission[missionID]
	
	if baseItem ~= nil then

		local addItem = mUList.mslist:add()
		addItem.userid = 0
		addItem.missionid = missionID
		addItem.finish = 1
		addItem.state = g_mission.state_finish
	end
	if amount > 7 then
		--如果超过7个了，则额外的补偿一个
		missionID = 110408
		baseItem = db_mission[missionID]
		if baseItem ~= nil then
			local addItem = mUList.mslist:add()
			addItem.userid = 0
			addItem.missionid = missionID
			addItem.finish = 1
			addItem.state = g_mission.state_finish
		end
	end
	--设置在一起
	MissionModel.SetUserMissionList(userID, g_mission.type_money, mUList)

end

function MissionModel.GetMissionNotice(userID)

	local getList = {}
	for k1,v1 in ipairs(MissionModel.typeList) do
		
		local nInfo = proplayer_pb.noticeinfo()
		local mUList = MissionModel.GetUserMissionList(userID, v1)
		nInfo.noticetype = 400+v1  --这里对应对应的类型
		
		
		for k2,v2 in ipairs(mUList.mslist) do
			if v2.state == g_mission.state_finish then
				nInfo.amount = nInfo.amount + 1;
			end
		end
		
		if nInfo.amount > 0 then
			table.insert(getList, nInfo)
		end
		
	end
	
	return getList
end







