
AgencyModel = {}
AgencyModel.redis_index = "redis_agency"

AgencyModel.agency_info = "agency_"
AgencyModel.agency_gmr = "agency_gmr_"		-- 赠送钻石列表


function AgencyModel.GetAgencyInfo(userID, agyInfo)
	if userID == nil or userID == 0 then
		return nil
	end
	--获取代理的信息
	if agyInfo == nil then
		agyInfo = st_human_pb.agencyinfo()
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
		agyInfo = st_human_pb.agencyinfo()
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

		sqlCase = "select * from dy_invite where touserid="..userID   --全部绑定我的邀请码的玩家
		
		mysqlItem:executeQuery(sqlCase)
		local countnum = 0
		while true do
			sqlData = mysqlItem:fetch({})
			if sqlData == nil then
				break
			end
			local addItem = agyInfo.invitelist:add()
			addItem.userid = userID
			addItem.binduserid = tonumber(sqlData[1])
			addItem.binddate = sqlData[4]
			countnum = countnum + 1
		end

		for i = 1, countnum do
			local pInfo = PlayerModel.GetPlayerInfo(agyInfo.invitelist[i].binduserid)
			agyInfo.invitelist[i].bindnickname = pInfo.nickname
			agyInfo.invitelist[i].bindfaceid = pInfo.face_1
		end
		agyInfo.userid  = userID
		
		-- 加载赠送钻石列表
		-- AgencyModel.loadGiveMoneyRecordList(agyInfo)
		
		redisItem:set(AgencyModel.agency_info..userID, agyInfo:SerializeToString(), AgencyModel.redis_index)
		return agyInfo
	end
	return nil
end

function AgencyModel.AddGiveMoneyRecord(userID, record)
	redisItem:lpush(AgencyModel.agency_gmr..userID, record:SerializeToString(), AgencyModel.redis_index)
end

-- 每次获取10条
function AgencyModel.GetGiveMoneyRecordList(userID, pageNum)
	local rediskey = AgencyModel.agency_gmr..userID
	if not redisItem:exists(rediskey, AgencyModel.redis_index) then
		AgencyModel.loadGiveMoneyRecordList(userID)
	end
	return redisItem:lrange(rediskey, (pageNum - 1)*10, pageNum*10 - 1, AgencyModel.redis_index)
end

--[[
	（从数据库中）加载代理赠送钻石的列表
	@param agyInfo: st_human_pb.agencyinfo	代理
	@returns void
--]]
function AgencyModel.loadGiveMoneyRecordList(userID)
	
	sqlCase = "select * from log_currencychangedrecord where opt_type=0 AND userid="..userID
	mysqlItem:executeQuery(sqlCase)
	while true do 
		sqlData = mysqlItem:fetch({})
		if nil == sqlData then
			break
		end
		
		local record = st_human_pb.givemoneyrecord()
		local remark = sqlData[8]
		record.userid = tonumber(string.sub(remark, remark:find(":") + 1))
		record.amount = - tonumber(sqlData[5])
		record.timemark = sqlData[6]

		local pInfo = PlayerModel.GetPlayerInfo(userID)
		if nil ~= pInfo then
			record.nickname = pInfo.nickname or ''
		end
		
		-- 写入缓存
		AgencyModel.AddGiveMoneyRecord(userID, record)
	end
end

--付费的ID，付费的金额
function AgencyModel.AgencyDivide(userID, payMoney, nickName)
	
	local agyInfo = AgencyModel.GetAgencyInfo(userID)
	if agyInfo == nil then
		return nil
	end
	--开始分配了,分配完毕
	
	--说明这个是玩家,然后看他上一级代理的级别，如果上一级是二级代理，二级拿30%，上级的上级拿10%，上上级拿5%，上上上级拿5%
	local agyInfo1 = AgencyModel.GetAgencyInfo(agyInfo.agent1)
	if agyInfo1 ~= nil then
		if agyInfo1.level == 2 then
			--他是二级代理,上级拿30%，上上级的上级拿10%，上上上级拿5%，上上上上级拿5%
			local sqlCase = ""
			if agyInfo.agent1 ~= 0 then
				local getNum = payMoney*0.3
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent1
				mysqlItem:execute(sqlCase)
				
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent1..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)
			end
			if agyInfo.agent2 ~= 0 then
				local getNum = payMoney*0.1
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent2
				mysqlItem:execute(sqlCase)
				
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent2..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)				
			end
			if agyInfo.agent3 ~= 0 then
				local getNum = payMoney*0.05
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent3
				mysqlItem:execute(sqlCase)

				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent3..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)				
			end
			if agyInfo.agent4 ~= 0 then
				local getNum = payMoney*0.05
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent4
				mysqlItem:execute(sqlCase)
				
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent4..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)				
			end
		else   --如果不是二级，就是1级的了
			local sqlCase = ""
			if agyInfo.agent1 ~= 0 then
				local getNum = payMoney*0.4
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent1
				mysqlItem:execute(sqlCase)
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent1..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)					
			end
			if agyInfo.agent2 ~= 0 then
				local getNum = payMoney*0.05
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent2
				mysqlItem:execute(sqlCase)

				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent2..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)					
			end
			if agyInfo.agent3 ~= 0 then
				local getNum = payMoney*0.05
				sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent3
				mysqlItem:execute(sqlCase)

				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent3..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
				mysqlItem:execute(sqlCase)			
			end
		end
	end	

	if agyInfo.soleagent ~= 0 then
		--这里是分给总代的
		local getNum = payMoney*0.15
		local sqlCase = "insert into ag_paysoleagent(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.soleagent..","..userID..",'"..nickName.."',99,"..payMoney..","..getNum..")"
		mysqlItem:execute(sqlCase)		
	end
	
end

function AgencyModel.InsertAgency(pInfo, myAgency)
	
	local sqlCase = "insert into ag_player(userid,account,expandcode,state,level,soleagent,agent1,agent2,agent3,agent4,qrcodeurl) values("
		..pInfo.userid..",'"..pInfo.userid.."','"..pInfo.userid.."',0,"..myAgency.level..","..myAgency.soleagent..","
		..myAgency.agent1..","..myAgency.agent2..","..myAgency.agent3..","..myAgency.agent4..",'"..myAgency.qrcodeurl.."')" 	
	mysqlItem:execute(sqlCase)
	sqlCase = "update ag_player set nickname='"..pInfo.nickname.."' where userid="..pInfo.userid
	mysqlItem:execute(sqlCase)
end

function AgencyModel.GetQrcodeUrl(userID)
	--根据不同的服务器，配置不同的二维吗分享
	if g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		return "http://www.yoyo168.cn/Uploads/hlyd/agency/"..userID..".jpg"
	elseif g_servername == "run_hubmj" then
		return "http://www.itcode.me/Uploads/xymj/agency/"..userID..".jpg"
	elseif g_servername == "run_gdmj" then
		return "http://www.tianyuwangluo.net//Uploads/mmmj/agency/"..userID..".jpg"
	end
end


--付费的ID，付费的金额
function AgencyModel.XymjDivide(userID, payMoney, nickName)
	
	local agyInfo = AgencyModel.GetAgencyInfo(userID)
	if agyInfo == nil then
		return nil
	end
	--开始分配了,分配完毕
	
	--说明这个是玩家,然后看他上一级代理的级别，如果上一级是二级代理，二级拿30%，上级的上级拿10%，上上级拿5%，上上上级拿5%
	local agyInfo1 = AgencyModel.GetAgencyInfo(agyInfo.agent1)
	if agyInfo1 ~= nil then

		local sqlCase = ""
		if agyInfo.agent1 ~= 0 then
			local getNum = payMoney*0.3
			sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent1
			mysqlItem:execute(sqlCase)
			sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent1..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
			mysqlItem:execute(sqlCase)					
		end
		if agyInfo.agent2 ~= 0 then
			local getNum = payMoney*0.15
			sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent2
			mysqlItem:execute(sqlCase)
			sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent2..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
			mysqlItem:execute(sqlCase)					
		end
		if agyInfo.agent3 ~= 0 then
			local getNum = payMoney*0.05
			sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum.." where userid="..agyInfo.agent3
			mysqlItem:execute(sqlCase)

			sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.agent3..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
			mysqlItem:execute(sqlCase)			
		end
	end	

	if agyInfo.soleagent ~= 0 then
		--这里是分给总代的
		local getNum = payMoney*0.1
		local sqlCase = "insert into ag_paysoleagent(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.soleagent..","..userID..",'"..nickName.."',99,"..payMoney..","..getNum..")"
		mysqlItem:execute(sqlCase)		
	end
	
end

function AgencyModel.XymjWeekCount(agency, addMoney)
	
	if g_servername == "run_hubmj" then
		if agency.moneymark >= 688 or agency.level ~= 4 then
			return nil
		end
	elseif g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		if agency.moneymark >= 1500 then
			return nil
		end
	end
	
	--看看时间周期
	local marktm = TimeUtils.GetTableTime(agency.datemark)
	--判断是不是一周
	local currTm = TimeUtils.GetTableTime()
	
	local isNew = false
	if currTm.wday < marktm.wday then
		--已经是新的一周了
		--agency.moneymark = addMoney
		isNew = true
	else
		local diffDay = TimeUtils.DifferentDay(marktm, currTm)
		if diffDay > 7 then
			isNew = true
		end
	end
	
	if isNew == true then
		agency.moneymark = tonumber(addMoney)
		agency.datemark = TimeUtils.GetTimeString()
	else
		agency.moneymark = agency.moneymark + tonumber(addMoney)
	end
end


function AgencyModel.ZsmjDivideDetail(agyInfo,payMoney,getNumList,grade,userID,nickName)

	--传入代理的等级，以及分
	--返回是否结束，以及下级总代理提成的奖金
	if grade > #getNumList then
		return nil
	end

	if grade > 1 and agyInfo.level <92 then
		--二级以上的分成，只有一级以上的才能拿到奖励
		return nil
	end		
	
	local getAll = 0
	local moneymark = 0
	local isSole = false
	if agyInfo.level >= 94 then
		--如果总代以上的
		
		for i = grade,#getNumList do
			getAll = getAll + getNumList[i]
		end
		isSole = true
	else
		getAll = getNumList[grade]
	end
	
	if grade == 1 then
		moneymark = payMoney
	else
		moneymark = getAll
	end
		
	AgencyModel.ZsmjCount(agyInfo, moneymark)
	
	AgencyModel.SetAgency(agyInfo.userid,agyInfo)
	
	local sqlCase = ""
	sqlCase = "update ag_player set allmoney=allmoney+"..getAll..",takemoney=takemoney+"..getAll..",moneymark="..agyInfo.moneymark..",weekmoney="..agyInfo.weekmoney..",level="..agyInfo.level.." where userid="..agyInfo.userid
	mysqlItem:execute(sqlCase)
	
	sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.userid..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getAll..")"
	mysqlItem:execute(sqlCase)
	return isSole,moneymark
	
end
function AgencyModel.ZsmjZongDaiTiCheng(agyInfo,tiChengNum,payMoney,userID,nickName)
	if agyInfo == nil then
		return nil
	end
	tiChengNum = tiChengNum*0.03
	local sqlCase = ""
	sqlCase = "update ag_player set allmoney=allmoney+"..tiChengNum..",takemoney=takemoney+"..tiChengNum.." where userid="..agyInfo.userid
	mysqlItem:execute(sqlCase)
	
	sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.userid..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..tiChengNum..")"
	mysqlItem:execute(sqlCase)	
end
--付费的ID，付费的金额
function AgencyModel.ZsmjDivide(userID, payMoney, nickName)
	--掌上麻将，也叫小蛙麻将,的分配系统
	--一开始叫临时代理，下面的玩家充值满180以后，就会升为代理
	--临时代理升为二级代理，二级代理享受直属玩家充值的3成分成
	--二级代理业绩做够1500后，就会升为一级代理。
	--一级代理享有直属玩家40%的分成，享有下面二级代理10%的分成，享有一级代理下面玩家5%的分成。享受一级代理下面二级代理5%的分成
	--临时玩家 level = 90
	--二级代理 level = 91
	--一级代理 level = 92
	--总代理   level = 94
	--区域总代 level = 99
	
	local agyInfo = AgencyModel.GetAgencyInfo(userID)
	if agyInfo == nil then
		return nil
	end
	
	--开始分配了,分配完毕
	
	--说明这个是玩家,然后看他上一级代理的级别，如果上一级是二级代理，二级拿30%，上级的上级拿10%，上上级拿5%，上上上级拿5%
	local agyInfo1 = AgencyModel.GetAgencyInfo(agyInfo.agent1)
	local agyList = {}
	if agyInfo1 ~= nil then
	--agyList[1] = agyInfo1
		table.insert(agyList, agyInfo1)
	end

	
	local agyInfo2 = AgencyModel.GetAgencyInfo(agyInfo.agent2)
	if agyInfo2 ~= nil then
		table.insert(agyList, agyInfo2)
	end
	
	local agyInfo3 = AgencyModel.GetAgencyInfo(agyInfo.agent3)
	if agyInfo3 ~= nil then
		table.insert(agyList, agyInfo3)
	end

	local agyInfo4 = AgencyModel.GetAgencyInfo(agyInfo.agent4)
	if agyInfo4 ~= nil then
		table.insert(agyList, agyInfo3)
	end

	local getList = {}
	if agyInfo1 ~= nil then
		if agyInfo1.level == 91 then
			getList[1] = payMoney*0.3
			getList[2] = payMoney*0.1
			getList[3] = payMoney*0.05
			getList[4] = payMoney*0.05
		else
			getList[1] = payMoney*0.4
			getList[2] = payMoney*0.05
			getList[3] = payMoney*0.05
		end
		
		for i = 1,#agyList do
			local isEnd,numTemp = AgencyModel.ZsmjDivideDetail(agyList[i], payMoney, getList,i,userID,nickName)
			if isEnd == true then
				--看看下一个是不是总代，如果是总代，就拿到总代的提成
				if agyList[i+1] ~= nil and agyList[i+1].level >=94 then
					AgencyModel.ZsmjZongDaiTiCheng(agyList[i+1], numTemp,payMoney, userID, nickName)
				end
				break
			end
		end
				
	end

	if agyInfo.soleagent ~= 0 then
		--这里是分给总代的
		local getNum = payMoney*0.1
		local sqlCase = "insert into ag_paysoleagent(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.soleagent..","..userID..",'"..nickName.."',99,"..payMoney..","..getNum..")"
		mysqlItem:execute(sqlCase)
	end
	
end

function AgencyModel.ZsmjCount(agency, addMoney)
	
	
	--看看时间周期
	local marktm = TimeUtils.GetTableTime(agency.datemark)
	--判断是不是一周
	local currTm = TimeUtils.GetTableTime()
	
	local isNew = false
	if currTm.wday < marktm.wday then
		--已经是新的一周了
		--agency.moneymark = addMoney
		isNew = true
	else
		local diffDay = TimeUtils.DifferentDay(marktm, currTm)
		if diffDay > 7 then
			isNew = true
		end
	end
	
	if isNew == true then
		agency.weekmoney = tonumber(addMoney)
		agency.datemark = TimeUtils.GetTimeString()
		sqlCase = "update ag_player set datemark='"..agency.datemark.."' where userid="..agency.userid   --这里需要更新一个参数
		mysqlItem:execute(sqlCase)
	else
		agency.weekmoney = agency.weekmoney + tonumber(addMoney)
	end
	
	if agency.level == 91 and agency.weekmoney >= 1500 then
		agency.level = 92
	end
	
	agency.moneymark = agency.moneymark + addMoney
	
end


function AgencyModel.MmmjDivide(userID, payMoney, nickName)

	--掌上麻将，也叫小蛙麻将,的分配系统
	--一开始叫临时代理，下面的玩家充值满180以后，就会升为代理
	--临时代理升为二级代理，二级代理享受直属玩家充值的3成分成
	--二级代理业绩做够1500后，就会升为一级代理。
	--一级代理享有直属玩家40%的分成，享有下面二级代理10%的分成，享有一级代理下面玩家5%的分成。享受一级代理下面二级代理5%的分成
	--临时玩家 level = 90
	--推广员:91
	--二级:92
	--一级:93
	--总代:94
	--总代理   level = 94
	--区域总代 level = 99
	
	local agyInfo = AgencyModel.GetAgencyInfo(userID)
	if agyInfo == nil or agyInfo.level == 99 then
		--如果是总代自己充的。那就全部到后台
		return nil
	end
	
	--开始分配了,分配完毕
	
	--说明这个是玩家,然后看他上一级代理的级别，如果上一级是二级代理，二级拿30%，上级的上级拿10%，上上级拿5%，上上上级拿5%
	local agyInfo1 = AgencyModel.GetAgencyInfo(agyInfo.agent1)
	local agyList = {}
	if agyInfo1 ~= nil then

		if agyInfo1.level == 100 then
			--特级总代，拿7成
			AgencyModel.MmmjDivideDetail(agyInfo1, payMoney, payMoney*0.7,userID,nickName)
		elseif agyInfo1.level == 99 then
			--
			--如果上级代理是总代。就拿6成
			AgencyModel.MmmjDivideDetail(agyInfo1, payMoney, payMoney*0.6,userID,nickName)
			
			local tjAgency = AgencyModel.GetAgencyInfo(agyInfo1.agent1)
			if tjAgency ~= nil and tjAgency.level == 100 then
				--这位兄弟也能拿10%
				AgencyModel.MmmjDivideDetail(tjAgency, payMoney, payMoney*0.1,userID,nickName)
			end
			
		else
			local agyInfo2 = AgencyModel.GetAgencyInfo(agyInfo.agent2)
			
			if agyInfo2 ~= nil then
			
				if agyInfo2.level == 100 then
					--金牌代理下面的代理
					AgencyModel.MmmjDivideDetail(agyInfo1, payMoney, payMoney*0.5,userID,nickName)
					AgencyModel.MmmjDivideDetail(agyInfo2, payMoney, payMoney*0.2,userID,nickName)
				elseif agyInfo2.level == 99 then
					AgencyModel.MmmjDivideDetail(agyInfo1, payMoney, payMoney*0.5,userID,nickName)
					AgencyModel.MmmjDivideDetail(agyInfo2, payMoney, payMoney*0.1,userID,nickName)	
					local tjAgency = AgencyModel.GetAgencyInfo(agyInfo2.agent1)
					if tjAgency ~= nil and tjAgency.level == 100 then
						--这位兄弟也能拿10%
						AgencyModel.MmmjDivideDetail(tjAgency, payMoney, payMoney*0.1,userID,nickName)
					end
				else
					AgencyModel.MmmjDivideDetail(agyInfo1, payMoney, payMoney*0.35,userID,nickName)
					AgencyModel.MmmjDivideDetail(agyInfo2, payMoney, payMoney*0.15,userID,nickName)
					
					local soleagent = AgencyModel.GetAgencyInfo(agyInfo.soleagent)
					--剩下的就是给区域总代理的包干了
					if soleagent ~= nil then
						--总代上面有特级代理
						if soleagent.level == 100 then
							AgencyModel.MmmjDivideDetail(soleagent, payMoney, payMoney*0.2,userID,nickName)
						else
							AgencyModel.MmmjDivideDetail(soleagent, payMoney, payMoney*0.1,userID,nickName)
							
							local tjAgency = AgencyModel.GetAgencyInfo(soleagent.agent1)
							if tjAgency ~= nil and tjAgency.level == 100 then
								--这位兄弟也能拿10%
								AgencyModel.MmmjDivideDetail(tjAgency, payMoney, payMoney*0.1,userID,nickName)
							end							
						end
					
					end				
				end
			else
				local soleagent = AgencyModel.GetAgencyInfo(agyInfo.soleagent)
				--剩下的就是给区域总代理的包干了
				if soleagent ~= nil then
					--总代上面有特级代理
					if soleagent.level == 100 then
						AgencyModel.MmmjDivideDetail(soleagent, payMoney, payMoney*0.2,userID,nickName)
					else
						AgencyModel.MmmjDivideDetail(soleagent, payMoney, payMoney*0.1,userID,nickName)
						
						local tjAgency = AgencyModel.GetAgencyInfo(soleagent.agent1)
						if tjAgency ~= nil and tjAgency.level == 100 then
							--这位兄弟也能拿10%
							AgencyModel.MmmjDivideDetail(tjAgency, payMoney, payMoney*0.1,userID,nickName)
						end							
					end
				
				end					
			end
		end
	end
end

function AgencyModel.MmmjDivideDetail(agyInfo,payMoney,getNum,userID,nickName)

	if agyInfo == nil then
		return nil
	end
	
	agyInfo.moneymark = agyInfo.moneymark + getNum
	
	local sqlCase = ""
	sqlCase = "update ag_player set allmoney=allmoney+"..getNum..",takemoney=takemoney+"..getNum..",moneymark="..agyInfo.moneymark.." where userid="..agyInfo.userid
	mysqlItem:execute(sqlCase)
	
	sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..agyInfo.userid..","..userID..",'"..nickName.."',"..agyInfo.level..","..payMoney..","..getNum..")"
	mysqlItem:execute(sqlCase)
	return isSole,moneymark
end

--[[
	赠送钻石
	@param pAgyInfo: msg_human_pb.cggetplayerinfo	代理玩家
	@param pInfo: msg_human_pb.cggetplayerinfo		目标玩家
	@param amount: number							赠送钻石数量
	@returns void
--]]
function AgencyModel.GiveMoney(pAgyInfo, pInfo, amount)
	-- 扣除代理money
	local initial = pAgyInfo.money
	PlayerModel.DecMoneyWithoutSysCount(pAgyInfo, amount, "agency", "代理向玩家赠送")
	PlayerModel.SetPlayerInfo(pAgyInfo)

	local remark = "give money to:"..pInfo.userid
	PlayerModel.InsertCurrencyChangedRecord(pAgyInfo.userid, initial, - amount, 0, 0, remark)
	
	-- 增加玩家money
	initial = pInfo.money
	PlayerModel.AddMoneyWithoutSysCount(pInfo, amount, "agency", "玩家收到代理赠送")
	PlayerModel.SetPlayerInfo(pInfo)

	remark = "give money from:"..pAgyInfo.userid
	PlayerModel.InsertCurrencyChangedRecord(pInfo.userid, initial, amount, 0, 1, remark)

	-- 更新缓存
	--local agency = AgencyModel.GetAgencyInfo(pAgyInfo.userid)
	--local item = agency.givemoneylist:add()
	local item = st_human_pb.givemoneyrecord()
	item.userid = pInfo.userid
	item.nickname = pInfo.nickname
	item.amount = amount
	item.timemark = TimeUtils.GetTimeString()
	--AgencyModel.SetAgency(agency.userid, agency)
	LogServer.AddJetton("agency", amount)
	AgencyModel.AddGiveMoneyRecord(pAgyInfo.userid, item)
end
