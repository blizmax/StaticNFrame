module("PszText", package.seeall)

function chupai(tInfo, pokerList)
	if pokerList == nil then
		return false 
	end
	
	--获得牌型
	local pType = GetPokerType(pokerList)
	if pType == PszText.shibai then
		return false
	end
	--排序
	local sortlist =  chupaiSort(pokerList)
	
	--判断能不能出牌
	if false == audoMatching(tInfo, sortlist, pType) then
		return false
	end
	
	tInfo.pType = pType
	tInfo.userid = 1515
	--抹去旧的牌
	while #tInfo.pList > 0 do
		tInfo.pList:remove(1)
	end
	--记得新的牌
	for k, v in ipairs(pokerList) do
		addItem = tInfo.pList:append(v)
	end

	return true
	
	
end

--判断牌型
function GetPokerType(pokerList)
	local dianList = {}			--点数列表
	local dianListLen = 0		--点数列表长度
	table.sort(pokerList,function(a,b)    --按点数升序排序后的列表
		local dianshu_a = math.mod(a, 100)
		local dianshu_b = math.mod(b, 100)
		return dianshu_a < dianshu_b
	end)
	
	for i = 1,#pokerList do
		local dianshu = math.mod(pokerList[i], 100)
		if dianList[dianshu] == nil then 
			dianList[dianshu] = 1
			dianListLen = dianListLen + 1	
		else
			dianList[dianshu] = dianList[dianshu] + 1
		end
	end
	
		--按手牌数量分类去计算牌型
	if #pokerList == 1 then
		return PszText.danpai   		--单牌

	elseif #pokerList == 2 then
		if 	huaListLen == 1 then
			if pokerList[1] == 514 or pokerList[1] == 515 then
				return PszText.wangzha	--王炸
			end
		end
		if dianListLen == 1 then
			return PszText.duizi		--对子
		end

	elseif #pokerList == 3 then
		if dianListLen == 1 then
			return PszText.sanzhang		--三张
		end

	elseif #pokerList == 4 then
		if dianListLen == 1 then
			return PszText.zhadan		--炸弹
		elseif dianListLen == 2 then
			for k,v in pairs(dianList) do 
				if v == 3 or v == 1 then 
					return PszText.sandaiyi		--三带一
				end
			end
		end
	elseif #pokerList == 5 then
		if dianListLen == 2 then
			return PszText.sandaier			--三带二
		end
		
	elseif #pokerList == 6 then
		if dianListLen == 2 then
			for k,v in pairs(dianList) do 
				if v == 2 or v == 4 then 
					return PszText.sidaier			--四带二
				end
			end
		end
	end
	
	local max = math.mod(pokerList[#pokerList], 100)
	local min =  math.mod(pokerList[1], 100)
	
	
	--判断是不是顺子
	if #pokerList >= 5 then
		if dianListLen == #pokerList then
			
			if math.abs(max - min) == #pokerList - 1 then
				return PszText.suizi				--顺子
			end
		end
	end
	--判断是不是连对
	if #pokerList >= 6 and math.mod(#pokerList, 2) == 0 then
		if dianListLen == #pokerList/2 then 
			local tmp = 0
			for k,v in pairs(dianList) do 
				if v == 2 then 
					tmp = tmp + 1		
				end
			end
			if tmp == #pokerList/2 and math.abs(max - min) == #pokerList/2 - 1 then
				return PszText.liandui				--连对
			end
		end
	end
	--判断是不是三顺
	if #pokerList >= 9 and math.mod(#pokerList, 3) == 0 then
		if dianListLen == #pokerList/3 then 
			local tmp = 0
			for k,v in pairs(dianList) do 
				if v == 3 then 
					tmp = tmp + 1		
				end
			end
			if tmp == #pokerList/3 and math.abs(max - min) == #pokerList/3 - 1 then
				return PszText.sansui				--三顺
			end
		end
	end
	--判断是不是火箭
	if #pokerList >= 8 and math.mod(#pokerList, 4) == 0 then
		if dianListLen == #pokerList/4 then 
			local tmp = 0
			for k,v in pairs(dianList) do 
				if v == 4 then 
					tmp = tmp + 1		
				end
			end
			if tmp == #pokerList/4 and math.abs(max - min) == #pokerList/4 - 1 then
				return PszText.huojian				--火箭
			end
		end
	end
	--判断是不是飞机
	if #pokerList >= 12 then
		local tmp3 = 0
		local tmp2 = 0
		local tmp = {}
		for k,v in pairs(dianList) do 
			if v == 3 then 
				table.insert(tmp, k)
				tmp3 = tmp3 + 1	
			elseif v == 2 then
				tmp2 = tmp2 + 1	
			end
		end
		
		table.sort(tmp)
		--看看是不是连续的
		if tmp3 >= 3 and math.abs(tmp[tmp3] - tmp[1]) == tmp3 - 1 then
			if #pokerList == (tmp3 * 3) + tmp3 then
				return PszText.feiji 				--飞机
			elseif #pokerList == (tmp3 * 3) + (tmp3 * 2) then
				if tmp3 == tmp2 then
					return PszText.feijidaier 
				end
			end
		end
	end
	return PszText.shibai
	
end

--判断能否出牌
function audoMatching(tInfo, palyPoker, pokerType)
	
	--轮一轮没打 或者 第一个出牌
	if tInfo.userid == tInfo.actionuser or tInfo.pType == 0 then
		return true
	end
	
	--上家出火箭 
	if tInfo.pType == PszText.huojian and pokerType == PszText.huojian then
		if #tInfo.pList == #palyPoker then
			return ComparePoker(palyPoker[1], tInfo.pList[1])
		end
	--上家出王炸	
	elseif tInfo.pType == PszText.wangzha and pokerType == PszText.huojian then
		return true
	--上家出炸弹
	elseif tInfo.pType == PszText.zhadan and pokerType >= PszText.zhadan then
		if pokerType > PszText.zhadan then
			return true
		elseif pokerType == PszText.zhadan then
			return ComparePoker(palyPoker[1], tInfo.pList[1])
		end

	--上家出小于炸弹
	elseif tInfo.pType <= PszText.zhadan and pokerType >= PszText.zhadan then
		return true
	end
	
	--其他能出牌的情况
	--都出单牌
	if #tInfo.pList == #palyPoker then
		local isok = false
		if tInfo.pType == PszText.danpai and pokerType == PszText.danpai then
			isok = true
		--都出单牌
		elseif tInfo.pType == PszText.danpai and pokerType == PszText.danpai then
			isok = true
		--都出顺子
		elseif tInfo.pType == PszText.suizi and pokerType == PszText.suizi then
			isok = true
		--都出对子
		elseif tInfo.pType == PszText.duizi and pokerType == PszText.duizi then
			isok = true
		--都出连对
		elseif tInfo.pType == PszText.liandui and pokerType == PszText.liandui then
			isok = true
		--都出三张
		elseif tInfo.pType == PszText.sanzhang and pokerType == PszText.sanzhang then
			isok = true
		--都出三带一
		elseif tInfo.pType == PszText.sandaiyi and pokerType == PszText.sandaiyi then
			isok = true
		--都出三带二
		elseif tInfo.pType == PszText.sandaier and pokerType == PszText.sandaier then
			isok = true
		--都出三顺子
		elseif tInfo.pType == PszText.sansui and pokerType == PszText.sansui then	
			isok = true
		--都出飞机带一
		elseif tInfo.pType == PszText.feiji and pokerType == PszText.feiji then
			isok = true
		--都出飞机带二
		elseif tInfo.pType == PszText.feijidaier and pokerType == PszText.feijidaier then
			isok = true
		--都出四带二
		elseif tInfo.pType == PszText.sidaier and pokerType == PszText.sidaier then
			isok = true
		end
		if isok == true then
			return ComparePoker(palyPoker[1], tInfo.pList[1])
		end
	end 
	return false
end

--牌型排序
function chupaiSort(pokerList)
	local dianList = {}			--点数列表
	for i = 1,#pokerList do
		local dianshu = math.mod(pokerList[i], 100)
		if dianList[dianshu] == nil then 
			dianList[dianshu] = {pokerList[i]}
		else
			table.insert(dianList[dianshu], pokerList[i])
		end
	end
	local tmp3 = {}
	local tmp = {}
	for k,v in pairs(dianList)do
		if #v >= 3 then
			table.insert(tmp3, k)
		else 
			table.insert(tmp, k)
		end
	end
	table.sort(tmp3, function(a,b)return (a> b) end)
	table.sort(tmp, function(a,b)return (a> b) end)
	local sortlist = {}
	for k,v in ipairs(tmp3) do
		for c, b in ipairs(dianList[v])do
			table.insert(sortlist, b)
		end
	end
	for k,v in ipairs(tmp) do
		table.insert(sortlist, dianList[v][1])
	end
	return sortlist
end

--比较两张牌的大小
function ComparePoker(a, b)
	local dian_a = math.mod(a, 100)
	local dian_b = math.mod(b, 100)
	return dian_a > dian_b
	
end

PszText = {}
PszText.shibai = 0				--失败匹配不到牌型

PszText.danpai = 1				--单张
PszText.suizi = 2				--顺子

PszText.duizi = 3				--对子
PszText.liandui = 4				--连对

PszText.sanzhang = 5			--三张
PszText.sandaiyi = 6			--三带一
PszText.sandaier = 7			--三带二

PszText.feiji = 8				--飞机
PszText.feijidaier = 9			--飞机带翅膀

PszText.sansui = 10				--三顺子

PszText.sidaier = 11			--四带二

PszText.zhadan = 12				--炸弹
PszText.wangzha = 13			--王炸
PszText.huojian = 14			--火箭












