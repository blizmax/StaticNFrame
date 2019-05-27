
--该文件是调试使用的，正式环境中，不使用该文件

module("HttpDebug", package.seeall)

function gettablelist(rcvData)

	if g_isDebug ~= 1 then
		return "error"
	end
	local jsonArr = luajson.decode(rcvData)
	
	if jsonArr['category'] == '20' then
		--欢乐赢豆
	elseif jsonArr['category'] == '21' then
		--欢乐竞技
	elseif jsonArr['category'] == '22' then
		--广东麻将
		if package.loaded["Gdmj.Model.GdmjModel"] == nil then
			return "error"
		end
		retList = GdmjModel.GetTableList()
	elseif jsonArr['category'] == '23' then
		--跑胡子
	elseif jsonArr['category'] == '24' then
		--欢乐牛牛
	elseif jsonArr['category'] == '25' then
		--百人牛牛
	elseif jsonArr['category'] == '26' then
		--老虎机
	elseif jsonArr['category'] == '27' then
		--火锅英雄
	elseif jsonArr['category'] == '28' then
		--湖北麻将
		if package.loaded["Hubmj.Model.HubmjModel"] == nil then
			return "error"
		end	
		retList = HubmjModel.GetTableList()
	elseif jsonArr['category'] == '29' then
		--十点半
		if package.loaded["Sdb.Model.SdbModel"] == nil then
			return "error"
		end			
		retList = SdbModel.SdbGetTableList()
	elseif jsonArr['category'] == '30' then
		--斗牛
		if package.loaded["DouNiu.Model.DouNiuModel"] == nil then
			return "error"
		end			
		retList = DouNiuModel.GetTableList()		
	elseif jsonArr['category'] == '31' then
		--拼三张
		if package.loaded["DouNiu.Model.DouNiuModel"] == nil then
			return "error"
		end
		retList = PszModel.GetTableList()			
	elseif jsonArr['category'] == '32' then
		--牌九
		if package.loaded["DouNiu.Model.DouNiuModel"] == nil then
			return "error"
		end
		retList = PaiJiuModel.GetTableList()
	end
	return luajson.encode(retList)
end

function getpubpoker(rcvData)


	if g_isDebug ~= 1 then
		return "error"
	end
	local jsonArr = luajson.decode(rcvData)
	
	local retList = {}	
	if jsonArr['category'] == '20' then
		--欢乐赢豆
	elseif jsonArr['category'] == '21' then
		--欢乐竞技
	elseif jsonArr['category'] == '22' then
		--广东麻将
		
		local tInfo = GdmjModel.GetTableInfo(jsonArr['tableid'])
		if tInfo == nil then
			return "error"
		end
		for k,v in ipairs(tInfo.publicpoker) do
			table.insert(retList, v)
		end
	elseif jsonArr['category'] == '23' then
		--跑胡子
	elseif jsonArr['category'] == '24' then
		--欢乐牛牛
	elseif jsonArr['category'] == '25' then
		--百人牛牛
	elseif jsonArr['category'] == '26' then
		--老虎机
	elseif jsonArr['category'] == '27' then
		--火锅英雄
	elseif jsonArr['category'] == '28' then
		--湖北麻将
		local tInfo = HubmjModel.GetTableInfo(jsonArr['tableid'])
		if tInfo == nil then
			return "error"
		end
		for k,v in ipairs(tInfo.publicpoker) do
			table.insert(retList, v)
		end			
	elseif jsonArr['category'] == '29' then
		--十点半
		local tInfo = SdbModel.SdbGetTableInfo(jsonArr['tableid'])
		if tInfo == nil then
			return "error"
		end
		for k,v in ipairs(tInfo.pokerlist) do
			table.insert(retList, v)
		end 
	elseif jsonArr['category'] == '30' then
		--斗牛	
		local tInfo = DouNiuModel.GetTableInfo(jsonArr['tableid'])
		retList = DouNiuModel.GetUserList(tInfo)
	elseif jsonArr['category'] == '31' then
		--拼三张
		local tInfo = PszModel.GetTableInfo(jsonArr['tableid'])
		retList = PszService.GetUserIDList(tInfo)
	elseif jsonArr['category'] == '32' then
		--牌九
		local tInfo = PaiJiuModel.GetTableInfo(jsonArr['tableid'])
		if tInfo ~= nil then
			retList = PaiJiuService.GetUserIDList(tInfo)		
		end
	end
	table.sort(retList, function(a,b) return a < b end)
	return luajson.encode(retList)
end

function nextpoker(rcvData)

	if g_isDebug ~= 1 then
		return "error"
	end
	local retList = {}
	local jsonArr = luajson.decode(rcvData)	
	if jsonArr['category'] == '20' then
		--欢乐赢豆
	elseif jsonArr['category'] == '21' then
		--欢乐竞技
	elseif jsonArr['category'] == '22' then
		--广东麻将
		GdmjModel.SetNextCard(jsonArr['tableid'],jsonArr['nextcard'])	
	elseif jsonArr['category'] == '23' then
		--跑胡子
	elseif jsonArr['category'] == '24' then
		--欢乐牛牛
	elseif jsonArr['category'] == '25' then
		--百人牛牛
	elseif jsonArr['category'] == '26' then
		--老虎机
	elseif jsonArr['category'] == '27' then
		--火锅英雄
	elseif jsonArr['category'] == '28' then
		--湖北麻将
		HubmjModel.SetNextCard(jsonArr['tableid'],jsonArr['nextcard'])	
	elseif jsonArr['category'] == '29' then
		--十点半
		SdbModel.SetNextCard(jsonArr['tableid'],jsonArr['nextcard'])	
	elseif jsonArr['category'] == '30' then
		--斗牛
		local cardList = {}
		for k,v in pairs(jsonArr) do
			if k == v then
				table.insert(cardList, k)
			end
		end
		if jsonArr['tableid'] ~= nil and jsonArr['userid'] ~= nil then
			DouNiuModel.SetDebugNextCard(jsonArr['tableid'],jsonArr['userid'],luajson.encode(cardList))
		end
	elseif jsonArr['category'] == '31' then
		--拼三张
		local cardList = {}
		for k,v in pairs(jsonArr) do
			if k == v then
				table.insert(cardList, k)
			end
		end
		if jsonArr['tableid'] ~= nil and jsonArr['userid'] ~= nil then
			PszModel.SetDebugNextCard(jsonArr['tableid'],jsonArr['userid'],luajson.encode(cardList))
		end		
	elseif jsonArr['category'] == '32' then
		--牌九
		local cardList = {}
		for k,v in pairs(jsonArr) do
			local num = tonumber(k)
			if num ~= nil and num > 0 then
				table.insert(cardList,v)
			end
		end
		if jsonArr['tableid'] ~= nil and jsonArr['userid'] ~= nil then
			PaiJiuModel.SetDebugNextCard(jsonArr['tableid'],jsonArr['userid'],luajson.encode(cardList))
		end			
	else
		
	end
	table.sort(retList, function(a,b) return a < b end)
	return luajson.encode(retList)
end

