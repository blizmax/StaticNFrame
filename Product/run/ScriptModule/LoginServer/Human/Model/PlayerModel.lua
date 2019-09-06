
PlayerModel = {}
PlayerModel.redis_index = "redis_player"
PlayerModel.kit_userList = "player_kit"

PlayerModel.player_jetton = "player_jetton_"   --这是一个跟用户ID相关的jetton，用于保存玩家的金币信息
PlayerModel.player_update = "player_update_"   --这是一个哈希类别，用于保存玩家的哈希信息，如果在这个列表中有记录，说明需要更新到数据库


PlayerModel.player_winjetton = "player_winjetton_"
PlayerModel.player_lostjetton = "player_losejetton_"


PlayerModel.Player_phone_account = "player_phone"  --通过手机号码去找到对应的账号。

PlayerModel.player_bankjetton = "player_bank_list"

function PlayerModel.LoadPlayerInfoByCID(account,password,pInfo, bindType)
	
	local sqlCase = "select * from dy_player where account='"..account.."'"
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	
	if sqlData == nil then
		LogBehavior.Error(account, "player", "LoadPlayerInfoByCID",-1,"账号不存在,account="..account)
		return ReturnCode["account_not_exist"]
	end

	if password ~= sqlData[4] and bindType ~= BindType.bindType_WeiXin then
		--微信登入不需要检查密码
		LogBehavior.Error(account,"player", "LoadPlayerInfoByCID", ReturnCode["password_not_match"],"登录密码不匹配,password="..password)
		return ReturnCode["account_not_exist"]
	end
	
	if pInfo == nil then
		pInfo = st_human_pb.playerinfo()
	end
	
	pInfo.userid = tonumber(sqlData[1])
	
	pInfo.cid = sqlData[2]
	pInfo.account = sqlData[3]
	pInfo.password = sqlData[4]
	pInfo.nickname = string.restorehtmlspecialchars(sqlData[5])
	
	pInfo.face_1 = sqlData[6]
	pInfo.face_2 = sqlData[7]
	pInfo.face_3 = sqlData[8]
	pInfo.face_4 = sqlData[9]
	pInfo.regdate = sqlData[10]
	
	pInfo.sex = tonumber(sqlData[11])
	pInfo.age = tonumber(sqlData[12])
	pInfo.email = sqlData[13]
	
	pInfo.phonenum = sqlData[14]
	pInfo.exp = tonumber(sqlData[15])
	pInfo.level = tonumber(sqlData[16])
	pInfo.viplevel = tonumber(sqlData[17])
	pInfo.jetton = sqlData[18]
	pInfo.gold = tonumber(sqlData[19])
	pInfo.money = tonumber(sqlData[20])
	
	pInfo.channel = sqlData[21]
	pInfo.province = sqlData[22]
	pInfo.city = sqlData[23]
	pInfo.popularity = tonumber(sqlData[24])
	pInfo.idenstatus = tonumber(sqlData[25])
	pInfo.bindtype = tonumber(sqlData[26])
	
	pInfo.bindnick = sqlData[27]
	pInfo.platformid = sqlData[28]
	pInfo.imei = sqlData[29]
	pInfo.devname = string.restorehtmlspecialchars(sqlData[30])
	pInfo.ip = sqlData[31]
	pInfo.mobiletype = tonumber(sqlData[32])
	
	pInfo.lasttime = tonumber(sqlData[33])
	pInfo.penulttime = tonumber(sqlData[34])
	pInfo.silent = tonumber(sqlData[35])
	pInfo.isban = tonumber(sqlData[36])
	pInfo.isrobot = tonumber(sqlData[37])
	pInfo.playtype = tonumber(sqlData[38])
	pInfo.description = sqlData[39]
	pInfo.blacklist = tonumber(sqlData[40])
	pInfo.propertyid = tonumber(sqlData[41])
	pInfo.ticket = tonumber(sqlData[42])
	pInfo.location = "在线"
	pInfo.bank_password = sqlData[43] or ""
	pInfo.bank_jetton = sqlData[44] or "0"
	
	--[[
	local achieveInfo = MissionModel.GetUserAchieveInfo(pInfo.userid)
	if db_mission[achieveInfo.missionid] ~= nil then
		--取得玩家的称号
		pInfo.title = db_mission[achieveInfo.missionid][db_missionIndex.name]
	else
		pInfo.title = "暂无称号" 
	enddec
	]]
	--LogBehavior.Info(pInfo, "player", "LoadPlayerInfoByCID", 0, "加载数据库玩家信息成功")
	local tJson = {}
	table.insert(tJson, pInfo.password)
	table.insert(tJson, pInfo.userid)
	redisItem:set("account"..pInfo.account, luajson.encode(tJson), PlayerModel.redis_index)
	redisItem:set("playerinfo"..pInfo.userid, pInfo:SerializeToString(), PlayerModel.redis_index)
	redisItem:hset(PlayerModel.player_jetton, pInfo.userid, tonumber(pInfo.jetton), PlayerModel.redis_index)
	return 0
end

function PlayerModel.GetPlayerInfoByCID(account, password, pInfo, bindType)
	
	if true == redisItem:exists("account"..account, PlayerModel.redis_index) then
		local strUserID = redisItem:get("account"..account, PlayerModel.redis_index)
		local arr = luajson.decode(strUserID)
		
		--微信登入，不需要检查密码
		if arr[1] ~= password  and bindType ~= BindType.bindType_WeiXin then
			LogBehavior.Error(account,"player", "GetPlayerInfoByCID", ReturnCode["password_not_match"],"登录密码不匹配,password="..password)
			return ReturnCode["password_not_match"]
		end

		if nil == PlayerModel.GetPlayerInfo(tonumber(arr[2]), pInfo) then
			LogBehavior.Error(account,"player", "GetPlayerInfoByCID", ReturnCode["account_not_exist"],"账号不存在，userid="..arr[2])
			return ReturnCode["account_not_exist"] 
		end
		return 0
	end
	
	return PlayerModel.LoadPlayerInfoByCID(account, password, pInfo, bindType)
end

function PlayerModel.GetPlayerInfo(playerID, pInfo)

	if tonumber(playerID) < 1000  then
		return nil
	end
	
	if pInfo == nil then
		pInfo = st_human_pb.playerinfo()
	end
	if true == redisItem:exists("playerinfo"..playerID, PlayerModel.redis_index) then
		local strGet = redisItem:get("playerinfo"..playerID, PlayerModel.redis_index)
		pInfo:ParseFromString(strGet)
		
		pInfo.jetton = tostring(PlayerModel.GetJettonFromRedis(playerID))  --从缓存中取
		pInfo.bank_jetton = PlayerModel.GetBankJetton(playerID)
		return pInfo
	end
	
	return PlayerModel.LoadPlayer(playerID, pInfo)
end



function PlayerModel.GetInitFaceID(Sex)

	--[[
	local boyList = {'81001_1001','81002_1001','81003_1001','81004_1001','81005_1001','81006_1001','81007_1001','81008_1001','81009_1001','81010_1001','81011_1001','81012_1001',
					'81013_1001','81014_1001','81015_1001','81016_1001','81017_1001','81018_1001','81019_1001','81020_1001'}
	local girlList = {'82001_1001','82002_1001','82003_1001','82004_1001','82005_1001','82006_1001','82007_1001','82008_1001','82009_1001','82010_1001','82011_1001','82012_1001',
					'82013_1001','82014_1001','82015_1001','82016_1001','82017_1001','82018_1001','82019_1001','82020_1001'}					
	local neutralList = {'83001_1001','83002_1001','83003_1001','83004_1001','83005_1001','83006_1001','83007_1001','83008_1001','83009_1001','83010_1001','83011_1001','83012_1001',
					'83013_1001','83014_1001','83015_1001','83016_1001','83017_1001','83018_1001'}
	--]]
	local boyList =  {'90006_1001','90007_1001','90008_1001','90009_1001','90010_1001'}	
	local girlList = {'90001_1001','90002_1001','90003_1001','90004_1001','90005_1001'}
	
	local neutralList = {'90001_1001','90002_1001','90003_1001','90004_1001','90005_1001','90006_1001','90007_1001','90008_1001','90009_1001','90010_1001'}	
	
	
	
	local faceID
	if Sex == 1 then
		local indexNum = math.myrandom(1,#boyList)
		faceID = boyList[indexNum]
	elseif Sex == 2 then
		local indexNum = math.myrandom(1,#girlList)
		faceID = girlList[indexNum]
	else
		local indexNum = math.myrandom(1,#neutralList)
		faceID = neutralList[indexNum]
	end	
	
	return faceID
end

function PlayerModel.CreatePlayer(cgmsg)
	
	
	cgmsg.nickname = string.filter_spec_chars(cgmsg.nickname) -- 去掉特殊的符号, add by landy 190629
	cgmsg.sex = cgmsg.sex == 0 and math.myrandom(1,2) or cgmsg.sex

	local sqlCase = "insert into dy_player(cid,account,password,nickname,channel,province,city,bindtype,bindnick,platformid,imei,devname,mobiletype,jetton,money,lasttime,face_1,sex,email,phonenum, bank_jetton) values('"..
	cgmsg.cid.."','"..cgmsg.account.."','"..cgmsg.password.."','"..string.htmlspecialchars(cgmsg.nickname).."','"..cgmsg.channel.."','"..cgmsg.province.."','"..cgmsg.city.."',"..cgmsg.bindtype
	..",'"..cgmsg.bindnick.."','"..cgmsg.platformid.."','"..cgmsg.imei.."','"..string.htmlspecialchars(cgmsg.devname).."',"..cgmsg.mobiletype..","..g_initData.jetton..","..g_initData.money..","..TimeUtils.GetTime()..",'"..PlayerModel.GetInitFaceID(cgmsg.sex).."',"..cgmsg.sex..",'"..cgmsg.email.."','"..cgmsg.phonenum.."', 0)"
	local ret = mysqlItem:execute(sqlCase)

	if ret == nil then
		LogFile("player", sqlCase)
		LogBehavior.Error(cgmsg.cid,"Player", "CreatePlayer", -1, "创建角色失败")
		return nil
	end
	
	SysCountService.IssueJetton(g_initData.jetton) --初始化部分也要统计进来
	SysCountService.PlayerCount()
end

function PlayerModel.DwcBindAgency(pInfo, expandCode, platformid)

	if expandCode == nil then
		return nil
	end
	if true then
		return 
	end
	local sqlCase = "select userid from ag_player where expandcode='"..expandCode.."' order by id desc"
	
	mysqlItem:executeQuery(sqlCase)
	local toUserid = mysqlItem:fetch()
	if toUserid == nil then
		return nil
	end	
	
	
	local toAgency = AgentModel.GetAgentInfo(tonumber(toUserid))
	
	
	if toAgency == nil then
		--说明被绑定的玩家还没有绑定别人。
		return nil
	end
	
	--一旦绑定别人了，就会在后台插入一个记录
	local sqlCase = ""
	myAgency = st_agent_pb.agentinfo()

	myAgency.userid = pInfo.userid
	myAgency.invitestr = tostring(pInfo.userid)
	myAgency.level = 91
	myAgency.soleagent = toAgency.level == 99 and toAgency.userid or toAgency.soleagent   --如果他自己是总代，就属于他自己，如果他自己不是总代，就属于他的总代
	myAgency.agent1 = toAgency.userid       --属于他自己的玩家
	myAgency.agent2 = toAgency.agent1       --自己的玩家的上一级
	myAgency.agent3 = toAgency.agent2    
	myAgency.agent4 = toAgency.agent3
	myAgency.qrcodeurl = AgentModel.GetQrcodeUrl(pInfo.userid)
	myAgency.exclusiveurl = AgentModel.GetQrcodeUrl(userID)
	AgentModel.InsertAgency(pInfo, myAgency)
	AgentModel.SetAgent(pInfo.userid, myAgency)	

	--local sqlCase = "update ag_player set agent1num=agent1num+1 where userid="..toAgency.userid
	--SqlServer.rpush(sqlCase)

end

function PlayerModel.CheckIsBindAgency(pInfo, unionID)

	--在每个玩家创建角色的时候，检查是否绑定代理了。
	if unionID == nil or unionID == '' then
		return nil
	end
	local sqlCase = "select binduserid from ag_author where unionid='"..unionID.."' order by id desc"
	
	mysqlItem:executeQuery(sqlCase)
	local invitestr = mysqlItem:fetch()
	if invitestr == nil then
		return nil
	end

	local toAgency = AgentModel.GetAgentInfo(tonumber(invitestr))
	
	
	if toAgency == nil then
		--说明被绑定的玩家还没有绑定别人。
		return nil
	end
	
	--一旦绑定别人了，就会在后台插入一个记录
	local sqlCase = ""
	myAgency = st_agent_pb.agentinfo()
	if toAgency.level == 0 then   --说明这个是玩家，绑定的代理跟该玩家一样的
		myAgency.userid = pInfo.userid
		myAgency.invitestr = tostring(pInfo.userid)
		myAgency.level = 0
		myAgency.soleagent = toAgency.soleagent
		myAgency.agent1 = toAgency.agent1
		myAgency.agent2 = toAgency.agent2
		myAgency.agent3 = toAgency.agent3
		myAgency.agent4 = toAgency.agent4
		myAgency.qrcodeurl = AgentModel.GetQrcodeUrl(pInfo.userid)
		AgentModel.InsertAgency(pInfo, myAgency)
		AgentModel.SetAgent(pInfo.userid, myAgency)
		
	else
		myAgency.userid = pInfo.userid
		myAgency.invitestr = tostring(pInfo.userid)
		myAgency.level = 0
		myAgency.soleagent = toAgency.level == 99 and toAgency.userid or toAgency.soleagent   --如果他自己是总代，就属于他自己，如果他自己不是总代，就属于他的总代
		myAgency.agent1 = toAgency.userid       --属于他自己的玩家
		myAgency.agent2 = toAgency.agent1       --自己的玩家的上一级
		myAgency.agent3 = toAgency.agent2    
		myAgency.agent4 = toAgency.agent3
		myAgency.qrcodeurl = AgentModel.GetQrcodeUrl(pInfo.userid)

		AgentModel.InsertAgency(pInfo, myAgency)
		AgentModel.SetAgent(pInfo.userid, myAgency)	
	end

	
end

function PlayerModel.LoadPlayer(userID, pInfo)
	
	local sqlCase = "select * from dy_player where userid="..userID
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		LogBehavior.Error(userID, "player", "LoadPlayer",-1,"加载数据库玩家信息失败")
		return nil
	end
	if pInfo == nil then
		pInfo = st_human_pb.playerinfo()
	end
	pInfo.userid = tonumber(sqlData[1])
	
	pInfo.cid = sqlData[2]
	pInfo.account = sqlData[3]
	pInfo.password = sqlData[4]
	pInfo.nickname = string.restorehtmlspecialchars(sqlData[5])
	
	pInfo.face_1 = sqlData[6]
	pInfo.face_2 = sqlData[7]
	pInfo.face_3 = sqlData[8]
	pInfo.face_4 = sqlData[9]
	pInfo.regdate = sqlData[10]
	
	pInfo.sex = tonumber(sqlData[11])
	pInfo.age = tonumber(sqlData[12])
	pInfo.email = sqlData[13]
	
	pInfo.phonenum = sqlData[14]
	pInfo.exp = tonumber(sqlData[15])
	pInfo.level = tonumber(sqlData[16])
	pInfo.viplevel = tonumber(sqlData[17])
	pInfo.jetton = sqlData[18]
	pInfo.gold = tonumber(sqlData[19])
	pInfo.money = tonumber(sqlData[20])
	
	pInfo.channel = sqlData[21]
	pInfo.province = sqlData[22]
	pInfo.city = sqlData[23]
	pInfo.popularity = tonumber(sqlData[24])
	pInfo.idenstatus = tonumber(sqlData[25])
	pInfo.bindtype = tonumber(sqlData[26])
	
	pInfo.bindnick = sqlData[27]
	pInfo.platformid = sqlData[28]
	pInfo.imei = sqlData[29]
	pInfo.devname = string.restorehtmlspecialchars(sqlData[30])
	
	pInfo.mobiletype = tonumber(sqlData[32])
	
	pInfo.lasttime = tonumber(sqlData[33])
	pInfo.penulttime = tonumber(sqlData[34])
	pInfo.silent = tonumber(sqlData[35])
	pInfo.isban = tonumber(sqlData[36])
	pInfo.isrobot = tonumber(sqlData[37])
	pInfo.playtype = tonumber(sqlData[38])
	pInfo.description = sqlData[39]
	pInfo.blacklist = tonumber(sqlData[40])
	pInfo.propertyid = tonumber(sqlData[41])
	
	pInfo.ticket = tonumber(sqlData[42])
	pInfo.location = "在线"
	pInfo.bank_password = sqlData[43] or ""
	pInfo.bank_jetton = sqlData[44] or "0"
	pInfo.ip = sqlData[45] or ""
	redisItem:set("cidinfo"..pInfo.cid, pInfo.userid, PlayerModel.redis_index) 
	redisItem:set("playerinfo"..pInfo.userid, pInfo:SerializeToString(), PlayerModel.redis_index)
	redisItem:hset(PlayerModel.player_jetton, pInfo.userid, pInfo.jetton, PlayerModel.redis_index)
	
	return pInfo
end


function PlayerModel.ChangeSimpleInfo( pInfo, pSimpleInfo )
	if pSimpleInfo == nil then
		pSimpleInfo = st_human_pb.playersimpleinfo()
	end
	pSimpleInfo.userid = pInfo.userid
	pSimpleInfo.nickname = pInfo.nickname
	pSimpleInfo.sex = pInfo.sex
	pSimpleInfo.level = pInfo.level
	pSimpleInfo.viplevel = pInfo.viplevel
	pSimpleInfo.jetton = pInfo.jetton
	pSimpleInfo.gold = pInfo.gold
	pSimpleInfo.face_1 = pInfo.face_1
	pSimpleInfo.location = pInfo.location
	pSimpleInfo.propertyid = pInfo.propertyid
	pSimpleInfo.popularity = pInfo.popularity
	pSimpleInfo.ip = pInfo.ip
	pSimpleInfo.location_x = pInfo.location_x
	pSimpleInfo.location_y = pInfo.location_y
	return pSimpleInfo
end

function PlayerModel.GetSimpleInfo( userID, pSimpleInfo )
	if pSimpleInfo == nil then
		pSimpleInfo = st_human_pb.playersimpleinfo()
	end

	local pInfo = PlayerModel.GetPlayerInfo(userID)
	
	pSimpleInfo.userid = pInfo.userid
	pSimpleInfo.nickname = pInfo.nickname
	pSimpleInfo.sex = pInfo.sex
	pSimpleInfo.level = pInfo.level
	pSimpleInfo.viplevel = pInfo.viplevel
	pSimpleInfo.jetton = pInfo.jetton
	pSimpleInfo.face_1 = pInfo.face_1
	pSimpleInfo.location = pInfo.location
	return pSimpleInfo
end

function PlayerModel.SetPlayerInfo(pInfo)
	redisItem:set("playerinfo"..pInfo.userid, pInfo:SerializeToString(), PlayerModel.redis_index)
end


function PlayerModel.DayFirstCheck(userID, pInfo)

	if pInfo == nil then
		pInfo = PlayerModel.GetPlayerInfo(userID)
	end
	
	MissionModel.ResetUserDrawNum(userID)
	MissionModel.DayFirstLogin(userID)    --先把任务的列表清空
	GoodsModel.DayFirstLogin(userID)        --在对物品做检查物品的时间，然后在检查任务的
	--MailModel.UserDayCheck(userID)        --只有是第一次登陆，才会检查这个
	--FriendsModel.DayFirstCheck(userID)
	--MajiangHistory.DailyCheck(userID)
	AgentModel.DayFirstLogin(userID)
	if g_servername == "run_hlyd" or g_servername == "run_test" or g_servername == "run_tyyd" then
		
		pInfo.gold = g_hlydDefine.day_init_gold
		PlayerModel.SetPlayerInfo(pInfo)
		MailModel.HlydDayAdd(pInfo)
	end
end

function PlayerModel.UpdateLoginTime(userID)
	--更新玩家的登录时间，主要用于玩家在线的时候的隔天的严重
	--这里可能需要加锁限制，还没有想好怎么加，暂时先不加
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	local timeSec = TimeUtils.GetTime();
	pInfo.penulttime = pInfo.lasttime
	pInfo.lasttime = timeSec
	local sqlCase = "update dy_player set penulttime="..pInfo.penulttime..",lasttime="..pInfo.lasttime.." where userid="..pInfo.userid
	
	PlayerModel.SetPlayerInfo(pInfo)
	--LogModel.LogPlayerPush(sqlCase)
	LogModel.GameSqlServer(sqlCase)
end

function PlayerModel.PlayerLogin(pInfo, isNewPlayer)

	local timeSec = TimeUtils.GetTime();
	
	if isNewPlayer == true then
		LogServer.NewPlayer(pInfo)
	end
		
	local diffDay = TimeUtils.DifferentDay(pInfo.lasttime, timeSec)
		
	--表示当天是第一次登陆
	if diffDay > 0 then
		PlayerModel.DayFirstCheck(pInfo.userid, pInfo)
	end
		
	
	
	NoticeModel.PlayerLogin(pInfo.userid)
	
	LogServer.PlayerLogin(pInfo)
	
	pInfo.penulttime = pInfo.lasttime
	pInfo.lasttime = timeSec
	pInfo.location = "在线"
	luaPrint(pInfo.userid.." login")
	local sqlCase = "update dy_player set penulttime="..pInfo.penulttime..",lasttime="..pInfo.lasttime.." where userid="..pInfo.userid
	PlayerModel.SetPlayerInfo(pInfo)
	LogModel.GameSqlServer(sqlCase)
end

function PlayerModel.AddMoney(pInfo, amount, modules, logMsg)

	
	pInfo.money = pInfo.money + amount
	
	local sqlCase = "update dy_player set money="..pInfo.money.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--这里加入log的统计
	
	SysCountService.IssueMoney(amount)
	--LogBehavior.Info(pInfo, modules, "PlayerModel.AddMoney", 0, logMsg..",获得钻石"..amount.."个")
end

function PlayerModel.AddJetton(pInfo, amount, modules, logMsg)
	if amount == 0 then
		return 
	end
	--local tempJetton = tonumber(pInfo.jetton) + amount
	--pInfo.jetton = tostring(tempJetton)



	if amount > 0 and modules ~= "pay" and modules ~= "give" and modules ~= "bind" and modules ~= "award" and modules ~= "vippay" and modules ~= "player" and modules ~= "award" then
		RankModel.AddDailyWin( pInfo.userid, amount )
	end


	pInfo.jetton = tostring( PlayerModel.AddJettonByUserID(pInfo.userid, amount, modules, logMsg) )
end




function PlayerModel.AddJettonByUserID(userID, amount, modules, logMsg)
	if amount == 0 then
		return PlayerModel.GetJettonFromRedis(userID)
	end

	amount = math.floor(amount)

	if false == RobotService.IsRobot(userID) then
		--LogBehavior.InfoJetton(userID, modules, "PlayerModel.AddJetton", 0, logMsg..":金币"..amount.."个", amount, tonumber(amount))

		--if modules ~= "shop" and modules ~= "ExchangeGifts" and modules ~= "zhuangzhang" and modules ~= "give" and modules ~= "pay" then
		if modules ~= "giftgold" and modules ~= "bind" and modules ~= "shop" and modules ~= "ExchangeGifts" and modules ~= "zhuangzhang" 
			and modules ~= "give" and modules ~= "CommissionGet" and modules ~= "bankruptbubsidy" and modules ~= "WiringDrawMoney" and modules ~= "WithdrawDeposit" and modules ~= "bank" and modules ~= "pay" and modules ~= "DrawBonus" then
			--这里开始统计
			--local sqlCase = 

			redisItem:hincrby(PlayerModel.player_winjetton, userID, amount, PlayerModel.redis_index)

		end	
		PlayerModel.IncrJettonToRedis(userID, amount, 0)
	else
		--LogBehavior.InfoJetton(pInfo, modules, "robot-AddJetton", 0, logMsg..":金币"..amount.."个", amount, tonumber(pInfo.jetton))
		PlayerModel.IncrJettonToRedis(userID, amount, 1)
	end
	
	local userJetton = PlayerModel.GetJettonFromRedis(userID)
	LogServer.CheckMaxJetton(userID, userJetton)
	return userJetton
end


function PlayerModel.IncrJettonToRedis(userID, jetton, IsRobot)
	
	if IsRobot == 0 then
		ThreadManager.PlayerLock(userID)
	end
	--jetton = tostring(jetton)
	--if tonumber(jetton) < 0 then
		--print("jetton===================="..jetton)
		--redisItem:hincrby(PlayerModel.player_jetton, userID, jetton, PlayerModel.redis_index)
	--else
	--	print("jetton===================="..jetton)
	--	redisItem:hincrby(PlayerModel.player_jetton, userID, 0 + tonumber(jetton), PlayerModel.redis_index)
	--end
	--redisItem:hset(PlayerModel.player_update, userID, 1, PlayerModel.redis_index)
	
	local getNum = PlayerModel.GetJettonFromRedis(userID)
	getNum = getNum + tonumber(jetton)
	local getStr = string.numtostring(getNum)
	
	redisItem:hset(PlayerModel.player_jetton, userID, getStr, PlayerModel.redis_index)
	
	if IsRobot == 0 then
		ThreadManager.PlayerUnLock(userID)
	end
	
	redisItem:hset(PlayerModel.player_update, userID, 1, PlayerModel.redis_index)
end

function PlayerModel.GetJettonFromRedis(userID)
	local jetton = redisItem:hget(PlayerModel.player_jetton, userID, PlayerModel.redis_index)
	
	if jetton == nil then
		local pInfo = PlayerModel.LoadPlayer(userID)
		jetton = pInfo.jetton
		
		
		PlayerModel.IncrJettonToRedis(userID, jetton, pInfo.isrobot)
	end
	
	return tonumber(jetton)
end
function PlayerModel.GetAllUpdatePlayer()
	return redisItem:hkeys(PlayerModel.player_update, PlayerModel.redis_index)
end


function PlayerModel.AddGold(pInfo, amount, modules, logMsg)
	pInfo.gold = pInfo.gold + amount
	local sqlCase = "update dy_player set gold="..pInfo.gold.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	
	--这里加入log的统计
end

function PlayerModel.AddPopular(pInfo, amount, modules, logMsg)
	pInfo.popularity = pInfo.popularity + amount
	local sqlCase = "update dy_player set popularity="..pInfo.popularity.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	PopularModel.AddRank(pInfo.userid, pInfo.popularity)
end

function PlayerModel.DecMoney(pInfo, amount, modules, logMsg)	
	pInfo.money = pInfo.money - amount
	local sqlCase = "update dy_player set money="..pInfo.money.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	SysCountService.RecycleMoney(amount)
	LogBehavior.Info(pInfo, modules, "PlayerModel.DecMoney", 0, logMsg..":钻石"..amount.."个")
end

function PlayerModel.DecJetton(pInfo, amount, modules, logMsg)
	if amount == 0 then
		return 
	end	
	
	if amount > tonumber(pInfo.jetton) then
		--这里是扣分的，不能扣到负分。
		amount = pInfo.jetton
	end
		
	pInfo.jetton = tostring( PlayerModel.DecJettonByUserID(pInfo.userid, tonumber(amount), modules, logMsg) )

end

function PlayerModel.DecJettonByUserID(userID, amount, modules, logMsg)
	
	if false == RobotService.IsRobot(userID) then
		--LogBehavior.InfoJetton(userID, modules, "PlayerModel.DecJetton", 0, logMsg..":金币"..amount.."个", -amount, tonumber(amount))
		
		--if modules ~= "shop" and modules ~= "ExchangeGifts" and modules ~= "zhuangzhang" and modules ~= "give" and modules ~= "WithdrawDeposit" then
		if modules ~= "giftgold" and modules ~= "pay" and modules ~= "shop" and modules ~= "ExchangeGifts" and modules ~= "zhuangzhang" 
			and modules ~= "give" and modules ~= "CommissionGet" and modules ~= "bankruptbubsidy" and modules ~= "WithdrawDeposit" and modules ~= "bank" then
			redisItem:hincrby(PlayerModel.player_winjetton, userID, 0-amount, PlayerModel.redis_index)
		end
		PlayerModel.IncrJettonToRedis(userID, -amount, 0)
	else
		--LogBehavior.InfoJetton(pInfo, modules, "robot-DecJetton", 0, logMsg..":金币"..amount.."个", -amount, tonumber(pInfo.jetton))
		PlayerModel.IncrJettonToRedis(userID, -amount, 1)
	end	

		
	return PlayerModel.GetJettonFromRedis(userID)
end

function PlayerModel.DecGold(pInfo, amount, modules, logMsg)
	pInfo.gold = pInfo.gold - amount
	local sqlCase = "update dy_player set gold="..pInfo.gold.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)

end


function PlayerModel.DecPopular(pInfo, amount, modules, logMsg)
	pInfo.popularity = pInfo.popularity - amount
	local sqlCase = "update dy_player set popularity="..pInfo.popularity.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	PopularModel.AddRank(pInfo.userid, pInfo.popularity)
end

--这里个添加的是排除了礼包之外的情况
function PlayerModel.AddGoodsList(pInfo, gAllList,addList, modules, logMsg)
	
	for k, v in ipairs(addList) do
		local category = math.modf(v[1]/1000)

		if category == g_goodsCategory.currency then
			if v[1] == g_goodsID.money then
				PlayerModel.AddMoney(pInfo, v[2], modules, logMsg)
			elseif v[1] == g_goodsID.jetton then
				PlayerModel.AddJetton(pInfo, v[2], modules, logMsg)
				local sqlCase = "update log_player set giftgoldday=giftgoldday+"..v[2]..",giftgoldweek=giftgoldweek+"
				..v[2]..",giftgoldmonth=giftgoldmonth+"..v[2]..",giftgoldall=giftgoldall+"..v[2].." where userid="..pInfo.userid
				mysqlLog:execute(sqlCase)
			elseif v[1] == 1003 then
				PlayerModel.AddGold(pInfo, v[2], modules, logMsg)
			end
		else
			GoodsModel.AddGoods(pInfo, gAllList, v[1], v[2], modules, logMsg)
		end
	end
end

function PlayerModel.GetRandomCID()
	local strList = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g'
	,'h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'}
	local ret = ""
	for i = 1,8 do
		local len = math.myrandom(1,#strList)
		ret = ret..strList[len]
	end
	return ret
end

function PlayerModel.SendPlayerInfo(pInfo, typeList)

	if pInfo.isrobot == 1 then
		return nil
	end
	
	local gcUpdate = msg_human_pb.gcupdateplayerinfo()
	
	for k,v in ipairs(typeList) do

		if pInfo[v] ~= nil then
			gcUpdate.typelist:append(v)
			local test = pInfo[v]
			gcUpdate.valuelist:append(tostring(pInfo[v]))
		end
	end
	gcUpdate.result = 0
	SendMessage(pInfo.userid, PacketCode[1026].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())

end

function PlayerModel.SendJetton(pInfo)
	if pInfo.isrobot == 1 then
		return nil
	end
	
	local gcUpdate = msg_human_pb.gcupdateplayerinfo()

	gcUpdate.typelist:append("jetton")
	gcUpdate.valuelist:append(tostring(pInfo.jetton))
	gcUpdate.result = 0
	SendMessage(pInfo.userid, PacketCode[1026].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())

end

function PlayerModel.SendMoneyJetton(pInfo)
	if pInfo.isrobot == 1 then
		return nil
	end
	
	local gcUpdate = msg_human_pb.gcupdateplayerinfo()
	
	gcUpdate.typelist:append("jetton")
	gcUpdate.valuelist:append(pInfo.jetton)
	
	gcUpdate.typelist:append("money")
	gcUpdate.valuelist:append(tostring(pInfo.money))	
	

	gcUpdate.result = 0
	SendMessage(pInfo.userid, PacketCode[1026].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())

end


function PlayerModel.Exit(userID)
	--当有socket关掉或者退出的时候，调用该函数
	
	--local pInfo = PlayerModel.GetPlayerInfo(userID)
	--pInfo.location = "离线"
	--PlayerModel.SetPlayerInfo(pInfo)
end


function PlayerModel.PlayerReport(userID, IP)
	
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	
	if pInfo ~= nil and pInfo.ip ~= IP and IP ~= '' then
		
		--local sqlCase = "update dy_player set ip='"..IP.."' where userid="..userID
		--mysqlItem:execute(sqlCase)
		--pInfo.ip = IP		
		--PlayerModel.SetPlayerInfo(pInfo)
		local data = {}
		data['userid'] = userID
		data['ip'] = IP
		--processWork("UserIPCheck",luajson.encode(data))
		LocationModel.PushList(data)
	end

end

function PlayerModel.GetPlayerTableInfo(userID) --检查玩家是否在牌桌中
	
	--首先检查是否在欢乐赢豆中
	local tableID = 0

	local tableID = PlayerModel.GetCurrTableID(userID)
	
	return 0,tonumber(tableID)
end


function PlayerModel.GetCurrTableID(userID)
	--检查当前玩家是否在牌桌中，返回对应的ID和，tableuserlist
	local tableID =  0 

	
	if g_servername == "run_dwc858" then
		tableID = DouNiuModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = DouNiuModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_douNiuDefine.game_type
			end
		end

		tableID = ErshModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = ErshModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_ErshDefine.game_type		
			end
		end
		
		tableID = BcbmModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = BcbmModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_bcbmDefine.game_type		
			end
		end
		
		tableID = FqzsModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = FqzsModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_fqzsDefine.game_type		
			end
		end
		
		tableID = FruitModel.GetUserTableID(userID)

		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = FruitModel.GetTableInfo(tableID) 
			if tInfo ~= nil then
				
				return tableID, g_fruitDefine.game_type
			end
		end
		
		tableID = BarccatatModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_barccatatDefine.game_type
		end	
		
		tableID = PszModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_PszDefine.game_type
		end
		tableID = BrnnModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = BrnnModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_brnnDefine.game_type
			end
		end		
		tableID = LhdbModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_lhdbDefine.game_type
		end
		
		tableID = TgpdModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_tgpdDefine.game_type
		end
		
		tableID = LkpyModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_lkpyDefine.game_type
		end
		
		tableID = YqsModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_yqsDefine.game_type
		end
		
		tableID = ForestModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return tableID, g_forestDefine.game_type
		end

		tableID = HongHeiModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = HongHeiModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_hongheiDefine.game_type		
			end
		end
        
        tableID = LhdModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = LhdModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_lhdDefine.game_type		
			end
		end	
		
		tableID = DdzModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = DdzModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_DdzDefine.game_type		
			end
		end	

        tableID = CjmpModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = CjmpModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_cjmpDefine.game_type		
			end
		end	

        tableID = SicboModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = SicboModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_sicboDefine.game_type		
			end
		end	
		
		tableID = TexasModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = TexasModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				
				return tableID, g_TexasDefine.game_type		
			end
		end	
		
		tableID = SlhbModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = SlhbModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_slhbDefine.game_type		
			end
		end	

        tableID = CaiPiaoXyftModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = CaiPiaoXyftModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_caipiaoDefine.xyft_game_type		
			end
		end	

        tableID = CaiPiaoLhcModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = CaiPiaoLhcModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_caipiaoDefine.lhc_game_type		
			end
		end
        
        tableID = CaiPiaoBjscModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = CaiPiaoBjscModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_caipiaoDefine.bjsc_game_type		
			end
		end		

		tableID = TtzModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = TtzModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_TtzDefine.game_type		
			end
		end	

		tableID = FenFenCaiModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = FenFenCaiModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				return tableID, g_fenfencaiDefine.game_type		
			end
		end		
		
		--tableID = SanGongModel.SanGongGetUserTableID(userID)
		--if tableID ~= nil and tonumber(tableID) ~= 0 then
			---local tInfo = SanGongModel.SanGongGetTableInfo(tableID)
		--	if tInfo ~= nil then
			--	return tableID, g_sangongDefine.game_type		
		--	end
		--end		
	end

	
	return 0,0
end


function PlayerModel.ModifyPInfo(pInfo,modMsg)
	--传入的是具备repeated的协议结构
	local sqlCase="update dy_player set level="..pInfo.level
	for k,v in ipairs(modMsg.typelist) do
		
		if v == "password" then
			pInfo.password = modMsg.valuelist[k]
			sqlCase = sqlCase..",password='"..pInfo.password.."'"
		elseif v == "nickname" then
			pInfo.nickname = modMsg.valuelist[k]
			sqlCase = sqlCase..",nickname='"..pInfo.nickname.."'"
		elseif v == "sex" then
			pInfo.sex = tonumber(modMsg.valuelist[k])
			sqlCase = sqlCase..",sex="..pInfo.sex
		elseif v == "age" then
			pInfo.age = tonumber(modMsg.valuelist[k])
			sqlCase = sqlCase..",age="..pInfo.age
		elseif v == "email" then
			pInfo.email = modMsg.valuelist[k]
			sqlCase = sqlCase..",email='"..pInfo.email.."'"
		elseif v == "face_1" then
			pInfo.face_1 = modMsg.valuelist[k]
			sqlCase = sqlCase..",face_1='"..pInfo.face_1.."'"
		elseif v == "face_2" then
			pInfo.face_2 = modMsg.valuelist[k]
			sqlCase = sqlCase..",face_2='"..pInfo.face_2.."'"
		elseif v == "face_3" then
			pInfo.face_3 = modMsg.valuelist[k]
			sqlCase = sqlCase..",face_3='"..pInfo.face_3.."'"
		elseif v == "face_4" then
			pInfo.face_4 = modMsg.valuelist[k]
			sqlCase = sqlCase..",face_4='"..pInfo.face_4.."'"
		elseif v == "description" then
			pInfo.description = modMsg.valuelist[k]
			sqlCase = sqlCase..",description='"..pInfo.description.."'"
		elseif v == "location_x" then
			pInfo.location_x = modMsg.valuelist[k]
		elseif v == "location_y" then
			pInfo.location_y = modMsg.valuelist[k]
		elseif v == "bindnick" then
			pInfo.bindnick = modMsg.valuelist[k]
			sqlCase = sqlCase..",bindnick='"..pInfo.bindnick.."'"
		elseif v == "ip" then
			pInfo.ip = modMsg.valuelist[k]
			sqlCase = sqlCase..",ip='"..pInfo.ip..","
		elseif v == "province" then
			pInfo.province = modMsg.valuelist[k]
			sqlCase = sqlCase..",province='"..pInfo.province.."'"
		end
		
	end
	
	sqlCase = sqlCase.." where userid="..pInfo.userid
	mysqlItem:execute(sqlCase)
end

--[[
	货币变动记录
	@param userID: number			用户ID
	@param initial:					起始值
	@param amount: number			变动数量
	@param currencyType: number		货币类型,0为money,1为jetton,2为bankjetton
	@param optType: number			操作类型,0为代理赠送钻石,1为玩家收到代理赠送的钻石,2为银行存款,3为银行取款,4为银行转账
	@param remark: string			说明
	@returns void
--]]
function PlayerModel.InsertCurrencyChangedRecord(userID, initial, amount, currencyType, optType, remark)
	local sqlCase = "insert into log_currencychangedrecord(userid, currency_type, initial, amount, opt_type, remark)"
		.."values("..userID..", "..currencyType..", "..initial..", "..amount..", "..optType..", '"..remark.."')"
	SqlServer.rpush(sqlCase)
end

--获取玩家抽奖信息
function PlayerModel.GetUserLuckWheelInfo(userID,luckNum)

    local sqlCase = "select * from dy_luckwheel where userid='"..userID.."'and lucknum= '"..luckNum.."'"
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	local userinfo  = {}
	if sqlData == nil then
		return nil
	end
    userinfo["lucknum"] = tonumber(sqlData[2])
    userinfo["userid"] = tonumber(sqlData[3])
    userinfo["restate"] = tonumber(sqlData[4])
    userinfo["jettonlist"] = sqlData[5]
    userinfo["rejetton"] = tonumber(sqlData[6])
    userinfo["achievement"] = sqlData[7]
    userinfo["createdate"] = sqlData[8]
    userinfo["updatedate"] = sqlData[9]
    return userinfo
end

--获取玩家抽奖信息
function PlayerModel.InsertUserLuckWheelInfo(userID,luckNum,restate,jettonlist,rejetton,achievement)

    local sqlCase = "insert into dy_luckwheel(lucknum, userid, restate, jettonlist,rejetton,achievement)"
		.."values("..luckNum..", "..userID..", "..restate..", '"..jettonlist.."', "..rejetton..", '"..achievement.."')"
	mysqlItem:execute(sqlCase)
    return userinfo
end


function PlayerModel.GetAccountByPhone(phonenum)
	--从绑定的账号中获取
	
	if phonenum == nil or phonenum == "" then
		return nil
	end
	
	if true == redisItem:hexists( PlayerModel.Player_phone_account, phonenum,  PlayerModel.redis_index) then
		return redisItem:hget( PlayerModel.Player_phone_account, phonenum, PlayerModel.redis_index )
	end
	
	local sqlCase = "select account from dy_player where phonenum='"..phonenum.."'"
	mysqlItem:executeQuery(sqlCase)
	local account = mysqlItem:fetch()
	if account ~= nil then
		redisItem:hset( PlayerModel.Player_phone_account, phonenum, account , PlayerModel.redis_index )
	end
	return account
end

function PlayerModel.GetBankJetton(userID)
	local bankJetton = redisItem:hget( PlayerModel.player_bankjetton, userID, PlayerModel.redis_index )
	if bankJetton == nil then
		local sqlCase = "select bank_jetton from dy_player where userid="..userID
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch()
		sqlData = sqlData == nil and 0 or tonumber(sqlData)
		
		redisItem:hset( PlayerModel.player_bankjetton, userID, sqlData,  PlayerModel.redis_index )
		bankJetton = tostring(sqlData)
	end
	return bankJetton
end

function PlayerModel.IncrBankJetton(userID, addCount)
	redisItem:hincrby( PlayerModel.player_bankjetton, userID, addCount, PlayerModel.redis_index )
end

function PlayerModel.CountPondConfig(pondInfo, allpour, allwin)
	if pondInfo.historytotalpour == "" then
		pondInfo.historytotalpour = "0"
	end
	if pondInfo.todaytotalpour == "" then
		pondInfo.todaytotalpour = "0"
	end
	if pondInfo.historywinpour == "" then
		pondInfo.historywinpour = "0"
	end
	if pondInfo.todaywinpour == "" then
		pondInfo.todaywinpour = "0"
	end
	pondInfo.historytotalpour = tostring(tonumber(pondInfo.historytotalpour) + tonumber(allpour))
	pondInfo.todaytotalpour = tostring(tonumber(pondInfo.todaytotalpour) + tonumber(allpour))

	pondInfo.historywinpour = tostring(tonumber(pondInfo.historywinpour) + tonumber(allwin))
	pondInfo.todaywinpour = tostring(tonumber(pondInfo.todaywinpour) + tonumber(allwin))

	if tonumber(pondInfo.historytotalpour) > 0 then
		pondInfo.historywinrate = tonumber(pondInfo.historywinpour) / tonumber(pondInfo.historytotalpour) * 100
	end

	if tonumber(pondInfo.todaytotalpour) > 0 then
		pondInfo.todaywinrate = tonumber(pondInfo.todaywinpour) / tonumber(pondInfo.todaytotalpour) * 100
	end
end