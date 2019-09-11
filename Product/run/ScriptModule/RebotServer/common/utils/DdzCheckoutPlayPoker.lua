DdzCheckoutPlayPoker = {}

-------检查玩家出的牌是否能压上家（只支持没有癞子的）---------------------
function DdzCheckoutPlayPoker.playCheckout(playusrid, Cardlist, playPokerList, lastuserid, lastpoker, lastType)
	
	--检查是否存在手牌里
	for k, v in ipairs(playPokerList) do
		local isEx = false
		for k1, v1 in ipairs(Cardlist) do
			if v1 == v then
				isEx = true
				break
			end
		end
		if isEx == false then
			return 1, 0, playPokerList  --该牌不存在
		end
	end
	
	local pokerType = DdzCheckoutPlayPoker.GetGeneralCardType(playPokerList)

	--判断牌型
	if pokerType == g_DdzCard_type.invalid then
		return 2, pokerType, playPokerList  	--牌型错误
	end
	--牌比对
	if lastuserid ~= playusrid and lastuserid ~= 0 then
		if false == DdzCheckoutPlayPoker.pokerCompare(pokerType, playPokerList, lastType, lastpoker) then
			return 3, pokerType, playPokerList --牌型打不了上家
		end
	end

	return 0, pokerType, playPokerList
end

--获得牌型_没有癞子的
function DdzCheckoutPlayPoker.GetGeneralCardType(CardList)
	--排序
	DdzCheckoutPlayPoker.DdzLogicSort(CardList)

	if #CardList == 1 then --单张
		return g_DdzCard_type.danzhang
	elseif #CardList == 2 then	--对子
		if CardList[1] == 525 and CardList[2] == 515 then
			return g_DdzCard_type.kongBomb
		end
		return (math.mod(CardList[1], 100) == math.mod(CardList[2], 100)) and g_DdzCard_type.duizi or g_DdzCard_type.invalid
	elseif #CardList == 4 or #CardList == 3 then	--三张牌与炸弹
		local tmp = math.mod(CardList[1], 100)
		local SameNum = 0 
		for i = 1, #CardList do
			if tmp ~= math.mod(CardList[i], 100) then
				break
			end
			SameNum = SameNum + 1
		end
		if SameNum == #CardList then
			return (SameNum == 3) and g_DdzCard_type.szp or g_DdzCard_type.stiffBomb
		end 
		if #CardList == 3  then
			return g_DdzCard_type.invalid
		end
	end
	
	--其他牌型
	if #CardList >= 4 then 
		--分析扑克
		local tagAnalyseResult = DdzCheckoutPlayPoker.AnalysebCardData(CardList) 
		 --四牌判断
		if tagAnalyseResult.fourcount > 0 then --四牌判断
			if (tagAnalyseResult.fourcount == 1) and (#CardList == 6) then
				return g_DdzCard_type.sidaier 
			end
			if (tagAnalyseResult.fourcount == 1) and (tagAnalyseResult.twocount == 2) and (#CardList == 8) then
				return g_DdzCard_type.sidaier 
			end
			return g_DdzCard_type.invalid
		end
		--三牌判断
		if	tagAnalyseResult.threecount > 0 then
			--连牌判断
			if tagAnalyseResult.threecount > 1 then
				if tagAnalyseResult.threelist[1] == 15 then
					return g_DdzCard_type.invalid
				end	
				for i = 2, #tagAnalyseResult.threelist do
					if tagAnalyseResult.threelist[i] ~= (tagAnalyseResult.threelist[1] - (i -1)) then
						return g_DdzCard_type.invalid
					end
				end
			end
			--三顺子
			if (tagAnalyseResult.threecount * 3) == #CardList then 
				return g_DdzCard_type.sanshunzi 
			end
			--三带一 与 飞机
			if tagAnalyseResult.threecount == (#CardList - tagAnalyseResult.threecount * 3) then
				if tagAnalyseResult.threecount == 1 then
					return g_DdzCard_type.sandaiyi 
				else 
					return g_DdzCard_type.feiji
				end
			end
			--三带二 与 飞机
			if (tagAnalyseResult.twocount == tagAnalyseResult.threecount) and (#CardList == (tagAnalyseResult.twocount * 2 + tagAnalyseResult.threecount * 3)) then
				if tagAnalyseResult.twocount == 1 then
					return g_DdzCard_type.sandaier
				else
					return g_DdzCard_type.feiji
				end
			end
		end
		--两张判断
		if tagAnalyseResult.twocount >= 3 then
			if tagAnalyseResult.twolist[1] ~= 15 then
				for i = 2, tagAnalyseResult.twocount do 
					if tagAnalyseResult.twolist[i] ~= (tagAnalyseResult.twolist[1] - (i -1))then
						return g_DdzCard_type.invalid
					end
				end
				if (tagAnalyseResult.twocount * 2) == #CardList then
					return g_DdzCard_type.liandui
				end
			end
			return g_DdzCard_type.invalid
		end
			
		--单张判断
		if (tagAnalyseResult.onecount >= 5) and (tagAnalyseResult.onecount == #CardList) then
			if tagAnalyseResult.onelist[1] >= 15 then
				return g_DdzCard_type.invalid
			end
			for i = 2, #tagAnalyseResult.onelist do
				if tagAnalyseResult.onelist[i] ~= (tagAnalyseResult.onelist[1] - (i - 1)) then
					return g_DdzCard_type.invalid
				end
			end
			return g_DdzCard_type.shunzi
		end
		return g_DdzCard_type.invalid
	end
	return g_DdzCard_type.invalid

end


--按逻辑值排序
function DdzCheckoutPlayPoker.DdzLogicSort(inList)
	if #inList < 2 then
		return inList
	end
	table.sort(inList,function(a,b)    --按点数升序排序后的列表
		local dianshu_a = math.mod(a, 100)
		if dianshu_a == 1 then
			dianshu_a = 14
		elseif dianshu_a == 2 then
			dianshu_a = 15
		end
		local dianshu_b = math.mod(b, 100)
		if dianshu_b == 1 then
			dianshu_b = 14
		elseif dianshu_b == 2 then
			dianshu_b = 15
		end
		return dianshu_a > dianshu_b
	end)
	return inList
end

function DdzCheckoutPlayPoker.AnalysebCardData(pokerList, tagAnalyseResult)
	if tagAnalyseResult == nil or tagAnalyseResult == 0 then
		tagAnalyseResult = st_ddz_pb.ddztaganalyseresult()
	end
	
	local SameCount = 1
	local CardValueTemp = 0
	local logicValue = math.mod(pokerList[1], 100)
	if logicValue == 1 then
		logicValue = 14 
	elseif logicValue == 2 then
		logicValue = 15
	elseif logicValue == 15 then
		logicValue = 16
	end
	local tmp = {}
	for i = 2, #pokerList do
		CardValueTemp = math.mod(pokerList[i], 100)
		if CardValueTemp == 1 then
			CardValueTemp = 14 
		elseif CardValueTemp == 2 then
			CardValueTemp = 15
		elseif CardValueTemp == 15 then
			CardValueTemp = 16
		end
		if SameCount == 1 then
			table.insert(tmp, pokerList[i - 1])
		end
		if logicValue == CardValueTemp then
			SameCount = SameCount + 1
			table.insert(tmp, pokerList[i])
		end
		if CardValueTemp ~= logicValue or i == #pokerList  then
			if SameCount == 2 then
				for k, v in ipairs(tmp) do
					tagAnalyseResult.twocarddata:append(v)
				end
				tagAnalyseResult.twolist:append(logicValue)
				tagAnalyseResult.twocount = tagAnalyseResult.twocount + 1
			elseif  SameCount == 3 then
				for k, v in ipairs(tmp) do
					tagAnalyseResult.threecarddata:append(v)
				end
				tagAnalyseResult.threelist:append(logicValue)
				tagAnalyseResult.threecount = tagAnalyseResult.threecount + 1
			elseif  SameCount == 4 then
				for k, v in ipairs(tmp) do
					tagAnalyseResult.fourlist:append(v)
				end
				tagAnalyseResult.fourlist:append(logicValue)
				tagAnalyseResult.fourcount = tagAnalyseResult.fourcount + 1
			end
			tmp = {}
		end
		
		if CardValueTemp ~= logicValue then
			if SameCount == 1 then
				if i ~= #pokerList then
					tagAnalyseResult.onecarddata:append(pokerList[i - 1])	
					tagAnalyseResult.onelist:append(logicValue)
					tagAnalyseResult.onecount = tagAnalyseResult.onecount + 1
				else
					tagAnalyseResult.onecarddata:append(pokerList[i - 1])
					tagAnalyseResult.onelist:append(logicValue)
					tagAnalyseResult.onecarddata:append(pokerList[i])
					tagAnalyseResult.onelist:append(CardValueTemp)
					tagAnalyseResult.onecount = tagAnalyseResult.onecount + 2
				end
			else
				if i == #pokerList then
					tagAnalyseResult.onecarddata:append(pokerList[i])
					tagAnalyseResult.onelist:append(logicValue)
					tagAnalyseResult.onecount = tagAnalyseResult.onecount + 1
				end
			end
			SameCount = 1
			logicValue = CardValueTemp
		end
	end
	return tagAnalyseResult
end

--牌比对(pokerType_1赢返回true)
function DdzCheckoutPlayPoker.pokerCompare(pokerType_1, pokerList_1, pokerType_2, pokerList_2)
	--检测是否打得过上家
	--类型判断
	if (pokerType_1 >= g_DdzCard_type.softBomb) and  (pokerType_2 < g_DdzCard_type.softBomb) then
		return true
	end
	if (pokerType_1 < g_DdzCard_type.softBomb) and  (pokerType_2 >= g_DdzCard_type.softBomb) then
		return false
	end
	if (pokerType_1 ~= pokerType_2) and (pokerType_1 >= g_DdzCard_type.softBomb) and (pokerType_2 >= g_DdzCard_type.softBomb) then
		return pokerType_1 > pokerType_2 
	end
	--逻辑判断
	if (pokerType_1 ~= pokerType_2) or (#pokerList_1 ~= #pokerList_2) then
		return false
	end
	
	--相同牌型比较
	--飞机或者三带一
	if  pokerType_1 == g_DdzCard_type.sandaiyi or pokerType_1 == g_DdzCard_type.sandaier or pokerType_1 == g_DdzCard_type.feiji then
		local tagAnalyseResult_1 = DdzCheckoutPlayPoker.AnalysebCardData(pokerList_1)
		local tagAnalyseResult_2 = DdzCheckoutPlayPoker.AnalysebCardData(pokerList_2)
		local tmp_1 = DdzCheckoutPlayPoker.get_LogicValues(tagAnalyseResult_1.threelist[1])
		local tmp_2 = DdzCheckoutPlayPoker.get_LogicValues(tagAnalyseResult_2.threelist[1])
		return tmp_1 > tmp_2
	--四带二	
	elseif pokerType_1 == g_DdzCard_type.sidaier then
		local tagAnalyseResult_1 = DdzCheckoutPlayPoker.AnalysebCardData(pokerList_1)
		local tagAnalyseResult_2 = DdzCheckoutPlayPoker.AnalysebCardData(pokerList_2)
		local tmp_1 = DdzCheckoutPlayPoker.get_LogicValues(tagAnalyseResult_1.fourlist[1])
		local tmp_2 = DdzCheckoutPlayPoker.get_LogicValues(tagAnalyseResult_2.fourlist[1])
		return tmp_1 > tmp_2
	else
		local tmp_1 = DdzCheckoutPlayPoker.get_LogicValues(pokerList_1[1])
		local tmp_2 = DdzCheckoutPlayPoker.get_LogicValues(pokerList_2[1])
		return tmp_1 > tmp_2
	end
	return false
end

function DdzCheckoutPlayPoker.get_LogicValues(value)
	local LogicValues = math.mod(value, 100) 
	if LogicValues == 1 then
		LogicValues = 14
	elseif LogicValues == 2 then
		LogicValues = 15
	elseif LogicValues == 15 then
		LogicValues = 24
	end
	return LogicValues
end
