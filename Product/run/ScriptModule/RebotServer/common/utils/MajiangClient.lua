

MajiangClient = {}


--handList：手上的十四章牌型
--guiPubList:公共的鬼牌列表，没有则为空表
--vipInfo：vipInfo信息，如果vip信息为nil，则走通用的渠道逻辑
--返回：{{11,{12,25}},{22,{14,32}} 第一个是需要打出去的牌，后面是听牌的列表 

function MajiangClient.GetTingList(handList, guiPubList, vipInfo, openList)
	
	local retList = {}
	local cardList = {}
	local guiList = {}
	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
			end
		end
		if isExist == false then
			table.insert(cardList, v1)
		else
			table.insert(guiList, v1)
		end
	end
	
	table.sort(cardList,function(a,b) return a < b end) --先进行排列
	table.sort(guiList, function(a,b) return a < b end)
	
	
	local cardLen = #cardList
	local guiLen = #guiList + 1 --这里需要加1
	
	for i = 1, cardLen do  --遍历取得胡牌的列表
		local tempList = table.clone(cardList)
		table.remove(tempList, i) --把i剔除
		--剩下的13张牌和鬼牌组成14的牌型
		
		local huType,huList = MajiangClient.Dispatch(tempList, guiLen, vipInfo, guiPubList, openList)
		if 0 ~= huType then
			local getList = {}
			table.insert(getList, cardList[i])
			table.insert(getList, huList)
			table.insert(retList, getList)
		end
	end
	return retList
end

--返回：{{11,{12,25}},{22,{14,32}} 第一个是需要打出去的牌，后面是听牌的列表
function MajiangClient.GetQiDuiTingList(handList, guiPubList)
	--这个是做为七对的判断的
	local retList = {}
	local cardList = {}
	local guiList = {}
	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
			end
		end
		if isExist == false then
			table.insert(cardList, v1)
		else
			table.insert(guiList, v1)
		end
	end
	
	table.sort(cardList,function(a,b) return a < b end) --先进行排列
	table.sort(guiList, function(a,b) return a < b end)
	
	
	local cardLen = #cardList
	local guiLen = #guiList + 1 --这里需要加1
	
	for i = 1, cardLen do
		
		local tempList = table.clone(cardList)
		table.remove(tempList, i)
		
		local list55 = MajiangUtils.Get55CardSList(tempList)
		
		if #tempList == guiLen then
			local tingList = {}
			tingList[1] = cardList[i]
			tingList[2] = tempList
			table.insert(retList, tingList)
		elseif #tempList < guiLen then
			--说明还有鬼牌剩余的
			local tingList = {}
			tingList[1] = cardList[i]
			tingList[2] = g_gdmjDefine.mark_alllist
			table.insert(retList, tingList)
		end
	end
	return retList
end

function MajiangClient.GdmjTingList(handList, guiPubList, vipInfo, openList)
	return MajiangClient.GetTingList(handList, guiPubList, vipInfo, openList)
end


function MajiangClient.HubmjTingList(handList, guiPubList,vipInfo)
	
	for i = #handList,1,-1 do
		if handList[i] > 50 then
			--只有有花牌都不能听牌,一定要把花牌打完才能听牌
			return {}
		end	
	end

	local guiLen = 0
	if #guiPubList > 0 then
		for i = 1,#handList do
			for k = 1,#guiPubList do
				if handList[i] == guiPubList[k] then
					guiLen = guiLen + 1
				end
			end
		end
	end
	
	if vipInfo.mjtype == g_hubmjType.type_yldd then

		if guiLen > 1 then
			--如果是一癞到底，超过两个癞子不能胡
			return {}
		end
	elseif vipInfo.mjtype == g_hubmjType.type_yjly then
		if vipInfo.playtype == 3 then
			--如果一脚癞有选择的是无癞到底
			if guiLen > 0 then
				--如果是一癞到底，超过两个癞子不能胡
				return {}
			end			
		else
			if guiLen > 1 then
				--如果是一癞到底，超过两个癞子不能胡
				return {}
			end			
		end		
	end
	return MajiangClient.GetTingList(handList, guiPubList, vipInfo)
	
end

--获取吃的列表
--暂时不支持有鬼牌列表的
--返回吃的列表{{12,13,14},{13,14,15},{14,15,16}}
function MajiangClient.GetChiList(handList, guiPubList, pokerID)
	
	if guiPubList ~= nil then
		
		local maxLen = #guiPubList
		maxLen = 4*maxLen
		maxLen = #handList > maxLen and maxLen or #handList
		for k,v in ipairs(guiPubList) do
			
			if pokerID == v then
				return {}
			end
			
			for i = maxLen,1, -1 do
				if handList[i] == v then
					table.remove(handList,i)
				end
				
			end
		end
	end
	
	if pokerID > 40 then
		return {}
	end
	
	local retList = {}	
	local startPos = pokerID - 2
	local endPos = pokerID + 2
	table.sort(handList,function(a,b) return a < b end)
	if math.floor(startPos/10) ~= math.floor(pokerID/10) then
		startPos = math.floor(pokerID/10)*10 + 1
	end
	
	if math.floor(endPos/10) ~= math.floor(pokerID/10) then
		endPos = math.floor(pokerID/10)*10 + 9
	end
	local checkList = {}
	for i = startPos, endPos do
		checkList[i] = 0
	end

	for k,v in ipairs(handList) do
		if math.floor(v/10) == math.floor(pokerID/10) and (v >= (pokerID - 2)) and (v <= (pokerID + 2)) then
			checkList[v] = checkList[v] + 1
		end
	end
	
	checkList[pokerID] = 4   --这里需要设置成4,因为这里最大的长度不会超过4
	for i = 1,4 do

		if checkList[startPos] ~= 0 and checkList[startPos + 1] ~= 0 and checkList[startPos + 2] ~= 0 then
			
			local getList = {}
			table.insert(getList, startPos)
			table.insert(getList, startPos+1)
			table.insert(getList, startPos+2)

			table.insert(retList, getList)
		end
		checkList[startPos] = 0
		
		startPos = startPos + 1
		
		if startPos > (endPos - 2) then
			break
		end
	end
	return retList
end



--[[
--由于卡五星需要扣牌，所以返回的结构是：
--{[1]={可扣列表},
-- [2]={
		[1]={
			 [1] = 打出牌
			 [2] = {听牌列表}
			}
		[2] = {
			[1] = 打出牌
			[2] = {听牌列表}
			}
		}
	}
]]
function MajiangClient.KwxTingList(handList, guiPubList, vipInfo)
	
	table.sort(handList,function(a,b) return a < b end)
	
	local retList = {}
	local cardList = table.clone(handList)
		
	local ting555List = {}
	--[[
	while true do
		
		local retCard,retTempList = MajiangClient.Kwx555Work(cardList, guiPubList, vipInfo)
		
		if retCard == 0 then
			table.insert(retList, ting555List)
			table.insert(retList, retTempList)
			break
		else
			table.insert(ting555List, retCard)
			cardList = table.clone(retTempList)
		end
		
	end
	]]
	local handTemp = table.clone(handList)
	ting555List = MajiangUtils.Get555CardSList(handTemp)
	local tingList = MajiangClient.GetTingList(handList, guiPubList, vipInfo)
	
	table.insert(retList, ting555List)
	table.insert(retList, tingList)
	return retList
end


function MajiangClient.Kwx555Work(cardList, guiPubList, vipInfo)

	local tempList = table.clone(cardList)
	
	local pair555List = MajiangUtils.Get555PairArrList(tempList)
	
	for k,v in pairs(pair555List) do
		
		local retList = MajiangClient.GetTingList(v, guiPubList, vipInfo)
		
		if #retList > 0 then
			--只要有，立刻就返回
			return k,v
		end
	end
	local retList = MajiangClient.GetTingList(cardList, guiPubList, vipInfo)
	return 0,retList
end

function MajiangClient.Dispatch(cardList, guiLen, vipInfo, guiPubList, openList)
	local huType = 0
	local huList = {}
	
	if vipInfo.mjtype == g_hubmjType.type_yjly or vipInfo.mjtype == g_hubmjType.type_yldd or vipInfo.mjtype == g_hubmjType.type_jzhh or vipInfo.mjtype == g_hubmjType.type_jhmj then
		huType,huList =  MajiangClient.HubmjCommon(cardList, guiLen, vipInfo, guiPubList)
	elseif vipInfo.mjtype == g_gdmjType.type_hzz or vipInfo.mjtype == g_gdmjType.type_hdz or vipInfo.mjtype == g_gdmjType.type_hbz then
		huType,huList =  MajiangClient.HzzCommon(cardList, guiLen, vipInfo, openList)
	else
		huType,huList =  MajiangClient.GdmjCommon(cardList, guiLen, vipInfo)
	end
	
	return huType, huList
end

--对于卡五星来说，只需要鬼的长度就行
function MajiangClient.KwxCommon(cardList, guiLen, vipInfo)
	local huType
	local huList = {}

	huType,huList = MajiangHuUtils.QiDui(cardList, guiLen)
	
	if huType == 0 then
		--只有卡五星不存在的情况下才往下面走
		huType,huList = MajiangHuUtils.CommonHu(cardList, guiLen)
	end
	
	
	
	if 0 ~= huType then
		huList = MajiangUtils.TrimList(huList)
	end
	return huType, huList
end

function MajiangClient.GdmjCommon(cardList, guiLen, vipInfo)
	local huType
	local huList = {}

	huType,huList = MajiangHuUtils.QiDui(cardList, guiLen)
	
	if huType == 0 then
		--只有卡五星不存在的情况下才往下面走
		huType,huList = MajiangHuUtils.CommonHu(cardList, guiLen)
	end
	
	
	
	if 0 ~= huType then
		huList = MajiangUtils.TrimList(huList)
	end
	return huType, huList	
end

function MajiangClient.HzzCommon(cardList, guiLen, vipInfo, openList)
	local huType
	local huList = {}

	if vipInfo.kehuqidui == 1 then
		huType,huList = MajiangHuUtils.QiDui(cardList, guiLen)
	end
	if 0 ~= huType then
		huList = MajiangUtils.TrimList(huList)
	end
		--只有卡五星不存在的情况下才往下面走
	huType,huList = MajiangHuUtils.CommonHu(cardList, guiLen)
	
	if 0 ~= huType then
		huList = MajiangUtils.TrimList(huList)
		for i = #huList,1,-1 do
			local handList = table.clone(cardList)
			if vipInfo.bukejihu ~= 0 and true == MajiangHuUtils.IsHzzJihu(handList,huList[i], openList) then
				table.remove(huList, i)
			end
		end
		if #huList == 0 then
			huType = 0
		end
	end
	return huType, huList		
end

function MajiangClient.HubmjCommon(cardList, guiLen, vipInfo, guiPubList)
	local huType
	local huList = {}

	huType,huList = MajiangHuUtils.CommonHu(cardList, guiLen)	
	if 0 ~= huType then
		local isRemove = false
		if vipInfo.mjtype == g_hubmjType.type_yldd then
			if guiLen > 1 then
				isRemove = true
			end
		elseif vipInfo.mjtype == g_hubmjType.type_yjly then
			if vipInfo.playtype == 3 then
				isRemove = true
			else
				if guiLen > 1 then
					isRemove = true
				end				
			end
			
		end
		if isRemove == true and guiPubList ~= nil and #guiPubList > 0 then
			for i = 1,#huList do
				if huList[i] == guiPubList[1] then
					table.remove(huList,i)
					break
				end
			end
		end
		if #huList == 0 then
			huType = 0
		else
			huList = MajiangUtils.TrimList(huList)
		end
	end
	return huType, huList		
end

--该函数是用与前端做杠牌列表判断用的，当前端收到需要杠的通知的时候，就调用该接口，
--参数说明：手牌列表，碰牌列表（一定是碰牌列表）,vipInfo,最后拿到的手牌
--guiBase癞子的前一个牌，如果没有，则为0
function MajiangClient.GetGangList(handList, pengList, guiBase, tarChairid)
	local gangList = {}
	local handCount = {}

	if tarChairid ~= 0 then
		--表示是有目标的
		return gangList
	end
	
	for k1,v1 in ipairs(handList) do
		if handCount[v1] ~= nil then
			handCount[v1] = handCount[v1] + 1
		else
			handCount[v1] = 1
		end
		for k2,v2 in ipairs(pengList) do
			if v1 == v2 then
				table.insert(gangList, v1)
			end
		end
	end
	for k,v in pairs(handCount) do
		if v == 4 then
			table.insert(gangList, k)
		elseif v == 3 and k == guiBase then
			table.insert(gangList, k)
		end
	end
	return gangList
end

function MajiangClient.Test(testList)
	
	local huList= MajiangClient.KwxTingList(testList, {})
	local i = 0
end


