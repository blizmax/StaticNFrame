--俱乐部自2018-04-15开始换成直接的魅力值的方式，改掉之前的每日排行榜的机制。
--一起的接口还是不变。AddUserJiFen变成直接修改魅力值的地方，请注意

JulebuService = {}

function JulebuService.Init()
	--在初始化的时候，需要删除所有的online列表
	--[[
	--现在换回这几显示的。不用这样的排行榜了
	local onLineKeys = redisItem:keys(JulebuModel.online_list.."*", JulebuModel.redis_index)
	
	if onLineKeys ~= nil then
		for k,v in pairs(onLineKeys) do
			redisItem:del(v,JulebuModel.redis_index)
		end
	end
	
	JulebuModel.LoadTodayAll()
	--在初始化的时候，检查是否需要恢复之前的积分情况
	]]
end

--创建游戏的时候，
function JulebuService.CreateGame(gameType, julebuID, tableID, maxUserNum,
							userNum, des, julebuType,playingtype, useridList,
							maxgames, nowgames)
	
	
	if true == JingSaiModel.IsJingSai(julebuID) then
		--如果是竞赛中的比赛，跟俱乐部一点关系都没有了。
		JingSaiService.CreateGame(gameType, julebuID, tableID, maxUserNum, userNum, des)
		return
	end
	
	
	local gcUpdate = msg_julebu_pb.gcjulebutableupdate()
	--local jGame = st_julebu_pb.julebutable()
	gcUpdate.jtinfo.julebuid = julebuID
	gcUpdate.jtinfo.tableid = tableID
	gcUpdate.jtinfo.maxusernum = maxUserNum
	gcUpdate.jtinfo.usernum = userNum
	gcUpdate.jtinfo.gametype = gameType
	gcUpdate.jtinfo.des = des
	gcUpdate.jtinfo.julebutype = julebuType
	gcUpdate.jtinfo.maxgames = maxgames
	gcUpdate.jtinfo.nowgames = nowgames
	gcUpdate.jtinfo.playingtype= playingtype
	
	for k,v in ipairs(useridList)do
		gcUpdate.jtinfo.useridlist:append(v)
		local pInfo = PlayerModel.GetPlayerInfo(v)
		gcUpdate.jtinfo.facelist:append(pInfo.face_1)
		gcUpdate.jtinfo.nickname:append(pInfo.nickname)
	end
	JulebuModel.SetGameInfo(gcUpdate.jtinfo)
	JulebuModel.PushGame(julebuID, tableID, julebuType)
	
	gcUpdate.result = 0
	gcUpdate.opttype = 1   --增加
	local onList = JulebuModel.GetOnlineList(julebuID)
	SendMessage(onList,PacketCode[3228].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
end

function JulebuService.JulebuGameStart(julebuID, tableID, julebuType)

	if true == JingSaiModel.IsJingSai(julebuID) then
		--如果是竞赛中的比赛，跟俱乐部一点关系都没有了。
		JingSaiService.JulebuGameStart(julebuID, tableID)
		return
	end

	
	local strGet = JulebuModel.GetGameInfo(julebuID, tableID)
	if strGet ~= nil then
		
		local gcUpdate = msg_julebu_pb.gcjulebutableupdate()
		gcUpdate.jtinfo:ParseFromString(strGet)
		gcUpdate.opttype = 2
		gcUpdate.jtinfo.state = 2    --游戏开始了
		gcUpdate.jtinfo.julebutype = julebuType
		JulebuModel.SetGameInfo(gcUpdate.jtinfo)
		gcUpdate.result = 0
		local onList = JulebuModel.GetOnlineList(julebuID)
		SendMessage(onList,PacketCode[3228].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
	end
end

function JulebuService.GameEnd(julebuID, tableID,julebuType, playingtype)

	if true == JingSaiModel.IsJingSai(julebuID) then
		--如果是竞赛中的比赛，跟俱乐部一点关系都没有了。
		JingSaiService.GameEnd(julebuID, tableID)
		return
	end	
	
	JulebuModel.DelGameInfo(julebuID, tableID, julebuType)
	local gcUpdate = msg_julebu_pb.gcjulebutableupdate()
	gcUpdate.jtinfo.julebuid = julebuID
	gcUpdate.jtinfo.tableid = tableID
	gcUpdate.jtinfo.julebutype = julebuType
	gcUpdate.jtinfo.playingtype = playingtype
	gcUpdate.opttype = 3
	gcUpdate.result = 0
	local onList = JulebuModel.GetOnlineList(julebuID)
	SendMessage(onList,PacketCode[3228].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
end

function JulebuService.GameUpdate(julebuID, tableID, userNum, julebuType,userid)

	if true == JingSaiModel.IsJingSai(julebuID) then
		--如果是竞赛中的比赛，跟俱乐部一点关系都没有了。
		JingSaiService.GameUpdate(julebuID, tableID, userNum)
		return
	end		
	
	local strGet = JulebuModel.GetGameInfo(julebuID, tableID)
	if strGet ~= nil then
		local gcUpdate = msg_julebu_pb.gcjulebutableupdate()
		gcUpdate.jtinfo:ParseFromString(strGet)
		gcUpdate.opttype = 2
		if gcUpdate.jtinfo.usernum > userNum then
			for i = #gcUpdate.jtinfo.useridlist, 1, -1 do
				if userid == gcUpdate.jtinfo.useridlist[i] then
					gcUpdate.jtinfo.useridlist:remove(i)
					gcUpdate.jtinfo.facelist:remove(i)
					gcUpdate.jtinfo.nickname:remove(i)
					break
				end
			end
		elseif gcUpdate.jtinfo.usernum < userNum then
			gcUpdate.jtinfo.useridlist:append(userid)
			local pInfo = PlayerModel.GetPlayerInfo(userid)
			gcUpdate.jtinfo.facelist:append(pInfo.face_1)
			gcUpdate.jtinfo.nickname:append(pInfo.nickname)
		end
		gcUpdate.jtinfo.usernum = userNum
		gcUpdate.jtinfo.julebutype = julebuType
		JulebuModel.SetGameInfo(gcUpdate.jtinfo)
		gcUpdate.result = 0
		local onList = JulebuModel.GetOnlineList(julebuID)
		SendMessage(onList,PacketCode[3228].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
	end
end

function JulebuService.GameCount(julebuID, tableID, userArr, moduleType)

	--这里需要加一个防护，防止重复计费
	--已经出现两次重复计费的
		
	if true == JulebuModel.IsCount(julebuID..tableID) then
		--如果已经被统计过了，就不用重复统计了。
		LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, julebuID,   "recount game count julebuID="..julebuID..", tableID="..tableID)
		return
	end

	if true == JingSaiModel.IsJingSai(julebuID) then
		--如果是竞赛中的比赛，跟俱乐部一点关系都没有了。
		JingSaiService.GameCount(julebuID, tableID, userArr, moduleType)
		return
	end
	
	table.sort(userArr['userlist'], function(a,b) return a['jifen'] > b['jifen'] end)
	
	local pumpingNum = 1
	local pumpingRatio = 0
	local pumpingOrigin = 0
	
	local jInfo = JulebuModel.GetJulebuInfo(julebuID)
	--检查更新昨日数据
	local temptime = os.date("*t", os.time())
	if temptime.day ~= jInfo.updateday and temptime.hour > 7 then
		jInfo.updateday = temptime.day
		jInfo.yesterdaypump = jInfo.todaypump
		jInfo.todaypump = 0
	end
	
	--JulebuService.CheckoutUpdateYesterDayRecord(jInfo)
	if jInfo ~= 0 then
		if jInfo.faceaward_figure == 1 then
			pumpingNum = 99
		elseif jInfo.faceaward_figure == 2 then 
			pumpingNum = 1 
		elseif jInfo.faceaward_figure == 3 then 
			pumpingNum = 1 
		elseif jInfo.faceaward_figure == 4 then 
			pumpingNum = 2 
		elseif jInfo.faceaward_figure == 5 then 
			pumpingNum = 3 
		elseif jInfo.faceaward_figure == 6 then 
			pumpingNum = 4 
		end
		if jInfo.faceaward_num == 1 then
			pumpingRatio = 0
		elseif jInfo.faceaward_num == 2 then
			pumpingRatio = 1	
		elseif jInfo.faceaward_num == 3 then
			pumpingRatio = 2
		elseif jInfo.faceaward_num == 4 then
			pumpingRatio = 3
		elseif jInfo.faceaward_num == 5 then
			pumpingRatio = 4
		elseif jInfo.faceaward_num == 6 then
			pumpingRatio = 5
		elseif jInfo.faceaward_num == 7 then
			pumpingRatio = 6
		elseif jInfo.faceaward_num == 8 then
			pumpingRatio = 7
		elseif jInfo.faceaward_num == 9 then
			pumpingRatio = 8
		elseif jInfo.faceaward_num == 10 then
			pumpingRatio = 9
		elseif jInfo.faceaward_num == 11 then
			pumpingRatio = 10
		end

		if jInfo.faceaward_count == 1 then
			pumpingOrigin = 0
		elseif jInfo.faceaward_count == 2 then
			pumpingOrigin = 50
		elseif jInfo.faceaward_count == 3 then
			pumpingOrigin = 100
		elseif jInfo.faceaward_count == 4 then
			pumpingOrigin = 200
		elseif jInfo.faceaward_count == 5 then
			pumpingOrigin = 300
		elseif jInfo.faceaward_count == 6 then
			pumpingOrigin = 400
		elseif jInfo.faceaward_count == 7 then
			pumpingOrigin = 500
		elseif jInfo.faceaward_count == 8 then
			pumpingOrigin = 600
		elseif jInfo.faceaward_count == 9 then
			pumpingOrigin = 700
		elseif jInfo.faceaward_count == 10 then
			pumpingOrigin = 800
		elseif jInfo.faceaward_count == 11 then
			pumpingOrigin = 900
		elseif jInfo.faceaward_count == 12 then
			pumpingOrigin = 100
		elseif jInfo.faceaward_count == 13 then
			pumpingOrigin = 1100
		elseif jInfo.faceaward_count == 14 then
			pumpingOrigin = 1200
		elseif jInfo.faceaward_count == 15 then
			pumpingOrigin = 1300
		end
	end
	local tmpNum = 1
	for k,v in ipairs(userArr['userlist']) do
		local Count = v['jifen']
		if tonumber(k) == 1 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 1)
		end
		
		if Count >= 500 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 4)
		elseif Count >= 300 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 3)
		elseif Count >= 100 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 2)
		end	
		local pumpingcount = 0
		if Count > 0 and tmpNum <= pumpingNum and pumpingOrigin <= Count and pumpingRatio > 0 then
			pumpingcount = math.ceil(Count * (pumpingRatio/100))
			--pumpingcount =  > Count and Count or pumpingRatio
			Count = Count - pumpingcount
			tmpNum = tmpNum + 1
			
			jInfo.todaypump = jInfo.todaypump + pumpingcount

			local jMember = JulebuModel.GetUserMemberInfo(julebuID, v['userid'])

			
			LogBehavior.Info(v['userid'], "julebu="..julebuID, "rate", pumpingcount, "tableID="..tableID)  --记录返奖的额度

			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, julebuID,   "userid="..v['userid'].."higherid="..jMember.higherid..", pumpingNum="..pumpingNum..", Count="..Count)
			if jMember.higherid ~= 0 then
				--说明这个玩家是被邀请进来的。需要把一半的佣金分给这个兄弟
				local num = 0.5
				if jInfo.proportion then
					if jInfo.proportion == 1 then
						num = 0.6
					elseif jInfo.proportion == 2 then
						num = 0.7
					elseif jInfo.proportion == 3 then
						num = 0.8
					end
				end
				local getJifen = math.floor(pumpingcount * num)
				pumpingcount = pumpingcount - getJifen
				JulebuModel.AddGetJiFen(julebuID, jMember.higherid, v['userid'], getJifen)
			else
				
			end
			LogBehavior.Info(v['userid'], "julebu="..julebuID, "rate", pumpingcount, "tableID="..tableID)  --记录返奖的额度
		end
		
		
		JulebuService.StatisticsPartnerRecordDay(jInfo, v['userid'], v['jifen'], tonumber(k), pumpingcount) --把这个也到合伙人里面去
		JulebuModel.AddUserJiFen(julebuID, v['userid'], Count, tableID)  --把积分同步更新]]
	end
	
	JulebuModel.SetJulebuInfo(jInfo)
	JulebuModel.SetJiFenCheck(julebuID..tableID) 
end
function JulebuService.ServerLoop(currTime)
	--在这里轮询中，做每天结算的工作
	--只有在晚上八点的时候，才对对应的玩家做统计
	--
	if true then
		--摸摸这边不能走每天结算的情况了
		return
	end

end

function JulebuService.LoopDebug(currTime)
--先判断时间，如果是晚上十点中
	local isCount = false

	if nil == string.find(currTime,"0:00") --[[and nil == string.find(currTime,"00:00")--]] then 
		return 
	end
	
	local sqlCase = "SELECT count(id) FROM dy_julebu where status=99"
	mysqlItem:executeQuery(sqlCase)
	local count = mysqlItem:fetch()
	count = tonumber(count)
	local cycleNUm = math.floor(count/1000) + 1
	local startNum = 0
	local idList = {}
	for i = 1,cycleNUm do
		--这样做是为了保证一定会被取完
		sqlCase = "select id from dy_julebu where status="..g_JulebuDefine.mstatus_normal.." limit "..startNum..",1000" --每次取1000个
		mysqlItem:executeQuery(sqlCase)
		
		while true do
			local sqlData = mysqlItem:fetch()
			if sqlData == nil then
				break
			end
			table.insert(idList, tonumber(sqlData))
			--这里判断那个俱乐部需要结算的
			--一定要放到一个列表中，才不会影响mysql缓存里面的 数据
		end
		--
		startNum = startNum + 1000
	end

	for i = 1,#idList do
		--从缓存中把数据读出来
		local jifenTemp = JulebuModel.GetTodayJiFen(idList[i])
		--这里需要进行排序
		local jifenList = {}
		local function insertJiFen(listTemp, item)   --先进行排序
			--if item[2] == 0 then
			--	return   --积分是0的，不用参加排行榜
			--end
			local tempItem = table.clone(item)
			table.insert(listTemp, item)
			for n = #listTemp, 2, -1 do
				if listTemp[n - 1][2] < listTemp[n][2] then
					listTemp[n][1] = listTemp[n-1][1]
					listTemp[n][2] = listTemp[n-1][2]
					listTemp[n - 1][1] = tempItem[1]
					listTemp[n - 1][2] = tempItem[2]
				else
					break
				end
			end
		end
		
		for k,v in pairs(jifenTemp) do
			
			local addItem = {}
			addItem[1] = k
			addItem[2] = tonumber(v)
			insertJiFen(jifenList, addItem)
		end
		
		
		--开始进行积分了
		--取完后立刻删除
		
		if #jifenList > 0 then
		
			local zjList = st_julebu_pb.julebuzhanjiinfo()
			local jfDay = st_julebu_pb.julebuzhanjiday()
			for m=1,#jifenList do
				local addItem = jfDay.mlist:add()
				local isExist = true
				if nil == JulebuModel.GetUserMemberInfo(idList[i], jifenList[m][1], addItem) then
					--表示玩家已经退出牌友圈了
					JulebuModel.LoadUserMemberInfo(idList[i], jifenList[m][1], addItem)  --从数据库中加载
					isExist = false
				end
				addItem.jifen = tonumber(jifenList[m][2])
				
				if addItem.jifen < 0 then
					--只有在积分是负数的时候，才显示积分
					addItem.limitjifen = addItem.limitjifen + addItem.jifen
					local sqlCase = "update dy_julebu_member set limitjifen="..addItem.limitjifen.." where julebuid="..idList[i].." and userid="..jifenList[m][1]
					mysqlItem:execute(sqlCase)
				end
				if true == isExist then
					JulebuModel.AddToMemberList(idList[i], addItem)  --设计
				end
				
				local addMemberDay = st_julebu_pb.julebumemberzhanjiday()
				addMemberDay.userid = addItem.userid
				addMemberDay.jifen = addItem.jifen
				addMemberDay.timemark = currTime
				
				JulebuModel.SetMemberDay(idList[i], addMemberDay.userid, addMemberDay)     --这里设定每个玩家的情况
				JulebuModel.ReNameDayUserZhanji( idList[i], addMemberDay.userid, currTime )  --这里需要rename当天的值
			end
 			JulebuModel.GetUserMemberInfo(idList[i], jifenList[1][1], zjList.maxuser)
			
			zjList.timemark = currTime
			jfDay.timemark = currTime
			JulebuModel.SetHistory(idList[i],zjList)
			JulebuModel.DelTodayJifen(idList[i])
			
			JulebuModel.SetDayHistory(idList[i], currTime, jfDay)
		end
		--每个循环结束的时候，更新时间
	end		
end

function JulebuService.CheckJiFen(julebuID, userID, jifendoorsill)
	if julebuID == nil or julebuID == 0 then
		return true
	end
	
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if  member == nil or member == 0 then
		return false
	end
	
	if member.limitjifen < jifendoorsill then
		return false
	end	
	return true
end


function JulebuService.IsJoinIn(userID, checkID)

	local julebuID = checkID
	if true == JingSaiModel.IsJingSai(checkID) then
		--如果是竞赛的情况下
		local jsInfo = JingSaiModel.GetJingSaiInfo(checkID) 
		if jsInfo ~= nil then
			julebuID = jsInfo.julebuid
		else
			return true
		end
	end	
	return JulebuModel.IsJoinIn(userID, julebuID)
end

function JulebuModel.IsBiSai(julebuID)
	return JingSaiModel.IsJingSai(julebuID)
end


function JulebuService.GetJiFen(julebuID, userID)
	if julebuID == 0 then
		return 0
	end
	if true == JingSaiModel.IsJingSai(julebuID) then
		return 0
	end
	
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if  member == nil or member == 0 then
		return 0
	end	
	return member.limitjifen
end


function JulebuService.CreatTable(tInfo, gameType)
	
	local jInfo = JulebuModel.GetJulebuInfo(tInfo.julebuid)
	if jInfo == nil then
		return 
	end
	if gameType == g_JulebuDefine.modules_douniu then
		local cgC = DouNiuEvent.GetCreateInfo(tInfo, jInfo.ownerid)
		DouNiuEvent.CreateGame(cgC, jInfo.ownerid)
	elseif gameType == g_JulebuDefine.modules_pdk then
		local cgC = PdkEvent.GetCreateInfo(tInfo, jInfo.ownerid)
		PdkEvent.CreateGame(cgC, jInfo.ownerid)
	elseif gameType == g_JulebuDefine.modules_psz then
		local cgC = PszEvent.GetCreateInfo(tInfo, jInfo.ownerid)
		PszEvent.CreateGame(cgC, jInfo.ownerid)
	end
	
end

function JulebuService.CheckIsAdmin(jInfo,userid)
	
	local len = #jInfo.adminlist
	for i = len,1,-1 do
	--从后面往回看
		if jInfo.adminlist[i] == userid then
			return true
		end
	end
	return false
end


function JulebuService.RecordDayingjia(julebuID, userid, falg)

	local bInfo = st_julebu_pb.julebumember()
	JulebuModel.GetUserMemberInfo(julebuID, userid, bInfo)
	if bInfo ~= 0 then
		if falg == 1 then
			bInfo.dayingjia = bInfo.dayingjia + 1
		elseif  falg == 2 then
			bInfo.fen100 = bInfo.fen100 + 1
		elseif  falg == 3 then
			bInfo.fen300 = bInfo.fen300 + 1
		elseif  falg == 4 then
			bInfo.fen500 = bInfo.fen500 + 1
		end
		JulebuModel.AddToMemberList(julebuID, bInfo)
	end
end

function JulebuService.checkoutsuperiorid(julebuid, userid)
	local sqlCase = "select superiorid from dy_julebu_member  where userid="..userid.." and julebuid="..julebuid
	mysqlItem:executeQuery(sqlCase)
	sqlData = mysqlItem:fetch()
	if sqlData == nil then
		return 0 
	else
		return tonumber(sqlData)
	end

end

function JulebuService.DelPartner(julebuid, userid)
	
	JulebuModel.DelPartnerInfo(julebuid, userid)
	local resultlist = {}
	local sqlCase = "select userid from dy_julebu_member  where superiorid="..userid.." and julebuid="..julebuid
	mysqlItem:executeQuery(sqlCase)
	while true do
		local sqlData = mysqlItem:fetch()
		if sqlData == nil then
			break
		end
		JulebuModel.DelPartnerInfo(julebuid, tonumber(sqlData))
	end
	sqlCase = "update dy_julebu_member set superiorid = 0 where superiorid="..userid.." and julebuid="..julebuid
	mysqlItem:executeQuery(sqlCase)
end

function JulebuService.StatisticsPartnerRecordDay(jInfo, userid, count, isdayingjia, consume)
	local superiorid = JulebuService.checkoutsuperiorid(jInfo.julebuid, userid)
	if superiorid == nil or superiorid == 0 then
		return 
	end
	
	local tmp = JulebuModel.GetPartnerInfo(jInfo.julebuid, userid)
	local PartnerInfo = st_julebu_pb.julebupartnerinfo()
	PartnerInfo:ParseFromString(tmp)
	PartnerInfo.gamenum = PartnerInfo.gamenum + 1
	PartnerInfo.todayscore = PartnerInfo.todayscore + count
	if isdayingjia == 1 then
		PartnerInfo.todaydayingjia = PartnerInfo.todaydayingjia + 1
	end
	PartnerInfo.todaygamenum = PartnerInfo.todaygamenum + 1
	PartnerInfo.todayconsume = PartnerInfo.todayconsume + consume
	JulebuModel.SetPartnerInfo(jInfo.julebuid, userid, PartnerInfo)
	
	local tmp1 = JulebuModel.GetPartnerInfo(jInfo.julebuid, superiorid)
	local PartnerInfo1 = st_julebu_pb.julebupartnerinfo()
	PartnerInfo1:ParseFromString(tmp1)
	PartnerInfo1.gamenum = PartnerInfo1.gamenum + 1
	if isdayingjia == 1 then
		PartnerInfo1.todaydayingjia = PartnerInfo1.todaydayingjia + 1
	end
	PartnerInfo1.todaygamenum = PartnerInfo1.todaygamenum + 1
	PartnerInfo1.todayconsume = PartnerInfo1.todayconsume + consume
	JulebuModel.SetPartnerInfo(jInfo.julebuid, superiorid, PartnerInfo1)
end

function JulebuService.CheckoutUpdateYesterDayRecord(jInfo)
	
	local nowtime = TimeUtils.GetTime()
	local daytime = TimeUtils.GetTimeDay(nowtime)
	
	local jIdaytime = TimeUtils.GetTimeDay(jInfo.settlementday)
	jIdaytime = jIdaytime == nil and 0 or jIdaytime
	
	local distanceTime = daytime - jIdaytime

	if distanceTime == 1 then
		local sqlCase = "select userid from dy_julebu_member where julebuid="..jInfo.julebuid
		mysqlItem:executeQuery(sqlCase)
		while true do
			local sqlData = mysqlItem:fetch()
				if sqlData == nil then
				break
			end
			local tmp = JulebuModel.GetPartnerInfo(jInfo.julebuid, tonumber(sqlData))
			local PartnerInfo = st_julebu_pb.julebupartnerinfo()
			PartnerInfo:ParseFromString(tmp)
			PartnerInfo.yesterdayscore = PartnerInfo.todayscore
			PartnerInfo.yesterdaydayingjia = PartnerInfo.todaydayingjia 
			PartnerInfo.yesterdaygamenum =	PartnerInfo.todaygamenum 
			PartnerInfo.yesterdayconsume = PartnerInfo.todayconsume
			PartnerInfo.todayscore = 0
			PartnerInfo.todaydayingjia = 0
			PartnerInfo.todaygamenum = 0
			PartnerInfo.todayconsume = 0
			JulebuModel.SetPartnerInfo(jInfo.julebuid, tonumber(sqlData), PartnerInfo)
		end
		jInfo.settlementday = nowtime
	elseif distanceTime > 1 then
		for k, v in ipairs(jInfo.partnerlist) do 
			local sqlCase = "select userid from dy_julebu_member  where julebuid="..jInfo.julebuid
			mysqlItem:executeQuery(sqlCase)
			while true do
				local sqlData = mysqlItem:fetch()
				if sqlData == nil then
					break
				end
				local tmp = JulebuModel.GetPartnerInfo(jInfo.julebuid, tonumber(sqlData))
				local PartnerInfo = st_julebu_pb.julebupartnerinfo()
				PartnerInfo:ParseFromString(tmp)
				PartnerInfo.yesterdayscore = 0
				PartnerInfo.yesterdaydayingjia = 0
				PartnerInfo.yesterdaygamenum =	0
				PartnerInfo.yesterdayconsume = 0
				PartnerInfo.todayscore = 0
				PartnerInfo.todaydayingjia = 0
				PartnerInfo.todaygamenum = 0
				PartnerInfo.todayconsume = 0
				JulebuModel.SetPartnerInfo(jInfo.julebuid, tonumber(sqlData), PartnerInfo)
			end
		end
		jInfo.settlementday = nowtime
	end
	JulebuModel.SetJulebuInfo(jInfo)
	
end

function JulebuService.CheckDelJuelbuPlayer(jInfo, userid)
	
	local isPartner = false 
	for k, v in ipairs(jInfo.partnerlist) do 
		if v == userid then
			isPartner = true 
			break
		end
	end
	local Partnerlist = {}
	if isPartner == true then
		for i = #jInfo.partnerlist, 1, -1 do 
			if jInfo.partnerlist[i] == userid then
				jInfo.partnerlist:remove(i)	
			else
				table.insert(Partnerlist, jInfo.partnerlist[i])
			end
		end
		JulebuService.DelPartner(jInfo.julebuid, userid)
		local sqlCase = "update dy_julebu set partnerlist='"..luajson.encode(Partnerlist).."' where id="..jInfo.julebuid
		SqlServer.rpush(sqlCase)
		JulebuModel.SetJulebuInfo(jInfo)
	else 
		if 0 ~= JulebuService.checkoutsuperiorid(jInfo.julebuid, userid) then
			JulebuModel.DelPartnerInfo(jInfo.julebuid, userid)
			local sqlCase = "update dy_julebu_member set superiorid=0 where userid="..userid.." and julebuid="..jInfo.julebuid
			mysqlItem:executeQuery(sqlCase)
		end
	end
end

function JulebuService.CheckoutUpdateJuelbuPlayInfo(jInfo)
	
	local nowtime = TimeUtils.GetTime()
	local daytime = TimeUtils.GetTimeDay(nowtime)
	local jIdaytime = TimeUtils.GetTimeDay(jInfo.settlementday)
	jIdaytime = jIdaytime == nil and 0 or jIdaytime
	local distanceTime = daytime - jIdaytime

	if distanceTime >= 1 then
		local getList = JulebuModel.GetMemberList(jInfo.julebuid)
		if getList ~= nil then
			for k, v in pairs(getList) do
				local bInfo = st_julebu_pb.julebumember()
				bInfo:ParseFromString(v)
				bInfo.dayingjia = 0
				bInfo.fen100 = 0
				bInfo.fen300 = 0
				bInfo.fen500 = 0
				JulebuModel.AddToMemberList(jInfo.julebuid, bInfo)
			end
		end
		jInfo.settlementday = nowtime
		JulebuModel.SetJulebuInfo(jInfo)
	end
	
end