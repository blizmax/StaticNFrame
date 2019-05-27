
HzzmjHuHelper = {}

function HzzmjHuHelper.CheckHuType(tItem, pos, pokerID)
	--在取得听牌列表的时候，调用该函数，或者每个牌胡牌的类型
	--惠州庄没有吃,只有碰，杠。
	if pokerID < 10 then
		return 0
	end
	local pengGangList = {}
	local chiList = {}
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			for k2,v2 in ipairs(v.pokerlist) do
				table.insert(chiList, v2)  --如果是吃的情况，就需要全部加进来
			end
		else
			table.insert(pengGangList, v.pokerlist[1])  --
		end
		
	end
	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos]) --取得手牌	
	table.AscInsert(handList, pokerID)
	if 0 ~= MajiangHuUtils.ShiSanYao(handList, 0) then
		--如果是十三幺
		return g_gdmjWinType.type_hzz_shisanyao   --十三幺
	end

	if tItem.m_vipRoomInfo.kehuqidui == 1 then
		local duiziList = table.clone(handList)
		local list55 = MajiangUtils.Get55CardSList(duiziList)
		
		if #list55 == 7 then
			--七对算清一色,七对算清一色
			return g_gdmjWinType.type_hzz_duiduihu
		end
	end
	local winType = HzzmjHuHelper.ZiYaoJiu(pengGangList, handList)
	if winType ~= 0 then
		return winType
	end


	local ziNum = 0
	local cardList = {}
	cardList[1] = {}
	cardList[2] = {}
	cardList[3] = {}
	
	for k,v in ipairs(pengGangList) do
		if v < 40 then
			local cardType = math.floor(v/10)   --这里需要插入三个
			table.insert(cardList[cardType], v)
			table.insert(cardList[cardType], v)
			table.insert(cardList[cardType], v)
		else
			ziNum = ziNum + 1
		end
	end
	for k,v in ipairs(chiList) do
		local cardType = math.floor(v/10)
		table.insert(cardList[cardType], v)
	end
	for k,v in ipairs(handList) do
		if v < 40 then
			local cardType = math.floor(v/10)   --这里需要插入三个
			table.insert(cardList[cardType], v)
		else
			ziNum = ziNum + 1
		end
	end	
	cardTypeLen = 0
	for k,v in ipairs(cardList) do
		if #v ~= 0 then
			cardTypeLen = cardTypeLen + 1
		end
	end
	
	if cardTypeLen == 1 then
		--绝对的清一色
		if ziNum > 0 then
			--再判断是不是碰碰胡了
			if true == HzzmjHuHelper.IsPengPengHu(cardList) then
				--清一色+碰碰胡+字牌
				return g_gdmjWinType.type_hzz_qingyise
			else
				--噢噢噢，噢噢噢，这里就是杂色了
				return g_gdmjWinType.type_hzz_zase
			end
		else
			--这个屌啊
			if true == HzzmjHuHelper.IsPengPengHu(cardList) then
				--清一色+碰碰胡
				return g_gdmjWinType.type_hzz_dage
			else
				return g_gdmjWinType.type_hzz_qingyise
			end
			
		end
	else
		if true == HzzmjHuHelper.IsPengPengHu(cardList) then
			return g_gdmjWinType.type_hzz_duiduihu
		end
	end
	
	if tItem.m_vipRoomInfo.bukejihu == 0 then
		--这里就是鸡胡了
		return g_gdmjWinType.type_hzz_jihu
	end
	return 0
end
function HzzmjHuHelper.IsShiSanYao(handList)
	if #handList ~= 14 then
		return false
	end
	local markList = {11,19,21,29,31,39,41,42,43,44,51,52,53}
	local tempList = table.clone(handList)
	--table.sort(tempList, function(a,b) return a < b end)
	for k1,v1 in ipairs(markList) do
		local isExist = false
		for k2,v2 in ipairs(tempList) do
			if v1 == v2 then
				table.remove(tempList,k2)
				isExist = true
				break
			end
		end
		if isExist == false then
			return false
		end
	end
	if #tempList ~= 1 then
		return false
	end
	
	for k,v in ipairs(markList) do
		if v == tempList[1] then
			return true
		end
	end
	return false
end

function HzzmjHuHelper.ZiYaoJiu(pengGangList, handList)
	--判断是否全字以及全部是幺九
	local tempList = {0,0,0,0,0}
	for k,v in ipairs(pengGangList) do
		if v < 40 then
			if 1 ~= math.mod(v,10) and 9 ~= math.mod(v,10) then
				return 0
			end
		end
		local tempType = math.floor(v/10)
		tempList[tempType] = tempList[tempType] + 1
	end
	for k,v in ipairs(handList) do
		if v < 40 then
			if 1 ~= math.mod(v,10) and 9 ~= math.mod(v,10) then
				return 0
			end
		end
		local tempType = math.floor(v/10)
		tempList[tempType] = tempList[tempType] + 1
	end	
	
	if tempList[1] == 0 and tempList[2] == 0 and tempList[3] == 0 then
		--全风了
		return g_gdmjWinType.type_hzz_shisanyao  --字一色
	end
	
	if tempList[4] == 0 and tempList[5] == 0 then
		--这个是全幺九
		return g_gdmjWinType.type_hzz_shisanyao
	else
		local tempLen = 0
		for i = 1,3 do   --这里只看前面三个就可以了
			if tempList[i] ~= 0 then
				tempLen = tempLen + 1
			end
		end
		
		--新的惠博庄在这里是当大哥的。
		--if tempLen == 1 then
			return g_gdmjWinType.type_hzz_dage   --清幺九
		--else
		--	return g_gdmjWinType.type_hzz_qingyise    --杂幺九
		--end
	end
end

function HzzmjHuHelper.IsPengPengHu(cardArrList)
	--判断是不是碰碰胡
	--这个是分开后的
	for k,v in ipairs(cardArrList) do
		
		if #v ~= 0 then
			
			local arrTemp = {0,0,0,0,0,0,0,0,0}
			for k2,v2 in ipairs(v) do
				arrTemp[math.mod(v2,10)] = arrTemp[math.mod(v2,10)] + 1
			end
			
			local arrLen = #v
			if math.mod(arrLen,3) ~= 0 then
				--说明这个是包括两个将牌的
				arrLen = arrLen + 1
			end
			
			local lenTemp = 0   --取得了有牌的长度
			for k2,v2 in ipairs(arrTemp) do
				if v2 == 4 or v2 == 1 then
					return false  --只有有四个和有一个的情况，都没有希望的
				end
				if v2 ~= 0 then
					lenTemp = lenTemp + 1
				end
			end
			if lenTemp ~= math.floor(arrLen/3) then
				return false
			end
		end
	end
	return true
end

function HzzmjHuHelper.BaoDaGeNotice(mjUser)
	--清一色碰碰胡，清幺九，十三幺，字一色，全幺九
	
	local cardList = {}
	for k, v in ipairs(mjUser.mjpokerlist) do
		table.insert(cardList, v.pokerlist[1])
	end
	
	if #cardList ~= 3 then
		return false
	end
	
	local typeList = {}     --类型
	local valueList = {}    --面值
	for i = 1,3 do
		typeList[i] =  math.floor(cardList[i]/10)
		valueList[i] = math.mod(cardList[i],10) --取得面值
	end
	
	if typeList[1] == typeList[2] and typeList[1] == typeList[3] then
		return true
	end
	
	if typeList[1] > 3 and typeList[2] > 3 and typeList[3] > 3 then
		return true
	elseif typeList[1] <=3 and typeList[2] <= 3 and typeList[3] <= 3 then
		if valueList[1] == 1 or valueList[1] == 9 then
			--有一个是9
			if valueList[2] == 1 or valueList[2] == 9 then
				if valueList[3] == 1 or valueList[3] == 9 then
					--全幺九
					return true
				end
			end
		end		
	end	
	

	
	--下面开始判断清幺九
	if typeList[1] > 3 then
		if typeList[2] > 3 then
			if valueList[3] == 1 or valueList[3] == 9 then
				return true
			end
		elseif typeList[3] > 3 then
			if valueList[2] == 1 or valueList[2] == 9 then
				return true
			end
		elseif typeList[2] == typeList[3] then
			if (valueList[2] == 1 and valueList[3] == 9) or (valueList[3] == 1 and valueList[2] == 9) then
				return true
			end
		end
		
	elseif typeList[2] > 3 then
		
		if typeList[3] > 3 then
			if valueList[1] == 1 or valueList[1] == 9 then
				return true
			end
		elseif typeList[1] == typeList[3] then
			if (valueList[1] == 1 and valueList[3] == 9) or (valueList[3] == 1 and valueList[1] == 9) then
				return true
			end
		end
		
	elseif typeList[3] > 3 then
		if typeList[1] == typeList[2] then
			if (valueList[1] == 1 and valueList[2] == 9) or (valueList[2] == 1 and valueList[1] == 9) then
				return true
			end
		end			
 	end
	
	

	return false
end