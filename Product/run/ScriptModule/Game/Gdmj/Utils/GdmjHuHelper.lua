
GdmjHuHelper = {}

function GdmjHuHelper.GetHuList(handList, guiPubList, mjType, vipInfo)
	--传入手牌列表，鬼牌列表，牌桌类型，已经贵宾房的信息
	--返回的是，听牌的列表
	--{11,12,32}
	--注意：如果返回的是{-1}  表示全部牌都可以
	
	--table.sort( handList, function(a,b) return a < b end )
	--table.sort( guiPubList, function(a, b) return a < b end )
	local guiLen = 1   --首先，鬼牌的长度是1,因为本来就需要一个鬼牌的
	local cardList = {}
	local lenTemp = #guiPubList
	
	if lenTemp > 0 then		
		for k1,v1 in ipairs(handList) do
			
			local isExist = false
			
			for k2,v2 in ipairs(guiPubList) do
				if v1 == v2 then
					isExist = true
					break
				end
			end
			if isExist == true then
				guiLen = guiLen + 1
			else
				table.insert(cardList, v1)
			end
		end
	else
		cardList = table.clone(handList)
	end
	
	table.sort( cardList, function(a,b) return a < b end )
	if mjType == g_gdmjType.type_tdh then
		--如果是推倒胡
		return GdmjHuHelper.HuListTdh(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_ztdh then
		return GdmjHuHelper.HuListZptdh(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_jph then
		return GdmjHuHelper.HuListJph(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_jbc then
		--走推倒胡的路
		return GdmjHuHelper.HuListJbc(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_hzz or mjType == g_gdmjType.type_hdz or mjType == g_gdmjType.type_hbz then
		return GdmjHuHelper.HuListHzz(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_hzmj then
		return GdmjHuHelper.HuListZptdh(cardList, guiLen, vipInfo, guiPubList)
	elseif mjType == g_gdmjType.type_rpmj then
		return GdmjHuHelper.HuListRpmj(cardList, guiLen, vipInfo, guiPubList)
	end
	
end

function GdmjHuHelper.HuListTdh(handList, guiLen, vipInfo, guiPubList)
	
	local retList = {}
	local allList = {}
	if vipInfo.kehuqidui == 1 then
		--如果推倒胡中可胡七对
		local huType,huList = MajiangHuUtils.QiDui(handList,guiLen)
		if huType ~= 0 then
			if huList == -1 then    --如果是-1，表示直接可以胡了
				return {-1}
			else
				for k,v in ipairs(huList) do
					allList[v] = 1
				end
			end
		end
	end

	local huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
		
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList
	
end

function GdmjHuHelper.HuListRpmj(handList, guiLen, vipInfo, guiPubList)

	local retList = {}
	local allList = {}

	if vipInfo.shisanyao == 1 then
		--十三幺胡牌
		local huType, huList = MajiangHuUtils.ShiSanYao(handList,guiLen)  --十三幺的判断
		if huType ~= 0 then
			for k,v in ipairs(huList) do
				if v == -1 then
					return {-1}
				else
					allList[v] = 1
				end
			end
			for k,v in ipairs(guiPubList) do
				--把鬼牌也加上
				allList[v] = 1
			end			
		end
	end

	local huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
				
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList
	
end

function GdmjHuHelper.HuListZptdh(handList, guiLen, vipInfo, guiPubList)
	
	if guiLen>=5 and vipInfo.siguihupai == 1 then
		--四鬼胡牌,就直接胡牌
		return {-1}
	end
	
	
	
	
	
	local retList = {}
	local allList = {}
	
	if vipInfo.qiduisibei == 1 or vipInfo.kehuqidui == 1 then
		--如果推倒胡中可胡七对
		local huType,huList = MajiangHuUtils.QiDui(handList,guiLen)
		if huType ~= 0 then
			
			if huList == -1 then    --如果是-1，表示直接可以胡了
				return {-1}
			else
				
				for k,v in ipairs(huList) do
					if v == -1 then
						return {-1}
					else
						allList[v] = 1
					end
				end
			end
		end
	end

	if vipInfo.shisanyao == 1 then
		--十三幺胡牌
		local huType, huList = MajiangHuUtils.ShiSanYao(handList,guiLen)  --十三幺的判断
		if huType ~= 0 then
			for k,v in ipairs(huList) do
				if v == -1 then
					return {-1}
				else
					allList[v] = 1
				end
			end
			for k,v in ipairs(guiPubList) do
				--把鬼牌也加上
				allList[v] = 1
			end			
		end
	end

	local huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
				
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList
	
end

function GdmjHuHelper.HuListJph(handList, guiLen, vipInfo, guiPubList)
	
	local retList = {}
	local allList = {}

	local huType,huList = MajiangHuUtils.ShiSanYao(handList, guiLen)
	
	if huType ~= 0 then
		if huList == -1 then
			return {-1}
		end
		return huList
	end
	
	
		--如果推倒胡中可胡七对
	local huType,huList = MajiangHuUtils.JiuLianBaoDeng(handList,guiLen)
	if huType ~= 0 then
		
		if huList == -1 then    --如果是-1，表示直接可以胡了
			return {-1}
		else
			
			for k,v in ipairs(huList) do
				
				allList[v] = 1
				
			end
		end
	end

	

	local huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
				
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList
	
end

function GdmjHuHelper.HuListJbc(handList, guiLen, vipInfo, guiPubList)
	local retList = {}
	local allList = {}

		--如果推倒胡中可胡七对
	local huType,huList = MajiangHuUtils.QiDui(handList,guiLen)
	if huType ~= 0 then
		
		if huList == -1 then    --如果是-1，表示直接可以胡了
			return {-1}
		else
			
			for k,v in ipairs(huList) do
				
				allList[v] = 1
				
			end
		end
	end
	

	huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
		
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList	
end


function GdmjHuHelper.HuListHzz(handList, guiLen, vipInfo, guiPubList)
	

	local retList = {}
	local allList = {}
	

	if vipInfo.kehuqidui == 1 then
		--可胡七对的情况下
		huType,huList = MajiangHuUtils.QiDui(handList,guiLen)
		if huType ~= 0 then
			for k,v in ipairs(huList) do
				if v == -1 then
					return {-1}
				else
					allList[v] = 1
				end
			end
			for k,v in ipairs(guiPubList) do
				--把鬼牌也加上
				allList[v] = 1
			end			
			for k,v in pairs(allList) do
				table.AscInsert(retList,k)
			end
			--这里不能返回啊
			--return retList			
		end		
	end

	--十三幺胡牌
	local huType, huList = MajiangHuUtils.ShiSanYao(handList,guiLen)  --十三幺的判断
	if huType ~= 0 then
		for k,v in ipairs(huList) do
			if v == -1 then
				return {-1}
			else
				allList[v] = 1
			end
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end			
	end

	huType,huList = MajiangHuUtils.CommonHu(handList,guiLen)
	
	if huType ~= 0 then
				
		for k,v in ipairs(huList) do
			if v == -1 then    --如果存在-1的情况，就直接返回了
				return {-1}
			else
				allList[v] = 1
			end
			
		end
		for k,v in ipairs(guiPubList) do
			--把鬼牌也加上
			allList[v] = 1
		end
	end
	
	for k,v in pairs(allList) do
		table.AscInsert(retList,k)
	end

	return retList
	
end


function GdmjHuHelper.IsQiDui(tItem, pos)
	
	if #tItem.m_userList[pos].handpoker ~= 14 then
		--如果长度不对，肯定就不行了
		return 0
	end

	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	--MajiangUtils.Get55CardSList()
	local guiLen = 0   --首先，鬼牌的长度是1,因为本来就需要一个鬼牌的
	local cardList = {}
	local lenTemp = #guiPubList
	
	if lenTemp > 0 then
		for k1,v1 in ipairs(handList) do
			local isExist = false
			for k2,v2 in ipairs(guiPubList) do
				if v1 == v2 then
					isExist = true
					break
				end
			end
			if isExist == true then
				guiLen = guiLen + 1
			else
				table.insert(cardList, v1)
			end
		end
	end
	
	local list55 = MajiangUtils.Get55CardSList(cardList)
	if guiLen >= #cardList then
		return g_gdmjWinType.type_qixiaodui
	end
	return 0
end

function GdmjHuHelper.IsSiGui(tItem, pos)  --四鬼胡牌
	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	local guiLen = 0
	if #guiPubList > 0 then
		for k1,v1 in ipairs(handList) do
			for k2,v2 in ipairs(guiPubList) do
				if v1 == v2 then
					guiLen = guiLen + 1
					break
				end
			end
		end
	end
	if guiLen >= 4 then
		return g_gdmjWinType.type_siguihupai
	end
	return 0
end

function GdmjHuHelper.IsQingYiSe(tItem, pos)
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)	
	--local guiLen = 0   --首先，鬼牌的长度是1,因为本来就需要一个鬼牌的
	--local cardList = {}
	local cardType = 0
	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		
		if isExist == false then
			--表示不是鬼牌
			if cardType == 0 then
				cardType = math.floor(v1/10)   --取得卡牌的类型
				
			else
				if cardType ~= math.floor(v1/10) then
					return 0
				end
			end
			
		end
		
	end
	
	--然后还是需要检查，碰杠，吃的牌
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_peng or v.pokertype == g_gdmjAction.type_gang then
			--这里只是检查第一个就行了
			if cardType == 0 then
				cardType = math.floor(v.pokerlist[1]/10)   --取得卡牌的类型
				
			else
				if cardType ~= math.floor(v.pokerlist[1]/10) then
					return 0
				end
			end			
		elseif v.pokertype == g_gdmjAction.type_chi then
			
			for k2,v2 in ipairs(v.pokerlist) do
				if cardType == 0 then
					cardType = math.floor(v2/10)   --取得卡牌的类型
					
				else
					if cardType ~= math.floor(v2/10) then
						return 0
					end
				end					
			end
		end
	end
	return g_gdmjWinType.type_qingyise    --这里就是清一色了
end

function GdmjHuHelper.IsQuanFeng(tItem, pos)
	--全风
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)	
	--local guiLen = 0   --首先，鬼牌的长度是1,因为本来就需要一个鬼牌的
	--local cardList = {}
	local cardType = 0
	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == false then
			--表示不是鬼牌
			if v1 < 40 then   --40以下的就不是风了
				return 0
			end
		end
	end
	
	--然后还是需要检查，碰杠，吃的牌
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_peng or v.pokertype == g_gdmjAction.type_gang then
			--这里只是检查第一个就行了
			if v.pokerlist[1] < 40 then
				return 0
			end
	
		elseif v.pokertype == g_gdmjAction.type_chi then
			
			for k2,v2 in ipairs(v.pokerlist) do
				if v2 < 40 then
					return 0
				end
			end
		end
	end
	return g_gdmjWinType.type_quanfeng    	
end

function GdmjHuHelper.IsPengPengHu(tItem, pos)
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			--这个是吃
			return 0
		end
	end

	local cardList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	--MajiangUtils.Get55CardSList()
	local guiLen = 0   --首先，鬼牌的长度是1,因为本来就需要一个鬼牌的
	--local cardList = {}
	local cardTempList = {}
	local lenTemp = #guiPubList
	if lenTemp > 0 then
		for k1,v1 in ipairs(cardList) do
			local isExist = false
			for k2,v2 in ipairs(guiPubList) do
				if v1 == v2 then
					isExist = true
					break
				end
			end
			if isExist == true then
				guiLen = guiLen + 1
			else
				table.insert(cardTempList, v1)
			end
		end
	end
	
	local list555 = MajiangUtils.Get555CardSList(cardTempList)
	--取到三个后剩下的牌型
	table.sort(cardTempList, function(a,b)   --先排序
	return a < b
	end)
	--到这里，cardList不可能为0的，如果为0，就悲剧了
	if #cardTempList == 0 then
		--如果这里等于0，表示有一对鬼牌
		return g_gdmjWinType.type_pengpenghu
	end
	
	local typeNum = 1 --计算牌型个数
	local cardType = cardTempList[1] --取第一个，从第二个算起
	for i = 2,#cardTempList do
		if cardTempList[i] ~= cardType then
			typeNum = typeNum + 1
			cardType = cardTempList[i]
		end
	end
	
	--取得个数之后，就开始算，差几个才能凑成3X+2
	
	local needNum = typeNum*3 - 1 - #cardTempList  --一定要明白这个算法
	
	if needNum > guiLen then
		return 0
	end
	return g_gdmjWinType.type_pengpenghu
	
end

function GdmjHuHelper.IsHunYiSe(tItem, pos)
	--这是混一色
	local checkType = 0
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] < 40 then
			if checkType == 0 then
				--这里取得要检查的类型
				checkType = math.floor(v.pokerlist[1]/10)
			elseif checkType ~= math.floor(v.pokerlist[1]/10) then
				return 0
			end
		end
	end
	
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		
		if v < 40 then
			if checkType == 0 then
				checkType = math.floor(v/10)
			elseif checkType ~= math.floor(v/10) then
				return 0
			end
		end
	end
	return g_gdmjWinType.type_hunyise
end
function GdmjHuHelper.IsQingYaoJiu(tItem, pos)
	--是否是清幺九，意思是，全部牌都是幺九
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			
			for k2,v2 in ipairs(v.pokerlist) do
				if math.mod(v2,10) ~= 1 and math.mod(v2,10) ~= 9 then
					return 0
				end
			end
		else
			if math.mod(v.pokerlist[1],10) ~= 1 and math.mod(v.pokerlist[1],10) ~= 9 then
				return 0
			end
		end
	end

	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local cardList = {}

	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == false then
			if v1 > 40 then
				return 0
			elseif math.mod(v1,10) ~= 1 and math.mod(v1,10) ~= 9 then
				return 0
			end
		end
	end

	return g_gdmjWinType.type_qingyaojiu
end
function GdmjHuHelper.IsHunYaoJiu(tItem, pos)
	--混幺九，只要含有幺九即可
	--
	local hasList = {}  --至少要含有一个幺

	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			
			for k2,v2 in ipairs(v.pokerlist) do
				
				
				
				if math.mod(v2,10) ~= 1 and math.mod(v2,10) ~= 9 then
					if v2 < 40 then
						return 0
					end
				else
					hasList[math.mod(v2,10)] = 1
				end
			end
		else
			if math.mod(v.pokerlist[1],10) ~= 1 and math.mod(v.pokerlist[1],10) ~= 9 then
				if v.pokerlist[1] < 40 then
					return 0
				end
			else
				hasList[math.mod(v.pokerlist[1],10)] = 1
			end
		end
	end

	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local cardList = {}

	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == false then
			if math.mod(v1,10) ~= 1 and math.mod(v1,10) ~= 9 then
				if v1 < 40 then
					return 0
				end
			else
				hasList[math.mod(v1,10)] = 1
			end
		end
	end

	local hasLen = 0
	for k,v in pairs(hasList) do
		hasLen = hasLen + 1
	end
	if hasLen < 2 then
		--一定要含有幺九
		return 0
	end
	return g_gdmjWinType.type_hunyaojiu	
end

function GdmjHuHelper.IsShiSanYao(tItem, pos)
	--在最后赢牌的时候，判断是否是十三幺
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local markList = {11,19,21,29,31,39,41,42,43,44,51,52,53}
	
	local guiLen = 0
	for k1,v1 in ipairs(handList) do
		
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == true then
			guiLen = guiLen + 1
		else
			for k3,v3 in ipairs(markList) do
				if v1 == v3 then
					table.remove(markList, k3)
					break
				end
			end
		end
	end
	
	if guiLen >= #markList then
		return g_gdmjWinType.type_shisanyao
	end
	return 0
end

function GdmjHuHelper.IsDaOrXiaoSiXi(tItem, pos)
	--判断是否是大或者小四喜
	local checkList = {0,0,0,0}
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] > 40 and v.pokerlist[1] < 50 then
			checkList[ math.mod(v.pokerlist[1],10) ] = 3
		end
	end
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		
		if v > 40 and v < 50 then
			checkList[ math.mod(v,10) ] = checkList[ math.mod(v,10) ] + 1
		end
		
	end
	
	local retType = g_gdmjWinType.type_dasixi
	local maxNum = 0
	for i = 1,tItem.m_maxUser do
		
		if checkList[i] < 2 then
			return 0
		elseif checkList[i] == 2 then
			retType = g_gdmjWinType.type_xiaosixi
		else
			maxNum = maxNum + 1
		end
	end
	
	if maxNum == 0 then
		return 0
	else
		return retType
	end
end

function GdmjHuHelper.IsDaOrXiaoSanYuan(tItem, pos)
	--判断是否是大或者小四喜
	local checkList = {0,0,0}
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] > 50 and v.pokerlist[1] < 60 then
			checkList[ math.mod(v.pokerlist[1],10) ] = 3
		end
	end
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		
		if v > 50 and v < 60 then
			checkList[ math.mod(v,10) ] = checkList[ math.mod(v,10) ] + 1
		end
		
	end
	
	local retType = g_gdmjWinType.type_dasanyuan
	local maxNum = 0
	for i = 1,3 do
		
		if checkList[i] < 2 then
			return 0
		elseif checkList[i] == 2 then
			retType = g_gdmjWinType.type_xiaosixi
		else
			maxNum = maxNum + 1
		end
	end
	
	if maxNum == 0 then
		return 0
	else
		return retType
	end	
end

function GdmjHuHelper.IsZiYiShe(tItem,pos)
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] < 40 then
			return 0
		end
	end
	
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		if v < 40 then
			return 0
		end
	end
	return g_gdmjWinType.type_ziyise
end

function GdmjHuHelper.IsPingHu(tItem, pos)
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype ~= g_gdmjAction.type_chi then
			return 0
		end
	end
	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	
	local cardList = {}
	cardList[1] = {}
	cardList[2] = {}
	cardList[3] = {}
	cardList[4] = {}
	for k,v in ipairs(handList) do
		
		if v > 40 then
			table.insert(cardList[4], v)
		else
			table.insert(cardList[ math.floor(v/10) ],v)
		end
		
	end
	
	if #cardList[4] > 2 then
		--在这里，字牌只能做将，如果超过两个，肯定不行了。
		return 0
	end
	
	for i = 1,3 do
		
		if math.mod(#cardList[i],3) == 2 then
			--这里是有一对做将	
			local listArr = MajiangUtils.Get55PairList(cardList[i])  --先取出要做将的列表
			local isExist = false
			for k,v in pairs(listArr) do
				
				local list456 = MajiangUtils.Get456CardSArrList(v)
				if #v == 0 then
					isExist = true
					break
				end
			end
			if isExist == false then
				return 0
			end
		else
			local list456 = MajiangUtils.Get456CardSArrList(cardList[i])
			
			if #cardList[i] ~= 0 then
				return 0
			end
		end
	end
	return g_gdmjWinType.type_pinghu
end


function GdmjHuHelper.IsZiYiShe(tItem,pos)
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] < 40 then
			return 0
		end
	end
	
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		if v < 40 then
			return 0
		end
	end
	return g_gdmjWinType.type_ziyise
end

function GdmjHuHelper.HzzCheckBeforeJihu(tItem, pos, pokerID)
	--该函数是插入胡牌列表前做的判断	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local cardList = {}
	cardList[1] = {}
	cardList[2] = {}
	cardList[3] = {}
	cardList[4] = {}
	
	table.AscInsert(handList, pokerID)
	
	local markList = {11,19,21,29,31,39,41,42,43,44,51,52,53}
	local markList2 = {11,19,21,29,31,39,41,42,43,44,51,52,53}
	--先判断是不是十三幺
	local isShiSanYao = 2
	if #handList == 14 then
		for k,v in ipairs(handList) do
			local isExist = false
			for i = 1,#markList do
				if v == markList[i] then
					isExist = true
					table.remove(markList, i)
					break
				end
			end
			
			if isExist == false then
				for i = 1,#markList2 do
					if markList2[i] == v and isShiSanYao == 2 then
						isExist = true
						isShiSanYao = 1
						break
					end
				end
			end
			
			if isExist == false then
				isShiSanYao = 0
			end
		end
	else
		isShiSanYao = 0
	end
	
	if isShiSanYao == 1 then
		return false
	end
	
	for k,v in ipairs(handList) do
		
		if v > 40 then
			table.insert(cardList[4], v)
		else
			table.insert(cardList[ math.floor(v/10) ],v)
		end
		
	end


	local typeLen = 0
	local isZase = false
	for i = 1,3 do
		if #cardList[i] > 0 then
			typeLen = typeLen + 1
			
			if isZase == false then
				-- 
				local tempList = {}
				for k,v in ipairs(cardList[i]) do
					if tempList[v] == nil then
						tempList[v] = 1
					else
						tempList[v] = tempList[v] + 1
					end
				end
				
				local tempLen = 0
				local tempNum = 0
				for k,v in pairs(tempList) do
					tempLen = tempLen + 1
					tempNum = tempNum + v
				end
				
				local tempNumLen = math.floor(tempNum/3)
				if 0 ~= math.mod(tempNum,3) then   --这里说明是7个牌的，将牌在这里
					tempNumLen = tempNumLen + 1
				end
				
				if tempNumLen < tempLen then
					isZase = true
				end
			end
		end
	end
	
	
	if isZase == true and typeLen > 1 then
		--这样就是鸡胡了
		return true
	end
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_peng or v.pokertype == g_gdmjAction.type_gang then
			if v.pokerlist[1] < 40 then
				local typeTemp = math.floor(v.pokerlist[1]/10)
				if #cardList[typeTemp] == 0 then
					table.insert(cardList[typeTemp],v.pokerlist[1])
					typeLen = typeLen + 1
				end
			end
		end
	end
	if isZase == true and typeLen > 1 then
		--这样就是鸡胡了
		return true
	end
	
	return false
end

function GdmjHuHelper.IsDaiFeng(tItem,pos)
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] > 40 then
			return 1
		end
	end
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		if v > 40 then
			return 1
		end
	end
	return 0
end

function GdmjHuHelper.IsZaSe(tItem, pos)
	--杂色：全是一色和风的鸡胡
	local cardType = 0
	
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		local tempType = math.floor(v/10)
		if tempType < 4 then
			
			if cardType == 0 then
				cardType = tempType
			elseif cardType ~= tempType then
				return 0
			end
			
		end
	end
	
	for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
		local tempType = math.floor(v.pokerlist[1]/10)
		if tempType < 4 then
			
			if cardType == 0 then
				cardType = tempType
			elseif cardType ~= tempType then
				return 0
			end
		end
	end
	return 1
end


function GdmjHuHelper.HzzZiYaoJiu(tItem, pos)
	--判断惠州庄是字一色还是清幺九
	--惠州庄的清幺九,幺九的清一色+字牌

	--是否是清幺九，意思是，全部牌都是幺九
	
	local pokerList = {0,0,0,0,0}
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			return 0
		else
			if v.pokerlist[1] < 40 then
				if math.mod(v.pokerlist[1],10) ~= 1 and math.mod(v.pokerlist[1],10) ~= 9 then
					return 0
				end
			end
			local pokerType = math.floor(v.pokerlist[1]/10)
			pokerList[pokerType] = pokerList[pokerType] + 1
		end
	end

	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local cardList = {}

	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == false then
			if v1 < 40 then
				if math.mod(v1,10) ~= 1 and math.mod(v1,10) ~= 9 then
					return 0
				end
			end
			local pokerType = math.floor(v1/10)
			pokerList[pokerType] = pokerList[pokerType] + 1
		end
		
	end
	--如果走到这里，说明肯定是幺九的牌型了,至于是杂幺九，清幺九，还是全幺九呢
	
	local lenTemp = 0
	for i = 1,3 do
		if pokerList[i] ~= 0 then
			lenTemp = lenTemp + 1
		end
	end	
	
	if lenTemp == 0 then
		--说明全是字牌
		return g_gdmjWinType.type_ziyise
	end
	
	
	if pokerList[4] == 0 and pokerList[5] == 0 then
		--说明没有风牌
		return g_gdmjWinType.type_hzz_quanyaojiu
	else
		--三个里面有两个是0
		if lenTemp == 1 then
			--清幺九
			return g_gdmjWinType.type_hzz_qingyaojiu
		else
			return g_gdmjWinType.type_hzz_zayaojiu
		end
	end

	return g_gdmjWinType.type_qingyaojiu		
end


function GdmjHuHelper.HzzQingYaoJiu(tItem, pos)
	--惠州庄的清幺九,幺九的清一色+字牌

	--是否是清幺九，意思是，全部牌都是幺九
	
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_chi then
			
			for k2,v2 in ipairs(v.pokerlist) do
				if math.mod(v2,10) ~= 1 and math.mod(v2,10) ~= 9 then
					return 0
				end
			end
		else
			if math.mod(v.pokerlist[1],10) ~= 1 and math.mod(v.pokerlist[1],10) ~= 9 then
				return 0
			end
		end
	end

	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	local guiPubList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local cardList = {}

	for k1,v1 in ipairs(handList) do
		local isExist = false
		for k2,v2 in ipairs(guiPubList) do
			if v1 == v2 then
				isExist = true
				break
			end
		end
		if isExist == false then
			if v1 > 40 then
				return 0
			elseif math.mod(v1,10) ~= 1 and math.mod(v1,10) ~= 9 then
				return 0
			end
		end
	end

	return g_gdmjWinType.type_qingyaojiu	
end

function GdmjHuHelper.DaChuhzjiafen(tItem, pos)   --打出红中加分
	local count = 0
	for k,v in ipairs(tItem.m_userList[pos].mjpokerlist) do
		if v.pokerlist[1] == 51 then
			--return 1
			count = #v.pokerlist
		end
	end	
	for k,v in ipairs(tItem.m_userList[pos].outpoker) do
		if v == 51 then
			--return 1
			count = count + 1
		end
	end			
	return count
end

function GdmjHuHelper.GskhJiama(tItem, pos)   --杠上开花加码
	return 0
end
function GdmjHuHelper.WuhzJiafen(tItem, pos)   --无红中加分
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		if v == 51 then
			return 0
		end
	end		
	return 1
end



