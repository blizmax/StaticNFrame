LogServer = {}


function LogServer.Init()
	
	local strDay = TimeUtils.GetDayString()
	
	local strDaySec = TimeUtils.GetTime(strDay)
	

	for i = 1, 2000 do 
		local sqlCase = "insert into log_sysdaily(dateid) values('"..strDay.."')"
		if nil == mysqlItem:execute(sqlCase) then
			break
		end
		strDaySec = strDaySec + g_daySeconds
		strDay = TimeUtils.GetDayString(strDaySec)
	end
	
end


function LogServer.NewPlayer(pInfo)
	--有新玩家进来的时候
	--在log_player表中插入一条记录
	--更新log_sysdaily中玩家总数以及最近玩家人数
	
	local sqlCase = "insert into log_player(userid, nickname) values("..pInfo.userid..",'"..pInfo.nickname.."')"
	mysqlItem:execute(sqlCase)
		
	local strDay = TimeUtils.GetDayString()
	sqlCase = "update log_sysdaily set newplayer=newplayer+1,activeplayer=activeplayer+1 where dateid='"..strDay.."'"--新玩家数
	

	SqlServer.rpush(sqlCase)
	redisItem:incr("allplayernum", "allplayernum")
end

function LogServer.PlayerLogin(pInfo)
	local timeSec = TimeUtils.GetTime()

	if pInfo.lasttime == 0 then
		pInfo.lasttime = timeSec
	end
	
	
	local difDay = TimeUtils.DifferentDay(pInfo.lasttime, timeSec)
	local regDay = TimeUtils.DifferentDay(pInfo.regdate, timeSec) + 1
	local sqlCase = "update log_player set logincount=logincount+1"
	if difDay > 0 then
		sqlCase = sqlCase..",loginday=loginday+1"
	end
	
	local dayid
	if regDay <= 30 and regDay > 0 then
		dayid = "day"..regDay
	elseif regDay <= 60 then
		dayid = "day30"
	elseif regDay <= 90 then
		dayid = "day90"
	elseif regDay <= 120 then
		dayid = "day120"
	elseif regDay <= 150 then
		dayid = "day150"		
	elseif regDay <= 180 then
		dayid = "day180"
	elseif regDay <= 210 then
		dayid = "day210"
	elseif regDay <= 240 then
		dayid = "day240"
	elseif regDay <= 270 then
		dayid = "day270"
	elseif regDay <= 300 then
		dayid = "day300"
	elseif regDay <= 330 then
		dayid = "day330"
	else
		dayid = "day360"
	end
	sqlCase = sqlCase..","..dayid.."="..dayid.."+1"
	sqlCase = sqlCase..",penultdate=lastdate,lastdate='"..TimeUtils.GetTimeString().."' where userid="..pInfo.userid

	SqlServer.rpush(sqlCase)
		
	sqlCase = "update log_sysdaily set logincount=logincount+1"
	if difDay > 0 then
		sqlCase = sqlCase..", activeplayer=activeplayer+1"
	end
	sqlCase = sqlCase.." where dateid='"..TimeUtils.GetDayString().."'"
	SqlServer.rpush(sqlCase)
	SysCountService.LoginTime()
end

function LogServer.ReportInfo(reportTime)

	if reportTime == nil then
		return nil
	end
		

	local minutes = TimeUtils.GetDayMinutes(reportTime)

	local onlineNum = OnlineModel.GetOnlineNum()

	onlineNum = onlineNum == nil and 0 or onlineNum
	onlineNum = string.format("%#x",onlineNum)
	onlineNum = string.sub(onlineNum,3,string.len(onlineNum))
	
	minutes = string.format("%#x",minutes)
	minutes = string.sub(minutes,3,string.len(minutes))	
	
	local sqlCase = "update log_sysdaily set onlinelist=concat(onlinelist,'"..onlineNum.."|'),minulist=concat(minulist,'"..minutes.."|') where dateid='"..TimeUtils.GetDayString().."'"
	if g_servername == "run_xwpk" then
		sqlCase = "update log_sysdaily_xwpk set onlinelist=concat(onlinelist,'"..onlineNum.."|'),minulist=concat(minulist,'"..minutes.."|') where dateid='"..TimeUtils.GetDayString().."'"
	end
	
	mysqlItem:execute(sqlCase)
end


function LogServer.AddJetton(modules, amount)

	local sqlCase = ''
	if modules == "douniu" then
		sqlCase = "update log_sysdaily set remain7=remain7+"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "sys" then
		sqlCase = "update log_sysdaily set remain2=remain2+"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "psz" then
		sqlCase = "update log_sysdaily set remain15=remain15+"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "agency" then
		sqlCase = "update log_sysdaily set remain3=remain3+"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "pdk" then
		sqlCase = "update log_sysdaily set remain30=remain30+"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	end
	if sqlCase ~= '' then
		mysqlItem:execute(sqlCase)
	end
end

function LogServer.DecJetton(modules, amount)
	local sqlCase = ''
	if modules == "douniu" then
		sqlCase = "update log_sysdaily set remain7=remain7-"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "psz" then
		sqlCase = "update log_sysdaily set remain15=remain15-"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	elseif modules == "pdk" then
		sqlCase = "update log_sysdaily set remain30=remain30-"..amount.." where dateid='"..TimeUtils.GetDayString().."'"
	end
	if sqlCase ~= '' then
		mysqlItem:execute(sqlCase)
	end	
end
