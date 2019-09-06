
LogBehavior = {}
--该类用于记录玩家所有行为的log


function LogBehavior.Info(pInfo, category, tag, retcode, msg)
	local sqlCase = ""
	if type(pInfo) == "number" then
		sqlCase = "insert into log_behavior(userid,createdate,category,tag,retcode,msg) values("..pInfo..",'"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	elseif type(pInfo) == "string" then
		sqlCase = "insert into log_behavior(nickname,createdate,category,tag,retcode,msg) values('"..pInfo.."','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"

	else
		sqlCase = "insert into log_behavior(userid,nickname,createdate,category,tag,retcode,msg) values("..pInfo.userid..",'"..pInfo.nickname.."','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	end
	
	LogModel.LogBehaviorPush(sqlCase)
end

function LogBehavior.InfoJetton(pInfo, category, tag, retcode, msg, modNum, allNum)
	local sqlCase = ""
	if type(pInfo) == "number" then
		sqlCase = "insert into log_behavior(userid,createdate,category,tag,costamount,getamount,retcode,msg) values("..pInfo..",'"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..modNum..","..allNum..","..retcode..",'"..msg.."')"
	elseif type(pInfo) == "string" then
		sqlCase = "insert into log_behavior(nickname,createdate,category,tag,costamount,getamount,retcode,msg) values('"..pInfo.."','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..modNum..","..allNum..","..retcode..",'"..msg.."')"
	else
		sqlCase = "insert into log_behavior(userid,nickname,createdate,category,tag,costamount,getamount,retcode,msg) values("..pInfo.userid..",'"..pInfo.nickname.."','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..modNum..","..allNum..","..retcode..",'"..msg.."')"
	end
	
	LogModel.LogBehaviorPush(sqlCase)
end

function LogBehavior.Warning(pInfo, category, tag, retcode, msg)
	local sqlCase = ""
	if type(pInfo) == "number" then
		sqlCase = "insert into log_behavior(userid,level,createdate,category,tag,retcode,msg) values("..pInfo..",'warning','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	elseif type(pInfo) == "string" then
		sqlCase = "insert into log_behavior(nickname,level,createdate,category,tag,retcode,msg) values('"..pInfo.."','warning','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	else
		sqlCase = "insert into log_behavior(userid,nickname,level,createdate,category,tag,retcode,msg) values("..pInfo.userid..",'"..pInfo.nickname.."','warning','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	end
	LogModel.LogBehaviorPush(sqlCase)
end

function LogBehavior.Error(pInfo, category, tag, retcode, msg)
	local sqlCase = ""
	if type(pInfo) == "number" then
		sqlCase = "insert into log_behavior(userid,level,createdate,category,tag,retcode,msg) values("..pInfo..",'error','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	
	elseif type(pInfo) == "string" then
		sqlCase = "insert into log_behavior(nickname,level,createdate,category,tag,retcode,msg) values('"..pInfo.."','error','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	else
		sqlCase = "insert into log_behavior(userid,nickname,level,createdate,category,tag,retcode,msg) values("..pInfo.userid..",'"..pInfo.nickname.."','error','"..
			TimeUtils.GetTimeString().."','"..category.."','"..tag.."',"..retcode..",'"..msg.."')"
	end
	LogModel.LogBehaviorPush(sqlCase)
end
