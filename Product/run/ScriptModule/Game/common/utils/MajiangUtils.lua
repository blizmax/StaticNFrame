MajiangUtils = {}

--[[
函数功能：传入一个整形的麻将列表，获得一个555以上的麻将列表
]]
function MajiangUtils.Get555CardS(cardList) --{11,11,11}

	local count = 0
	local markID = 0
	for k,v in ipairs(cardList) do
		if markID ~= v then
			markID = v
			count = 1
		else
			count = count + 1
			if count == 3 then
				break
			end
		end
	end
	
	local retList = {}
	
	if count == 3  then
		
		local len = 1
		while count > 0 and len <= #cardList do
			if cardList[len] == markID then
				table.insert(retList, markID) --加到对应的列表中。
				table.remove(cardList, len)    --同时
				count = count - 1
			else
				len = len + 1
			end
		end
		
	end
	return retList
	
	
end

function MajiangUtils.Get555CardSArrList(cardList) --获取二维的列表：{{11,11,11}，{12,12,12}}
	local arr555List = {}
	for i = 1, 10 do
		local list555 = MajiangUtils.Get555CardS(cardList)
		if list555 == nil or #list555 == 0 then
			break
		end
		table.insert(arr555List, list555)
	end
	return arr555List
end

function MajiangUtils.Get555CardSList(cardList)   --获取一维列表{11，12}
	local arrList = {}
	for i = 1, 10 do
		local list555 = MajiangUtils.Get555CardS(cardList)
		if list555 == nil or #list555 == 0 then
			break
		end
		table.insert(arrList, list555[1])
	end
	return arrList
end


--传入卡牌列表，取得可能的555和剩余的牌的数组组合
--例如：{11,11,11,22,22,22,34,35}
--得到{{{11},{22,22,22,34,35}},{{22},{11,11,11,34,35}}}
function MajiangUtils.Get555PairArrList(cardList)
	
	local tempList = table.clone(cardList)
	local list555 = MajiangUtils.Get555CardSList(tempList)
	local retList = {}
	if #list555 > 0 then
		for k1,v1 in ipairs(list555) do
			local markLen = 0
			local inList = {}
			for k2,v2 in ipairs(cardList) do
				if v1 == v2 and markLen < 3 then
					markLen = markLen + 1
				else
					table.insert(inList, v2)
				end
			end
			retList[v1] = inList
		end
	end
	return retList
end

--[[
函数功能：传入一个整形的麻将列表，获得一个456的麻将列表
取得最优的顺子列表
]]

function MajiangUtils.Get456CardS(cardList)
	if #cardList < 3 then
		return {}
	end
	local getLen = 1
	local getValue = cardList[1]
	local getPos = 1
	local cardLen = #cardList
	for i = 2,cardLen do --从第二个开始比较
		if cardList[i] == (getValue + 1) then
			getLen = getLen + 1
			getValue = cardList[i]
		elseif cardList[i] == getValue then
			
		else
			getValue = cardList[i]
			getLen = 1
		end
		
		getPos = i
		if getLen >= 3 then
			break
		end
	end
	if getLen < 3 then
		return {}
	end
	local retList = {}
	for i = getPos,1,-1 do
		--从后面往回删除
		if cardList[i] == getValue then
			table.AscInsert(retList,getValue)
			getValue = getValue - 1
			table.remove(cardList,i)
		end
		if #retList >= 3 then
			break
		end
	end
	return retList
end


function MajiangUtils.Get456CardSRobot(cardList) --这个是给机器人用的，智能取出需要的456的类型

	if #cardList <= 0 then
		return {}
	end
	
	local cardArrList = {}
	cardArrList[1] = {}
	cardArrList[2] = {}
	cardArrList[3] = {}
	
	for k,v in ipairs(cardList) do
		local cardType = math.floor(v/10)
		if cardType > 3 then
			break
		else
			local isExist = false
			for k1,v1 in ipairs(cardArrList[cardType]) do
				if v1[1] == v then
					table.insert(v1, v)
					isExist = true
				end
			end
			if isExist == false then
				local tempList = {}
				table.insert(tempList,v)
				table.insert(cardArrList[cardType], tempList)
			end
		end
	end
	--到这里已经分成了
	--{{{11,11},{12,12}},{{23,23},{26}},{}} --这种的模式了
	--先按照类型分开各个类别
	
	local function Card456Work(cardUniArrList)
		--传入单一的列表
		local retTempList = {}
		if #cardUniArrList < 3 then
			--如果这里的长度小于三
			--立刻就可以返回了
			return {}
		elseif #cardUniArrList == 3 then
			--如果长度等于三
			--就看看是不是连续的
			if (cardUniArrList[1][1] + 1) == cardUniArrList[2][1]  and (cardUniArrList[2][1] + 1) == cardUniArrList[3][1] then
				--如果相等了，就说明取到一个了
				table.insert(retTempList,cardUniArrList[1][1])
				table.insert(retTempList,cardUniArrList[2][1])
				table.insert(retTempList,cardUniArrList[3][1])
			end
		else --if #cardUniArrList == 4 then
			--如果长度等于4
			--如果是大于四的长度
			--首先，计算连续的长度
			
			local markLen = 1
			local markPos = 1
			local markValue = cardUniArrList[1][1] --先去第一个值
			for i = 2,#cardUniArrList do
				if cardUniArrList[i][1] == (markValue + 1) then
					--表示是连续的
					markLen = markLen + 1
				else --如果不相等
					if markLen >= 3 then
						--如果连续的长度大于等于三了
						break
					end
					markLen = 1
					markPos = i
				end
				markValue = cardUniArrList[i][1]
			end
			if markLen < 3 then --连续的长度小于三
				return {}
			end
			
			local startPos = markPos
			if markLen == 4 then
				--如果长度是4
				if #cardUniArrList[markPos] > 1 then
					--如果开始长度是一对的
					startPos = markPos + 1
				end
			elseif markLen == 5 then
				--如果长度是5
				if #cardUniArrList[markPos] > 1 or #cardUniArrList[markPos + 1] > 2 then
					startPos = markPos + 2
				end
			elseif markLen == 6 then
				startPos = markPos
			elseif markLen == 7 then --如果是7个长度，并且，第一个是一对的时候
				if #cardUniArrList[markPos] > 1 then
					startPos = markPos + 1
				end
			elseif markLen == 8 then
				--如果长度是5
				if #cardUniArrList[markPos] > 1 or #cardUniArrList[markPos + 1] > 2 then
					startPos = markPos + 2
				end				
			end
			table.insert(retTempList, cardUniArrList[startPos][1])
			table.insert(retTempList, cardUniArrList[startPos + 1][1])
			table.insert(retTempList, cardUniArrList[startPos + 2][1])
		end
		return retTempList
	end
	
	local retList = {}

	for k,v in ipairs(cardArrList) do
		retList = Card456Work(v)
		if retList ~= nil and #retList > 0 then
			break
		end
	end
	
	
	for k1,v1 in ipairs(retList) do
		--把取到的参数从cardList列表中删除
		for k2,v2 in ipairs(cardList) do
			
			if v1 == v2 then
				table.remove(cardList, k2)
				break
			end
			
		end
	end
	return retList
end

--[[
函数功能：返回一个455(556)或者55的牌型
]]
function MajiangUtils.Get456CardSArrList(cardList)
	local arr456List = {}
	for i = 1, 10 do
		local list456 = MajiangUtils.Get456CardS(cardList)
		if list456 == nil or #list456 == 0 then
			break
		end
		table.insert(arr456List, list456)
	end
	return arr456List
end

--[[
函数功能：返回一个456的列表牌型，该函数仅供机器人调用
]]
function MajiangUtils.GetRobot456CardSArrList(cardList)
	local arr456List = {}
	for i = 1, 10 do
		local list456 = MajiangUtils.Get456CardSRobot(cardList)
		if list456 == nil or #list456 == 0 then
			break
		end
		table.insert(arr456List, list456)
	end
	return arr456List
end

function MajiangUtils.Get55CardS(cardList)
	local count = 0
	local markID = 0
	for k,v in ipairs(cardList) do
		if markID ~= v then
			markID = v
			count = 1
		else
			count = count + 1
			if count == 2 then
				break
			end
		end
	end
	
	local retList = {}
	
	if count == 2  then
		
		local len = 1
		while count > 0 and len <= #cardList do
			if cardList[len] == markID then
				table.insert(retList, markID) --加到对应的列表中。
				table.remove(cardList, len)    --同时
				count = count - 1
			else
				len = len + 1
			end
		end
		
	end
	return retList	
end

function MajiangUtils.Get55CardSArrList(cardList)  --获取二维的列表 {{11,11}，{12,12}}
	local arr55List = {}
	for i = 1, 10 do
		local list55 = MajiangUtils.Get55CardS(cardList)
		if list55 == nil or #list55 == 0 then
			break
		end
		table.insert(arr55List, list55)
	end
	return arr55List	
end

function MajiangUtils.Get55CardSList(cardList)  --获取一维的列表 {11,12}
	local arrList = {}
	for i = 1, 10 do
		local list55 = MajiangUtils.Get55CardS(cardList)
		if list55 == nil or #list55 == 0 then
			break
		end
		table.insert(arrList, list55[1])
	end
	return arrList	
end

function MajiangUtils.Get55PairList(cardList) 
	
	local tempList = table.clone(cardList)
	
	local list55 = MajiangUtils.Get55CardSList(tempList)
	local retPair = {}
	local countList = {}
	for k1,v1 in ipairs(list55) do
		retPair[v1] = {}
		local count = 0
		
		for k2,v2 in ipairs(cardList) do
			
			if v1 == v2 then
				count = count+1
				
				if count > 2 then
					table.insert(retPair[v1],v2)
				end
			else
				table.insert(retPair[v1],v2)
			end
			
		end

	end
	return retPair
end

function MajiangUtils.GetCycleList(startPos, chairLen)
	if chairLen == nil or chairLen == 0 then
		chairLen = 4
	end
	--传入一个开始的值，得到剩下的顺序列表
	local retList = {}
	
	for i = startPos + 1, chairLen do
		table.insert(retList, i)
	end
	
	for i = 1, startPos - 1 do
		table.insert(retList, i)
	end
	
	return retList
end

function MajiangUtils.SendAction(gcmsg, tItem)

	local gcAction = msg_gdmj_pb.gcgdmjaction()
	gcAction:ParseFromString(gcmsg:SerializeToString())
	
	local dataStr = tItem.m_tInfo.frameid..","

	dataStr = dataStr..";actiontype="
	if #gcAction.actiontype > 0 then
		for k,v in ipairs(gcAction.actiontype) do
			dataStr= dataStr..v..","
		end
	end
	dataStr = dataStr..";actpokerid="
	if #gcAction.actpokerid > 0 then
		for k,v in ipairs(gcAction.actpokerid) do
			dataStr= dataStr..v..","
		end
	end
	dataStr = dataStr..";tarlist="
	if #gcAction.tarplayer > 0 then
		for k,v in ipairs(gcAction.tarplayer) do
			dataStr = dataStr..v.userid..","
		end
	end
	for k1,v1 in ipairs(gcAction.actplayer) do
		dataStr = dataStr..";actplayer="..v1.userid..";"..v1.chairid..";handpoker:"
		for k2,v2 in ipairs(v1.handpoker) do
			dataStr = dataStr..v2..","
		end
		dataStr = dataStr..";tinglist:"
		if #v1.tinglist > 0 then
			for k2,v2 in ipairs(v1.tinglist) do
				dataStr = dataStr..v2..","
			end
		end
		dataStr = dataStr..";mjpokerlist:"
		

		if #v1.mjpokerlist > 0 then
			for k2,v2 in ipairs(v1.mjpokerlist) do		
				dataStr = dataStr.."type="..v2.pokertype..","
				dataStr = dataStr.."detail="..v2.typedetail..","
				dataStr = dataStr.."list:"
				for k3,v3 in ipairs(v2.pokerlist) do
					dataStr = dataStr..v3..","
				end
			end	
		end
	end
	
	dataStr=dataStr..";showtype="..gcAction.showpoker.pokertype..";typedetail="..gcAction.showpoker.typedetail..";showtar="..gcAction.showpoker.tarchairid..";showpoker="	

	if #gcAction.showpoker.pokerlist > 0 then
		for k,v in ipairs(gcAction.showpoker.pokerlist) do
			dataStr=dataStr..v..","
		end
	end
	LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, tItem.m_tInfo.tableid,"action+++"..dataStr)
end

function MajiangUtils.SendUpdate(gcmsg, tItem)

	local gcUpdate = msg_gdmj_pb.gcgdmjupdate()
	gcUpdate:ParseFromString(gcmsg:SerializeToString())
	
	local dataStr = tItem.m_tInfo.frameid..","
	
	dataStr = dataStr.."actchairid="
	for k,v in ipairs(gcUpdate.actchairid) do
		dataStr=dataStr..v..","
	end
	
	dataStr = dataStr.."tarchairid="..gcUpdate.tarchairid..","
	dataStr = dataStr.."pokerid="..gcUpdate.pokerid..","
	dataStr = dataStr.."actpokerid="..gcUpdate.actpokerid..","
	
	dataStr = dataStr.."actiontype:"
	for k,v in ipairs(gcUpdate.actiontype) do
		dataStr = dataStr..v..","
	end
	
	LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, tItem.m_tInfo.tableid,"update---"..dataStr)
	
end

--传入一个胡牌的列表，把重复的去掉，然后把剩下的排列，再返回
function MajiangUtils.TrimList(huList)
	
	local tempList = {}
	for k,v in ipairs(huList) do
		tempList[v] = 1
	end	
	huList = {}
	for k,v in pairs(tempList) do --这里是pairs
		table.AscInsert(huList, k)
	end
	
	return huList
	
end