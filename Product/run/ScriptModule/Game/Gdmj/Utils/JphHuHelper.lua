
--针对鸡平胡中复杂的算番关系，专门用一个文件来给鸡平胡算番


JphHuHelper = {}

function JphHuHelper.GetFanData(tItem, pos)  --获取
	local winType = 0
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	
	winType = JphHuHelper.IsFirstWin(tItem, pos)
	--local retList = {}
	if winType ~= 0 then

		return 6, "天胡" --retList
	end
	
	winType = JphHuHelper.IsJiuBaoLianDeng(tItem, pos)
	if winType ~= 0 then
		return 6, "九宝莲灯"
	end
	
	winType = GdmjHuHelper.IsDaOrXiaoSanYuan(tItem, pos)
	
	if winType ~= 0 then
		local retList = {}
		if winType == g_gdmjWinType.type_dasanyuan then

			return 6,"大三元"
		else
			return 5,"小三元"
		end
		
		return retList[1],retList[2]
	end
	
	winType = GdmjHuHelper.IsDaOrXiaoSiXi(tItem,pos)
	if winType ~= 0 then

		if winType == g_gdmjWinType.type_dasixi then

			return 6,"大四喜"
		else
			return 5,"小四喜"		
		end
	end
	
	winType = GdmjHuHelper.IsShiSanYao(tItem, pos)
	if winType ~= 0 then

		return 6,"十三幺"
	end
	
	winType = GdmjHuHelper.IsQingYaoJiu(tItem,pos)
	if winType ~= 0 then

		return 6,"清幺九"
	end
	
	winType = GdmjHuHelper.IsZiYiShe(tItem, pos)
	if winType ~= 0 then

		return 6,"字一色"
	end
	
	winType = GdmjHuHelper.IsHunYaoJiu(tItem, pos)
	if winType ~= 0 then

		return 5,"混幺九"
	end
	
	local isQingYiSe = GdmjHuHelper.IsQingYiSe(tItem, pos)

	--上面就是返回的打的类型了
	
	local isPengPengHu = GdmjHuHelper.IsPengPengHu(tItem, pos)
	
	
	if isQingYiSe ~= 0 then
		if isPengPengHu ~= 0 then
			return 5,"清碰"
		else
			return 4,"清一色"
		end
	end
	
	local isHunYiSe = GdmjHuHelper.IsHunYiSe(tItem, pos)
	if isPengPengHu ~= 0 and isHunYiSe ~= 0 then
		return 4,"混碰"
	end
	
	if isPengPengHu ~= 0 then

		return 2,"碰碰胡"
	end
	if isHunYiSe ~= 0 then
		return 2,"混一色"		
	end
	
	--local winType = JphHuHelper.IsJiHu()
	local pingHu = GdmjHuHelper.IsPingHu(tItem, pos)
	if pingHu ~= 0 then
		return 1,"平胡"			
	end

	return 0,"鸡胡"
end

function JphHuHelper.GetAddFanData(tItem, pos)
	
	local FengCard = math.floor(tItem.m_tInfo.fengid/10) + 40
	local QuanCard = math.mod(tItem.m_tInfo.fengid,10) + 40
	
	local checkList = {}
	checkList[FengCard] = 0
	checkList[QuanCard] = 0
	
	checkList[51] = 0
	checkList[52] = 0
	checkList[53] = 0
	
	for k,v in ipairs(tItem.m_userList[pos].handpoker) do
		if checkList[v] ~= nil then
			checkList[v] = checkList[v] + 1
		end
	end
	local retList = {}
	local sanYuan = 0
	for k,v in pairs(checkList) do
		if v >= 3 then
			local addItem = {}
			addItem[1] = 1
			if k == FengCard then
				addItem[2] = "风位"
				table.insert(retList, addItem)
			elseif k == QuanCard then
				addItem[2] = "风圈"
				table.insert(retList, addItem)
			else
				sanYuan = sanYuan + 1
			end
			
			
		end
		
	end
	
	if sanYuan ~= 0 then
		local addItem = {}
		addItem[1] = sanYuan
		addItem[2] = "三元牌+"..sanYuan
		table.insert(retList, addItem)
	end
	return retList
end

function JphHuHelper.CheckDianPaoHu(tItem, pos, pokerID)
	--在点炮胡的时候做检查，传入结构体，需要检查的座位ID，要检查的牌
	
	local needFan = tItem.m_vipRoomInfo.fantype
	if needFan == 1 then
		--无番起胡,在这里就可以了
		return true
	elseif needFan == 2 then
		--一番起胡，由于是点炮胡，不存在自摸加番的情况下， 所以，必须检查是不是鸡胡
		local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
		table.AscInsert(handList, pokerID)
		
		if false == JphHuHelper.IsJiHu(handList) then
			--如果不是鸡胡，就好了
			return true
		end
		--如果是鸡胡
		--就需要看是不是清一色了，
		if 0 ~= GdmjHuHelper.IsQingYiSe(tItem, pos) then
			return true  --清一色啊
		end
		
		--先检查风位
		--再检查风圈
		
		local function CheckTableLast(tItem, pos, checkList)
			for k1,v1 in ipairs(tItem.m_userList[pos].mjpokerlist) do
				for k2,v2 in ipairs(checkList) do
					if v.pokerlist[1] == v2 then
						table.remove(checkList,k2)
						break
					end
				end
			end
			for k1,v1 in ipairs(tItem.m_userList[pos].handpoker) do
				
				for k2,v2 in ipairs(checkList) do 
					if v1 == v2 then
						table.remove(checkList, k2)
						break
					end
				end
			end
			return #checkList
		end
		
		--检查是否存在中、发、白
		local checkList = {51,52,53}
		
		if 0 == CheckTableLast(tItem, pos, {51,52,53}) then
			return true
		end
		
		if 0 == CheckTableLast(tItem, pos, {41,42,43,44}) then
			return true
		end
	end
	
end

function JphHuHelper.IsJiHu(cardList)

	if #cardList == 14 then
		--这里需要判断是否是平胡，全是顺子的
		local tempList = table.clone(cardList)
		local list456 = MajiangUtils.Get456CardSArrList(tempList)
		
		if #tempList == 2 then
			--这里是平湖啊
			return false
		end
	end
	
	--检查是否是鸡胡
	local tempList = table.clone(cardList)
	local list555 = MajiangUtils.Get555CardSList(tempList)
	
	if #tempList > 2 then
		--只要剩余列表大于2，就必定不是碰碰胡了，可以返回去了
		return true
	end
end

--这个判断是不是天河，地和，人和
function JphHuHelper.IsFirstWin(tItem, pos)
	
	for i = 1,tItem.m_maxUser do
		if i ~= pos then
			if #tItem.m_userList[i].outpoker > 1 or #tItem.m_userList[i].mjpokerlist > 0 then
				return 0
			end
		else
			if #tItem.m_userList[i].outpoker > 0 or #tItem.m_userList[i].mjpokerlist > 0 then
				return 0
			end			
		end
	end
	--这里就是天和
	
	return g_gdmjWinType.type_tianhe
	--return true
end


function JphHuHelper.IsJiuBaoLianDeng(tItem, pos)
	
	if #tItem.m_userList[pos].mjpokerlist > 0 then
		return 0
	end
	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	
	local cardType = math.floor(handList[1]/10)
	
	
	local checkList = {0,0,0,0,0,0,0,0,0}
	for i = 1,#handList do
		if math.floor(handList[i]/10) ~= cardType then
			return 0
		end
		
		local num = math.mod(handList[i],10)
		checkList[num] = checkList[num] + 1
 	end
	
	
	if checkList[1] < 3 or checkList[9] < 3 then
		return 0
	end
	
	for i = 2,8 do
		if checkList[i] == 0 then
			return 0
		end
	end
	return g_gdmjWinType.type_jiulianbaodeng
end


function JphHuHelper.BaoQiangGangHu(tItem, tarChairID, pokerID)
	
	for k,v in ipairs(tItem.m_userList[tarChairID]) do
		if v.pokertype == g_gdmjAction.type_peng then
			if v.pokerlist[1] == pokerID then
				--这里是碰后抢杠胡的
				return 1
			end
		end
	end
	
	return 0
end