--这个是设置斗牛历史记录的地方
--这里不用这个
DouNiuHistoryModel = {}
DouNiuHistoryModel.redis_index = "douniu_hty"
DouNiuHistory1 = {}
DouNiuHistory1.redis_daycount = "douniu_daycount"


function DouNiuHistoryModel.GetDayCount()
	return redisItem:lpush(DouNiuHistory1.redis_daycount,1, DouNiuHistoryModel.redis_index)
end

function DouNiuHistoryModel.SetHistory(tInfo, isDissolve, DNcount)
	--每次在麻将结算的时候调用该函数
	local redisKey = "douniuhtyinfo_"..math.floor(tInfo.frameid/100)
	--print("redisKey = "..redisKey)
	local dnHistory = st_douniu_pb.douniuhistory()
	
	if true == redisItem:exists(redisKey, DouNiuHistoryModel.redis_index) then
		local strGet = redisItem:get(redisKey, DouNiuHistoryModel.redis_index)
		dnHistory:ParseFromString(strGet)
	end
	
	if #dnHistory.hitemlist == 0 then
		--这是第一局		
		dnHistory.tableid = tInfo.tableid
		
		dnHistory.tablemark = math.floor(tInfo.frameid/100)   --等于第一场的ID
		dnHistory.gametime = TimeUtils.GetTime()
		dnHistory.dntypelist = tInfo.dntype
		dnHistory.playnummax = tInfo.maxplaynum
		dnHistory.difen = tInfo.difen
		dnHistory.ownerid = tInfo.ownerid
	end
		
	if isDissolve == 0 then
		local addItem = dnHistory.hitemlist:add()
		addItem.leg = tInfo.usevipnum
		addItem.bankerID = tInfo.bankerID
		if tInfo.dntype == 	g_douNiuType.type_tbnn then
			addItem.bankerID = 0
		end

		for k,v in ipairs(tInfo.situser) do
			if v.state == g_douniuPlayStatus.status_play then
				local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
				addItem.useridlist:append(v.userid)
				addItem.nicklist:append(tempInfo.nickname)
				if DNcount[k] ~= nil then
					addItem.winlist:append(DNcount[k])
				end
				addItem.nntypelist:append(v.nntype)
				if v.basicsmul == 0 then
					v.basicsmul = 1
				end
				addItem.qzbslist:append(v.basicsmul)
				if tInfo.dntype == g_douNiuType.type_tbnn then
					v.jetton = tInfo.difen 
				end
				addItem.pournumlist:append(v.jetton)
				local poker = addItem.pokerlist:add()
				for k,t in ipairs(v.handpoker) do
					poker.handpoker:append(t)
				end
				addItem.isBuyJetton = 0
				if tInfo.playerbuycode == 1 then
					addItem.isBuyJetton = 1
					local addBuy = addItem.buylist:add()
					addBuy.userid = v.buyjetton.userid
					addBuy.pour = v.buyjetton.pour
				end
			end
		end
	end
	if tInfo.usevipnum == tInfo.maxplaynum or isDissolve == 1  then
		for k,v in ipairs(tInfo.situser) do
			local tempInfo = PlayerModel.GetPlayerInfo(v.userid)
			local isEX = false 
			local pos = 0
			for k = 1, #dnHistory.useridlist do
				if v.userid == dnHistory.useridlist[k] then
					pos = k
					isEX = true
				end
			end
			if isEX == false then
				dnHistory.useridlist:append(v.userid)
				dnHistory.nicklist:append(tempInfo.nickname)
				dnHistory.winlist:append(v.countjetton)
			else
				dnHistory.winlist[pos] = dnHistory.winlist[pos] + v.countjetton
			end
			
		end
		
		if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
			for k,v in ipairs(tInfo.situser) do
				dnHistory.limitjifen:append(v.carryjetton)
			end
		end
		
		local userArrtTemp = {}
	
		for i = 1, #dnHistory.useridlist do 
			pInfo = PlayerModel.GetPlayerInfo(dnHistory.useridlist[i])
			local tmp = {}
			tmp["userid"] = dnHistory.useridlist[i]
			tmp["nick"] = pInfo.nickname
			tmp["faceid"] = pInfo.face_1
			tmp["jifen"] = dnHistory.winlist[i]
			DouNiuHistoryModel.SetPlayerHistory(dnHistory.useridlist[i],dnHistory.tablemark)  --在这里就设置好了，为什么在这里设置呢，自己想 
			table.insert(userArrtTemp,tmp)
		end

		--游戏结束战报的发送俱乐部
		DouNiuEvent.JulebuGameCount(tInfo, userArrtTemp)
	end
	
	redisItem:set(redisKey,dnHistory:SerializeToString(), DouNiuHistoryModel.redis_index)
	redisItem:expire(redisKey, g_daySeconds*7, DouNiuHistoryModel.redis_index) --设置三天的时间
end


function DouNiuHistoryModel.GetHistroy(tableMark,htyItem)
	
	local redisKey = "douniuhtyinfo_"..tableMark
	
	if htyItem == nil then
		htyItem = st_douniu_pb.douniuhistory()
	end
	
	local strGet = redisItem:get(redisKey, DouNiuHistoryModel.redis_index )
	
	if strGet == nil or #strGet == 0 then
		return nil
	end
	
	htyItem:ParseFromString(strGet)

	return htyItem
	
end

function DouNiuHistoryModel.HistoryExist(tableMark)
	return redisItem:exists("douniuhtyinfo_"..tableMark,DouNiuHistoryModel.redis_index )
end


function DouNiuHistoryModel.DelSdbHistroy(tableMark)
	local redisKey = "douniuhtyinfo_"..tableMark
	redisItem:del(redisKey, DouNiuHistoryModel.redis_index)
end

function DouNiuHistoryModel.SetPlayerHistory(userID,tableMark)

	local redisKey = "douniuhtyuserid_"..userID
	local len = redisItem:lpush(redisKey,tableMark, DouNiuHistoryModel.redis_index)
	
	if len > 50 then
		--最低只是保存50个
		redisItem:rpop(redisKey, DouNiuHistoryModel.redis_index)
		DouNiuHistoryModel.DelSdbHistroy(tableMark)
	end	
end

--一次给5个战绩
function DouNiuHistoryModel.GetPlayerHistory(userID, pageNum)
	return redisItem:lrange("douniuhtyuserid_"..userID, (pageNum - 1)*5, pageNum*5 - 1, DouNiuHistoryModel.redis_index)
end



