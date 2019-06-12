
module("AccountLogin", package.seeall)

--1.检查用户是否存在缓存中，如果缓存中有，就从缓存中取需要的数据，如果缓存中
--没有就从数据库拉去，并且保存到缓存中
--2.如果用户资料不存在，则创建用户
--3.对用户登录情况进行统计
function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgaccountlogin()
	local gcmsg = msg_human_pb.gcaccountlogin()

	cgmsg:ParseFromString(buffer)

	--gcmsg.result = 1
	--if cgmsg.imei == nil and g_isDebug ~= 0 then
	--	gcmsg.result = ReturnCode["imei_not_exist"]
	--	return 0, 0, gcmsg:ByteSize(), gcmsg:SerializeBToString()
	--end
	if cgmsg.cid == nil or cgmsg.cid == "" then
		--如果cid是空的，就随机分配一个CID给他
		cgmsg.cid = PlayerModel.GetRandomCID()
		
		LogBehavior.Info(cgmsg.cid, "login", "AccountLogin", 0, "自动创建随机的CID="..cgmsg.cid)
		--return 0, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	local quickLogin = false
	if cgmsg.account == nil or cgmsg.account == "" then
		quickLogin = true
		cgmsg.account = cgmsg.cid
		cgmsg.password = cgmsg.cid
		LogBehavior.Info(cgmsg.cid,"login","AccountLogin",0,"快速登录，account="..cgmsg.account)
	end

	if cgmsg.nickname== nil or cgmsg.nickname == "" then
		if cgmsg.devname ~= "" then
			cgmsg.nickname = cgmsg.devname
		else
			cgmsg.nickname = cgmsg.cid
		end
	end
	--luaDump(cgmsg)
	gcmsg.result = 0
	gcmsg.pinfo.userid = 0
	local ret = PlayerModel.GetPlayerInfoByCID(cgmsg.account, cgmsg.password, gcmsg.pinfo)
	local isNewPlayer = false
	if ret == ReturnCode["account_not_exist"] then
		--反正账号不存在
		
		if cgmsg.channel == g_channel["self_channel"] and quickLogin == false then
			--如果是自有渠道，并且不是快速登录的
			gcmsg.result = ReturnCode["account_not_exist"]
			LogBehavior.Error(cgmsg.account, "login", "AccountLogin",1, "自有渠道账号登入不存在，account="..cgmsg.account..",cid="..cgmsg.cid)
			return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		PlayerModel.CreatePlayer(cgmsg)
		if PlayerModel.GetPlayerInfoByCID(cgmsg.account, cgmsg.password, gcmsg.pinfo) ~= 0 then
			--创建角色失败
			gcmsg.result = ReturnCode["create_player_fail"]
			LogBehavior.Error(cgmsg.account, "login", "AccountLogin",1, "创建角色失败，account="..cgmsg.account..",cid="..cgmsg.cid)
			return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		
		isNewPlayer = true
		redisItem:incr("playerall", PlayerModel.redis_index)
		PlayerStatsModel.CreateStats(gcmsg.pinfo.userid, cgmsg.nickname)
		--PlayerModel.CheckIsBindAgency(gcmsg.pinfo, cgmsg.platformid)   --在这里检查是否绑定了别的推广码
	elseif ret == ReturnCode["password_not_match"] then
		--密码不匹配
		gcmsg.result = ReturnCode["password_not_match"]
		LogBehavior.Error(cgmsg.account, "login", "AccountLogin",2, "登入失败，密码不匹配="..cgmsg.account..",password="..cgmsg.password)
		return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		
	end
	
	--这里检查踢出重复登录的情况
	--local rrrr = OnlineModel.CheckOnline(gcmsg.pinfo.userid, gcmsg.pinfo.viplevel)
	if nil ~= OnlineModel.CheckOnline(gcmsg.pinfo.userid, gcmsg.pinfo.viplevel) then
		local gckit = msg_human_pb.gckitplayer()
		gckit.result = 0
		gckit.kittype = 1
		gckit.msg = "您的账号已在别处登入，若非本人操作，请联系系统管理员！"
		SendMessage(gcmsg.pinfo.userid, PacketCode[1008].client, gckit:ByteSize(), gckit:SerializeToString())
		redisItem:sadd(PlayerModel.kit_userList, gcmsg.pinfo.userid, PlayerModel.redis_index)
	end


	if gcmsg.pinfo.blacklist ~= 0 then
		gcmsg.result = ReturnCode["account_login_error"]
		return gcmsg.pinfo.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	PlayerModel.PlayerLogin(gcmsg.pinfo, isNewPlayer)

	--对登录恢复做检查
	--if true == redisItem:hexists("playerrace", gcmsg.pinfo.userid) then
	
	--	gcmsg.tableid = tonumber(redisItem:hget("playerrace", gcmsg.pinfo.userid),PlayerModel.redis_index)
	--	gcmsg.tabletype = g_loginTable.type_texas
	--end

	--local tableID = MajiangModel.CheckPlaying(gcmsg.pinfo.userid)
	
	--if tableID ~= nil then
	--	gcmsg.tableid = tonumber(tableID)
	--	gcmsg.tabletype = g_loginTable.type_majiang
	--end
	
	gcmsg.tabletype,gcmsg.tableid = PlayerModel.GetPlayerTableInfo(gcmsg.pinfo.userid)
	
	OnlineModel.PlayerLogin(gcmsg.pinfo)
	PlayerModel.SetPlayerInfo(gcmsg.pinfo)
	gcmsg.result = ReturnCode["success"]
	gcmsg.systime = TimeUtils.GetTime()   --给客户端用于校准的时间戳
	LogBehavior.Info(gcmsg.pinfo,"login", "AccountLogin", 0, "玩家登入游戏")
	
	return gcmsg.pinfo.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()

end