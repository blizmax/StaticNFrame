
PlayerModel = {}
PlayerModel.redis_index = "redis_player"

PlayerModel.kit_userList = "player_kit"

function PlayerModel.LoadPlayerInfoByCID(account,password,pInfo)
	
	local sqlCase = "select * from dy_player where account='"..account.."'"
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	
	if sqlData == nil then
		LogBehavior.Error(account, "player", "LoadPlayerInfoByCID",-1,"账号不存在,account="..account)
		return ReturnCode["account_not_exist"]
	end

	if password ~= sqlData[4] then
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
	pInfo.jetton = tonumber(sqlData[18])
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
	pInfo.agencystatus = tonumber(sqlData[42])
	pInfo.location = "在线"
	pInfo.registration1 = sqlData[45] 
	pInfo.registration2 = sqlData[46] 
	pInfo.registration3 = sqlData[47] 
	pInfo.signinmonth = tonumber(sqlData[48]) 
	pInfo.bsignin = tonumber(sqlData[49]) 
	pInfo.turntablenum = tonumber(sqlData[50]) 
	pInfo.tengames = tonumber(sqlData[51]) 
	pInfo.todayshare = tonumber(sqlData[52]) 
	pInfo.caninvite = tonumber(sqlData[53])
	pInfo.totalgames = tonumber(sqlData[54]) or 0
	--[[
	local achieveInfo = MissionModel.GetUserAchieveInfo(pInfo.userid)
	if db_mission[achieveInfo.missionid] ~= nil then
		--取得玩家的称号
		pInfo.title = db_mission[achieveInfo.missionid][db_missionIndex.name]
	else
		pInfo.title = "暂无称号" 
	end
	]]
	--LogBehavior.Info(pInfo, "player", "LoadPlayerInfoByCID", 0, "加载数据库玩家信息成功")
	local tJson = {}
	table.insert(tJson, pInfo.password)
	table.insert(tJson, pInfo.userid)
	redisItem:set("account"..pInfo.account, luajson.encode(tJson), PlayerModel.redis_index)
	redisItem:set("playerinfo"..pInfo.userid, pInfo:SerializeToString(), PlayerModel.redis_index)
	return 0
end

function PlayerModel.GetPlayerInfoByCID(account, password, pInfo)
	
	if true == redisItem:exists("account"..account, PlayerModel.redis_index) then
		local strUserID = redisItem:get("account"..account, PlayerModel.redis_index)
		local arr = luajson.decode(strUserID)

		if arr[1] ~= password then
			LogBehavior.Error(account,"player", "GetPlayerInfoByCID", ReturnCode["password_not_match"],"登录密码不匹配,password="..password)
			return ReturnCode["password_not_match"]
		end

		if nil == PlayerModel.GetPlayerInfo(tonumber(arr[2]), pInfo) then
			LogBehavior.Error(account,"player", "GetPlayerInfoByCID", ReturnCode["account_not_exist"],"账号不存在，userid="..arr[2])
			return ReturnCode["account_not_exist"] 
		end
		return 0
	end
	
	return PlayerModel.LoadPlayerInfoByCID(account, password, pInfo)
end

function PlayerModel.GetPlayerInfo(playerID, pInfo)

	if playerID == 0 then
		return nil
	end
	
	if pInfo == nil then
		pInfo = st_human_pb.playerinfo()
	end
	if true == redisItem:exists("playerinfo"..playerID, PlayerModel.redis_index) then
		local strGet = redisItem:get("playerinfo"..playerID, PlayerModel.redis_index)
		pInfo:ParseFromString(strGet)
		return pInfo
	end
	
	return PlayerModel.LoadPlayer(playerID, pInfo)
end

function PlayerModel.GetInitFaceID()
	local facdIDList = {"90001_1001","90002_1001","90003_1001","90004_1001","90005_1001","90006_1001",
	"90007_1001","90008_1001","90009_1001","90010_1001","90011_1001","90012_1001","90013_1001",
	"90014_1001","90015_1001","90016_1001","90017_1001","90018_1001"
	}
	
	local indexID = math.myrandom(1,#facdIDList)
	return facdIDList[indexID]
end

function PlayerModel.CreatePlayer(cgmsg)
	local sqlCase = "insert into dy_player(cid,account,password,nickname,channel,province,city,bindtype,bindnick,platformid,imei,devname,mobiletype,jetton,money,lasttime,face_1,sex) values('"..
	cgmsg.cid.."','"..cgmsg.account.."','"..cgmsg.password.."','"..string.htmlspecialchars(cgmsg.nickname).."','"..cgmsg.channel.."','"..cgmsg.province.."','"..cgmsg.city.."',"..cgmsg.bindtype
	..",'"..cgmsg.bindnick.."','"..cgmsg.platformid.."','"..cgmsg.imei.."','"..string.htmlspecialchars(cgmsg.devname).."',"..cgmsg.mobiletype..","..g_initData.jetton..","..g_initData.money..","..TimeUtils.GetTime()..",'"..PlayerModel.GetInitFaceID().."',"..cgmsg.sex..")"
	local ret = mysqlItem:execute(sqlCase)
	
	LogFile("new_player","sqlCase="..sqlCase)
	if ret == nil then
		LogBehavior.Error(cgmsg.cid,"Player", "CreatePlayer", -1, "创建角色失败")
		return nil
	end
	SysCountService.IssueJetton(g_initData.jetton) --初始化部分也要统计进来
	SysCountService.PlayerCount()
end


function PlayerModel.CheckIsBindAgency(pInfo, unionID)

	--在每个玩家创建角色的时候，检查是否绑定代理了。
	if unionID == nil or unionID == '' then
		return nil
	end
	local sqlCase = "select binduserid,bindjulebuid from ag_author where unionid='"..unionID.."' order by id desc"
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		return nil
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(sqlData[2])
	if jInfo == nil then
		return nil
	end
	local higherid = tonumber( sqlData[1] )
	sqlCase="insert into dy_julebu_member(userid,julebuid,higherid,status) values("..pInfo.userid..","..sqlData[2]..","..sqlData[1]..",99)"

	mysqlItem:execute(sqlCase)
	
	sqlCase = "select id from dy_julebu_member where userid="..pInfo.userid.." and julebuid="..sqlData[2]
	local julebuID = tonumber(sqlData[2])
	mysqlItem:executeQuery(sqlCase)
	sqlData = mysqlItem:fetch()
	if sqlData == nil then
		return nil
	end

	local aItem = st_julebu_pb.julebumember()
	aItem.userid = pInfo.userid
	aItem.nickname = pInfo.nickname
	aItem.faceid = pInfo.face_1
	aItem.id = tonumber(sqlData)
	aItem.higherid = higherid --tonumber(sqlData[1])
	JulebuModel.AddToMemberList(julebuID, aItem)
	JulebuModel.SetJulebuInfo(jInfo)
	JulebuModel.SetUserJulebuList(pInfo.userid, jInfo.julebuid)	

	jInfo.membernum = jInfo.membernum + 1
	sqlCase = "update dy_julebu set membernum="..jInfo.membernum.." where id="..jInfo.julebuid
	mysqlItem:execute(sqlCase)


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
	pInfo.jetton = tonumber(sqlData[18])
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
	
	pInfo.agencystatus = tonumber(sqlData[42])
	pInfo.location = "在线"
	pInfo.registration1 = sqlData[45] 
	pInfo.registration2 = sqlData[46] 
	pInfo.registration3 = sqlData[47] 
	pInfo.signinmonth = tonumber(sqlData[48]) 
	pInfo.bsignin = tonumber(sqlData[49]) 
	pInfo.turntablenum = tonumber(sqlData[50]) 
	pInfo.tengames = tonumber(sqlData[51]) 
	pInfo.todayshare = tonumber(sqlData[52]) 
	pInfo.caninvite = tonumber(sqlData[53])
	pInfo.totalgames = tonumber(sqlData[54]) or 0
	--[[  --先把封号给去掉
	local achieveInfo = MissionModel.GetUserAchieveInfo(pInfo.userid)
	if db_mission[achieveInfo.missionid] ~= nil then
		--取得玩家的称号
		pInfo.title = db_mission[achieveInfo.missionid][db_missionIndex.name]
	else
		pInfo.title = "暂无称号" 
	end
	]]
	redisItem:set("cidinfo"..pInfo.cid, pInfo.userid, PlayerModel.redis_index) 
	redisItem:set("playerinfo"..pInfo.userid, pInfo:SerializeToString(), PlayerModel.redis_index)
	--LogBehavior.Info(pInfo, "player", "LoadPlayer",0,"加载数据库玩家信息成功")
	
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
	PlayerModel.ResetSomeDate(pInfo) --重置玩家的某些数据
	
	
	--MissionModel.DayFirstLogin(userID)    --先把任务的列表清空
	GoodsModel.DayFirstLogin(userID)  --在对物品做检查物品的时间，然后在检查任务的
	--MailModel.UserDayCheck(userID)  --只有是第一次登陆，才会检查这个
	--FriendsModel.DayFirstCheck(userID)
	--MajiangHistory.DailyCheck(userID)
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
		
	PlayerStatsModel.PlayerLogin(pInfo.userid, diffDay)
	
	NoticeModel.PlayerLogin(pInfo.userid)
	
	LogServer.PlayerLogin(pInfo, diffDay)
	
	pInfo.penulttime = pInfo.lasttime
	pInfo.lasttime = timeSec
	pInfo.location = "在线"
	luaPrint(pInfo.userid.." login")
	local sqlCase = "update dy_player set penulttime="..pInfo.penulttime..",lasttime="..pInfo.lasttime.." where userid="..pInfo.userid
	PlayerModel.SetPlayerInfo(pInfo)
	PlayerModel.PlayerStatus(pInfo.userid,g_playerState.state_resume)
end

function PlayerModel.AddMoney(pInfo, amount, modules, logMsg)

	
	pInfo.money = pInfo.money + amount
	
	local sqlCase = "update dy_player set money="..pInfo.money.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--这里加入log的统计
	--PlayerStatsModel.CheckMaxMoney(pInfo)
	--SysCountService.IssueMoney(amount)
	LogBehavior.Info(pInfo, modules, "PlayerModel.AddMoney", 0, logMsg..",获得钻石"..amount.."个")
	LogServer.AddJetton(modules, amount)
end

--[[
	扣除钻石 但不添加到系统经济
	@param pInfo: msg_human_pb.cggetplayerinfo	玩家信息
	@param amount: number						数量
	@param modules: string						模块
	@param logMsg: string						日志
	@returns void
--]]
function PlayerModel.AddMoneyWithoutSysCount (pInfo, amount, modules, logMsg)
	pInfo.money = pInfo.money + amount
	
	local sqlCase = "update dy_player set money="..pInfo.money.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--这里加入log的统计
	PlayerStatsModel.CheckMaxMoney(pInfo)
	--SysCountService.IssueMoney(amount)
	LogBehavior.Info(pInfo, modules, "PlayerModel.AddMoneyWithoutSysCount", 0, logMsg..",获得钻石"..amount.."个")
end

function PlayerModel.AddJetton(pInfo, amount, modules, logMsg)
	if amount == 0 then
		return 
	end
	pInfo.jetton = pInfo.jetton + amount
	local sqlCase = "update dy_player set jetton="..pInfo.jetton.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--这里加入log的统计
	--这里是欢乐抓豆，暂时先不做统计
	--RankModel.AddJettonList(pInfo.userid,pInfo.jetton)
	--if pInfo.isrobot == 0 then
		--如果不是机器人
		--PlayerMsgModel.CheckMaxJetton(pInfo)
	--else
		--如果是机器人
		--RobotModel.UpdateJetton(pInfo.userid, pInfo.jetton)
	--end
	--PlayerStatsModel.CheckMaxJetton(pInfo)
	--JettonModel.AddRank(pInfo.userid,pInfo.jetton)
	--SysCountService.IssueJetton(amount)
	
	
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
	--SqlServer.rpush(sqlCase)
	--SysCountService.RecycleMoney(amount)
	LogBehavior.Info(pInfo, modules, "PlayerModel.DecMoney", 0, logMsg..":钻石"..amount.."个")
	LogServer.DecJetton(modules, amount)
end

--[[
	扣除钻石 但不添加到系统经济
	@param pInfo: msg_human_pb.cggetplayerinfo	玩家信息
	@param amount: number						数量
	@param modules: string						模块
	@param logMsg: string						日志
	@returns void
--]]
function PlayerModel.DecMoneyWithoutSysCount (pInfo, amount, modules, logMsg)
	pInfo.money = pInfo.money - amount
	local sqlCase = "update dy_player set money="..pInfo.money.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--SysCountService.RecycleMoney(amount)
	LogBehavior.Info(pInfo, modules, "PlayerModel.DecMoneyWithoutSysCount", 0, logMsg..":钻石"..amount.."个")
end

function PlayerModel.DecJetton(pInfo, amount, modules, logMsg)
	if amount == 0 then
		return 
	end	
	pInfo.jetton = pInfo.jetton - amount
	local sqlCase = "update dy_player set jetton="..pInfo.jetton.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--RankModel.AddJettonList(pInfo.userid,pInfo.jetton)
	--MissionModel.BankruptCheck(pInfo) --对破产补偿进行检查
	LogBehavior.Info(pInfo, modules, "PlayerModel.DecJetton", 0, logMsg..":金币"..amount.."个")
	--这里加入log的统计
	--SysCountService.RecycleJetton(amount)
	--JettonModel.AddRank(pInfo.userid,pInfo.jetton)
	
end

function PlayerModel.DecGold(pInfo, amount, modules, logMsg)
	pInfo.gold = pInfo.gold - amount
	local sqlCase = "update dy_player set gold="..pInfo.gold.." where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	--RankModel.AddJettonList(pInfo.userid,pInfo.jetton)
	--MissionModel.BankruptCheck(pInfo) --对破产补偿进行检查
	--LogBehavior.Info(pInfo, modules, "PlayerModel.DecJetton", 0, logMsg..":金币"..amount.."个")
	--这里加入log的统计
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
	gcUpdate.valuelist:append(tostring(pInfo.jetton))
	
	gcUpdate.typelist:append("money")
	gcUpdate.valuelist:append(tostring(pInfo.money))	
	

	gcUpdate.result = 0
	SendMessage(pInfo.userid, PacketCode[1026].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())

end


function PlayerModel.Exit(userID)
	--当有socket关掉或者退出的时候，调用该函数
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	pInfo.location = "离线"
	PlayerModel.SetPlayerInfo(pInfo)
end


function PlayerModel.PlayerReport(userID, IP)
	
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	
	if pInfo ~= nil and pInfo.ip ~= IP then
		
		local sqlCase = "update dy_player set ip='"..IP.."' where userid="..userID
		mysqlItem:execute(sqlCase)
		pInfo.ip = IP
		
		PlayerModel.SetPlayerInfo(pInfo)
	end
	
end

function PlayerModel.GetPlayerTableInfo(userID) --检查玩家是否在牌桌中
	
	--首先检查是否在欢乐赢豆中
	local tableID = 0
	
	if g_servername == "run_gdmj" then
				
		tableID = GdmjModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return g_logintable.type_gdmj, tonumber(tableID)
		end

		tableID = DouNiuModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return g_logintable.type_douniu, tonumber(tableID)
		end

		tableID = PszModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			return g_logintable.type_psz, tonumber(tableID)
		end
	end

	return 0,0
end

function PlayerModel.PlayerStatus(userID,status)  --退出游戏的控制

	local gcmsg = msg_human_pb.gcplayerstatus()
	gcmsg.userid = userID
	gcmsg.status = status
	gcmsg.result = 0
	
	if userID == nil or userID == 0 then
		--这里应该是出现了0的情况，所以在这里多加一个防护
		return userID, 0,0,gcmsg:SerializeToString()
	end
	
	if status == g_playerState.state_leave then 
		PlayerModel.Exit(userID)
	elseif status == g_playerState.state_resume then
		local pInfo = PlayerModel.GetPlayerInfo(userID)
		pInfo.location = "在线"
		PlayerModel.SetPlayerInfo(pInfo)
	end
	
	local tableID, userList = PlayerModel.GetCurrTableID(userID)
	if tableID ~= 0 then
		SendMessage(userList,PacketCode[1024].client,gcmsg:ByteSize(), gcmsg:SerializeToString())
	end
	return userID, 0, 0, gcmsg:SerializeToString()
end

function PlayerModel.GetCurrTableID(userID)
	--检查当前玩家是否在牌桌中，返回对应的ID和，tableuserlist
	local tableID =  0 

	if g_servername == "run_gdmj" then
		
		tableID = GdmjModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = GdmjModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				local userList = GdmjWork.GetUserList(tInfo, userID)
				return tableID, userList
			end
		end

		tableID = DouNiuModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = DouNiuModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				local userList = DouNiuModel.GetUserList(tInfo)
				return tableID, userList		
			end
		end

		tableID = PszModel.GetUserTableID(userID)
		if tableID ~= nil and tonumber(tableID) ~= 0 then
			local tInfo = PszModel.GetTableInfo(tableID)
			if tInfo ~= nil then
				local userList = PszService.GetUserIDList(tInfo,exceptID)
				return tableID, userList		
			end
		end
	end
	
	return 0,{}
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
	@param currencyType: number		货币类型,0为money
	@param optType: number			操作类型,0为代理赠送钻石,1为玩家收到代理赠送的钻石
	@param remark: string			说明
	@returns void
--]]


function PlayerModel.InsertCurrencyChangedRecord(userID, initial, amount, currencyType, optType, remark)
	local sqlCase = "insert into log_currencychangedrecord(userid, currency_type, initial, amount, opt_type, remark)"
		.."values("..userID..", "..currencyType..", "..initial..", "..amount..", "..optType..", '"..remark.."')"
	SqlServer.rpush(sqlCase)
end

--累计局数
function PlayerModel.TenGames(pInfo)
	if pInfo and pInfo.tengames then
		pInfo.tengames = pInfo.tengames + 1
		pInfo.totalgames = pInfo.totalgames + 1
		if pInfo.tengames == 10 then
			pInfo.turntablenum = pInfo.turntablenum + 1
		end
		local modMsg = {55,58,}
		PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
		PlayerModel.SetPlayerInfo(pInfo)
	end
end

function PlayerModel.ResetSomeDate(pInfo)
	if pInfo then
		pInfo.turntablenum = 1  --每天第一次登陆送一次转盘次数
		pInfo.bsignin = 0		--清除已签到记录
		pInfo.tengames = 0		--清除当天的开局次数
		pInfo.todayshare = 0	--重置当天是否分享
		local modMsg = {53,54,55,56}
		PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
	end
end

function PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
	--传入的是具备repeated的协议结构
	local sqlCase="update dy_player set level="..pInfo.level
	for k,v in ipairs(modMsg) do
		
		if v == 49 then
			sqlCase = sqlCase..",registration1='"..pInfo.registration1.."'"
		elseif v == 50 then
			sqlCase = sqlCase..",registration2='"..pInfo.registration2.."'"
		elseif v == 51 then
			sqlCase = sqlCase..",registration3='"..pInfo.registration3.."'"	
		elseif v == 52 then
			sqlCase = sqlCase..",signinmonth='"..pInfo.signinmonth.."'"	
		elseif v == 53 then
			sqlCase = sqlCase..",bsignin='"..pInfo.bsignin.."'"	
		elseif v == 54 then
			sqlCase = sqlCase..",turntablenum='"..pInfo.turntablenum.."'"	
		elseif v == 55 then
			sqlCase = sqlCase..",tengames='"..pInfo.tengames.."'"	
		elseif v == 56 then
			sqlCase = sqlCase..",todayshare='"..pInfo.todayshare.."'"	
		elseif v == 57 then
			sqlCase = sqlCase..",caninvite='"..pInfo.caninvite.."'"	
		elseif v == 58 then
			sqlCase = sqlCase..",totalgames='"..pInfo.totalgames.."'"
		end
		
	end
	
	sqlCase = sqlCase.." where userid="..pInfo.userid
	mysqlItem:execute(sqlCase)
end