PszEvent = {}


function PszEvent.JulebuGameCreate(tInfo)  --牌桌处于等待状态的函数
	
	if tInfo.julebuid == 0 then
		return
	end
	local strDes = ""
	--strDes = tInfo.maxplaynum.."局"
	if tInfo.payway == 2 then
		strDes = strDes.."AA付费"
	end
	--底分
	strDes = strDes.."底分: "..tInfo.difen
	
	--[[双倍比牌
	if tInfo.doublecomp == g_PszDefine.doublecomp_two then
		strDes = strDes..",双倍比牌"
	end
	
	--最大投注回合
	if tInfo.boutcapp == g_PszDefine.boutcapp_5 then
		strDes = strDes..",十轮封顶"
	elseif tInfo.boutcapp == g_PszDefine.boutcapp_10 then
		strDes = strDes..",二十轮封顶"
	elseif tInfo.boutcapp == g_PszDefine.boutcapp_15 then
		strDes = strDes..",三十轮封顶"
	end
	
	--比牌回合
	if tInfo.mincomp == g_PszDefine.mincomp_1 then
		strDes = strDes..",一轮可比"
	elseif tInfo.mincomp == g_PszDefine.mincomp_2 then
		strDes = strDes..",二轮可比"
	elseif tInfo.mincomp == g_PszDefine.mincomp_3 then
		strDes = strDes..",三轮可比"
	end
	
	--闷牌
	if tInfo.menpai == g_PszDefine.menpai_0 then
		strDes = strDes..",不闷牌"
	elseif tInfo.menpai == g_PszDefine.menpai_1 then
		strDes = strDes..",闷一轮"
	elseif tInfo.menpai == g_PszDefine.menpai_2 then
		strDes = strDes..",闷二轮"
	elseif tInfo.menpai == g_PszDefine.menpai_3 then
		strDes = strDes..",闷三轮"
	elseif tInfo.menpai == g_PszDefine.menpai_4 then
		strDes = strDes..",闷四轮"
	elseif tInfo.menpai == g_PszDefine.menpai_5 then
		strDes = strDes..",闷五轮"
	elseif tInfo.menpai == g_PszDefine.menpai_6 then
		strDes = strDes..",闷六轮"
	elseif tInfo.menpai == g_PszDefine.menpai_7 then
		strDes = strDes..",闷七轮"
	elseif tInfo.menpai == g_PszDefine.menpai_8 then
		strDes = strDes..",闷八轮"
	elseif tInfo.menpai == g_PszDefine.menpai_9 then
		strDes = strDes..",闷九轮"
	elseif tInfo.menpai == g_PszDefine.menpai_10 then
		strDes = strDes..",闷十轮"
	end]]
	local useridList = {}
	for k,v in ipairs(tInfo.situser)do 
		table.insert(useridList, v.userid)
	end
	
	JulebuService.CreateGame(g_JulebuDefine.modules_psz, tInfo.julebuid, tInfo.tableid, tInfo.maxuser, #tInfo.situser, strDes,
								tInfo.julebutype, g_PszType.type_psz, useridList,tInfo.maxplaynum, tInfo.usevipnum)
end

function PszEvent.JulebuGameEnd(julebuID, tableID,julebuType, playingtype)  --牌桌处于等待状态的函数
	
	if julebuID == 0 then
		return
	end
	JulebuService.GameEnd(julebuID, tableID,julebuType, playingtype)
end

function PszEvent.JulebuGameUpdate(julebuID, tableID, userNum,julebuType,userid)  --牌桌处于等待状态的函数
	if julebuID == 0 then
		return
	end
	JulebuService.GameUpdate(julebuID, tableID, userNum,julebuType,userid)
end

function PszEvent.JulebuGameCount(tInfo, userArrtTemp)
	--userArr 是玩家的结算列表：包含：userid,nickname,faceid,jifen   --
	if tInfo.julebuid == 0 then
		--判断俱乐部是否为0
		return
	end

	
	local userArr = {}
	userArr['tableid'] = tInfo.tableid
	userArr['time'] = TimeUtils.GetTimeString()
	userArr['userlist'] = userArrtTemp
	userArr['module'] = "拼三张"	
	if SpecialModel ~= nil then
		SpecialModel.GameEnd(userArr)
	end
	
	if tInfo.julebuid == 0 or tInfo.julebutype ~= 2 then
		--判断俱乐部是否为0
		return
	end
	JulebuService.GameCount(tInfo.julebuid, tInfo.tableid, userArr, g_JulebuDefine.modules_psz)
end


function PszEvent.JulebuGameStart(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.JulebuGameStart(tInfo.julebuid, tInfo.tableid,tInfo.julebutype)		
end

function PszEvent.CheckJiFen(julebuID, userID)
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end
	--return true
	--return JulebuService.CheckJiFen(julebuID, userID, jInfo.gamecount)
	return JulebuService.CheckJiFen(julebuID, userID, jInfo.gamecount)
end

function PszEvent.IsBiSai(julebuID)
	--比赛场中，需要检查托管，所以，这里需要检查是否在比赛中
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if julebuID == 0 then
		return false
	end
	return JulebuModel.IsBiSai(julebuID, userID, jInfo.gamecount)
end

function PszEvent.CheckJiFenTemp(julebuID, userID, jiFen)
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

function PszEvent.CreateGame(cgmsg, julebuowner)

	--玩家同时创建的玩家不能超过十个
	local getList = PszModel.GetUserTableList(cgmsg.userid)
	local mum = 0
	if getList ~= nil then
		for k,v in pairs(getList) do
			mum = mum + 1
		end
	end
	if mum == 30 then 
		return ReturnCode["psz_create_exceed"]
	end
	local gcmsg = msg_psz_pb.gcpszjoinroom()
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		return 
	end
	--房主付费扣砖石
	if cgmsg.payway == 1 then
		if false == PszService.FeeDeductions(pInfo,cgmsg.paynum,cgmsg.paytype) then
			return ReturnCode["psz_money_short"]
		end
	end
	--随机获得房间ID
	gcmsg.hinfo.tableid = PszModel.GetRandomID() 
	ThreadManager.PszLock(gcmsg.hinfo.tableid)
	if gcmsg.hinfo.tableid == 0 then
		ThreadManager.PszUnLock(gcmsg.hinfo.tableid)
		return ReturnCode["psz_create_shibai"]
	end	
	--给房间信息赋值
	
	gcmsg.hinfo.maxuser = cgmsg.maxuser --g_PszDefine.max_user
	gcmsg.hinfo.paytype = cgmsg.paytype
	gcmsg.hinfo.difen = cgmsg.difen
	gcmsg.hinfo.state =  g_PszDefine.state_ready
	gcmsg.hinfo.timemark = g_PszTime.ready_time
	gcmsg.hinfo.maxplaynum = cgmsg.maxplaynum  --玩的局数
	gcmsg.hinfo.payway = cgmsg.payway
	gcmsg.hinfo.paynum = cgmsg.paynum
	gcmsg.hinfo.doublecomp = cgmsg.doublecomp
	gcmsg.hinfo.boutcapp = cgmsg.boutcapp
	gcmsg.hinfo.mincomp = cgmsg.mincomp
	gcmsg.hinfo.menpai = cgmsg.menpai
	gcmsg.hinfo.psztype = cgmsg.psztype
	gcmsg.hinfo.tongpai = cgmsg.tongpai    --同牌输赢规则
	gcmsg.hinfo.tonghua = cgmsg.tonghua    --同花收喜规则
	gcmsg.hinfo.baozi = cgmsg.baozi        --豹子收喜规则
	gcmsg.hinfo.qipai = cgmsg.qipai        --弃牌规则
	gcmsg.hinfo.JQK = cgmsg.JQK            --123>JQk
	gcmsg.hinfo.chibaozi = cgmsg.chibaozi  --235吃豹子规则
	gcmsg.hinfo.kanpai = cgmsg.kanpai      --顺序看牌规则
	gcmsg.hinfo.shunzi = cgmsg.shunzi      --顺子是否大于金花
	gcmsg.hinfo.jifen = cgmsg.jifen        --新规则底分
	--gcmsg.hinfo.autoready = cgmsg.autoready 
	
	gcmsg.hinfo.ownerid = cgmsg.userid    --房主的ID
	gcmsg.hinfo.usernum = 0
	gcmsg.hinfo.standuser:append(cgmsg.userid)
	gcmsg.hinfo.tabletype = g_PszDefine.table_srf   --私人房

	gcmsg.hinfo.restrict = cgmsg.restrict     --限制，游戏开始后能不能进入房间
	gcmsg.hinfo.autoready = g_PszDefine.autoready_OK
	gcmsg.hinfo.julebuid = cgmsg.julebuid 
	gcmsg.hinfo.julebutype = cgmsg.julebutype
	gcmsg.strvipinfo = cgmsg.strvipinfo
		
		
	--[[if gcmsg.hinfo.julebuid ~= 0 and gcmsg.hinfo.julebutype == 2 then
		local jInfo = JulebuModel.GetJulebuInfo(gcmsg.hinfo.julebuid)
		if jInfo ~= nil then
			gcmsg.hinfo.ruleset = jInfo.ruleset
			gcmsg.hinfo.bankercount = jInfo.bankercount	
		end
	end]]
	
	PszModel.SetUserTableList(cgmsg.userid, gcmsg.hinfo.tableid)
	redisItem:hset(PszModel.table_list, gcmsg.hinfo.tableid, gcmsg.hinfo.usernum,PszModel.redis_index)
	--加入俱乐部
	PszEvent.JulebuGameCreate(gcmsg.hinfo)
	
	PszModel.SetTableInfo(gcmsg.hinfo,1)
	ThreadManager.PszUnLock(gcmsg.hinfo.tableid)
	return 0
end

function PszEvent.GetCreateInfo(tInfo, userid)
	
	local cgCre = msg_psz_pb.cgpszcreate()
	cgCre.userid = userid
	cgCre.maxuser = tInfo.maxuser
	cgCre.difen = tInfo.difen
	cgCre.maxplaynum = tInfo.maxplaynum
	cgCre.payway = tInfo.payway
	cgCre.paynum = tInfo.paynum
	cgCre.doublecomp = tInfo.doublecomp
	cgCre.boutcapp = tInfo.boutcapp
	cgCre.mincomp = tInfo.mincomp
	cgCre.menpai = tInfo.menpai
	cgCre.restrict = tInfo.restrict
	cgCre.paytype = tInfo.paytype
	cgCre.julebuid = tInfo.julebuid
	cgCre.tongpai = tInfo.tongpai
	cgCre.tonghua = tInfo.tonghua
	cgCre.baozi = tInfo.baozi
	cgCre.qipai = tInfo.qipai
	cgCre.JQK = tInfo.JQK
	cgCre.chibaozi = tInfo.chibaozi
	cgCre.kanpai = tInfo.kanpai
	cgCre.shunzi = tInfo.shunzi
	cgCre.jifen = tInfo.jifen
	cgCre.julebutype = tInfo.julebutype

	return cgCre
end
