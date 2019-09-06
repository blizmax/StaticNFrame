
module("AccountLogin", package.seeall)

--1.检查用户是否存在缓存中，如果缓存中有，就从缓存中取需要的数据，如果缓存中
--没有就从数据库拉去，并且保存到缓存中
--2.如果用户资料不存在，则创建用户
--3.对用户登录情况进行统计
function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgaccountlogin()
	local gcmsg = msg_human_pb.gcaccountlogin()

	cgmsg:ParseFromString(buffer)

	
	if cgmsg.bindtype == BindType.bindType_phone then
		local sqlCase = "select account, password from dy_player where phonenum='"..cgmsg.account.."'"
		mysqlItem:executeQuery(sqlCase)

		local sqlData = mysqlItem:fetch({})

		if sqlData ~= nil then
			local account = sqlData[1]
			local password = sqlData[2]

			--在这里检查手机号码一斤验证码的合法性
			--暂时去掉了手机号码登入
			local sqlCase = "select * from dy_authcode where phonenum='"..cgmsg.account.."' and code='"..cgmsg.authcode.."' order by id desc"
			mysqlItem:executeQuery(sqlCase)
			local sqlData = mysqlItem:fetch({})
			if sqlData == nil then
				LogFile("login","phonenum not exist phonenum="..cgmsg.account..",cid="..cgmsg.cid..",password="..cgmsg.password)
				gcmsg.result = ReturnCode["phone_code_notexit"]
				return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
			local timeMark = tonumber(sqlData[3])
			local currTime = TimeUtils.GetTime()
			local diffTime = TimeUtils.DifferentTime(timeMark, currTime)
			if diffTime > 600 then
			--超过10分钟，验证码失效
				gcmsg.result = ReturnCode["phone_code_pastdue"]
				return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
			
			cgmsg.account = account
			cgmsg.password = password
		else
			gcmsg.result = ReturnCode["account_not_exist"]
			LogBehavior.Error(cgmsg.account, "login", "AccountLogin",1, "手机号码登录，手机号码未绑定不存在，account="..cgmsg.account..",cid="..cgmsg.cid)
			return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end	
	
	
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
	cgmsg.email = (string.gsub(cgmsg.email, "^%s*(.-)%s*$", "%1"))
	--cgmsg.email = string.lower(cgmsg.email)   --全部转小写
	if cgmsg.email ~= nil then
		local eLen = string.len(cgmsg.email)
		local stremail = string.sub( cgmsg.account, #cgmsg.account - eLen + 1, #cgmsg.account )
		if stremail ~= cgmsg.email then
			cgmsg.account = cgmsg.account..cgmsg.email    --这里是为了邀请码做准备的
		end
	end

	gcmsg.result = 0
	gcmsg.pinfo.userid = 0
	local ret = PlayerModel.GetPlayerInfoByCID(cgmsg.account, cgmsg.password, gcmsg.pinfo, cgmsg.bindtype)
	local isNewPlayer = false

	if ret == ReturnCode["account_not_exist"] then
		
		--if cgmsg.bindtype == BindType.bindType_WeiXin and false == AgentModel.isSaoMa(cgmsg.platformid) then
		--	gcmsg.result = ReturnCode["create_player_fail"]
		----	LogFile("login","create_player_fail not exist account="..cgmsg.account..",cid="..cgmsg.cid..",password="..cgmsg.password..",platformid="..cgmsg.platformid)
		--	return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		--end
		
		--反正账号不存在
		if cgmsg.channel == g_channel["self_channel"] and quickLogin == false then
			--如果是自有渠道，并且不是快速登录的
			gcmsg.result = ReturnCode["account_not_exist"]
			LogBehavior.Error(cgmsg.account, "login", "AccountLogin",1, "自有渠道账号登入不存在，account="..cgmsg.account..",cid="..cgmsg.cid)
			return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		
		if cgmsg.bindtype ~= BindType.bindType_TianYu then
			--只有默认快速登入的渠道，不需要添加MD5
			cgmsg.password = md5(cgmsg.password)              --这里给他加上MD5
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

		AgentModel.GetAgentInfo(gcmsg.pinfo.userid)
		AgentModel.CheckIsWiringBind(gcmsg.pinfo, cgmsg.platformid)
		
		if cgmsg.bindtype ~= BindType.bindType_WeiXin then
			local sqlCase = "update dy_player set nickname='玩家"..gcmsg.pinfo.userid.."' where userid="..gcmsg.pinfo.userid
			mysqlItem:execute(sqlCase)
			gcmsg.pinfo.nickname="玩家"..gcmsg.pinfo.userid
		end
		
	elseif ret == ReturnCode["password_not_match"] then
		--密码不匹配
		gcmsg.result = ReturnCode["password_not_match"]
		LogBehavior.Error(cgmsg.account, "login", "AccountLogin",2, "登入失败，密码不匹配="..cgmsg.account..",password="..cgmsg.password)
		return 0,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
		
	end
	
	if gcmsg.pinfo.isban == 1 then
		gcmsg.result = ReturnCode["race_not_exist"]
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
	
	local toAgency = AgentModel.GetAgentInfo(gcmsg.pinfo.userid)
	if toAgency == nil then
		gcmsg.pinfo.agent1 = 0
	else
		gcmsg.pinfo.agent1 = toAgency.agent1
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
	--LogBehavior.Info(gcmsg.pinfo,"login", "AccountLogin", 0, "玩家登入游戏")

	-- 回复钱,把玩家的银行密码删掉
	if 0 ~= string.len(gcmsg.pinfo.bank_password) then
		gcmsg.pinfo.bank_password = "1"		-- 用于告诉客户端已经设置了银行密码
	end
	
	return gcmsg.pinfo.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()

end