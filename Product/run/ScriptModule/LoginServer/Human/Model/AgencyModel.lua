
AgencyModel = {}
AgencyModel.redis_index = "redis_agency"

AgencyModel.agency_info = "agency_"
AgencyModel.agency_commission = "agency_commission"

AgencyModel.agency_daymark = "agency_daymark"     --用于标记抽佣分成的时间，每天只统计一遍。

AgencyModel.agency_ratio = "agency_ratio"

function AgencyModel.GetAgencyInfo(userID, agyInfo)
	if userID == nil or userID == 0 then
		return nil
	end
	--获取代理的信息
	if agyInfo == nil then
		agyInfo = st_agent_pb.agentinfo()
	end
	local strGet = redisItem:get(AgencyModel.agency_info..userID, AgencyModel.redis_index)
	
	if strGet ~= nil then
		agyInfo:ParseFromString(strGet)
		return agyInfo
	else
		return AgencyModel.LoadAgencyInfo(userID,agyInfo)
	end
end

function AgencyModel.SetAgency(userID, agyInfo)
	redisItem:set(AgencyModel.agency_info..userID, agyInfo:SerializeToString(), AgencyModel.redis_index)
end

function AgencyModel.LoadAgencyInfo(userID,agyInfo)
	
	
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
		agyInfo.qrcodeurl = sqlData[24]
		agyInfo.datemark = sqlData[25]
		agyInfo.moneymark = tonumber(sqlData[26])
		agyInfo.weekmoney = tonumber(sqlData[18])
		agyInfo.alipay = sqlData[33]
		agyInfo.bankaccount = sqlData[27]
		agyInfo.exclusiveurl = AgencyModel.Exclusiveurl(userID)

		agyInfo.userid  = userID
		
		
		--return agyInfo
	else
		local pInfo = PlayerModel.GetPlayerInfo(userID)
		agyInfo.invitestr = tostring(userID)
		agyInfo.level = 91
		agyInfo.qrcodeurl = AgencyModel.GetQrcodeUrl(userID)
		agyInfo.exclusiveurl = AgencyModel.Exclusiveurl(userID)
		agyInfo.soleagent = 0
		agyInfo.agent1 = 0
		agyInfo.agent2 = 0
		agyInfo.agent3 = 0
		agyInfo.agent4 = 0

		AgencyModel.InsertAgency(pInfo, agyInfo)
	end
	redisItem:set(AgencyModel.agency_info..userID, agyInfo:SerializeToString(), AgencyModel.redis_index)
	return agyInfo
end

--付费的ID，付费的金额

function AgencyModel.InsertAgency(pInfo, myAgency)
	
	local sqlCase = "insert into ag_player(userid,nickname,account,expandcode,state,level,soleagent,agent1,agent2,agent3,agent4,qrcodeurl) values("
		..pInfo.userid..",'"..pInfo.nickname.."','"..pInfo.userid.."','"..pInfo.userid.."',0,"..myAgency.level..","..myAgency.soleagent..","
		..myAgency.agent1..","..myAgency.agent2..","..myAgency.agent3..","..myAgency.agent4..",'"..myAgency.qrcodeurl.."')" 	
	mysqlItem:execute(sqlCase)
end

function AgencyModel.GetQrcodeUrl(userID)
	--根据不同的服务器，配置不同的二维吗分享
	--这里需要调用对应的接口，生成对应的二维码

	--processWork("MakeCode",userID)
	--local sendUrl = g_webUrl.."/index.php/client/makecode?bindcode="..userID.."&platfromid="..g_gamename
	--HttpGet(sendUrl)   --向网页端申请生成二维码。只生成一次。一定要注意这里的	
	
	return g_webUrl.."/Uploads/"..g_gamename.."/agency/"..userID..".jpg"
end


function AgencyModel.Exclusiveurl(userID)
	--根据不同的服务器，配置不同的二维吗分享
	
	return g_webUrl.."/Public/share/"..g_gamename..".html?bindcode="..userID.."&platfromid="..g_gamename
end




function AgencyModel.CommissionCount(userid, Count, gameType, tableType, pourJetton)
	
	if Count <= 0 then
		return Count
	end
	
	local pourJetton = pourJetton == nil and 0 or pourJetton 
	local Count = Count == nil and 0 or Count 
	local tmpCount = Count
	if g_brokerageType.mode == 1 then
		tmpCount = Count - pourJetton
	end
		
	local Ratio = AgencyModel.getGameCommissionRatio(gameType, tableType)
	local commission = tmpCount * Ratio
	commission = math.ceil(commission) 
	local realCount = Count - commission
	if RobotService.IsRobot(userid) == false and userid ~= 1 and commission > 0 then
		LogServer.PlayerFee(userid, commission)
		
		local sqlCase = "update dy_gamelist set dayfee=dayfee+"..commission..", weekfee=weekfee+"..commission..", monthfee=monthfee+"..commission..",allfee=allfee+"..commission..
			" where gametype="..gameType.." and tabletype="..tableType
		mysqlItem:executeQuery(sqlCase)
	end
	return realCount

end

function AgencyModel.getGameCommissionRatio(gameType, tableType)
	--return 0.02
	local ratio = redisItem:hget(AgencyModel.agency_ratio, gameType..tableType, AgencyModel.redis_index)
	return ratio == nil and AgencyModel.LoadGameCommissionRatio( gameType, tableType ) or tonumber(ratio)
end


function AgencyModel.LoadGameCommissionRatio(gameType, tableType)
	local sqlCase = "select feerate from dy_gamelist where gametype="..gameType.." and tabletype="..tableType
	mysqlItem:executeQuery(sqlCase)

	local ratio = mysqlItem:fetch()
	
	ratio = ratio == nil and 5 or tonumber(ratio)
	ratio = ratio/100
	redisItem:hset(AgencyModel.agency_ratio, gameType..tableType, ratio, AgencyModel.redis_index)
	return ratio
end


function AgencyModel.StatisticsCommission(userid, commission)
	redisItem:hincrby(AgencyModel.agency_commission, userid, commission,AgencyModel.redis_index)
end

function AgencyModel.GetUserCommission()
	return redisItem:hgetall(AgencyModel.agency_commission, AgencyModel.redis_index)
end


function AgencyModel.SetAgencyDaymark(dateSec)
	redisItem:set( AgencyModel.agency_daymark, dateSec, AgencyModel.redis_index)
end

function AgencyModel.GetAgencyDaymark()
	redisItem:get( AgencyModel.agency_daymark, AgencyModel.redis_index )
end