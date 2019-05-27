PlayerStatsModel = {}
PlayerStatsModel.redis_index = "player_stats"
PlayerStatsModel.redis_key = "playerstats"

function PlayerStatsModel.GetPlayerStats(userID, pMsg)
	
	if userID == 0 then
		return
	end
	if pMsg == nil then
		pMsg = st_human_pb.playerstats()
	end
	
	local strGet = redisItem:get(PlayerStatsModel.redis_key..userID, PlayerStatsModel.redis_index)
	if strGet ~= nil then
		pMsg:ParseFromString(strGet)
		return pMsg
	end
	
	local sqlCase = "select * from dy_playermsg where userid="..userID
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		local pInfo = PlayerModel.GetPlayerInfo(userID)
		sqlCase = "insert into dy_playermsg(userid,nickname) values("..userID..",'"..pInfo.nickname.."')"
		mysqlItem:execute(sqlCase)
		
		pMsg.userid = userID
		pMsg.mdynickamount = 0
		pMsg.loginday = 1
		pMsg.loginconday = 1
		pMsg.maxloginconday = 1
		pMsg.loginamount = 1
		pMsg.maxjetton = 10000
		pMsg.onlinetime = 0
		pMsg.payrmb = 0
		pMsg.callpayrmb = 0
		pMsg.paytime = 0
		pMsg.callpaytime = 0
		pMsg.maxmoney = 0
		pMsg.dayrmb = 0
		pMsg.daypaytime = 0
	else
		pMsg.userid = userID
		pMsg.mdynickamount = tonumber(sqlData[3])
		pMsg.loginday = tonumber(sqlData[4])
		pMsg.loginconday = tonumber(sqlData[5])
		pMsg.maxloginconday = tonumber(sqlData[6])
		pMsg.loginamount = tonumber(sqlData[7])
		pMsg.onlinetime = tonumber(sqlData[8])
		pMsg.maxjetton = tonumber(sqlData[9])
		pMsg.payrmb = tonumber(sqlData[10])
		pMsg.callpayrmb = tonumber(sqlData[11])
		pMsg.paytime = tonumber(sqlData[12])
		pMsg.callpaytime = tonumber(sqlData[13])
		pMsg.maxmoney = tonumber(sqlData[14])
		pMsg.dayrmb = tonumber(sqlData[15])
		pMsg.daypaytime = tonumber(sqlData[16])
	end
	PlayerStatsModel.SetPlayerMsg(pMsg)
	return pMsg
end

function PlayerStatsModel.CreateStats(userID, nickName)
	sqlCase = "insert into dy_playermsg(userid,nickname) values("..userID..",'"..nickName.."')"
	mysqlItem:execute(sqlCase)	
end

function PlayerStatsModel.SetPlayerMsg(pMsg)
	redisItem:set(PlayerStatsModel.redis_key..pMsg.userid, pMsg:SerializeToString(), PlayerStatsModel.redis_index)
end

function PlayerStatsModel.PlayerLogin(userID, diffDay)

	if true then
		return nil
	end

	diffDay = diffDay < 0 and 0 or diffDay
	
	local pMsg = PlayerStatsModel.GetPlayerStats(userID)
	
	local sqlCase = ""
	pMsg.loginamount = pMsg.loginamount + 1
	if diffDay == 0 then
		
		sqlCase = "update dy_playermsg set loginamount="..pMsg.loginamount..",lastlogin='"..TimeUtils.GetTimeString().."' where userid="..userID
	elseif diffDay == 1 then
		pMsg.loginday = pMsg.loginday + 1
		pMsg.loginconday = pMsg.loginconday + 1
		pMsg.dayrmb = 0
		pMsg.daypaytime = 0
		if pMsg.loginconday > pMsg.maxloginconday then
			--同时更新最大连续登录次数，这里同时需要触发成就
			pMsg.maxloginconday = pMsg.loginconday
			
			--MissionManager.MissionUpdate(userID,MissionConType.con_2109 ,pMsg.maxloginconday)
		end		
		sqlCase = "update dy_playermsg set loginamount="..pMsg.loginamount..",loginday="..pMsg.loginday..",loginconday="..pMsg.loginconday..",maxloginconday="..pMsg.maxloginconday..",lastlogin='"..TimeUtils.GetTimeString().."' where userid="..userID
		
		--任务进行管理
		--MissionManager.MissionUpdate(userID,MissionConType.con_2108 ,pMsg.loginday)
		--MissionManager.MissionUpdate(userID,MissionConType.con_2109 ,pMsg.loginconday)
		--MissionManager.LoginConAward(userID,MissionConType.con_1104 ,pMsg.loginconday)
	else
		pMsg.loginday = pMsg.loginday + 1
		pMsg.loginconday = 1
		sqlCase = "update dy_playermsg set loginamount="..pMsg.loginamount..",loginday="..pMsg.loginday..",loginconday="..pMsg.loginconday..",lastlogin='"..TimeUtils.GetTimeString().."' where userid="..userID
		pMsg.dayrmb = 0
		pMsg.daypaytime = 0
		--任务进行管理
		--MissionManager.MissionUpdate(userID,MissionConType.con_2108 ,pMsg.loginday)
		--MissionManager.LoginConAward(userID,MissionConType.con_1104 ,pMsg.loginconday)  --在第一天登录的时候也要发送登录奖励
	end
	
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pMsg)
end

--检查玩牌的最高筹码
function PlayerStatsModel.CheckMaxJetton(pInfo)
	
	local pMsg = PlayerStatsModel.GetPlayerStats(pInfo.userid)
	if pInfo.jetton <= pMsg.maxjetton then
		return nil
	end
	pMsg.maxjetton = pInfo.jetton
	local sqlCase = "update dy_playermsg set maxjetton="..pMsg.maxjetton.." where userid="..pMsg.userid
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pMsg)
	--MissionManager.InsertUpdate(pMsg.userid, MissionConType.con_2105, pMsg.maxjetton)
end

function PlayerStatsModel.ModifyNickName(pMsg)
	pMsg.mdynickamount = pMsg.mdynickamount + 1
	local sqlCase =  "update dy_playermsg set mdynickamount="..pMsg.mdynickamount.." where userid="..pMsg.userid
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pMsg)
end


function PlayerStatsModel.CheckMaxMoney(pInfo,pStats)

	if pStats == nil then
		pStats = PlayerStatsModel.GetPlayerStats(pInfo.userid)
	end
	
	if pInfo.money <= pStats.maxmoney then
		return
	end
	
	pStats.maxmoney = pInfo.money
	local sqlCase = "update dy_playermsg set maxmoney="..pStats.maxmoney.." where userid="..pStats.userid
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pStats)	
end

function PlayerStatsModel.PaySuccess(pInfo, payRmb, pStats)
	
	if pStats == nil then
		pStats = PlayerStatsModel.GetPlayerStats(pInfo.userid)
	end
	
	pStats.payrmb = pStats.payrmb + payRmb
	pStats.paytime = pStats.paytime + 1
	
	pStats.dayrmb = pStats.dayrmb + payRmb
	pStats.daypaytime = pStats.daypaytime + 1
	
	local sqlCase = "update dy_playermsg set payrmb="..pStats.payrmb..",paytime="..pStats.paytime..",dayrmb="..pStats.dayrmb..",daypaytime="..pStats.daypaytime..",lastpay='"..TimeUtils.GetTimeString().."' where userid="..pInfo.userid
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pStats)
	
	sqlCase = "update log_sysdaily set paycount=paycount+"..payRmb..",paytimes=paytimes+1"-- where dateid='"..strDay.."'"--新玩家数		
	
	if pStats.dayrmb == payRmb then
		sqlCase = sqlCase..",payplayer=payplayer+1"
		SysCountService.PayPlayerCount()
	end
	if pStats.payrmb == payRmb then
		sqlCase = sqlCase..",newpayplayer=newpayplayer+1"
		sqlCase = sqlCase..",newpaycount=newpaycount+"..payRmb
	end
	sqlCase = sqlCase.." where dateid='"..TimeUtils.GetDayString().."'"
	SqlServer.rpush(sqlCase)
	SysCountService.PayTimeCount()
	SysCountService.PayRmbCount(payRmb)
end

function PlayerStatsModel.CallPay(pInfo,payRmb,pStats)
	
	if pStats == nil then
		pStats = PlayerStatsModel.GetPlayerStats(pInfo.userid)
	end
	pStats.callpayrmb = payRmb
	pStats.callpaytime = pStats.callpaytime + 1
	
	local sqlCase = "update dy_playermsg set callpayrmb="..pStats.callpayrmb..",callpaytime="..pStats.callpaytime.." where userID="..pStats.userid
	SqlServer.rpush(sqlCase)
	PlayerStatsModel.SetPlayerMsg(pStats)
	
	sqlCase = "update log_sysdaily set callpaycount=callpaycount+"..payRmb..",callpaytimes=callpaytimes+1 where dateid='"..TimeUtils.GetDayString().."'"--新玩家数
	SqlServer.rpush(sqlCase)
	
	SysCountService.CallPayTimeCount()
	SysCountService.CallPayRmbCount(payRmb)
end

