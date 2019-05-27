NoticeModel = {}
--该类处理发送的通知
--包括：登录的时候好友useid列表，新邮件通知，任务完成通知，好友请求通知，轮盘免费数量通知，喇叭数
--不在登录的协议中进行检查，而是在登录的协议中发送登录的指令，在定时器的线程中进行检查。
--没过第二天的时候，在没分钟的定时器中进行所有系列的检查。
NoticeModel.redisKey = "notice_login"
NoticeModel.redis_index = "redis_notice"

function NoticeModel.Init()
	redisItem:del(NoticeModel.redisKey,NoticeModel.redis_index)
end

function NoticeModel.SeverLoop()

	
	local len = redisItem:llen(NoticeModel.redisKey, NoticeModel.redis_index)
	
	for k = 1, len do
		local userID = redisItem:lpop(NoticeModel.redisKey, NoticeModel.redis_index)
		
		NoticeModel.PlayerNotice(userID)
	end

end

function NoticeModel.PlayerLogin(userID)
	redisItem:lpush(NoticeModel.redisKey, userID, NoticeModel.redis_index)
end

function NoticeModel.DayCheck()

	if g_markTime.last.day == g_markTime.curr.day then
		return nil
	end
	
	--该函数在logTimer中调用，用于处理每天12点一过的时候的检查
	local onLineList = OnlineModel.GetOnlineList()
	for k,v in ipairs(onLineList) do
		PlayerModel.DayFirstCheck(v)   --对没个人的情况进行检查，并且通知
		NoticeModel.PlayerNotice(v)
		PlayerModel.UpdateLoginTime(v)  --同时更新登录时间
	end
end

function NoticeModel.PlayerNotice(userID)
	--这边只是检查广播的，不适合做其他的检查
	userID = tonumber(userID)
	local pInfo = PlayerModel.GetPlayerInfo(userID)
	local gcmsg = msg_human_pb.gcnoticeinfo()
	--获取小喇叭数量
--[[	local addItem = gcmsg.noticelist:add()
	addItem.noticetype,addItem.amount =  GoodsModel.GetHornAmount(userID)--]]
	
	--获取好友列表
--[[	local fList = FriendsModel.GetFriendList(userID)
	for k,v in ipairs(fList) do
		gcmsg.friendslist:append(tonumber(v))
	end--]]
	

	--检查任务领取的情况
	
	
--[[	local noticeList = MissionModel.GetMissionNotice(userID)
	for k,v in ipairs(noticeList) do
		addItem = gcmsg.noticelist:add()
		addItem.noticetype = v.noticetype
		addItem.amount = v.amount
		
	end--]]
	
	--幸运大转盘的
--[[	local rpInfo = RouletteModel.GetRoulettePlayer(userID)
	if rpInfo.freeamount >= 0 then
		addItem = gcmsg.noticelist:add()
		addItem.noticetype = g_noticeType.Roulette_free
		addItem.amount = rpInfo.freeamount		
	end--]]
	
	
	--检查邮件情况
	MailModel.UserLogin(pInfo) --邮件是必要要检查的
--[[	for k,v in ipairs(noticeList) do
		addItem = gcmsg.noticelist:add()
		addItem.noticetype = v.noticetype
		addItem.amount = v.amount
	end--]]
	--检查任务情况


	--FriendsModel.GetCallKey()
--[[	noticeList = FriendsModel.GetNoticeList(userID)
	
	for k,v in ipairs(noticeList) do
		addItem = gcmsg.noticelist:add()
		addItem.noticetype = v.noticetype
		addItem.amount = v.amount
	end	--]]
	
	--检查聊天的情况
	--ChatModel.LoginCheck(userID)
	
	gcmsg.result = 0
	SendMessage(userID,PacketCode[1030].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
	
	--同时也在这里发送广播信息，避免在登录的时候发送过多的协议
	BroadCastModel.SendLogin(userID)
end

function NoticeModel.SendNotice(userID, noticeType)
	--每当有需要更新红点的时候，发送改协议
	local gcmsg = msg_human_pb.gcnoticeinfo()
	local addItem = gcmsg.noticelist:add()
	addItem.noticetype = noticeType
	addItem.amount = 1
	gcmsg.result = 0
	
	SendMessage(userID,PacketCode[1030].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
end

