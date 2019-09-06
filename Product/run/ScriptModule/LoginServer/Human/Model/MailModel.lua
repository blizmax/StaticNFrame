MailModel = {}
MailModel.redis_index = "redis_mail"
--对于GM发给系统的邮件:
--validity表现系统要发给玩家的最长的有效期，-1表示这封GM邮件会永久起效果
--这是，userMail的createdate是当前时间,uMail的validity是7

--如果sysMail的validity
--系统所有发放的邮件都会有一个有效期
--发送给玩家的时候，同时附带上该有效期，所以玩家对于这封有奖的有效，肯定会比系统的要长。
--玩家每次登陆的时间，就检查系统邮件中有没有需要发送的邮件

MailModel.mail_sys = "mail_sys"
MailModel.mail_sys_time = "mail_sys_time"
MailModel.mail_list = "maillist_"
MailModel.mail_time = "mailtime_"

function MailModel.LoadSysMail()

	local tTime = TimeUtils.GetTableTime()
	local currDate = TimeUtils.GetDayString(tTime)
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)


	local sqlCase = "select * from dy_mail where senderid=1 and receiverid=1 and mailstate>0 and markdate>='"..currDate.."'"
	
	mysqlItem:executeQuery(sqlCase)
	
	for i = 1, 1000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		local mItem = st_human_pb.mailinfo()
		mItem.mailid = tonumber(sqlData[1])
		mItem.senderid = tonumber(sqlData[2])
		mItem.sender = sqlData[3]
		mItem.receiverid = tonumber(sqlData[4])
		mItem.receiver = sqlData[5]
		mItem.mailtype = tonumber(sqlData[6])
		mItem.title = sqlData[7]
		mItem.content = sqlData[8]
		mItem.senddate = sqlData[9]
		mItem.markdate = sqlData[10]
		mItem.mailstate = tonumber(sqlData[11])
		mItem.externdata = sqlData[12]
		mItem.remark = sqlData[13]
		mItem.validity = tonumber(sqlData[14])
		
		--计算超时时间
		redisItem:hset(MailModel.mail_sys, mItem.mailid, mItem:SerializeToString(), MailModel.redis_index)
		redisItem:hset(MailModel.mail_sys_time, mItem.mailid, currSec + (mItem.validity + 1)*g_daySeconds)
		
	end

end

function MailModel.GetSysMailList()
	--if true == redisItem:exists(MailModel.mail_sys, MailModel.redis_index) then
	--	local mList = st_human_pb.maillist()
	--	local strGet = redisItem:get(MailModel.mail_sys, MailModel.redis_index)
	--	mList:ParseFromString(strGet)
	--	return mList
	--end
	--return MailModel.LoadSysMail()
end


function MailModel.LoadMailList(userID)
	

	local tTime = TimeUtils.GetTableTime()
	local currDate = TimeUtils.GetDayString()
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)	
	
	
	local sqlCase = "select * from dy_mail where receiverid="..userID.." and mailstate>0 and markdate>="..currDate
	
	mysqlItem:executeQuery(sqlCase)
	for i = 1, 1000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		local mItem = st_human_pb.mailinfo() 
		mItem.mailid = tonumber(sqlData[1])
		mItem.senderid = tonumber(sqlData[2])
		mItem.sender = sqlData[3]
		mItem.receiverid = tonumber(sqlData[4])
		mItem.receiver = sqlData[5]
		mItem.mailtype = tonumber(sqlData[6])
		mItem.title = sqlData[7]
		mItem.content = sqlData[8]
		mItem.senddate = sqlData[9]
		mItem.markdate = sqlData[10]
		mItem.mailstate = tonumber(sqlData[11])
		mItem.externdata = sqlData[12]
		mItem.remark = sqlData[13]
		mItem.validity = tonumber(sqlData[14])
		
		redisItem:hset(MailModel.mail_list..userID,mItem.mailid, mItem:SerializeToString(),MailModel.redis_index)
		redisItem:hset(MailModel.mail_time..userID,mItem.mailid, (mItem.validity + 1)*g_daySeconds + currSec, MailModel.redis_index)
	end
	
	--return mList
end

function  MailModel.GetMailItem(userID, mailID, mItem)
	if mItem == nil then
		mItem = st_human_pb.mailinfo()
	end
	local strGet = redisItem:hget(MailModel.mail_list..userID, mailID, MailModel.redis_index)
	if strGet ~= nil then
		mItem:ParseFromString(strGet)
		return mItem
	else
		return nil
	end
end

function MailModel.SetSysMailList(mList)
	redisItem:set(MailModel.mail_sys,MailModel.mail_sys, mList:SerializeToString(), MailModel.redis_index)
end

function MailModel.SetMailItem(userID, mItem)
	
	redisItem:hset(MailModel.mail_list..userID, mItem.mailid, mItem:SerializeToString(), MailModel.redis_index)
end


function MailModel.DayCheck()
	
	local currSec = TimeUtils.GetTime()
	currSec = tonumber(currSec)
	--[[
	local mSysList = MailModel.GetSysMailList()
	local mCount = 1
	while true do

		if mCount > #mSysList.mSysList then
			break
		end

		local dayDiff = TimeUtils.DifferentDay(mSysList.mlist[mCount].markdate,g_markTime.curr)

		--表示已经超过有效期了
		--删除的情况下，mCount不用+1
		if dayDiff > 0 then 
			local sqlCase = "update dy_mail set mailstate=0 where id="..mSysList.mlist[mCount].mailid
			SqlServer.rpush(sqlCase)
			mSysList.mlist:remove(mCount)
		else
			mCount = mCount + 1
		end
	end
	MailModel.SetSysMailList(mSysList)
	]]
	
	local sysList = redisItem:hgetall(MailModel.mail_sys_time, MailModel.redis_index)
	
	for k,v in pairs(sysList) do
		if currSec > tonumber(v) then
			redisItem:hdel(MailModel.mail_sys_time,k,MailModel.redis_index)
			redisItem:hdel(MailModel.mail_sys,k,MailModel.redis_index)
		end
	end
	
end




function MailModel.AddMail(mItem)

	local sqlCase = "insert into dy_mail(senderid,sender,receiverid,receiver,mailtype,title,content,senddate,markdate,mailstate,externdata,remark,validity) values("..
	mItem.senderid..",'"..mItem.sender.."',"..mItem.receiverid..",'"..mItem.receiver.."',"..mItem.mailtype..",'"..mItem.title.."','"..mItem.content.."','"..mItem.senddate.."','"..
	mItem.markdate.."',"..mItem.mailstate..",'"..mItem.externdata.."','"..mItem.remark.."',"..mItem.validity..")"

	--LogFile("info", sqlCase)

	local mailID = mysqlItem:execute(sqlCase)
	if mailID == nil then
		return nil
	end
	
	sqlCase = "select id from dy_mail where receiverid="..mItem.receiverid.." and mailtype="..mItem.mailtype.." and mailstate>0 order by id desc limit 1"--这里一定要逆序，只有逆序取是第一个才准确
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch()  --这里调用的是第一个列表成员
	if sqlData ~= nil then
		mItem.mailid = tonumber(sqlData)
	end	
	local tTime = TimeUtils.GetTableTime()
	
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)	
	redisItem:hset(MailModel.mail_list..mItem.receiverid, mItem.mailid, mItem:SerializeToString(),MailModel.redis_index)
	redisItem:hset(MailModel.mail_time..mItem.receiverid, mItem.mailid, currSec + (mItem.validity + 1)*g_daySeconds, MailModel.redis_index)
	return mItem
end


function MailModel.UserLogin(pInfo)
	--玩家每次登录后检查是否有邮件需要更新的，以及是否有邮件未读和没领取的
	
	if false == redisItem:exists(MailModel.mail_time..pInfo.userid, MailModel.redis_index) then
		MailModel.LoadMailList(pInfo.userid)
	end
	
	local muList =  redisItem:hgetall(MailModel.mail_time..pInfo.userid, MailModel.redis_index)  --MailModel.GetMailList(pInfo.userid)
	local mSysList = redisItem:hgetall(MailModel.mail_sys_time, MailModel.redis_index)  --MailModel.GetSysMailList()	
	local currDate = TimeUtils.GetTableTime()	
	local addCount = 0	

	for k1,v1 in pairs(mSysList) do
		--local isMark = false
		--for k2,v2 in pairs(muList) do
			--if k1 == k2 then
			--end
		--end
		local sqlCase = "select mailid from dy_mail where receiverid="..pInfo.userid.." and remark='"..k1.."'"
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			--说明这个邮件需要添加到个人邮件里面的
			local addItem = st_human_pb.mailinfo()
			local strGet = redisItem:hget(MailModel.mail_sys, k1, MailModel.redis_index)
			if strGet ~= nil then
				addItem:ParseFromString(strGet)
				addItem.senderid = g_sysInfo.sys_id
				addItem.sender = g_sysInfo.sys_name
				addItem.receiverid = pInfo.userid
				addItem.receiver = pInfo.nickname
				addItem.senddate = TimeUtils.GetTimeString(currDate)
				addItem.validity = 7
				addItem.remark = tostring(k1)
				MailModel.AddMail(addItem)				
			end
			
		end
		
	end
	
	--在下面检查邮件有哪些已读没领取的
	--每次登录的时候都需要检查
	--然后发通知公告给客户端

--[[	local uReadInfo = st_human_pb.noticeinfo()  --未读的邮件列表
	local uGetInfo = st_human_pb.noticeinfo()   --未领取的邮件
	
	for k,v in ipairs(muList.mlist) do
		
		if v.mailstate == g_mailState.state_readadd or v.mailstate == g_mailState.state_unreadadd then
			uGetInfo.amount = uGetInfo.amount + 1
		elseif v.mailstate == g_mailState.state_unread then
			uReadInfo.amount = uReadInfo.amount + 1
		end
	end	

	local getList = {}
	if uReadInfo.amount >0 then
		uReadInfo.noticetype = g_noticeType.mail_unread
		table.insert(getList, uReadInfo)
	end
	
	if uGetInfo.amount > 0 then
		uGetInfo.noticetype = g_noticeType.mail_unget
		table.insert(getList, uGetInfo)		
	end
	return getList;--]]
	local currSec = TimeUtils.GetTime()
	currSec = tonumber(currSec)
	for k,v in pairs(muList) do
		if currSec > tonumber(v) then
			redisItem:hdel(MailModel.mail_list..pInfo.userid, k, MailModel.redis_index)
			redisItem:hdel(MailModel.mail_time..pInfo.userid, k, MailModel.redis_index)
		end
	end
	
	local muList_1 = redisItem:hgetall(MailModel.mail_list..pInfo.userid, MailModel.redis_index)
	local uReadInfo = st_human_pb.noticeinfo()  --未读的邮件列表
	local uGetInfo = st_human_pb.noticeinfo()   --未领取的邮件
	
	for k,v in pairs(muList_1) do
		local Not = st_human_pb.mailinfo() 
		Not:ParseFromString(v)
		if Not.mailstate == g_mailState.state_readadd or Not.mailstate == g_mailState.state_unreadadd then
			uGetInfo.amount = uGetInfo.amount + 1
		elseif Not.mailstate == g_mailState.state_unread then
			uReadInfo.amount = uReadInfo.amount + 1
		end
	end	

	local getList = {}
	if uReadInfo.amount >0 then
		uReadInfo.noticetype = g_noticeType.mail_unread
		table.insert(getList, uReadInfo)
	end
	
	if uGetInfo.amount > 0 then
		uGetInfo.noticetype = g_noticeType.mail_unget
		table.insert(getList, uGetInfo)		
	end
	return getList
end


function MailModel.UserDayCheck(userID)
	--玩家每天第一次登陆的时候，需要检查邮件的有效期
	--local currDate = TimeUtils.GetTableTime()
	--local uMList = MailModel.GetMailList(userID)
	--local mCount = 1	
	--while true do
	--	if mCount > #uMList.mlist then
	--		break
	--	end
	--	local dayDiff = TimeUtils.DifferentDay(uMList.mlist[mCount].senddate,currDate)
		--表示已经超过有效期了
		--删除的情况下，mCount不用+1
	--	if dayDiff > uMList.mlist[mCount].validity then 
	--		local sqlCase = "update dy_mail set mailstate=0 where id="..uMList.mlist[mCount].mailid
	--		SqlServer.rpush(sqlCase)
	--		uMList.mlist:remove(mCount)
	--	else
	--		mCount = mCount + 1
	--	end
	--end
	--MailModel.SetMailList(userID, uMList)
end


function MailModel.HlydDayAdd(pInfo)
	
	if g_servername ~= "run_hlyd" then
		return
	end
	
	--欢乐赢豆的游戏中，没天登录的时候，暂时用邮件给第一次登录的玩家发送邮件
	
	--玩家每次登录后检查是否有邮件需要更新的，以及是否有邮件未读和没领取的
	local addItem = st_human_pb.mailinfo() 
	
	addItem.title='每日登录赠送'
	addItem.content='每日首次登录赠送筹码3000，请领取！'
	addItem.senderid = 1
	addItem.sender = "系统"
	addItem.receiverid = pInfo.userid
	addItem.receiver = pInfo.nickname
	addItem.senddate = TimeUtils.GetTimeString(currDate)
	addItem.validity = 7
	addItem.externdata = "[[1002,3000]]"
	addItem.mailtype = 2
	addItem.mailstate = g_mailState.state_unreadadd
	local tTime = TimeUtils.GetTableTime()
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)
	currSec = currSec + g_daySeconds*2
	addItem.markdate = TimeUtils.GetDayString(currSec)
	
	MailModel.AddMail(addItem)
	
end

function MailModel.HlydWin(pInfo,dateStr,qiNum,jetton,awardStr,allNum, winNum ,desList)

	local addItem = st_human_pb.mailinfo() 
	
	addItem.title="第"..dateStr.."-"..qiNum.."期"
	addItem.content="第"..dateStr.."-"..qiNum.."期，开奖结果："..awardStr..",总和:"..allNum..",珠:"..winNum.."\n----------\n"

	if desList ~= nil then
		for k,v in ipairs(desList) do
			addItem.content = addItem.content..v.."\n"
		end
	end
	addItem.content = addItem.content.."----------\n".."本期盈亏："..jetton
	addItem.senderid = 1
	addItem.sender = "系统"
	addItem.receiverid = pInfo.userid
	addItem.receiver = pInfo.nickname
	addItem.senddate = TimeUtils.GetTimeString(currDate)
	addItem.validity = 7
	addItem.mailtype = g_mailType.SYS_Player
	addItem.mailstate = g_mailState.state_unread
	
	
	local tTime = TimeUtils.GetTableTime()
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)
	currSec = currSec + g_daySeconds*2
	addItem.markdate = TimeUtils.GetDayString(currSec)	
	MailModel.AddMail(addItem)
end

function MailModel.HlydWinSec(vCount)

	local addItem = st_human_pb.mailinfo() 
	
	addItem.title = vCount.timestr..":房间号-"..vCount.tableid
	--addItem.content="第"..dateStr.."-"..qiNum.."期，开奖结果："..awardStr..",总和:"..allNum..",珠:"..winNum.."\n----------\n"
	addItem.content = "积分排行榜：\n"
	
	for i = 1,#vCount.rankuserid do
		addItem.content = addItem.content..vCount.ranknick[i].."   "..vCount.initjifen[i].."    "..vCount.rankjifen[i].."\n"
	end
	
	
	addItem.senderid = 1
	addItem.sender = "系统"
	--addItem.receiverid = pInfo.userid
	--addItem.receiver = pInfo.nickname
	addItem.senddate = TimeUtils.GetTimeString(currDate)
	addItem.validity = 7
	addItem.mailtype = g_mailType.SYS_Player
	addItem.mailstate = g_mailState.state_unread	
	local tTime = TimeUtils.GetTableTime()
	tTime.hour = 0
	tTime.min = 0
	tTime.sec = 0
	local currSec = TimeUtils.GetTime(tTime)
	currSec = currSec + g_daySeconds*2
	addItem.markdate = TimeUtils.GetDayString(currSec)	
	
	for i = 1,#vCount.rankuserid do
		addItem.receiverid = vCount.rankuserid[i]
		addItem.receiver = vCount.ranknick[i]
		MailModel.AddMail(addItem)
	end	
	
end

