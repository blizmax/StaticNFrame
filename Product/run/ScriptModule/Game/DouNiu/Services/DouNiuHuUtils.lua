DouNiuHuUtils = {}

--发牌
function DouNiuHuUtils.DouNiufapai(tInfo, n)
	
	local gcmsg = msg_douniu_pb.gcdouniudeal()
	if n == 0 or n == nil then
		n = 5	
	end
		
	local cardInit = {}
	for i = 1,4 do
		for j = 1,13 do
			table.insert(cardInit, i*100 + j)
		end
	end
	
	if tInfo.kongwildcard >= 1 then
		for i = 1 ,tInfo.kongwildcard do
			table.insert(cardInit, 515)
			table.insert(cardInit, 525)  
		end
	end
	
	for i = 1,100 do
		--这里洗牌100次
		local index1 = math.myrandom(1, #cardInit)
		local index2 = math.myrandom(1, #cardInit)
		local numTemp = cardInit[index1]
		cardInit[index1] = cardInit[index2]
		cardInit[index2] = numTemp
	end	
	
	local cardList = {}
	
	--在这里先做一次随机
	for i = 1,100 do
		local index = math.myrandom(1,#cardInit)
		table.insert(cardList, cardInit[index])
		table.remove(cardInit, index)
		if #cardInit == 0 then
			break
		end
	end
	
	local userCardList = {}
	local chairCardList = {}
	for i = 1,#tInfo.situser do
		--if SpecialModel ~= nil then
		--	local preCardType = SpecialModel.NiuNiuCardType( tInfo.situser[i].userid )
		--	if preCardType ~= nil then
		--		userCardList[ tInfo.situser[i].userid ] = DouNiuHuUtils.PerGetCardList(cardList, tInfo, preCardType)
		--	end
		--end
		chairCardList[i] = {}
		for j = 1, 5 do
			--[[if i == 1 then
				local arrcard = {301,311,309,305,104}
				local pokerID = DouNiuHuUtils.GetRankCard(cardList,arrcard[j])
				table.insert(chairCardList[i], pokerID)
			--else]]
				local pokerID = DouNiuHuUtils.GetRankCard(cardList)
				table.insert(chairCardList[i], pokerID)
			--end
		end
		
	end
	
	DouNiuHuUtils.ChairSort(tInfo, chairCardList)
	
	--给每个坐下的玩家发牌
	for i = 1,#tInfo.situser do
		if tInfo.situser[i].state == g_douniuPlayStatus.status_next then
			
			local inList = chairCardList[i]
			
			if 5 == #inList then	
				tInfo.situser[i].nntype = DouNiuHuUtils.GetCardType(inList, tInfo)
				for j = 1, #inList do
					tInfo.situser[i].handpoker:append(inList[j])
				end	
			end

		end
	end	
end

--获得单张牌
function DouNiuHuUtils.GetRankCard(cList,cardNum)
	if cardNum == nil then
		local index = math.myrandom(1,#cList)
		local temp = cList[index]
		table.remove(cList, index)
		return temp
	else
		local temp = 0
		for i = 1,#cList do
			if cList[i] == cardNum then
				temp = cList[i]
				table.remove(cList, i)
				break
			end
		end
		return temp
	end
end

--检测四张牌的牌型
function DouNiuHuUtils.GetFourCardType(inList, tInfo)
	--任意取出三张，只要组成卡牌
	local cardList = {}
	local wildcard = {}
	for k,v in ipairs(inList) do
		if tInfo ~= nil and tInfo.kongwildcard >= 1 then
			if v == 515 or v == 525 then
				table.insert(wildcard, v)
			else
				table.insert(cardList,v)
			end
		else
			table.insert(cardList,v)
		end
	end
	
	if #cardList < 4 then
		return DouNiuHuUtils.GetWildCardType(cardList, wildcard, pokerList)
	end
	--取其中的任意三个。
	for i = 1,#cardList do
		for j = i + 1, #cardList do 
			for k = j + 1, #cardList do
				--判断这三个能不能组成牛牛
				local tempList = {}
				table.insert(tempList, cardList[i])
				table.insert(tempList, cardList[j])
				table.insert(tempList, cardList[k])
				
				if true == DouNiuHuUtils.ThreeCardIsNiuNiu(tempList) then
					--如果三张牌是牛，那就判断剩下的两张牌，是不是组成牛牛。
					local lastList = {}
					for m = 1,#cardList do
						if m ~= i and m ~= j and m ~= k then
							table.insert(lastList, cardList[m])
						end						
					end
					--如果是牛牛，需要把牌摆放在前面
					--剩下的两个
					if pokerList ~= nil and pokerList ~= 0 then
						for k,v in ipairs(lastList) do
							table.insert(pokerList, v)
						end	 
					end
					local retNum = DouNiuHuUtils.TwoCardDianNum(lastList)   --取到最后的卡牌的类型，这里先判断牛牛,其他的牌型再判断
					return retNum
				end
				
			end
		end
		
	end
	return g_douNiuCard_type.wu_niu   --如果到这里就是无牛了
end

--第二个参数为获得组成10的3张牌， 在提牌是才会用到
function DouNiuHuUtils.GetCardType(inList, tInfo, pokerList)
	--首先，判断有没有牛牛
	--任意取出三张，只要组成卡牌
	local cardList = {}
	local wildcard = {}
	for k,v in ipairs(inList) do
		if tInfo ~= nil and tInfo.kongwildcard >= 1 then
			if v == 515 or v == 525 then
				table.insert(wildcard, v)
			else
				table.insert(cardList,v)
			end
		else
			table.insert(cardList,v)
		end
	end
	
	if #cardList < 5 then
		return DouNiuHuUtils.GetWildCardType(cardList, wildcard, pokerList)
	end
	
	--先判断五小 > 炸弹 > 葫芦牛 > 五花牛 > 同花牛 > 顺子牛 
	local dianList = {}
	local dianListLen = 0
	local huaList = {}
	local huaListLen = 0
	local dianNum = 0
	local wuhuaLen = 0
	
	for i = 1,#cardList do
		local dianType = math.mod(cardList[i], 100)
		dianNum = dianNum + dianType
		
		if dianType > 10 then
			wuhuaLen = wuhuaLen + 1
		end
		
		if dianList[dianType] == nil then 
			dianList[dianType] = 1
			dianListLen = dianListLen + 1	
		else
			dianList[dianType] = dianList[dianType] + 1
		end
		
		local huaType = math.floor(cardList[i]/100)
		if huaList[huaType] == nil then 
			huaList[huaType] = 1
			huaListLen = huaListLen + 1	
		else
			huaList[huaType] = huaList[huaType] + 1
		end
	end
	
	local isshunzi = false    --是否是顺子
	if dianListLen >= 5 then
		local isLinked = true 
		local last = 0
		local tmp = 1
		local tmpList = {}
		local tmpListA = {}
		local isA = false
		for k,v in pairs(dianList) do
			local tmp = k
			if k == 1 then
				tmp = 14
				isA = true
			end
			table.insert(tmpList, k)
			table.insert(tmpListA, tmp)
		end 
		table.sort(tmpList)
		for k,v in pairs(tmpList) do
			if tmp == 1 then
				last = v
			elseif tmp > 1 then
				if math.abs(last - v) ~= 1 then
					isLinked = false
					break
				end
				last = v
			end
			tmp = tmp + 1
		end
		
		if isLinked == false and isA == true then
			isLinked = true 
			table.sort(tmpListA)
			local lastA = 0
			local tmpA = 1
			for k,v in pairs(tmpListA) do
				if tmpA == 1 then
					lastA = v
				elseif tmpA > 1 then
					if math.abs(lastA - v) ~= 1 then
						isLinked = false
						break
					end
					lastA = v
				end
				tmpA = tmpA + 1
			end
		end 
		
		if isLinked == true then 
			isshunzi = true
		end
	end
	
	if huaListLen == 1 and isshunzi == true then
		return g_douNiuCard_type.chongtian_niu	--返回冲天牛
	end
	
	if dianNum <= 10 then
		return g_douNiuCard_type.wu_xiao   --返回五小
	end
	
	if dianListLen == 2 then
		--如果长度只有两个
		for k,v in pairs(dianList) do
			if v == 1 or v == 4 then
				return g_douNiuCard_type.zha_dan --返回炸弹
			end
			if v == 2 or v == 3 then
				return g_douNiuCard_type.hulu_niu --返回葫芦牛
			end
		end
	end

	if wuhuaLen  == 5 then
		return g_douNiuCard_type.wu_hua	   --返回五花牛
	end
	
	if huaListLen == 1 then
		return g_douNiuCard_type.tonghua_niu	--返回同花牛
	end
	
	if isshunzi == true then
		return g_douNiuCard_type.shunzi_niu	--返回顺子牛
	end
 	
	--取其中的任意三个。
	for i = 1,#cardList do
		for j = i + 1, #cardList do 
			for k = j + 1, #cardList do
				--判断这三个能不能组成牛牛
				local tempList = {}
				table.insert(tempList, cardList[i])
				table.insert(tempList, cardList[j])
				table.insert(tempList, cardList[k])
				
				if true == DouNiuHuUtils.ThreeCardIsNiuNiu(tempList) then
					--如果三张牌是牛，那就判断剩下的两张牌，是不是组成牛牛。
					local lastList = {}
					for m = 1,#cardList do
						if m ~= i and m ~= j and m ~= k then
							table.insert(lastList, cardList[m])
						end						
					end
					--如果是牛牛，需要把牌摆放在前面
					--剩下的两个
					if pokerList ~= nil and pokerList ~= 0 then
						for k,v in ipairs(lastList) do
							table.insert(pokerList, v)
						end	 
					end
					local retNum = DouNiuHuUtils.TwoCardDianNum(lastList)   --取到最后的卡牌的类型，这里先判断牛牛,其他的牌型再判断
					return retNum
				end
				
			end
		end
		
	end
	return g_douNiuCard_type.wu_niu   --如果到这里就是无牛了

end

--获得牌型有癞子的
function DouNiuHuUtils.GetWildCardType(cardList, wildcard, pokerList)
	
	--先判断五小 > 炸弹 > 葫芦牛 > 五花牛 > 同花牛 > 顺子牛 
	local dianList = {}
	local dianListLen = 0
	local huaList = {}
	local huaListLen = 0
	local dianNum = 0
	local wuhuaLen = 0
	
	for i = 1,#cardList do
		local dianType = math.mod(cardList[i], 100)
		dianNum = dianNum + dianType
		
		if dianType > 10 then
			wuhuaLen = wuhuaLen + 1
		end
		
		if dianList[dianType] == nil then 
			dianList[dianType] = 1
			dianListLen = dianListLen + 1	
		else
			dianList[dianType] = dianList[dianType] + 1
		end
		
		local huaType = math.floor(cardList[i]/100)
		if huaList[huaType] == nil then 
			huaList[huaType] = 1
			huaListLen = huaListLen + 1	
		else
			huaList[huaType] = huaList[huaType] + 1
		end
	end
	
	local isshunzi = false
	if dianListLen >= #cardList then
		local isLinked = true
		local last = 0
		local tmp = 1
		local tmp2 = 0
		local tmp3 = 0
		local tmpList = {}
		local tmpListA = {}
		local isA = false
		for k,v in pairs(dianList) do
			local tmp = k
			if k == 1 then
				tmp = 14
				isA = true
			end
			table.insert(tmpList, k)
			table.insert(tmpListA, tmp)
		end 
		table.sort(tmpList)
		for k,v in pairs(tmpList) do
			if tmp == 1 then
				last = v
			elseif tmp > 1 then
				if math.abs(last - v) ~= 1 then
					if  math.abs(last - v) == 2 then 
						if tmp2 >= ((#cardList == 3) and 2 or 1) or tmp3 > 0 then
							isLinked = false
							break
						end
						tmp2 = tmp2 + 1
					elseif math.abs(last - v) == 3 then
						if tmp3 > 1 or (tmp3 == 0 and #cardList ~= 3) then 
							isLinked = false
							break
						end
						tmp3 = tmp3 + 1
					else 
						isLinked = false
						break
					end
				end
				last = v
			end
			tmp = tmp + 1
		end
		
		if isLinked == false and isA == true then
			isLinked = true 
			table.sort(tmpListA)
			local lastA = 0
			local tmpA = 1
			for k,v in pairs(tmpListA) do
				if tmpA == 1 then
					lastA = v
				elseif tmpA > 1 then
					if math.abs(lastA - v) ~= 1 then
						if  math.abs(lastA - v) == 2 then 
							if tmp2 >= ((#cardList == 3) and 2 or 1) or tmp3 > 0 then
								isLinked = false
								break
							end
							tmp2 = tmp2 + 1
						elseif math.abs(lastA - v) == 3 then
							if tmp3 > 1 or (tmp3 == 0 and #cardList ~= 3) then 
								isLinked = false
								break
							end
							tmp3 = tmp3 + 1
						else 
							isLinked = false
							break
						end
					end
					lastA = v
				end
				tmpA = tmpA + 1
			end
		end 
		
		
		if isLinked == true then 
			isshunzi = true
		end
	end
	if huaListLen == 1 and isshunzi == true then
		return g_douNiuCard_type.chongtian_niu --返回冲天牛
	end
	
	if #wildcard >= 4 then
		return g_douNiuCard_type.chongtian_niu
	end
	if #wildcard == 3 then
		local num = cardList[1] - cardList[2]
		if math.abs(num) > 3 then
			if dianNum <= (10 - (5 - #cardList)) then
				return g_douNiuCard_type.wu_xiao   --返回五小
			else
				return g_douNiuCard_type.zha_dan --返回炸弹
			end
		else
			return g_douNiuCard_type.chongtian_niu --返回冲天牛
		end	
	end
	
	if dianNum <= (10 - (5 - #cardList)) then
		return g_douNiuCard_type.wu_xiao   --返回五小
	end
	
	if dianListLen <= 2 then
		--如果长度只有两个
		for k,v in pairs(dianList) do
			if v == 1 or v == 3  then
				return g_douNiuCard_type.zha_dan --返回炸弹
			end
			if v == 2 then
				if #cardList == 3 then
					return g_douNiuCard_type.zha_dan --返回炸弹
				else
					return g_douNiuCard_type.hulu_niu --返回葫芦牛
				end	
			end
		end
	end

	if wuhuaLen  == #cardList then
		return g_douNiuCard_type.wu_hua	   --返回五花牛
	end
	
	if huaListLen == 1 then
		return g_douNiuCard_type.tonghua_niu	--返回同花牛
	end
	
	if isshunzi == true then 
		return g_douNiuCard_type.shunzi_niu
	end
	
		
	if #cardList == 3 then
		if pokerList ~= nil and pokerList ~= 0 then
			for k, v in ipairs(wildcard)do
				table.insert(pokerList, v)
			end
		end
		
		return g_douNiuCard_type.niu_niu
	end
		
	--取其中的任意三个。
	for i = 1,#cardList do
		for j = i + 1, #cardList do 
			for k = j + 1, #cardList do
				--判断这三个能不能组成牛牛
				local tempList = {}
				table.insert(tempList, cardList[i])
				table.insert(tempList, cardList[j])
				table.insert(tempList, cardList[k])
				
				if true == DouNiuHuUtils.ThreeCardIsNiuNiu(tempList) then
					if pokerList ~= nil and pokerList ~= 0 then
						table.insert(pokerList, wildcard[1])
						for k,v in ipairs(cardList) do
							if tempList[1] ~= v and tempList[2] ~= v and tempList[3] ~= v then
								table.insert(pokerList, v)
								break
							end
						end	 
					end
					return g_douNiuCard_type.niu_niu
				end	
			end
		end
	end
	
	local typeMax = g_douNiuCard_type.wu_niu 
	local TMP = {} 
	for i = 1,#cardList do
		for j = i + 1, #cardList do 
			local tempList = {}
			table.insert(tempList, cardList[i])
			table.insert(tempList, cardList[j])
			local tmp = DouNiuHuUtils.TwoCardDianNum(tempList)
			if typeMax < tmp then
				TMP[1] = cardList[i]
				TMP[2] = cardList[j]
				typeMax = tmp
			end
		end
	end
	if pokerList ~= nil and pokerList ~= 0 then
		for i = 1,#TMP do
			table.insert(pokerList, TMP[i])
		end
	end

	return typeMax

end

function DouNiuHuUtils.ThreeCardIsNiuNiu(cardList)
	--判断三张卡牌是不是牛牛
	local allNum = 0
	for i = 1,#cardList do
		--先不管花色
		cardList[i] = math.mod(cardList[i], 100) --取得面值的大小
		if cardList[i] >= 10 then
			--如果是10以上的面值
			cardList[i] = 0   --这里就置为0
		end
		allNum = cardList[i] + allNum
	end
	
	allNum = math.mod(allNum, 10) --这里一定要求模
	
	if allNum == 0 then
		--说名这三个牌组成牛牛
		return true
	end
	return false
end

function DouNiuHuUtils.TwoCardDianNum(cardList)   --返回对应的点数
	local allNum = 0
	for i = 1,#cardList do
		--先不管花色
		cardList[i] = math.mod(cardList[i], 100) --取得面值的大小
		if cardList[i] >= 10 then
			--如果是10以上的面值
			cardList[i] = 0   --这里就置为0
		end
		allNum = cardList[i] + allNum
	end
	allNum = math.mod(allNum, 10) --这里一定要求模
	if allNum == 0 then
		--说名这三个牌组成牛牛
		return 10
	end
	return allNum  --
	
end

--获得牌型的倍数列表
function DouNiuHuUtils.getDN_MultipleList(tInfo, multipleList)

	for j = 0, g_douNiuCard_type.niu_liu do
		multipleList[j] = 1
	end	
	if tInfo.typeodds == g_douNiuRate_typeone then
		multipleList[g_douNiuCard_type.niu_qi] = 2
		multipleList[g_douNiuCard_type.niu_ba] = 2
		multipleList[g_douNiuCard_type.niu_jiu] = 3
		multipleList[g_douNiuCard_type.niu_niu] = 4
		
		multipleList[g_douNiuCard_type.zha_dan] = 4
		multipleList[g_douNiuCard_type.wu_hua] = 4
		multipleList[g_douNiuCard_type.wu_xiao] = 4
		
		multipleList[g_douNiuCard_type.shunzi_niu] = 4
		multipleList[g_douNiuCard_type.hulu_niu] = 4
		multipleList[g_douNiuCard_type.tonghua_niu] = 4
		multipleList[g_douNiuCard_type.chongtian_niu] = 4
	elseif tInfo.typeodds == g_douNiuRate_typetwo then
		multipleList[g_douNiuCard_type.niu_qi] = 1
		multipleList[g_douNiuCard_type.niu_ba] = 2
		multipleList[g_douNiuCard_type.niu_jiu] = 2
		multipleList[g_douNiuCard_type.niu_niu] = 3
		
		multipleList[g_douNiuCard_type.zha_dan] = 3
		multipleList[g_douNiuCard_type.wu_hua] = 3
		multipleList[g_douNiuCard_type.wu_xiao] = 3
		
		multipleList[g_douNiuCard_type.shunzi_niu] = 3
		multipleList[g_douNiuCard_type.hulu_niu] = 3
		multipleList[g_douNiuCard_type.tonghua_niu] = 3
		multipleList[g_douNiuCard_type.chongtian_niu] = 3
	else
		multipleList[g_douNiuCard_type.wu_niu] = 1
		multipleList[g_douNiuCard_type.niu_yi] = 1
		multipleList[g_douNiuCard_type.niu_er] = 2
		multipleList[g_douNiuCard_type.niu_san] = 3
		multipleList[g_douNiuCard_type.niu_si] = 4
		multipleList[g_douNiuCard_type.niu_wu] = 5
		multipleList[g_douNiuCard_type.niu_liu] = 6
		multipleList[g_douNiuCard_type.niu_qi] = 7
		multipleList[g_douNiuCard_type.niu_ba] = 8
		multipleList[g_douNiuCard_type.niu_jiu] = 9
		multipleList[g_douNiuCard_type.niu_niu] = 10
		multipleList[g_douNiuCard_type.shunzi_niu] = 10
		multipleList[g_douNiuCard_type.tonghua_niu] = 10
		multipleList[g_douNiuCard_type.wu_hua] = 10
		multipleList[g_douNiuCard_type.hulu_niu] = 10
		multipleList[g_douNiuCard_type.zha_dan] = 10
		multipleList[g_douNiuCard_type.wu_xiao] = 10
		multipleList[g_douNiuCard_type.chongtian_niu] = 10
		
	end
	
	cgstrvipinfo = st_douniu_pb.douniuspecialtype()
	cgstrvipinfo:ParseFromString(tInfo.specialtype)
	
	if cgstrvipinfo.shunziNiu == 1 then
		multipleList[g_douNiuCard_type.shunzi_niu] = 5
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.shunzi_niu] = 15
		end
	end
	if cgstrvipinfo.tonghuaNiu == 1 then
		multipleList[g_douNiuCard_type.tonghua_niu] = 5
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.tonghua_niu] = 17
		end
	end
	if cgstrvipinfo.wuhua == 1 then
		multipleList[g_douNiuCard_type.wu_hua] = 5
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.wu_hua] = 16
		end
	end
	if cgstrvipinfo.huluNiu == 1 then
		multipleList[g_douNiuCard_type.hulu_niu] = 6
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.hulu_niu] = 18
		end
	end
	if cgstrvipinfo.zhadan == 1 then
		multipleList[g_douNiuCard_type.zha_dan] = 7
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.zha_dan] = 19
		end
	end
	if cgstrvipinfo.wuxiao == 1 then
		multipleList[g_douNiuCard_type.wu_xiao] = 8
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.wu_xiao] = 20
		end
	end
	if cgstrvipinfo.chongtianNiu == 1 then
		multipleList[g_douNiuCard_type.chongtian_niu] = 10
		if tInfo.typeodds == g_douNiuRate_typethree then
			multipleList[g_douNiuCard_type.chongtian_niu] = 21
		end
	end
	
	
	
end

--比较两个牌的大小
function DouNiuHuUtils.Compare(a,b, tInfo)
	
	local type_a = math.floor(a/100)
	local hua_a = math.mod(a, 100)
	local type_b = math.floor(b/100)
	local hua_b = math.mod(b,100)
	
	--同牌型有癞子的输
	if tInfo.kongwildcard >= 1 then
		if math.floor(type_a) == 5 and math.floor(type_b) ~= 5 then
			return false
		elseif math.floor(type_b) == 5 and math.floor(type_a) ~= 5 then
			return true
		end
	end
	--if hua_a > hua_b	
	if hua_b > hua_a then
		return false
	elseif hua_b < hua_a then
		return true
	else
		if type_b > type_a then
			return false
		else --这里不存在相等的概念
			return true
		end
	end	
end

--比较两个牌的大小
function DouNiuHuUtils.Compare_shunzi(a,b, tInfo)
	
	local type_a = math.floor(a/100)
	local hua_a = math.mod(a, 100)
	if hua_a == 1 then
		hua_a = 14
	end
	local type_b = math.floor(b/100)
	local hua_b = math.mod(b,100)
	if hua_b == 1 then
		hua_b = 14
	end
	
	--同牌型有癞子的输
	if tInfo.kongwildcard >= 1 then
		if math.floor(type_a) == 5 and math.floor(type_b) ~= 5 then
			return false
		elseif math.floor(type_b) == 5 and math.floor(type_a) ~= 5 then
			return true
		end
	end
	--if hua_a > hua_b	
	if hua_b > hua_a then
		return false
	elseif hua_b < hua_a then
		return true
	else
		if type_b > type_a then
			return false
		else --这里不存在相等的概念
			return true
		end
	end	
end

function DouNiuHuUtils.maxhandpoker(pos, tInfo)
	
	if tInfo.situser[pos].nntype == g_douNiuCard_type.hulu_niu then
		if tInfo.kongwildcard >= 1 then
			for k,v in ipairs(tInfo.situser[pos].handpoker) do
				if v == 525 or v == 515 then
					return v
				end
			end
		end
		local dianList = {}
		for k,v in ipairs(tInfo.situser[pos].handpoker) do
			local dianType = math.mod(v, 100)
			if dianList[dianType] == nil then 
				dianList[dianType] = 1
			else
				dianList[dianType] = dianList[dianType] + 1
				if dianList[dianType] == 3 then
					return v
				end
			end	
		end
	end
	
	local max = tInfo.situser[pos].handpoker[1]
	for i = 2, #tInfo.situser[pos].handpoker do 
		local type_a = math.mod(tInfo.situser[pos].handpoker[i],100)
		local type_b = math.mod(max, 100)
		local hua_a = math.floor(tInfo.situser[pos].handpoker[i]/100)
		local hua_b = math.floor(max/100)
		if type_a > type_b then
			max = tInfo.situser[pos].handpoker[i]
		elseif type_a < type_b then
			
		else
			if hua_a > hua_b then
				max = tInfo.situser[pos].handpoker[i]	
			end
		end
	end
	return max
end

function DouNiuHuUtils.maxhandpoker2(pos, tInfo)
	local num = 0
	for k,v in ipairs(tInfo.situser[pos].handpoker) do
		if v == 525 or v == 515 then
			num = num + 1
		end
	end
	
	if num == 5 then
		return 525
	end
	
	local max = tInfo.situser[pos].handpoker[1]
	for i = 2, #tInfo.situser[pos].handpoker do 
		local type_a = math.mod(tInfo.situser[pos].handpoker[i],100)
		local type_b = math.mod(max, 100)
		local hua_a = math.floor(tInfo.situser[pos].handpoker[i]/100)
		local hua_b = math.floor(max/100)
		if hua_b == 5 then
			max = tInfo.situser[pos].handpoker[i]
		else
			if type_a > type_b then
				max = tInfo.situser[pos].handpoker[i]
			elseif type_a < type_b then
				
			else
				if hua_a > hua_b then
					max = tInfo.situser[pos].handpoker[i]	
				end
			end
		end
	end
	return max
end

function DouNiuHuUtils.maxhandpoker3(pos, tInfo)
	local num = 0
	for k,v in ipairs(tInfo.situser[pos].handpoker) do
		if v == 525 or v == 515 then
			num = num + 1
		end
	end
	
	if num == 5 then
		return 525
	end

	local max = tInfo.situser[pos].handpoker[1]
	local index = 1
	if max == 525 or max == 515 then
		for i = 2, #tInfo.situser[pos].handpoker do
			local max = tInfo.situser[pos].handpoker[i]
			index = i
			if max ~= 525 and max ~= 515 then
				break
			end
		end
	end

	if max == 525 or max == 515 then
		return max
	end

	for i = index+1, #tInfo.situser[pos].handpoker do 
		local type_a = math.mod(tInfo.situser[pos].handpoker[i],100)
		local type_b = math.mod(max, 100)
		local hua_a = math.floor(tInfo.situser[pos].handpoker[i]/100)
		local hua_b = math.floor(max/100)
		if hua_b ~= 5 then
			if type_a > type_b then
				max = tInfo.situser[pos].handpoker[i]
			else
				if hua_a > hua_b then
					max = tInfo.situser[pos].handpoker[i]	
				end
			end
		end
	end
	return max
end

function DouNiuHuUtils.maxhandpoker_shunzi(pos, tInfo)
	
	local dianList = {}
	for i = 1, #tInfo.situser[pos].handpoker do
		local tmp = math.mod(tInfo.situser[pos].handpoker[i], 100)
		table.insert(dianList, {i, tmp})
	end
	table.sort(dianList, function(a,b)
				return a[2] < b[2]
			end	
	)
	if dianList[5][2] == 13 and dianList[1][2] == 1 then
		return tInfo.situser[pos].handpoker[dianList[1][1]]
	end
	
	return tInfo.situser[pos].handpoker[dianList[5][1]]
end

function DouNiuHuUtils.WhoWinner(tInfo)
	local winner = 1
	if #tInfo.situser == 1 then 
		return winner
	end
	for i = 2, #tInfo.situser do
		if tInfo.situser[i] == g_douniuPlayStatus.status_play then
			if false == DouNiuHuUtils.DouNiuWin(winner, i, tInfo) then
				winner = i
			end
		end
	end
	return winner
end

function DouNiuHuUtils.DouNiuWin(banker,v, tInfo)
	if tInfo.situser[banker].nntype > tInfo.situser[v].nntype then
		return true
	elseif tInfo.situser[banker].nntype == tInfo.situser[v].nntype then 
		if tInfo.situser[banker].nntype ~= g_douNiuCard_type.shunzi_niu then
			if tInfo.kongwildcard >= 1 then
				local flag1 = false
				local flag2	= false
				for k,v in ipairs(tInfo.situser[banker].handpoker) do
					if v == 525 or v == 515 then
						flag1 = true
					end
				end
				for k,i in ipairs(tInfo.situser[v].handpoker) do
					if i == 525 or v == 515 then
						flag2 = true
					end
				end
			end

			if flag1 and flag2 then
				local max1 = DouNiuHuUtils.maxhandpoker2(banker, tInfo)	
				local max2 = DouNiuHuUtils.maxhandpoker2(v, tInfo)
				for j = 1,#tInfo.situser[v].handpoker do		
					if false == DouNiuHuUtils.Compare(max1, max2, tInfo) then
						return false
					else
						return true
					end
				end
			else
				local max1 = DouNiuHuUtils.maxhandpoker(banker, tInfo)	
				local max2 = DouNiuHuUtils.maxhandpoker(v, tInfo)
				for j = 1,#tInfo.situser[v].handpoker do		
					if false == DouNiuHuUtils.Compare(max1, max2, tInfo) then
						return false
					else
						return true
					end
				end	
			end
		else
			local max1 = DouNiuHuUtils.maxhandpoker_shunzi(banker, tInfo)	
			local max2 = DouNiuHuUtils.maxhandpoker_shunzi(v, tInfo)
			if false == DouNiuHuUtils.Compare_shunzi(max1, max2, tInfo) then
				return false
			else
				return true
			end
		end	
	end
	return false
end


function DouNiuHuUtils.GetDebugCardList(tableID, userID, getList, pubList)
	
	if g_isDebug ~= 1 then
		return nil
	end
	
	local cardList = DouNiuModel.GetDebugNextCard(tableID, userID)
	
	if cardList == nil or #cardList == 0 then
		return nil
	end
	cardList = luajson.decode(cardList)
	for k1,v1 in ipairs(cardList) do
		if #getList >= 5 then
			break
		end
		local tempNum = tonumber(v1)
		
		for k2,v2 in ipairs(pubList) do
			if tempNum == v2 then
				table.remove(pubList, tempNum)
				table.insert(getList, tempNum)
				break
			end
		end
		
	end
	
	for i = (#getList + 1),5 do
		table.insert(getList,DouNiuHuUtils.GetRankCard(pubList))
	end
	return getList
end

function DouNiuHuUtils.PerGetCardList(cardList, tInfo, getType)
	for i = 1,5 do
		local retList = DouNiuHuUtils.DoPerGetCardList(cardList, tInfo, getType)
		if retList ~= nil then
			return retList
		end
	end
	return nil
end


function DouNiuHuUtils.DoPerGetCardList(cardList, tInfo, getType)

	if true then
		return
	end
	
	if getType == nil then
		return nil
	end
	
	local getList = {}
	for j = 1, 4 do
		local pokerID = DouNiuHuUtils.GetRankCard(cardList)
		table.insert(getList, pokerID)
	end
	--先取四张牌，然后再随机去另外一张
	
	local startIndex = math.myrandom(1,#cardList)
	
	for i = startIndex, #cardList do
		
		table.insert(getList, cardList[i]) --在最后一个插入
		
		if getType == DouNiuHuUtils.GetCardType(getList, tInfo) then
			table.remove(cardList, i)   --这里一定要移除掉
			return getList
		end
		
		table.remove(getList, 5)   --把最后一个移除掉，因为最后一个是最后插入的
		
	end
	
	for i = 1,startIndex do
		table.insert(getList, cardList[i]) --在最后一个插入
		
		if getType == DouNiuHuUtils.GetCardType(getList, tInfo) then
			table.remove(cardList, i)   --这里一定要移除掉
			return getList
		end
		
		table.remove(getList, 5)   --把最后一个移除掉，因为最后一个是最后插入的		
	end
	table.insertto(cardList, getList)   --要记得放回去
	return nil  
end


function DouNiuHuUtils.CradSort(tInfo)
	
	for z = 1, #tInfo.situser do
		if tInfo.situser[z].state == g_douniuPlayStatus.status_wait then
			if tInfo.situser[z].handpoker[5] ~= nil then
				tInfo.situser[z].endcard = tInfo.situser[z].handpoker[5]
			end
			if tInfo.situser[z].nntype >= g_douNiuCard_type.niu_yi and tInfo.situser[z].nntype <= g_douNiuCard_type.niu_niu then
				local laiziCount = 0
				local cardList = {}
				for k,v in ipairs(tInfo.situser[z].handpoker) do
					if v == 525 or v == 515  then
						laiziCount = laiziCount + 1
					end
					table.insert(cardList,v)
				end
				local isEx =false
				if tInfo.kongwildcard >= 1 and laiziCount > 0 then
					--两癞子
					if laiziCount == 2 then	
						local tmp = {}
						for i = 1,#cardList do
							if #tmp == 0 then
								if cardList[i] == 525 or cardList[i] == 515  then
									table.insert(tmp, i)
									break
								end
							end
						end
						for i = 1,#cardList do
							if #tmp == 1 then
								if cardList[i]  ~= 525 and cardList[i] ~= 515  then
									table.insert(tmp, i)
									break
								end
							end
						end
						local x = 1
						for m = 1,#cardList do
							if m ~= tmp[1] and m ~= tmp[2] then
								tInfo.situser[z].handpoker[x] = cardList[m]
								x = x + 1
							end	
						end
						tInfo.situser[z].handpoker[4] = cardList[tmp[1]]
						tInfo.situser[z].handpoker[5] = cardList[tmp[2]]
						isEx = true
					else
						--找两张牌加起来等于点数的
						for i = 1,#cardList do
							for j = i + 1, #cardList do 
								local tmp = {}
								local tmp_1 = {}
								table.insert(tmp, cardList[i])
								table.insert(tmp_1, cardList[i])
								table.insert(tmp, cardList[j])
								table.insert(tmp_1, cardList[j])
								if tInfo.situser[z].nntype == DouNiuHuUtils.TwoCardDianNum(tmp_1) then
									local x = 1
									for m = 1,#cardList do
										if m ~= i and m ~= j then
											tInfo.situser[z].handpoker[x] = cardList[m]
											x = x + 1
										end
									end
									tInfo.situser[z].handpoker[4] = tmp[1]
									tInfo.situser[z].handpoker[5] = tmp[2]
									isEx = true
								end
							end
						end
					end
				end
				--找3张牌加起来等于10的
				--取其中的任意三个。
				if isEx == false then
					for i = 1,#cardList do
						for j = i + 1, #cardList do 
							for k = j + 1, #cardList do
								--判断这三个能不能组成牛牛
								local tempList = {}
								table.insert(tempList, cardList[i])
								table.insert(tempList, cardList[j])
								table.insert(tempList, cardList[k])	
								if true == DouNiuHuUtils.ThreeCardIsNiuNiu(tempList) then
									--如果三张牌是牛，那就判断剩下的两张牌，是不是组成牛牛。	
									tInfo.situser[z].handpoker[1] = cardList[i]
									tInfo.situser[z].handpoker[2] = cardList[j]
									tInfo.situser[z].handpoker[3] = cardList[k]
									local inCount = 4
									for m = 1,#cardList do
										if m ~= i and m ~= j and m ~= k then
											tInfo.situser[z].handpoker[inCount] = cardList[m]
											inCount = inCount + 1
										end		
									end				
								end
							end
						end
					end
				end
			end
		end
	end
end



function DouNiuHuUtils.ChairSort(tInfo, chairCardList)


	local winUserID = SpecialModel.GetDouNiuWin(tInfo.tableid)
	local loseUserID = SpecialModel.GetDouNiuLose(tInfo.tableid)

	local winChair = 0
	local loseChair = 0
	
	if winUserID == 0 and loseUserID == 0 then
		return 
	end
	
	for i = 1, #tInfo.situser do
		if tInfo.situser[i].userid == winUserID then
			winChair = i
		elseif tInfo.situser[i].userid == loseUserID then
			loseChair = i
		end
	end
	local chairGetList = {}
	local maxIndex = 1
	local minIndex = 1
	for k_chair,v_chair in ipairs(chairCardList) do
		chairGetList[k_chair] = {}
		chairGetList[k_chair][1] = SpecialModel.GetCardType(v_chair)
		chairGetList[k_chair][2], chairGetList[k_chair][3] = SpecialModel.GetMaxCardInfo(v_chair)
		
		if k_chair > 1 then
			
			if chairGetList[k_chair][1] > chairGetList[maxIndex][1] then
				maxIndex = k_chair
			elseif chairGetList[k_chair][1] == chairGetList[maxIndex][1] then
				if chairGetList[k_chair][2] > chairGetList[maxIndex][2] then
					maxIndex = k_chair
				elseif chairGetList[k_chair][2] == chairGetList[maxIndex][2] then
					if chairGetList[k_chair][3] > chairGetList[maxIndex][3] then
						maxIndex = k_chair
					end
				end
				
			end
			
			if chairGetList[k_chair][1] < chairGetList[minIndex][1] then
				minIndex = k_chair
			elseif chairGetList[k_chair][1] == chairGetList[minIndex][1] then
				if chairGetList[k_chair][2] < chairGetList[minIndex][2] then
					minIndex = k_chair
				elseif chairGetList[k_chair][2] == chairGetList[minIndex][2] then
					if chairGetList[k_chair][3] < chairGetList[minIndex][3] then
						minIndex = k_chair
					end
				end
				
			end		
		end		
	end
	
	if winChair ~= 0 and maxIndex ~= 0 then
		--print("winChair="..winChair..",userID="..tInfo.situser[winChair].userid)
		local winTemp = table.clone(chairCardList[winChair])
		chairCardList[winChair] = chairCardList[maxIndex]
		chairCardList[maxIndex] = winTemp
	end
	
	if loseChair ~= 0 and minIndex ~= 0 then
		--print("loseChair="..loseChair..",userID="..tInfo.situser[loseChair].userid)
		local loseTemp = table.clone(chairCardList[loseChair])
		chairCardList[loseChair] = chairCardList[minIndex]
		chairCardList[minIndex] = loseTemp		
	end
end

--传入两个列表。如果，第一个大，就返回true，否则，返回false


