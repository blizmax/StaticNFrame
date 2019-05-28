DouNiuEvent = {}

function DouNiuEvent.JulebuGameCreate(tInfo)  --牌桌处于等待状态的函数
	
	if tInfo.julebuid == 0 then
		return
	end
	local strDes = ""
	
	--[[if tInfo.dntype == g_douNiuType.type_mpqz then
		strDes = "看牌抢庄"
	elseif tInfo.dntype == g_douNiuType.type_tbnn then
		strDes = "通比牛牛"
	elseif tInfo.dntype == g_douNiuType.type_gdzj then
		strDes = "固定庄家"
	elseif tInfo.dntype == g_douNiuType.type_nnsz then
		strDes = "牛牛轮庄"
	elseif tInfo.dntype == g_douNiuType.type_zyqz then
		strDes = "自由抢庄"
	end
	
	--strDes = strDes..","..tInfo.maxplaynum.."局"
	if tInfo.payway == 2 then
		strDes = strDes..",AA付费"
	else
		strDes = strDes..",房主付费"
	end]]
	--底分
	local tmp4 = tInfo.difen
	if tInfo.difen == g_douNiuPour_type.pour_1_2 then
		tmp4 = "1/2"
	elseif tInfo.difen == g_douNiuPour_type.pour_1_2_3 then
		tmp4 = "1/2/3"
	elseif tInfo.difen == g_douNiuPour_type.pour_2_4 then
		tmp4 = "2/4"
	elseif tInfo.difen == g_douNiuPour_type.pour_4_8 then
		tmp4 = "4/8"
	elseif tInfo.difen == g_douNiuPour_type.pour_5_10 then
		tmp4 = "5/10"
	end
	strDes = strDes.."底分: "..tmp4
	--[[strDes = strDes..",牌型倍数: "
	if tInfo.typeodds == g_douNiuRate_typeone then
		strDes = strDes.."牛牛x4 牛九x3 牛八x2 牛七x2"
	else
		strDes = strDes.."牛牛x3 牛九x2 牛八x2"
	end
	strDes = strDes..", 最大推注："..tInfo.TZmax.."倍"

	--明牌抢庄
	if tInfo.dntype == g_douNiuType.type_mpqz then
		strDes = strDes..",抢庄最高倍数: "
		if tInfo.bankermultiple == g_douNiuDefine.mul_one then
			strDes = strDes.."1倍"
		elseif tInfo.bankermultiple == g_douNiuDefine.mul_two then
			strDes = strDes.."2倍"
		elseif tInfo.bankermultiple == g_douNiuDefine.mul_three then
			strDes = strDes.."3倍"
		elseif tInfo.bankermultiple == g_douNiuDefine.mul_four then
			strDes = strDes.."4倍"
		end
	--固定庄家	
	elseif tInfo.dntype == g_douNiuType.type_gdzj then
		strDes = strDes..",庄家携带上限: "
		local tmp = tInfo.integralmax
		if tInfo.integralmax == 0 then
			tmp = "无"
		end
		strDes = strDes..tmp
	end
	
	--特殊牌型
	cgstrvipinfo = st_douniu_pb.douniuspecialtype()
	cgstrvipinfo:ParseFromString(tInfo.specialtype)
	
	if  cgstrvipinfo.shunziNiu == 1 then
		strDes = strDes..", 顺子牛x5"
	end
	if  cgstrvipinfo.tonghua_niu == 1 then
		strDes = strDes..", 同花牛x5"
	end
	if  cgstrvipinfo.wuhua == 1 then
		strDes = strDes..", 五花x5"
	end
	if  cgstrvipinfo.huluNiu == 1 then
		strDes = strDes..", 葫芦牛x6"
	end
	if  cgstrvipinfo.zhadan == 1 then
		strDes = strDes..", 炸弹x7"
	end
	if  cgstrvipinfo.wu_xiao == 1 then
		strDes = strDes..", 五小牛x8"
	end
	
	strDes = strDes..",高级选项: "
	if tInfo.banrubcard == 1 then
		strDes = strDes.."中途禁入 "
	end
	if tInfo.playerbuycode == 1 then
		strDes = strDes.."王牌癞子 "
	end
	if tInfo.pourrestrict == 1 then
		strDes = strDes.."闲家买码 "
	end
	if tInfo.kongwildcard == 1 then
		strDes = strDes.."下注限制 "
	end
	if tInfo.pourdouble == 1 then
		strDes = strDes.."下注加倍 "
	end]]
	local useridList = {}
	for k,v in ipairs(tInfo.situser)do 
		table.insert(useridList, v.userid)
	end

	JulebuService.CreateGame(g_JulebuDefine.modules_douniu, tInfo.julebuid, tInfo.tableid, tInfo.usermaxnum,
							#tInfo.situser, strDes, tInfo.julebutype,tInfo.dntype, useridList,
							tInfo.maxplaynum, tInfo.usevipnum)
end

function DouNiuEvent.JulebuGameEnd(julebuID, tableID, julebuType, playingtype)  --牌桌处于等待状态的函数
	
	if julebuID == 0 then
		return
	end
	JulebuService.GameEnd(julebuID, tableID, julebuType, playingtype)
end

function DouNiuEvent.JulebuGameUpdate(julebuID, tableID, userNum, julebuType, userid)  --牌桌处于等待状态的函数
	if julebuID == 0 then
		return
	end

	JulebuService.GameUpdate(julebuID, tableID, userNum, julebuType, userid)
end

function DouNiuEvent.JulebuGameCount(tInfo, userArrtTemp)
	--userArr 是玩家的结算列表：包含：userid,nickname,faceid,jifen   --
	
	local userArr = {}
	userArr['tableid'] = tInfo.tableid
	userArr['time'] = TimeUtils.GetTimeString()
	userArr['userlist'] = userArrtTemp
	userArr['module'] = "牛牛"	
	
	
	if SpecialModel ~= nil then
		SpecialModel.GameEnd(userArr)
	end
	
	if tInfo.julebuid == 0 or tInfo.julebutype ~= 2 then
		--判断俱乐部是否为0
		return
	end	
	JulebuService.GameCount(tInfo.julebuid, tInfo.tableid, userArr, g_JulebuDefine.modules_douniu)
end

function DouNiuEvent.JulebuGameStart(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.JulebuGameStart(tInfo.julebuid, tInfo.tableid,tInfo.julebuid)		
end

function DouNiuEvent.CheckJiFen(julebuID, userID)
		
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end
	return JulebuService.CheckJiFen(julebuID, userID, jInfo.gamecount)
end

function DouNiuEvent.CheckBankerJiFen(julebuID, userID)
		
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end
	return JulebuService.CheckJiFen(julebuID, userID, jInfo.bankercount)
end

function DouNiuEvent.IsBiSai(julebuID)
	--比赛场中，需要检查托管，所以，这里需要检查是否在比赛中
	if julebuID == 0 then
		return false
	end
	return JulebuModel.IsBiSai(julebuID)
end



function DouNiuEvent.CheckJiFenTemp(julebuID, userID, jiFen)
	if julebuID == nil or julebuID == 0 then
		return true
	end
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	member.jifen = JulebuModel.GetUserJiFen(julebuID, userID)
	if (member.limitjifen + member.jifen + jiFen) < 80 then
		--低于50就不能进入房间了
		return false
	end	
	return true
end

function DouNiuEvent.AddJiFen(julebuID, userID, JiFen)
	--把积分同步到俱乐部中
	JulebuModel.AddUserJiFen(julebuID, userID, JiFen)
end

function DouNiuEvent.CreateGame(cgmsg, julebuowner)

	--玩家同时创建的房间不能超过10个
	local getList = DouNiuModel.GetUserTableList(cgmsg.userid)
	local mun = 0
	for k,v in pairs(getList) do
		mun = mun + 1
	end
	if mun == 30 then
		return ReturnCode["douniu_create_exceed"]
	end
	
	local gcmsg = msg_douniu_pb.gcdouniujoinroom()
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		return
	end
	
	--房主付费扣钻石
	if cgmsg.payway == 1 then
		if false == DouNiuService.FeeDeductions(pInfo,cgmsg.paynum,cgmsg.paytype) then
			return ReturnCode["douniu_money_short"]
		end
	end
	
	--随机获得房间ID
	gcmsg.hinfo.tableid = DouNiuModel.GetRandomID() 
	ThreadManager.DouNiuLock(gcmsg.hinfo.tableid)
	if gcmsg.hinfo.tableid == 0 then
		ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)
		return ReturnCode["douniu_create_shibai"]
	end	
	
	--给房间信息赋值
	gcmsg.hinfo.paytype = cgmsg.paytype
	gcmsg.hinfo.bankermultiple = cgmsg.bankermultiple
	gcmsg.hinfo.frameid = 0
	gcmsg.hinfo.viptable = 1
	gcmsg.hinfo.dntype = cgmsg.dntype
	gcmsg.hinfo.difen = cgmsg.difen
	gcmsg.hinfo.state =  g_douNiuDefine.state_ready
	gcmsg.hinfo.timemark = g_douNiuTime.ready_time
	gcmsg.hinfo.maxplaynum = cgmsg.maxplaynum  --玩的局数
	gcmsg.hinfo.ownerid = cgmsg.userid    --房主的ID
	gcmsg.hinfo.usernum = 0
	--gcmsg.hinfo.standuser:append(cgmsg.userid)
	gcmsg.hinfo.tabletype = g_douNiuDefine.table_srf   --私人房
	gcmsg.hinfo.payway = cgmsg.payway
	gcmsg.hinfo.autoready = g_douNiuDefine.autoready_NO --cgmsg.autoready 
	gcmsg.hinfo.integralmax = 0		--cgmsg.integralmax      --携带上限
	gcmsg.hinfo.typeodds = cgmsg.typeodds 
	gcmsg.hinfo.specialtype = cgmsg.strvipinfo
	gcmsg.hinfo.paynum = cgmsg.paynum
	gcmsg.hinfo.usermaxnum = cgmsg.maxuser
	gcmsg.hinfo.autoStart = cgmsg.maxuser --cgmsg.autoStart
	gcmsg.hinfo.TZmax = cgmsg.TZmax
	gcmsg.hinfo.julebuid = cgmsg.julebuid
	gcmsg.hinfo.restrict = cgmsg.restrict
	gcmsg.hinfo.banrubcard = 0			--禁止搓牌
	gcmsg.hinfo.playerbuycode = cgmsg.playerbuycode 	--闲家买码
	gcmsg.hinfo.pourrestrict = cgmsg.pourrestrict 		--下注限制
	gcmsg.hinfo.kongwildcard = cgmsg.kongwildcard 		--王牌癞子
	gcmsg.hinfo.pourdouble = cgmsg.pourdouble 			--下注加倍
	gcmsg.hinfo.julebutype = cgmsg.julebutype
	
	
	if gcmsg.hinfo.julebuid ~= 0 and gcmsg.hinfo.julebutype == 2 then
		local jInfo = JulebuModel.GetJulebuInfo(gcmsg.hinfo.julebuid)
		if jInfo ~= nil then
			gcmsg.hinfo.ruleset = jInfo.ruleset
			gcmsg.hinfo.bankercount = jInfo.bankercount	
		end
	end
	--在自动准备的情况下
	--[[if gcmsg.hinfo.autoready == g_douNiuDefine.autoready_OK  then
		--把玩家加入坐下列表
		local chairidList = {0,0,0,0,0,0,0,0}
		for k,v in ipairs(gcmsg.hinfo.situser) do
			chairidList[v.chairid] = 1
		end
		local addItem = gcmsg.hinfo.situser:add()
		for k,v in ipairs(chairidList) do
			if v == 0 then
				addItem.chairid = k
				break
			end
		end				
		addItem.userid = cgmsg.userid
		addItem.state = g_douniuPlayStatus.status_ready
		gcmsg.hinfo.usernum = gcmsg.hinfo.usernum + 1

		if gcmsg.hinfo.dntype == g_douNiuType.type_gdzj and cgmsg.userid == gcmsg.hinfo.ownerid then
			addItem.carryjetton = gcmsg.hinfo.integralmax
		else
			addItem.carryjetton = 0
		end
		
		--把玩家从站立玩家列表删除
		for k,v in ipairs(gcmsg.hinfo.standuser) do
			if v == cgmsg.userid then
				gcmsg.hinfo.standuser:remove(k)
				break
			end
		end
	end	]]
	--把玩家加入房间
	--DouNiuModel.SetUserTableID(cgmsg.userid, gcmsg.hinfo.tableid)
	--把房间加入玩家的房间列表  
	DouNiuModel.SetUserTableList(cgmsg.userid, gcmsg.hinfo.tableid)
	redisItem:hset(DouNiuModel.table_list, gcmsg.hinfo.tableid, gcmsg.hinfo.usernum,DouNiuModel.redis_index)
	--加入俱乐部
	DouNiuEvent.JulebuGameCreate(gcmsg.hinfo)
	
	DouNiuModel.SetTableInfo(gcmsg.hinfo,1)
	ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)
	return 0
	--gcmsg.strvipinfo = gcmsg.hinfo.specialtype
	--gcmsg.result = 0
	--给玩家发生进入房间的协议
	--SendMessage(cgmsg.userid, PacketCode[3010].client, gcmsg:ByteSize(), gcmsg:SerializeToString())	
end

function DouNiuEvent.GetCreateInfo(tInfo, userid)
	
	local cgCre = msg_douniu_2_pb.cgdouniucreate()
	cgCre.userid = userid
	cgCre.maxuser = tInfo.usermaxnum
	cgCre.dntype = tInfo.dntype
	cgCre.difen = tInfo.difen
	cgCre.maxplaynum = tInfo.maxplaynum
	cgCre.payway = tInfo.payway
	cgCre.paynum = tInfo.paynum
	cgCre.typeodds = tInfo.typeodds
	cgCre.restrict = tInfo.restrict
	cgCre.integralmax = tInfo.integralmax
	cgCre.bankermultiple = tInfo.bankermultiple
	cgCre.autoready = tInfo.autoready
	cgCre.strvipinfo = tInfo.specialtype
	cgCre.paytype = tInfo.paytype
	cgCre.julebuid = tInfo.julebuid
	cgCre.autoStart = tInfo.autoStart
	cgCre.TZmax = tInfo.TZmax
	cgCre.banrubcard = tInfo.banrubcard
	cgCre.playerbuycode = tInfo.playerbuycode
	cgCre.pourrestrict = tInfo.pourrestrict
	cgCre.kongwildcard = tInfo.kongwildcard
	cgCre.pourdouble = tInfo.pourdouble
	cgCre.julebutype = tInfo.julebutype
	
	return cgCre
end