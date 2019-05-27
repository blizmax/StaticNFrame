--广东麻将的ai
--利用加权系数对手上的13张牌进行权重比对
--基本规则：AAA三条的最大,顺子的次子，有杠必杠，已听不碰
--AAA 顺子 一对 散牌


GdmjRobot = {}

local Com_555 = {}       --每次play的时候，初始化的列表
local Com_456 = {}
local Com_55 = {}
local Com_5 = {}
local Com_WeightList = {}
local Com_guiList = {}

function GdmjRobot.CountWeight()

	Com_WeightList = {}
	
	local function AddWeight(pokerID, weight)
		
		for k,v in ipairs(Com_WeightList) do
			if v[1] == pokerID then
				v[2] = v[2] + weight
				return k
			end
		end
		local weightItem = {}
		table.insert(weightItem, pokerID)
		table.insert(weightItem, weight)
		table.insert(Com_WeightList, weightItem)
	end
	
	
	for k,v in ipairs(Com_5) do
		local cardType = math.floor(v/10)
		local cardID = math.mod(v, 10)
		if cardType <= 3 then
			--如果是万，筒，索
			AddWeight(v, 1)
			if cardID ==1 or cardID == 9 then
				AddWeight(v, 1)
			elseif cardID == 2 or cardID == 8 then
				AddWeight(v, 2)
			else
				AddWeight(v, 3)
			end
			if k >= 2 and Com_5[k] == (Com_5[k - 1] + 1) then
				AddWeight(Com_5[k], 3)
				AddWeight(Com_5[k - 1], 3)
			end
			for k2,v2 in ipairs(Com_456) do   --如果能跟顺子组成一队
				if v == v2[1] or v == v2[3] then
					AddWeight(v, 3)
				elseif v == v2[1] - 1 or v == v2[3] + 1 then
					AddWeight(v, 2)
				end
			end
			
			for k2,v2 in ipairs(Com_55) do
				if v == v2[1] - 1 or v == v2[1] + 1 then
					AddWeight(v, 4)
				end
			end
			
		else
			
			AddWeight(v, 0)
		end
	end

	
	table.sort(Com_WeightList, function(a,b)
		return a[2] < b[2]
	end)
	
end


function GdmjRobot.CardAnalysis(tItem, pos)
	
	Com_guiList = {}
	Com_456 = {}
	Com_555 = {}
	Com_55 = {}
	Com_5 = {}
	
	for k1,v1 in ipairs(tItem.m_userList[pos].handpoker) do
		local isGui = false
		
		if tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
			if tItem.m_tInfo.tabletype == g_gdmjTableType["medium"] or tItem.m_tInfo.tabletype == g_gdmjTableType["serior"] then
				
				for k2,v2 in ipairs(tItem.m_tInfo.guilist) do
					if v1 == v2 then
						isGui = true
						break
					end
				end
			end
		end
		if isGui == true then
			table.insert(Com_guiList, v1)
		else
			table.insert(Com_5,v1)
		end
	end
	
	
	Com_555 = MajiangUtils.Get555CardSArrList(Com_5)
	
	--这里是取得顺子的。先取3条的，多余的在空出来
	Com_456 = MajiangUtils.GetRobot456CardSArrList(Com_5)
	
	--取得对子的列表
	Com_55 = MajiangUtils.Get55CardSArrList(Com_5)
	
end


function GdmjRobot.Play(tItem, pos, pokerID)
	--参数
	--机器人开始行动了	
	--首先从缓存中取出自己的东西
	
	
	GdmjRobot.CardAnalysis(tItem, pos)
	GdmjRobot.CountWeight()
	
	local actType = 0
	local pengType = 0
	local gangType = 0
	
	local huType = 0
	for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
		if v == g_gdmjAction.type_peng then
			pengType = v
		elseif v == g_gdmjAction.type_gang then
			gangType = v
		elseif v == g_gdmjAction.type_hu or v == g_gdmjAction.type_qiangganghu or v == g_gdmjAction.type_chihu then
			huType = v
		end
	end

	if huType ~= 0 then
		--如果判断到的类型
		return huType, tItem.m_tInfo.nextinfo.actpokerid
	end
	
	if gangType ~= 0 then
		return gangType, tItem.m_tInfo.nextinfo.actpokerid
	end

	if pengType == g_gdmjAction.type_peng then
		--碰的情况
		return GdmjRobot.PlayPeng(tItem, pos, pokerID)
		--
	end
	
	--全部的决策放在这里
	return GdmjRobot.PlayDetail(tItem,pos,pokerID)
end


function GdmjRobot.PlayPeng(tItem, pos, pokerID)

	
	--for tItem.m_tInfo.
	

	if #tItem.m_userList[pos].tinglist > 0  then --如果已经听了，就不碰了
		--已经停牌了，则不碰了
		return g_gdmjAction.type_guo,pokerID
	end
	if #Com_55 > 1 then
		--如果是两对以上，则一定碰
		return g_gdmjAction.type_peng,pokerID
	end
	
	--如果是一对情况，先暂时返回碰。后面再分析
	return g_gdmjAction.type_peng,pokerID
end

function GdmjRobot.PlayDetail(tItem, pos, pokerID)
	--决策的方法
	
	--这个是打牌的
	--先把牌放到机器里面
	--
	local retPokerID = 0
	

	if #Com_WeightList > 0 then   --
		local weight = Com_WeightList[1][2]
		local retIDList = {}
		for k,v in ipairs(Com_WeightList) do
			if v[2] ~= weight then
				break
			else
				table.insert(retIDList, v[1])
			end
		end
		--在这里开始判断其他的权重

		--先随机取一个
		
		local ranDom = math.myrandom(1,#retIDList)
		retPokerID = retIDList[ranDom]
		
	else  --如果长度为0，就是没有散牌，只有一种情况，碰后需要打一张牌
		
		if #Com_55 == 1 then   --如果只有一对了。那就不用凑对子了，从顺子里面挑一张
			
			if #Com_456 > 0 then   --如果存在顺子，则从顺子里面挑一个出来打，暂时是随机的，后面加入对还剩下几张牌的判断
			
				local ranDom = math.myrandom(1,#Com_456)
				local index = 1
				if math.mod(Com_456[ranDom][3], 10) == 9 then
					index = 3
				end
				for k,v in ipairs(Com_456[ranDom]) do
					if index == k then
						retPokerID = Com_456[ranDom][index]
					else
						table.insert(Com_5, Com_456[ranDom][index])  --插入到散牌列表中
					end
				end
				
				table.remove(Com_456, ranDom) --同时删除
			else
				retPokerID = Com_55[1][1]
				table.remove(Com_55, 1)
			end
		elseif #Com_55 > 1 then --如果对子的长度，则随机从对子里面哪一个出来打
			
			local ranDom = math.myrandom(1, #Com_55)
			retPokerID = Com_55[ranDom][1]
			table.insert(Com_5, Com_55[ranDom][1])
			table.remove(Com_55, ranDom)
		else
			retPokerID = tItem.m_userList[pos].handpoker[1]
			LogFile("error", "GdmjRobot.PlayDetail handpoker="..#tItem.m_userList[pos].handpoker)
		end
	
	end
		
	return g_gdmjAction.type_play, retPokerID
end

