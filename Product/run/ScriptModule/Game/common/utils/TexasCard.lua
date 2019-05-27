
--该类是用于判断德州扑克的牌型的

TexasCard = {}

g_texasCardType = {}
g_texasCardType.HIGH_CARD = 1          -- 高牌
g_texasCardType.PAIR = 2               -- 一对
g_texasCardType.TOW_PAIRS = 3          -- 两对
g_texasCardType.THREE_KIND = 4         -- 三条
g_texasCardType.STRAIGHT = 5           -- 顺子
g_texasCardType.FLUSH = 6              -- 同花
g_texasCardType.FULL_HOUSE = 7         -- 葫芦
g_texasCardType.FOUR_KIND = 8          -- 四条
g_texasCardType.STRAIGHT_FLUSH = 9    --  同花顺
g_texasCardType.ROYAL_FLUSH = 10       -- 皇家同花顺


--[[                               
牌面face 234....9TJQKA
花色suit 方块 梅花 红桃 黑桃 dchs
]]
function TexasCard.getFace(val)
	if val == "-" then return 0 end
	return tonumber(val)%100
end

function TexasCard.getSuit(val)
	if val == "-" then return 0 end
	--local str = string.sub(val,2,2)
	return tonumber(string.sub(val,1,1))
end                    

--小到大
function TexasCard.sortAsc( cards )
	table.sort(cards, function (a,b)
		return TexasCard.getFace(a) < TexasCard.getFace(b)
	end)
	return cards
end

--大到小
function TexasCard.sortDesc( cards )
	table.sort(cards, function (a,b)

		return TexasCard.getFace(a) > TexasCard.getFace(b)
	end)
	return cards
end

--按照类型来从大到小来牌型
function TexasCard.sortDescType( cards , cType)

	
	
	if cType ~= g_texasCardType.FULL_HOUSE then
		table.sort(cards, function (a,b)
			return TexasCard.getFace(a) > TexasCard.getFace(b)
		end)
	end
	
	return cards
end

--是否同花
function TexasCard.isFlush(cards)
	if #cards < 5 then return nil end
	--梅花 方块 红桃 黑桃
	local colorCard = {{}, {}, {}, {}}
	for i,card in ipairs(cards) do
		local color = TexasCard.getSuit(card)
		table.insert(colorCard[color], card)
	end
	for k,v in pairs(colorCard) do
		if #v >= 5 then
			return v,g_texasCardType.FLUSH
		end
	end
	return nil
end
--获取最大的同花数
function TexasCard.getMaxFlush(cards)
	
	local colorCard = {{}, {}, {}, {}}
	for i,card in ipairs(cards) do
		local color = TexasCard.getSuit(card)
		table.insert(colorCard[color], card)
	end
	
	table.sort(colorCard, function (a,b)
		return #a > #b
	end)
	return colorCard[1]
end

--是否顺子
function TexasCard.isStraight(cards)
	if cards == nil then
		return nil
	end
	--已经降序排好
	cards = clone(cards)
	for i,card in ipairs(cards) do
		if TexasCard.getFace(card) == 14 then
			
			local data = TexasCard.getSuit(card)*100 + 1
			table.insert(cards, data) -- A当作1用 如A2345
		end
	end
	
	local last_face = -1
	local last_card = -1
	local count = 1
	local faces = {}
	for i,v in ipairs(cards) do
		
		if last_face == TexasCard.getFace(v) then
			local ftmp = 1
		elseif last_face-1 ~= TexasCard.getFace(v) then
			faces = {}
		else
			table.insert(faces, last_card)
			if #faces == 4 then
				table.insert(faces,v)
				return faces,g_texasCardType.STRAIGHT
			end
		end
		last_face = TexasCard.getFace(v)
		last_card = v
	end
	return nil
end


function TexasCard.getMaxStraight(cards)
	cards = clone(cards)
	cards = TexasCard.sortDesc(cards)
	for i,card in ipairs(cards) do
		if TexasCard.getFace(card) == 14 then
			local data = TexasCard.getSuit(card)*100 + 1
			table.insert(cards, data) -- A当作1用 如A2345
		end
	end
	
	local last_face = -1
	--local last_card = -1
	local count = 1
	local faces = {}
	for i,v in ipairs(cards) do
		
		if last_face == TexasCard.getFace(v) then
			local ftmp = 1
		elseif last_face-1 ~= TexasCard.getFace(v) then
			faces = {}
		else
			table.insert(faces, v)
		end
		last_face = TexasCard.getFace(v)
		--last_card = v
	end
	return faces
end

--是否同花顺
function TexasCard.isStraightFlush(cards)
	local typeCards,_type = TexasCard.isFlush(cards)
	local straight,_type = TexasCard.isStraight(typeCards)
	if typeCards and straight then
		--对于A2345这种情况，A已经被变成1新加straight的table，实际是12345
		if TexasCard.getFace(straight[1]) == 14 then
			return straight,g_texasCardType.ROYAL_FLUSH
		else
			return straight,g_texasCardType.STRAIGHT_FLUSH
		end
	end

	--同花
	if typeCards then
		return table.slice(typeCards,1,5),g_texasCardType.FLUSH
	end
	--顺子
	straight,_type = TexasCard.isStraight(cards)
	if straight then
		return straight,g_texasCardType.STRAIGHT
	end
	return nil
end

--对子 三条 四条
function TexasCard.isXOfAKind(cards)
	local faces = {}
	local val
	for i,v in ipairs(cards) do
		val = TexasCard.getFace(v)
		faces[val] = faces[val] or {}
		table.insert(faces[val],v)
	end
	for i,v in pairs(faces) do
		if #v == 4 then
			return v,g_texasCardType.FOUR_KIND
		elseif #v == 3 then
			return v,g_texasCardType.THREE_KIND
		elseif #v == 2 then
			return v,g_texasCardType.PAIR --还没处理两对
		end
	end
	return nil
end

--是否两对，一对
function TexasCard.isPair(cards)
	-- 有3对可能，需排序
	local faces = {}
	local pairCard = {}
	for i,v in ipairs(cards) do
		val = TexasCard.getFace(v)
		faces[val] = faces[val] or {}
		--table.insert(faces[val], val)
		table.insert(faces[val], v)  --landy,这里不能传进val，应该插入v
		if #faces[val] == 2 then
			table.insert(pairCard,faces[val])
		end
	end
	if #pairCard >= 2 then
		
		return table.append(pairCard[1],pairCard[2]),g_texasCardType.TOW_PAIRS
	elseif #pairCard == 1 then
		return pairCard[1],g_texasCardType.PAIR
	end
	return nil
end

--是否葫芦
function TexasCard.isFullHouse(cards)
	local faces = {}
	local val
	for i,v in ipairs(cards) do
		val = TexasCard.getFace(v)
		faces[val] = faces[val] or {}
		table.insert(faces[val],v)
	end
	
	local cList = {}
	
	for i,v in pairs(faces) do
		if #v == 3 then
			--return nil
			for k2,v2 in ipairs(v) do
				table.insert(cList,1, v2)
			end
		elseif #v == 2 then
			for k2,v2 in ipairs(v) do
				table.insert(cList, v2)
			end
		end
	end

	if #cList == 5 then
		return cList, g_texasCardType.FULL_HOUSE
	else
		return nil
	end

end

--判断牌型
function TexasCard.getCardType(cards)
	cards = checktable(cards)
	TexasCard.sortDesc(cards) --排序好

	local typeCards,_type = {}
	if #cards >= 4 then
		--判断皇家同花顺 同花顺
		sflush,_type1 = TexasCard.isStraightFlush(cards)
		if _type1 == g_texasCardType.ROYAL_FLUSH or _type1 == g_texasCardType.STRAIGHT_FLUSH then
			return sflush,_type1
		end
		--是否四条
		typeCards,_type = TexasCard.isXOfAKind(cards)
		if _type == g_texasCardType.FOUR_KIND then
			return typeCards,_type
		end
		--是否葫芦
		typeCards,_type = TexasCard.isFullHouse(cards)
		if typeCards then
			return typeCards,_type
		end
		
		--是否同花 顺子
		if _type1 == g_texasCardType.FLUSH or _type1 == g_texasCardType.STRAIGHT then
			return sflush,_type1
		end
	end
	--是否三条
	typeCards,_type = TexasCard.isXOfAKind(cards)
	if _type == g_texasCardType.THREE_KIND then
		return typeCards,_type
	end
	--是否两对，一对
	typeCards,_type = TexasCard.isPair(cards)
	if typeCards then
		return typeCards,_type
	end

	return {cards[1]},g_texasCardType.HIGH_CARD
end

--传入扑克牌类型信息，取得剩下的扑克列表

function TexasCard.getCardLast(cards, typeCards)

	--cards = clone(cards)
	typeCards = clone(typeCards)
	
	--这两种情况是需要判断14变为1的情况的
	local len = #typeCards
	if TexasCard.getFace(typeCards[len]) == 1 then
		typeCards[len] = TexasCard.getSuit(typeCards[len])*100 + 14  --在这里变回14
	end
	
	local last = {}
	
	
	for i, v in pairs(cards) do
		
		local isMark = nil
		for k,m in pairs(typeCards) do
			if m == v then
				table.remove(typeCards, k)
				isMark = 1
				break
			end
		end

		if isMark == nil then
			table.insert(last, cards[i])
		end
	end
	
	return last
end

--输入玩家牌型列表，返回排序后的列表
--uCardList = {{userid,charid,typeid,typeCards,lastCards},{userid,charid,typeid,typeCards,lastCards}}

function TexasCard.Compare(alist, blist)

	
	if alist[3] > blist[3] then
		return true
	elseif alist[3] < blist[3] then
		return false
	end
	
	local len = #alist[4] <= #blist[4] and #alist[4] or #blist[4]
	for i = 1, len do
		if alist[4][i] > blist[4][i] then
			return true
		elseif alist[4][i] < blist[4][i] then
			return false
		end
	end
	
	len = #alist[5] <= #blist[5] and #alist[5] or #blist[5]

	local aTemp = clone(alist[5])
	local bTemp = clone(blist[5])

	aTemp = TexasCard.sortDesc(aTemp)  --这里需要经过排序
	bTemp = TexasCard.sortDesc(bTemp)
	
	for i = 1, len do
		if aTemp[i] > bTemp[i] then
			return true
		elseif aTemp[i] < bTemp[i] then
			return false
		end	
	end
	
	if alist[2] > blist[2] then
		return true
	else
		return false
	end

end



function TexasCard.Compare(alist, blist)

	               
	if alist[3] > blist[3] then
		return true
	elseif alist[3] < blist[3] then
		return false
	end
	
	
	
	local len = #alist[4] <= #blist[4] and #alist[4] or #blist[4]
	for i = 1, len do
		if TexasCard.getFace(alist[4][i]) > TexasCard.getFace(blist[4][i]) then
			return true
		elseif TexasCard.getFace(alist[4][i]) < TexasCard.getFace(blist[4][i]) then
			return false
		end
	end

	return false
end



function TexasCard.CardSortDesc(uCardList)

	table.sort(uCardList, TexasCard.Compare)
	
	local winLastFace = TexasCard.getFace(uCardList[1][4][5])
	local winNum = 1
	for i = 2, #uCardList do

		local isWinner = true
		for k,v in ipairs(uCardList[1][4]) do
			
			if TexasCard.getFace(uCardList[1][4][k]) ~= TexasCard.getFace(uCardList[i][4][k]) then
				isWinner = false
				break
			end
		end
		if isWinner == true then
			winNum = winNum + 1
		else
			break
		end
	end
	return winNum
end