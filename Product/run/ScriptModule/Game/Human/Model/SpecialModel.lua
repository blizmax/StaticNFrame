
SpecialModel = {}
SpecialModel.redis_index = "redis_special"
SpecialModel.redis_index = "special_"
SpecialModel.redis_table = "special_table_"
SpecialModel.redis_julebu = "special_julebu_"

SpecialModel.redis_douniu_win = "special_douniu_win"
SpecialModel.redis_douniu_lose = "special_douniu_lose"

SpecialModel.redis_psz_win = "special_psz_win"
SpecialModel.redis_psz_lose = "special_psz_lose"
 
SpecialModel.redis_pdk_win = "special_pdk_win"
SpecialModel.redis_pdk_lose = "special_pdk_lose"

function SpecialModel.GetSpecialInfo(userID, sInfo)
	
	if sInfo == nil then
		sInfo = st_human_pb.specialinfo()
	end
	
	local strGet = redisItem:get(SpecialModel.redis_index..userID, SpecialModel.redis_index)
	
	if strGet ~= nil then
		sInfo:ParseFromString(strGet)
	else
		SpecialModel.LoadSpecialInfo(userID, sInfo)
	end
	
	
	local currTime = TimeUtils.GetTime()
	

	if currTime > sInfo.starttime and sInfo.state == 0 and currTime < sInfo.endtime then
		--开始时间到了，这里不用更新到数据库
		--local sqlCase = "update ag_special set state = 1 where userid="..userID
		--mysqlItem:execute(sqlCase)
		sInfo.state = 1
		redisItem:set(SpecialModel.redis_index..userID, sInfo:SerializeToString(), SpecialModel.redis_index)		
	end
	
	
	if currTime > sInfo.endtime and sInfo.state > 0 then
		--超过时间了
		--只有大于0的情况下，才会去修改里面的值，如果小于0，表示已经删除了
		local sqlCase = "update ag_special set state = 0 where userid="..userID
		mysqlItem:execute(sqlCase)
		sInfo.state = 0
		redisItem:set(SpecialModel.redis_index..userID, sInfo:SerializeToString(), SpecialModel.redis_index)
		
	end
	
	
	
	return sInfo
end

function SpecialModel.LoadSpecialInfo(userID, sInfo)
	if sInfo == nil then
		sInfo = st_human_pb.specialinfo()
	end

	local sqlCase = "select * from ag_special where state > 0 and userid="..userID
	
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	
	if sqlData ~= nil then
		sInfo.userid = userID
		sInfo.starttime = TimeUtils.GetTime(sqlData[4])
		sInfo.endtime = TimeUtils.GetTime(sqlData[5])
		sInfo.rate = tonumber(sqlData[6])
		sInfo.state = tonumber(sqlData[7])
		if sqlData[10] ~= nil and string.len(sqlData[10]) > 1 then
			local gameList = luajson.decode(sqlData[10])
			for k,v in ipairs(gameList) do
				sInfo.gametype:append(tonumber(v))
			end
		end
		
		local currTime = TimeUtils.GetTime()
		
		if currTime < sInfo.starttime or currTime > sInfo.endtime then
			--就是开始的时间还没有到的
			sInfo.state = 0
		end
		
		
	else
		sInfo.userid = userID
		sInfo.state = 0
	end
	redisItem:set(SpecialModel.redis_index..userID, sInfo:SerializeToString(), SpecialModel.redis_index)
	return sInfo
end

function SpecialModel.SetSpecialInfo(sInfo)
	redisItem:set(SpecialModel.redis_index..sInfo.userid, sInfo:SerializeToString(), SpecialModel.redis_index)
end

function SpecialModel.GameEnd(countInfo)
	
	local strEnd = luajson.encode(countInfo['userlist'])
	
	for k, v in ipairs(countInfo['userlist']) do
		local sInfo = SpecialModel.GetSpecialInfo(v['userid'])
		if sInfo.state ~= 0 then
			local sqlCase = "insert into log_special(tableid,nickname,userid,timemark,specialstate,winnum,des) VALUES("..
			countInfo['tableid']..",'"..v['nick'].."',"..v['userid']..",'"..countInfo['time'].."',"..sInfo.state..","..v['jifen']..",'"..strEnd.."')"
			mysqlItem:execute(sqlCase)
			sInfo.peinum = 0
			sInfo.marknum = 0
			SpecialModel.SetSpecialInfo(sInfo)
		end
	end
	
end


function SpecialModel.MarkSetType(userID, cardType)

 	local sInfo = SpecialModel.GetSpecialInfo(userID)

	if sInfo == nil or sInfo.state ~= 1 then
		return false
	end
	
	sInfo.marknum = sInfo.marknum + 1
	sInfo.cardtype = cardType

	SpecialModel.SetSpecialInfo(sInfo)
	return true
end



function SpecialModel.MarkSetZhuang(userID)

 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 then
		return false
	end
	
	sInfo.marknum = sInfo.marknum + 1
	sInfo.iszhuang = 1
	luaPrint("SpecialModel.MarkAdd sInfo.marknum="..sInfo.marknum)
	SpecialModel.SetSpecialInfo(sInfo)
	return true
end

function SpecialModel.MarkAdd(userID)

 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 then
		return false
	end
	
	sInfo.marknum = sInfo.marknum + 1
	luaPrint("SpecialModel.MarkAdd sInfo.marknum="..sInfo.marknum)
	SpecialModel.SetSpecialInfo(sInfo)
	return true
end

function SpecialModel.NiuNiuCardType(userID)
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	
	if sInfo ~= nil then
		luaPrint("userID="..userID..",sInfo.marknum"..sInfo.marknum..", sInfo.state="..sInfo.state)
	end
	
	if sInfo == nil or sInfo.state ~= 1 or sInfo.marknum <= 0 then
		--记录对应的值
		return nil
	end	
	local isNiuNiu = false
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_douniu then
			isNiuNiu = true
		end
	end
	if isNiuNiu == false then
		return nil
	end	
	local cardType = sInfo.cardtype
	sInfo.marknum = 0
	sInfo.cardtype = 0
	SpecialModel.SetSpecialInfo(sInfo)		
	return cardType
end



function SpecialModel.MajiangDianCard(userID)
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	
	if sInfo ~= nil then
		luaPrint("userID="..userID..",sInfo.marknum"..sInfo.marknum..", sInfo.state="..sInfo.state)
	end
	
	if sInfo == nil or sInfo.state ~= 1 or sInfo.marknum <= 0 then
		--记录对应的值
		return false
	end	
	local isMajiang = false
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_gdmj then
			isMajiang = true
		end
	end
	if isMajiang == false then
		return false
	end	
		
	return isMajiang	
end



function SpecialModel.NiuNiuBanker(userID)
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	
	if sInfo ~= nil then
		luaPrint("userID="..userID..",sInfo.marknum"..sInfo.marknum..", sInfo.state="..sInfo.state)
	end
	
	if sInfo == nil or sInfo.state ~= 1 then
		--记录对应的值
		return false
	end	

	local isNiuNiu = false
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_douniu then
			isNiuNiu = true
		end
	end
	if isNiuNiu == false then
		return false
	end		
	
	local isZhuang = sInfo.iszhuang == 1
	
	sInfo.iszhuang = 0
	SpecialModel.SetSpecialInfo(sInfo)		
	return isZhuang	
end


function SpecialModel.SanGongIsNeed(userID)
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 or sInfo.marknum <= 0 then
		--记录对应的值
		return 0
	end
	
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_sangong then
			--这里会置为空
			sInfo.marknum = 0
			SpecialModel.SetSpecialInfo(sInfo)				
			return true
		end
	end
	return false
end


function SpecialModel.PaiJiuIsNeed(userID)
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 or sInfo.marknum <= 0 then
		--记录对应的值
		return 0
	end
	
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_paijiu then
			sInfo.marknum = 0
			SpecialModel.SetSpecialInfo(sInfo)				
			return true
		end
	end
	return false
end

function SpecialModel.SanGongGetPeiList(tInfo)
	--只会配八点，九点，或者或者混三公的牌数
	
	--先遍历把空余的牌取出来
	local cardList = {}
	for k,v in ipairs(tInfo.pokerlist) do
		--先把全部有的牌取出来
		table.insert(cardList, v)
	end
	local getList = {}
	local cardTypeList = {108,109,211}
	for i = 1,5 do
		--这里遍历5遍，如果还是没有取到，就算了
		
		local tempList = table.clone(cardList)
		
		getList = {}
		local getType = math.myrandom(1,#cardTypeList)
		getType = cardTypeList[getType]
		
		--这里遍历
		
		if getType ~= 211 then
			local index = math.myrandom(1,#tempList)

			table.insert(getList, tempList[index])
			table.remove(tempList, index)
			
			index = math.myrandom(1,#tempList)

			table.insert(getList, tempList[index])
			table.remove(tempList, index)	

			local typeTemp = SpecialModel.LastCardDianNum(getList)
			
			if typeTemp > 8 then
				
				for k,v in ipairs(tempList) do
					if math.mod(v,100) > 10 then
						
						table.insert(getList, v)
						table.remove(tempList, k)
						break
					end
				end
			else
				
				local needVale = math.mod(getType,100)
				needVale = needVale - typeTemp
				
				for k,v in ipairs(tempList) do
					if math.mod(v,100) == needVale then
						table.insert(getList, v)
						table.remove(tempList, k)
						break
					end
				end				
				
			end
			
			
		else
			
			--这里是取三个三公的
			
			for j = 1,10 do
				local index = math.myrandom(1,10)
				
				for k = index,#tempList do
					if math.mod(tempList[k], 100) > 10 then
						table.insert(getList, tempList[k])
						table.remove(tempList, k)
						break
					end
				end
 				
				if #getList >= 3 then
					break
				end
				
			end
		end
		if #getList >= 3 then
			break
		end		
	end
	

	if #getList == 0 then
		return getList
	end
	
	for k,v in ipairs(getList) do 
		for i = 1,#tInfo.pokerlist do
			if v == tInfo.pokerlist[i] then
				tInfo.pokerlist:remove(i)
				break
			end
		end
	end
	
	return getList
	
end


function SpecialModel.SanGongGetPokerID(tInfo, cardList)
	
	if #cardList ~= 2 then
		return 0
	end
	
	local cardValue = 0
	local getPoker = 0
	
	local getType = 0--  math.myrandom(1,3)	
	
	if math.mod(cardList[1], 100) > 10 and math.mod(cardList[2], 100) > 10 then
		
		local cardTypeList = {108,109,211}
		getType = math.myrandom(1,3)
		getType = cardTypeList[getType]
		if getType == 211 then
			for i = 1,tInfo.pokerlist do
				if math.mod(tInfo.pokerlist[i],100) > 10 then
					getPoker = tInfo.pokerlist[i]
					tInfo.pokerlist:remove(i)
					break
				end
			end
		end
	else
		local cardTypeList = {108,109}
		getType = math.myrandom(1,2)
		getType = cardTypeList[getType]		
	end
	
	if getPoker ~= 0 then
		return getPoker
	end
	
	local cardType = SpecialModel.LastCardDianNum(cardList)
	
	if cardType >= 8 then
		for i = 1,#tInfo.pokerlist do
			if math.mod(tInfo.pokerlist[i],100) >= 10 then
				getPoker = tInfo.pokerlist[i]
				tInfo.pokerlist:remove(i)
				break
			end
		end		
	else
		local needVale = math.mod(getType,100) - cardType
		
		for i = 1,#tInfo.pokerlist do
			if math.mod(tInfo.pokerlist[i],100) == needVale then
				getPoker = tInfo.pokerlist[i]
				tInfo.pokerlist:remove(i)
				break
			end
		end
	end
	
	luaPrint("getPoker="..getPoker)
	return getPoker
	
end


function SpecialModel.SetTableIDUserID(tableID, userID)

	if false == redisItem:exists( SpecialModel.redis_table..tableID, SpecialModel.redis_index ) then
		redisItem:set(SpecialModel.redis_table..tableID, userID, SpecialModel.redis_index)
	end

end

function SpecialModel.DelTableIDUserID(tableID, userID)
	redisItem:del(SpecialModel.redis_table..tableID, SpecialModel.redis_index)
end

function SpecialModel.LeaveTableUSerID(tableID, userID)
	
	local getUserID = SpecialModel.GetTbaleIDUserID(tableID)
	if getUserID ~= nil and tonumber(getUserID) == tonumber(userID) then
		
		redisItem:del(SpecialModel.redis_table..tableID, SpecialModel.redis_index)
	end
end

function SpecialModel.GetTbaleIDUserID(tableID)
	return redisItem:get(SpecialModel.redis_table..tableID, SpecialModel.redis_index)
end


function SpecialModel.SetJulebuUserID(julebuID, userID)
	
	redisItem:set(SpecialModel.redis_julebu..julebuID, userID, SpecialModel.redis_index)
	
end


function SpecialModel.GetJulebuUseID( julebuID , senderID )
	if julebuID == 0 then
		return senderID
	end
	
	local userID = redisItem:get( SpecialModel.redis_julebu..julebuID, SpecialModel.redis_index )
	return userID == nil and 0 or tonumber(userID)
end

function SpecialModel.NiuNiuSet( userID, tableID, beSetID, setType )
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 then
		--记录对应的值
		return false
	end
	
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_douniu then
			--这里会置为空
			
			if setType == g_douNiuDefine.spc_win then
				SpecialModel.SetDouNiuWin(tableID, beSetID)
			elseif setType == g_douNiuDefine.spc_lose then
				SpecialModel.SetDouNiuLose(tableID, beSetID)
			end
			
			
			return true
		end
	end
	return false	
end

function SpecialModel.SetDouNiuWin(tableID, userID)
	redisItem:hset(SpecialModel.redis_douniu_win, tableID, userID, SpecialModel.redis_index )
end

function SpecialModel.GetDouNiuWin(tableID)
	local getUserID = redisItem:hget( SpecialModel.redis_douniu_win, tableID,  SpecialModel.redis_index )
	redisItem:hdel( SpecialModel.redis_douniu_win, tableID,  SpecialModel.redis_index )
	return getUserID == nil and 0 or tonumber(getUserID)
end

function SpecialModel.SetDouNiuLose(tableID, userID)
	redisItem:hset(SpecialModel.redis_douniu_lose, tableID, userID, SpecialModel.redis_index )
end

function SpecialModel.GetDouNiuLose(tableID)
	local getUserID = redisItem:hget( SpecialModel.redis_douniu_lose, tableID,  SpecialModel.redis_index )
	redisItem:hdel( SpecialModel.redis_douniu_lose, tableID,  SpecialModel.redis_index )
	return getUserID == nil and 0 or tonumber(getUserID)
end


function SpecialModel.GetCardType(cardList)
	--这里只计算牛牛，不计算其他的
	
	for i = 1,#cardList do
		for j = i+1,#cardList do
			for k = j+1,#cardList do

				local threeList = {}
				table.insert(threeList, cardList[i])
				table.insert(threeList, cardList[j])
				table.insert(threeList, cardList[k])
				if true == DouNiuHuUtils.ThreeCardIsNiuNiu(threeList) then
					--如果三张牌是牛，那就判断剩下的两张牌，是不是组成牛牛。
					local lastList = {}
					local inCount = 4
					for m = 1,#cardList do
						if m ~= i and m ~= j and m ~= k then
							table.insert(lastList, cardList[m])
							inCount = inCount + 1
						end
					end
					--如果是牛牛，需要把牌摆放在前面
					--剩下的两个
					
					local dianShu = SpecialModel.LastCardDianNum(lastList)   --取到最后的卡牌的类型，这里先判断牛牛,其他的牌型再判断
					return dianShu == 0 and g_douNiuCard_type.niu_niu or dianShu
				end
			end
		end
	end	
	return g_douNiuCard_type.wu_niu
end

function SpecialModel.ThreeCardIsNiuNiu(cardList)
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

function SpecialModel.LastCardDianNum(cardList)   --返回对应的点数
	local allNum = 0
	for i = 1,#cardList do
		--先不管花色
		local tempNum = math.mod(cardList[i], 100) --取得面值的大小
		if tempNum >= 10 then
			--如果是10以上的面值
			tempNum = 0   --这里就置为0
		else
			
		end
		allNum = tempNum + allNum
	end
	allNum = math.mod(allNum, 10) --这里一定要求模
	--if allNum == 0 then
		--说名这三个牌组成牛牛
	--	return 10
	--end
	return allNum  --
	
end

function SpecialModel.GetMaxCardInfo(cardList)  --取得最大的牌面信息
	local maxNum = 0
	local maxType = 0
	for k,v in ipairs(cardList) do
		if math.mod( v, 100 ) > maxNum then
			maxNum = math.mod(v,100)
			maxType = math.floor(v/100)
		elseif math.mod(v, 100) == maxNum  and math.floor(v/100) > maxType then
			maxNum = math.mod(v,100)
			maxType = math.floor(v/100)
		end
	end
	return maxNum, maxType
end


function SpecialModel.PszSet( userID, tableID, beSetID, setType )
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 then
		--记录对应的值
		return false
	end
	
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_douniu then
			--这里会置为空
			
			if setType == g_PszDefine.spc_win then
				redisItem:hset(SpecialModel.redis_psz_win, tableID, beSetID, SpecialModel.redis_index )
			elseif setType == g_PszDefine.spc_lose then
				redisItem:hset(SpecialModel.redis_psz_lose, tableID, beSetID, SpecialModel.redis_index )
			end
			return true
		end
	end
	return false	
end


function SpecialModel.PszGetWinID(tableID)
	local getUserID = redisItem:hget( SpecialModel.redis_psz_win, tableID,  SpecialModel.redis_index )
	redisItem:hdel( SpecialModel.redis_psz_win, tableID,  SpecialModel.redis_index )
	return getUserID == nil and 0 or tonumber(getUserID)	
end



function SpecialModel.PdkSet( userID, tableID, beSetID, setType )
 	local sInfo = SpecialModel.GetSpecialInfo(userID)
	if sInfo == nil or sInfo.state ~= 1 then
		--记录对应的值
		return false
	end
	
	for k,v in ipairs(sInfo.gametype) do
		if v == g_logintable.type_douniu then
			--这里会置为空
			
			if setType == g_PszDefine.spc_win then
				redisItem:hset(SpecialModel.redis_pdk_win, tableID, beSetID, SpecialModel.redis_index )
			elseif setType == g_PszDefine.spc_lose then
				redisItem:hset(SpecialModel.redis_pdk_lose, tableID, beSetID, SpecialModel.redis_index )
			end
			return true
		end
	end
	return false	
end


function SpecialModel.PdkGetWinID(tableID)
	local getUserID = redisItem:hget( SpecialModel.redis_pdk_win, tableID,  SpecialModel.redis_index )
	redisItem:hdel( SpecialModel.redis_pdk_win, tableID,  SpecialModel.redis_index )
	return getUserID == nil and 0 or tonumber(getUserID)	
end