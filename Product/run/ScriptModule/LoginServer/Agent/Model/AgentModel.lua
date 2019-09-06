
AgentModel = {}
AgentModel.redis_index = "redis_agent"

AgentModel.agent_info = "agent_"
AgentModel.agent_commission = "agent_commission"

AgentModel.agent_daymark = "agent_daymark"     --用于标记抽佣分成的时间，每天只统计一遍。

AgentModel.agent_ratio = "agent_ratio"
AgentModel.agent_achievement = "agent_achievement"   --用于保存每个玩家的流水

AgentModel.agent_player_fee = "agent_player_fee"   --抽水列表，哈希列表，keys是 userid，values是值 代理中，玩家抽水的统计
AgentModel.agent_player_pour = "agent_player_pour"  --下注列表，哈希列表，keys是 userid，values是值 
AgentModel.agent_player_win = "agent_player_win"    --赢钱列表，哈希列表，keys是 userid，values是值 
function AgentModel.GetAgentInfo(userID, agyInfo)
	if userID == nil or userID == 0 then
		return nil
	end

	--获取代理的信息
	if agyInfo == nil then
		agyInfo = st_agent_pb.agentinfo()
	end
	local strGet = redisItem:get(AgentModel.agent_info..userID, AgentModel.redis_index)

	if strGet ~= nil then
		agyInfo:ParseFromString(strGet)
		agyInfo.qrcodeurl = AgentModel.GetQrcodeUrl(userID)
		agyInfo.exclusiveurl = AgentModel.GetQrcodeUrl(userID)

		return agyInfo
	else
		
		return AgentModel.LoadAgentInfo(userID,agyInfo)
	end
end

function AgentModel.SetAgent(userID, agyInfo)
	redisItem:set(AgentModel.agent_info..userID, agyInfo:SerializeToString(), AgentModel.redis_index)
end

function AgentModel.LoadAgentInfo(userID,agyInfo)
	

	if agyInfo == nil then
		agyInfo = st_agent_pb.agentinfo()
	end

	local sqlCase = "select * from ag_player where userid="..userID
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	
	if sqlData ~= nil then

		agyInfo.invitestr = sqlData[9]
		agyInfo.level = tonumber(sqlData[14])
		agyInfo.soleagent = tonumber(sqlData[19])
		agyInfo.agent1 = tonumber(sqlData[20])
		agyInfo.agent2 = tonumber(sqlData[21])
		agyInfo.agent3 = tonumber(sqlData[22])
		agyInfo.agent4 = tonumber(sqlData[23])
		agyInfo.qrcodeurl = AgentModel.GetQrcodeUrl(userID)
		agyInfo.datemark = sqlData[25]
		agyInfo.moneymark = tonumber(sqlData[26])
		agyInfo.weekmoney = tonumber(sqlData[18])
		agyInfo.alipay = sqlData[33]
		agyInfo.bankaccount = sqlData[27]
		agyInfo.exclusiveurl = AgentModel.GetQrcodeUrl(userID)
		
		agyInfo.grade = tonumber(sqlData[37])
		agyInfo.wiringid = tonumber(sqlData[38])
		agyInfo.userid  = tonumber(userID)

	else

		local pInfo = PlayerModel.GetPlayerInfo(userID)
		
		if pInfo ~= nil then
			agyInfo.userid = tonumber(userID)
			agyInfo.invitestr = tostring(userID)
			agyInfo.level = 91
			agyInfo.qrcodeurl = AgentModel.GetQrcodeUrl(userID)
			agyInfo.exclusiveurl = AgentModel.GetQrcodeUrl(userID)
			agyInfo.soleagent = 0
			agyInfo.agent1 = 0
			agyInfo.agent2 = 0
			agyInfo.agent3 = 0
			agyInfo.agent4 = 0
			agyInfo.grade = -1
			agyInfo.wiringid = 0
			AgentModel.InsertAgent(pInfo, agyInfo)
		else
			LogFile("error", "PlayerModel.GetPlayerInfo="..userID)
		end
	end
	redisItem:set(AgentModel.agent_info..userID, agyInfo:SerializeToString(), AgentModel.redis_index)
	return agyInfo
end

--付费的ID，付费的金额

function AgentModel.InsertAgent(pInfo, myAgent)
	
	local sqlCase = "insert into ag_player(userid,nickname,account,expandcode,state,level,soleagent,agent1,agent2,agent3,agent4,qrcodeurl,grade,wiringid,getrate) values("
		..pInfo.userid..",'"..pInfo.nickname.."','"..pInfo.userid.."','"..pInfo.userid.."',0,"..myAgent.level..","..myAgent.soleagent..","
		..myAgent.agent1..","..myAgent.agent2..","..myAgent.agent3..","..myAgent.agent4..",'"..myAgent.qrcodeurl.."',"..myAgent.grade..","..myAgent.wiringid..",1)" 	
	
	mysqlItem:execute(sqlCase)
end



function AgentModel.Exclusiveurl(userID)
	--根据不同的服务器，配置不同的二维吗分享
	
	return g_webUrl.."/Public/share/"..g_gamename..".html?bindcode="..userID.."&platfromid="..g_gamename
end


function AgentModel.CreateWiringQrcode(userID, wiringID)
	if g_gamename == "uudwc" then
		return g_webUrl.."/index.php/dwcgame/client/uudwcauthor?userid="..userID.."&wiringid="..wiringID
	else
		--return g_webUrl.."/index.php/"..g_gamename.."/client/wechatauthor?userid="..userID.."&wiringid="..wiringID
		return AgencyModel.Exclusiveurl(userID)
	end
end

function AgentModel.GetQrcodeUrl(userID)
	
	
	if g_gamename == "uudwc" then
		return g_webUrl.."/index.php/dwcgame/client/uudwcauthor?userid="..userID
	else
		--return g_webUrl.."/index.php/"..g_gamename.."/client/wechatauthor?userid="..userID
		return AgencyModel.Exclusiveurl(userID)
	end
	
end


--feeall ：收税总额
--pourAll：下注总额：在斗地主中，抢庄牛牛，拼三张中，是真是玩家投到桌子上的金币数
--winAll:系统赢玩家的钱
--gameType:游戏类型
--tableType：牌桌类型
function AgentModel.GameDataCommit(feeall, pourAll, winAll, gameType, tableType)
	
	if feeall == 0 and pourAll == 0 and winAll == 0 then
		return 
	end
	
	local tmpsql = "" 
	if gameType == g_DdzDefine.game_type then 	--斗地主
		tmpsql = tmpsql.."doudizhu=doudizhu+"..feeall..", ach_doudizhu=ach_doudizhu+"..pourAll..", win_doudizhu=win_doudizhu+"..winAll
	elseif gameType == g_lhdDefine.game_type then	--龙虎斗
		tmpsql = tmpsql.."longhuddou=longhuddou+"..feeall..", ach_longhudou=ach_longhudou+"..pourAll..", win_longhudou=win_longhudou+"..winAll
	elseif gameType == g_hongheiDefine.game_type then --红黑大战
		tmpsql = tmpsql.."honghei=honghei+"..feeall..", ach_honghei=ach_honghei+"..pourAll..", win_honghei=win_honghei+"..winAll
	elseif gameType == g_barccatatDefine.game_type then	--百家乐
		tmpsql = tmpsql.."baijiale=baijiale+"..feeall..", ach_baijiale=ach_baijiale+"..pourAll..", win_baijiale=win_baijiale+"..winAll
	elseif gameType == g_douNiuDefine.game_type then	--抢庄牛牛
		tmpsql = tmpsql.."qiangzhuangniuniu=qiangzhuangniuniu+"..feeall..", ach_qiangzhuangniuniu=ach_qiangzhuangniuniu+"..pourAll..", win_qiangzhuangniuniu=win_qiangzhuangniuniu+"..winAll
	elseif gameType == g_brnnDefine.game_type then		--百人牛牛
		tmpsql = tmpsql.."bairenniuniu=bairenniuniu+"..feeall..", ach_bairenniuniu=ach_bairenniuniu+"..pourAll..", win_bairenniuniu=win_bairenniuniu+"..winAll
	elseif gameType == g_PszDefine.game_type then		--扎金花
		tmpsql = tmpsql.."zhajinhua=zhajinhua+"..feeall..", ach_zhajinhua=ach_zhajinhua+"..pourAll..", win_zhajinhua=win_zhajinhua+"..winAll
	elseif gameType == g_bcbmDefine.game_type then		--奔驰宝马
		tmpsql = tmpsql.."benchibaoma=benchibaoma+"..feeall..", ach_beichibaoma=ach_beichibaoma+"..pourAll..", win_beichibaoma=win_beichibaoma+"..winAll
	elseif gameType == g_fqzsDefine.game_type then		--狮子王国
		tmpsql = tmpsql.."shiziwanghuo=shiziwanghuo+"..feeall..", ach_shiziwangguo=ach_shiziwangguo+"..pourAll..", win_shiziwangguo=win_shiziwangguo+"..winAll
	elseif gameType == g_sicboDefine.game_type  then	--骰宝
		tmpsql = tmpsql.."toubao=toubao+"..feeall..", ach_toubao=ach_toubao+"..pourAll..", win_toubao=win_toubao+"..winAll
	elseif gameType == g_fenfencaiDefine.game_type then --分分彩
		tmpsql = tmpsql.."fenfencai=fenfencai+"..feeall..", ach_fenfencai=ach_fenfencai+"..pourAll..", win_fenfencai=win_fenfencai+"..winAll
	elseif gameType == g_TtzDefine.game_type then --推筒子
		tmpsql = tmpsql.."ttz=ttz+"..feeall..", ach_ttz=ach_ttz+"..pourAll..", win_ttz=win_ttz+"..winAll
	elseif gameType == g_gdmjDefine.game_type then --二人麻将
		tmpsql = tmpsql.."ermj=ermj+"..feeall..", ach_ermj=ach_ermj+"..pourAll..", win_ermj=win_ermj+"..winAll
	end
	
	
	local strDay = TimeUtils.GetDayString()
	
	local moneyIn = 0
	local moneyOut = feeall
	if winAll > 0 then
		--系统赢钱，钱流走
		moneyOut = winAll + feeall
	else
		--系统输钱，钱流入
		moneyIn = 0 - winAll
	end
	
	local sqlCase = "update log_gameday set moneyout=moneyout-"..moneyOut..", allfee=allfee+"..feeall..", allach=allach+"..pourAll..", allwin=allwin+"..winAll..", "..tmpsql.." where datestr='"..strDay.."'"
	--mysqlItem:execute(sqlCase)
	LogModel.LogGameDetail(sqlCase)


	local sqlCase = "update log_sysdaily set allfee=allfee+"..feeall..",feeday=feeday+"..feeall..",feeweek=feeweek+"..feeall..",feemonth=feemonth+"..feeall..", allwin=allwin+"..winAll..", syswinday=syswinday+"..winAll..", syswinweek=syswinweek+"..winAll..", syswinmonth=syswinmonth+"..winAll.." where dateid='"..strDay.."'"
	
	--mysqlItem:execute(sqlCase)
	LogModel.LogSysdailyPush(sqlCase)
end


function AgentModel.CommissionCount(userid, Count, gameType, tableType, pourJetton, playerWin)
	
	if Count <= 0 then
		return Count
	end
	
	local pourJetton = pourJetton == nil and 0 or pourJetton 
	local Count = Count == nil and 0 or Count 
	local tmpCount = Count
	if g_brokerageType.mode == 1 then
		tmpCount = Count - pourJetton
	end
		
	local Ratio = AgentModel.getGameCommissionRatio(gameType, tableType)
	local commission = tmpCount * Ratio
	commission = math.ceil(commission) 
	local realCount = Count - commission
	if RobotService.IsRobot(userid) == false and userid ~= 1 and commission > 0 then
		LogServer.PlayerFee(userid, commission)

		LogServer.UserCommissionRecord(userid, gameType, tableType, commission)
		
		local sqlCase = "update dy_gamelist set dayfee=dayfee+"..commission..", weekfee=weekfee+"..commission..", monthfee=monthfee+"..commission..",allfee=allfee+"..commission..
			" where gametype="..gameType.." and tabletype="..tableType
		mysqlItem:executeQuery(sqlCase)

		local sqlCase2 = "update ag_player set moneymark = moneymark + "..commission .. " where userid = "..userid
		mysqlItem:executeQuery(sqlCase2)
	end

	return realCount
	
end

--统计玩家的流水，
--optType 值：1：下注的钱，2：赢回去的钱
--在拼三张，斗牛，斗地主中，也需要传入输的人的钱
function AgentModel.AchievementCount(userID, Count, optType, gameType, tableType, serviceCharge)  
	--这里统计，只是统计真正玩家，不统计机器人
	if Count <= 0 then
		return
	end
	
	if true == RobotService.IsRobot(userID) or userID == 1 then
		
	else
		if g_CommissionFwSwitch == 1 then
			--抽水不算流水
			local tmp = serviceCharge == nil and 0 or serviceCharge
			Count = Count - tmp
		end
		--暂时先统计这一个
		redisItem:hincrby(AgentModel.agent_achievement, userID, Count, AgentModel.redis_index)
		
	end
	
	
end

function AgentModel.getGameCommissionRatio(gameType, tableType)
	--return 0.02
	local ratio = redisItem:hget(AgentModel.agent_ratio, gameType..tableType, AgentModel.redis_index)
	return ratio == nil and AgentModel.LoadGameCommissionRatio( gameType, tableType ) or tonumber(ratio)
end


function AgentModel.LoadGameCommissionRatio(gameType, tableType)
	local sqlCase = "select feerate from dy_gamelist where gametype="..gameType.." and tabletype="..tableType
	mysqlItem:executeQuery(sqlCase)

	local ratio = mysqlItem:fetch()
	
	ratio = ratio == nil and 7.2 or tonumber(ratio)
	if ratio ~= 7.2 then
		ratio = 7.2
	end
	ratio = ratio/100
	redisItem:hset(AgentModel.agent_ratio, gameType..tableType, ratio, AgentModel.redis_index)
	return ratio
end


function AgentModel.StatisticsCommission(userid, commission)
	redisItem:hincrby(AgentModel.agent_commission, userid, commission,AgentModel.redis_index)
end

function AgentModel.GetUserCommission()
	return redisItem:hgetall(AgentModel.agent_commission, AgentModel.redis_index)
end


function AgentModel.SetAgentDaymark(dateSec)
	redisItem:set( AgentModel.agent_daymark, dateSec, AgentModel.redis_index)
end

function AgentModel.GetAgentDaymark()
	redisItem:get( AgentModel.agent_daymark, AgentModel.redis_index )
end


function AgentModel.DayFirstLogin(userID)

	processWork("UnlimitedIncrUserActivity", userID)
	
end



function AgentModel.CheckIsWiringBind(pInfo, unionID)
	
	
	--在每个玩家创建角色的时候，检查是否绑定代理了。
	local wiringID = 0
	local toAgencyUserID = 0
	
	LogFile("agent","unionID="..unionID)

		if unionID == nil or unionID == '' then
			return nil
		end
		local sqlCase = "select id,binduserid,wiringid from ag_author where unionid='"..unionID.."' order by id desc limit 1"
		
		LogFile("agent","sqlCase="..sqlCase)
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch({})

		if sqlData == nil then
			return nil
		end	
		LogFile("agent","sqlData="..luajson.encode( sqlData ))
		local wiringID =  sqlData[3] == nil and 0 or tonumber(sqlData[3])
		
		local toAgencyUserID = sqlData[2] == nil and 0 or tonumber(sqlData[2])
		LogFile("agent","toAgencyUserID="..toAgencyUserID)
		if toAgencyUserID == 0 then
			return
		end
		
	local toAgency = AgentModel.GetAgentInfo(toAgencyUserID)

	
	if toAgency == nil or toAgency.userid == 0 then
		return
	end

	
	local myAgent = st_agent_pb.agentinfo()
	local grade = wiringID > 0 and 0 or -1
	myAgent.qrcodeurl = AgentModel.GetQrcodeUrl(pInfo.userid)
	myAgent.invitestr = tostring(pInfo.userid)
	myAgent.userid = pInfo.userid
	myAgent.level = 91
	myAgent.wiringid = wiringID
	myAgent.grade = grade
	
	if wiringID == 0 then
		--如果排线ID为0，那么就说明这个兄弟扫的是直属玩家
		
	
		myAgent.soleagent = toAgency.level > 98 and toAgency.userid or toAgency.soleagent
		myAgent.agent1 = toAgency.userid
		myAgent.agent2 = toAgency.agent1
		myAgent.agent3 = toAgency.agent2
		myAgent.agent4 = toAgency.agent3
		myAgent.wiringid = 0 --toAgency.wiringid   --这里如果传入的排线ID是0，那么这里也应该是0,直属的玩家

	end	

	
	AgentModel.InsertAgent(pInfo, myAgent)
	
	local isPaixian = 0
	if wiringID ~= 0 then
		--如果排线ID不为0，那么这里留需要找到他的上级了。
		local sqlCase = "select count(userid) from ag_player where wiringid="..wiringID.." and userid<"..pInfo.userid.." and grade >= 0" 
		mysqlItem:executeQuery(sqlCase)
		grade = mysqlItem:fetch()
		
		sqlCase = "select userid from ag_player where wiringid="..wiringID.." and userid<"..pInfo.userid.." and grade >=0 order by userid desc limit 1"
		mysqlItem:executeQuery(sqlCase)
		
		local agent1 = mysqlItem:fetch()
		
		grade = grade ==  nil and  1 or (tonumber(grade) + 1)
		agent1 = agent1 == nil and sqlData[2] or agent1

		sqlCase = "update ag_player set grade="..grade..",agent1="..agent1.." where userid="..pInfo.userid
	
		mysqlItem:execute(sqlCase)
		myAgent.grade = grade
		myAgent.agent1 = tonumber(agent1)
		UnlimitedModel.AddUserIDToWiring(wiringID, pInfo.userid, grade)
		
		isPaixian = 1
		
	end
	
	UnlimitedModel.SetUserPreAgentID(pInfo.userid, myAgent.agent1)  --设置上级的ID
	
	local sendData = {}
	sendData['userid'] = pInfo.userid
	sendData['wiringid'] = myAgent.wiringid
	sendData['paixian'] = myAgent.wiringid
	processWork("UnlimitedIncrUserNum", luajson.encode(sendData))
	
	AgentModel.SetAgent(pInfo.userid, myAgent)
	

end




function AgentModel.CheckIsWiringBindPre(pInfo, unionID, bindUserID)
	
	
	--在每个玩家创建角色的时候，检查是否绑定代理了。
	local wiringID = 0
	local toAgencyUserID = bindUserID
	
	local toAgency = AgentModel.GetAgentInfo(toAgencyUserID)

	
	if toAgency == nil or toAgency.userid == 0 then
		return
	end

	
	local myAgent = st_agent_pb.agentinfo()
	local grade = wiringID > 0 and 0 or -1
	myAgent.qrcodeurl = AgentModel.GetQrcodeUrl(pInfo.userid)
	myAgent.invitestr = tostring(pInfo.userid)
	myAgent.userid = pInfo.userid
	myAgent.level = 91
	myAgent.wiringid = wiringID
	myAgent.grade = grade	
	
	
	
	
	
	if wiringID == 0 then
		--如果排线ID为0，那么就说明这个兄弟扫的是直属玩家
		
		myAgent.soleagent = toAgency.level > 98 and toAgency.userid or toAgency.soleagent
		myAgent.agent1 = toAgency.userid
		myAgent.agent2 = toAgency.agent1
		myAgent.agent3 = toAgency.agent2
		myAgent.agent4 = toAgency.agent3
		myAgent.wiringid = toAgency.wiringid

	end	

	
	AgentModel.InsertAgent(pInfo, myAgent)
	
	local isPaixian = 0
	if wiringID ~= 0 then
		--如果排线ID不为0，那么这里留需要找到他的上级了。
		local sqlCase = "select count(userid) from ag_player where wiringid="..wiringID.." and userid<"..pInfo.userid.." and grade >= 0" 
		mysqlItem:executeQuery(sqlCase)
		grade = mysqlItem:fetch()
		
		sqlCase = "select userid from ag_player where wiringid="..wiringID.." and userid<"..pInfo.userid.." and grade >=0 order by userid desc limit 1"
		mysqlItem:executeQuery(sqlCase)
		
		local agent1 = mysqlItem:fetch()
		
		grade = grade ==  nil and  1 or (tonumber(grade) + 1)
		agent1 = agent1 == nil and sqlData[2] or agent1

		sqlCase = "update ag_player set grade="..grade..",agent1="..agent1.." where userid="..pInfo.userid
	
		mysqlItem:execute(sqlCase)
		myAgent.grade = grade
		myAgent.agent1 = tonumber(agent1)
		UnlimitedModel.AddUserIDToWiring(wiringID, pInfo.userid, grade)
		
		isPaixian = 1
		
	end
	
	UnlimitedModel.SetUserPreAgentID(pInfo.userid, myAgent.agent1)  --设置上级的ID
	
	local sendData = {}
	sendData['userid'] = pInfo.userid
	sendData['wiringid'] = myAgent.wiringid
	sendData['paixian'] = isPaixian
	processWork("UnlimitedIncrUserNum", luajson.encode(sendData))
	
	AgentModel.SetAgent(pInfo.userid, myAgent)
	

end


function AgentModel.isSaoMa(unionID)

	if g_gamename ~= "uudwc" then
		return true
	end
 	
	if unionID == nil or unionID == '' then
		return false
	end

	local sqlCase = "select id,binduserid,wiringid from ag_author where unionid='"..unionID.."' order by id desc limit 1"
	
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})


	if sqlData == nil then
		return false
	end	
	return true
end


function AgentModel.ReDoAgent()
	
	local sqlCase = "select userid from ag_player where agent1=0 and userid>0 and level < 99 order by userid"
	local sqlData = ""
	mysqlItem:executeQuery(sqlCase)
	local userIDList = {}
	while true do
		sqlData = mysqlItem:fetch()
		if sqlData == nil then
			break
		end
		table.insert(userIDList, tonumber(sqlData))
	end
	
	for k,v_userid in ipairs(userIDList) do
		
		local myAgent = AgentModel.GetAgentInfo(v_userid)
		
		
		sqlCase = "select platformid from dy_player where userid="..v_userid 
		
		mysqlItem:executeQuery(sqlCase)
		
		sqlData = mysqlItem:fetch()
		
		if sqlData ~= nil and # sqlData > 0 then
			
			sqlCase = "select id,binduserid,wiringid from ag_author where unionid='"..sqlData.."' order by id desc limit 1"
			
			mysqlItem:executeQuery(sqlCase)
			
			
			sqlData = mysqlItem:fetch({})
			
			if sqlData ~= nil then
			
				local wiringID = sqlData[3] == nil and 0 or tonumber(sqlData[3])
				local bindUserID = sqlData[2] == nil and 0 or tonumber(sqlData[2])
				
				
				
				
				if wiringID == 0 then
					--这里绑定的是直属码
					if bindUserID ~= 0 then
						local toAgency = AgentModel.GetAgentInfo(bindUserID)
						
						if toAgency ~= nil and toAgency.agent1 ~= 0 then
							
							myAgent.agent1 = toAgency.userid
							myAgent.wiringid = toAgency.wiringid
							sqlCase = "update ag_player set agent1="..toAgency.agent1..",wiringid="..myAgent.wiringid.." where userid="..v_userid
							mysqlItem:execute(sqlCase)
							
							UnlimitedModel.SetUserPreAgentID(v_userid, myAgent.agent1)
							--不是排线。直属玩家
							local sendData = {}
							sendData['userid'] = v_userid
							sendData['wiringid'] = myAgent.wiringid
							sendData['paixian'] = 0
							processWork("UnlimitedIncrUserNum", luajson.encode(sendData))


							AgentModel.SetAgent(v_userid, myAgent)
							
						end
						
					end
				else
					--如果是排线
					local sqlCase = "select count(userid) from ag_player where wiringid="..wiringID.." and grade >= 0" 
					mysqlItem:executeQuery(sqlCase)
					grade = mysqlItem:fetch()
					
					sqlCase = "select userid from ag_player where wiringid="..wiringID.." order by grade desc limit 1"
					mysqlItem:executeQuery(sqlCase)
					
					local agent1 = mysqlItem:fetch()
					
					grade = grade ==  nil and  1 or (tonumber(grade) + 1)
					agent1 = agent1 == nil and sqlData[2] or agent1

					sqlCase = "update ag_player set grade="..grade..",agent1="..agent1..",wiringid="..wiringID.." where userid="..v_userid
				
					mysqlItem:execute(sqlCase)
					myAgent.grade = grade
					myAgent.agent1 = tonumber(agent1)
					UnlimitedModel.AddUserIDToWiring(wiringID, v_userid, grade)
					
					isPaixian = 1

					UnlimitedModel.SetUserPreAgentID(v_userid, myAgent.agent1)  --设置上级的ID
					
					local sendData = {}
					sendData['userid'] = v_userid
					sendData['wiringid'] = myAgent.wiringid
					sendData['paixian'] = 1
					processWork("UnlimitedIncrUserNum", luajson.encode(sendData))					
					AgentModel.SetAgent(v_userid, myAgent)

				end

			end
			
		end
		
		
	end
	
	
end

function AgentModel.IsMyAgent(userID, agent)
	local function  GetMyAgents(userID, agentList)
		local preAgentID = UnlimitedModel.GetUserPreAgentID(userID)

		if preAgentID == 0 then
			return
		end

		if agentList[tostring(preAgentID)] ~= nil then
			return
		end
		
		agentList[tostring(preAgentID)] = 0
		table.insert(agentList, preAgentID)

		GetMyAgents(preAgentID, agentList)
	end

	local userIDList = {}
	GetMyAgents(userID, userIDList)
	if userIDList[agent] == nil then
		return true
	end

	return false
end





