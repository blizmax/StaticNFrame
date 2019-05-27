--这是一个麻将胡牌的判断类
--返回胡牌的列表，如果有多个可能的组合，则返回多个
--这是给前后端公用的代码,这个是给赢牌前的判断
MajiangHuUtils = {}

--检查是否符合十三幺的特性

function MajiangHuUtils.ShiSanYao(cardList,guiLen)  --这里的
	if (#cardList + guiLen) ~= 14 then
		--如果长度不够十三个，就直接返回了，肯定不是十三幺
		return 0
	end
	
	local tempList = table.clone(cardList)  --这里不能够用cardList去处理
	local markList = {11,19,21,29,31,39,41,42,43,44,51,52,53}

	local mark = 1
	for i = 1,13 do
		local isExist = false
		for j = #tempList,1,-1 do
			if tempList[j] == markList[mark] then
				isExist = true
				table.remove(tempList,j)
			end
		end
		
		if isExist == true then
			table.remove(markList, mark)
		else
			mark = mark + 1
		end
		
		if #tempList ==0 or mark > #markList then
			break
		end
	end

	if #tempList > 0 then   --
		--如果剩余的长度大于1，说明肯定不是十三幺了
		return 0,{}  --返回长度0和空的类别
	end
	
	if #markList > guiLen then
		return 0,{}
	end
	
	if #markList == guiLen then
		return 1,markList
	end
	--如果鬼牌比这个还要长的话，说明是可以的。
	return 1, {11,19,21,29,31,39,41,42,43,44,51,52,53}
end

function MajiangHuUtils.JiuLianBaoDeng(cardList, guiLen)
	--九莲宝灯，必须是自己摸上来的牌，也就是门前清
	if (#cardList + guiLen) ~= 14 then
		--长度不是十三
		return 0,{}
	end
	local cardType = math.floor(cardList[1]/10)

	if cardType > 3 then   --如果是字牌
		return 0, {}
	end
	
	local tempList = {}
	for k,v in ipairs(cardList) do
		if cardType ~= math.floor(v/10) then
			return 0, {} --返回0
		else
			table.insert(tempList, math.mod(v,10))
		end
	end
	
	local markList = {1,1,1,2,3,4,5,6,7,8,9,9,9}
	local jiuLianBaoDeng = {1,1,1,2,3,4,5,6,7,8,9,9,9}
	for i = 1,13 do
		--只是13个长度就够了
		local isExist = false
		for k1,v1 in ipairs(tempList) do  --这里必须把temList 放在前面
			for k2,v2 in ipairs(markList) do
				if v1 == v2 then  --如果两个相等
					
					table.remove(tempList, k1) --两个列表都要删除，每次只删除一个
					table.remove(markList, k2) --两个列表都要删除，每次只删除一个
					isExist = true
					break
				end
			end
			break   --在这里，不管有没有找到，都需要break
		end
		if isExist == false or #tempList == 0 then  --只有这两种情况退出的
			break
		end
	end
	
	if #tempList > guiLen then
		--如果剩余的长度大于1，说明肯定不是九莲宝灯了
		return 0,{}  --返回长度0和空的类别
	elseif #tempList == guiLen then
		
		for i = 1,#tempList do
			tempList[i] = tempList[i] + cardType*10
		end
		
		return 1,tempList
	else
		local retList = {1,2,3,4,5,6,7,8,9}
		for i = 1,#retList do
			retList[i] = cardType*10 + retList[i]
		end
		return 1,retList
	end
	
end

function MajiangHuUtils.QiDui(cardList, guiLen)
	--这里判断是七对的
	--如果有碰或者杠了，就肯定不能组成七对了
	if (#cardList + guiLen) ~= 14 then
		--长度不是十三
		return 0,{}
	end
	
	local tempList = table.clone(cardList)
	local list55 = MajiangUtils.Get55CardSList(tempList)
	--取得了一对的列表
	
	--然后看看剩下的不能组成对子的列表的情况
	
	local len = #tempList - guiLen  --剩余的长度减去鬼牌的长度
	--首先这个长度绝对不是偶数。
	if len > 1 then
		--如果剩余的长度大于1，表示还不是听牌
		return 0, {}
	elseif len == 0 then
		--如果剩余的长度，刚好等于1，那就是，鬼牌抵消了一部分，那只要取得这个列表中的任意一个，就可以了.
		return 1,tempList
	else --如果还有鬼牌多的，那就猛了，只要取回任意的一个牌就行
		return 1,{-1}
	end
	
end

function MajiangHuUtils.SiGuiYingPai(cardList, guiList)
	if #guiList < 3 then
		return 0, {}
	elseif #guiList == 3 then
		return 1, guiList
	else
		--如果是四鬼以上的，直接赢牌
		return 1, {-1}           --g_gdmjDefine.mark_alllist		
	end
end





function MajiangHuUtils.CommonHu(cardList, guiLen)

	--按照将牌的包含鬼的程度来定
	
	local isHu,retList = MajiangHuUtils.GuiZuoJiang(cardList, guiLen)
	
 	if isHu ~= 0 then
		--为什么这里直接就返回了呢，因为这里是听万能牌了
		return 1,retList
	end
	
	isHu,retList = MajiangHuUtils.YiGuiZuoJiang(cardList, guiLen)
	
	local isHu2,huList2 = MajiangHuUtils.WuGuiZuoJiang(cardList, guiLen)
	if isHu2 ~= 0 then
		table.append(retList, huList2)
	end
	if #retList > 0 then
		return 1, retList
	else
		return 0,{}
	end
end


function MajiangHuUtils.GuiZuoJiang(cardList, guiLen)  --如果是鬼牌作为将牌的情况
	if guiLen<2 then
		return 0,{}
	end
	
	local guiTempLen = guiLen
	guiTempLen = guiLen - 2
	
	local isHu, huList = MajiangHuUtils.ProcessCardList(cardList,guiTempLen)
	if isHu ~= 0 then
		return 1,{-1}
	end
	return 0,{}
end

function MajiangHuUtils.YiGuiZuoJiang(cardList, guiLen)
	--一个鬼做将,另外一个牌从列表中来

	local guiTempLen = guiLen
	
	if guiTempLen < 1 then
		return 0,{}
	end

	guiTempLen = guiTempLen - 1
	
	local cardArrList = {}
	local pairList = {}   --保存对子的列表
	local mark = 0
	for i = 1,#cardList do
		if mark ~= cardList[i] then
			local tempList = table.clone(cardList)    --从所有的列表中取出来一个
			table.remove(tempList, i)
			mark = cardList[i]
			table.insert(cardArrList, tempList)
			table.insert(pairList, mark)
		end
	end
	
	local retList = {}
	
	for k,v in pairs(cardArrList) do
		--用剩下的牌型和鬼牌取操作
		local isHu,huList = MajiangHuUtils.ProcessCardList(v,guiTempLen)
		if isHu ~= 0 then   --只要有一个不是胡的，就直接返回
			table.insert(retList,pairList[k]) --这里不要忘了，把作将的鬼牌也要加进去。狗日的娘的
			table.append(retList, huList)
		end
		
	end
	
	if #retList > 0 then
		return 1, retList
	else
		return 0,{}
	end
end

function MajiangHuUtils.WuGuiZuoJiang(cardList, guiLen)
	--这个是没有鬼牌作将的情况下的，那奖牌只能够从cardList 里面取了
	local pairArrList = MajiangHuUtils.GetPairList(cardList)
	
	local retList = {}
	
	for k,v in pairs(pairArrList) do
		--用剩下的牌型和鬼牌取操作
		local isHu,huList = MajiangHuUtils.ProcessCardList(v,guiLen)
		if isHu ~= 0 then  --只要有一个不是胡的，就直接返回了
			table.append(retList, huList)
		end
		
	end
	
	if #retList > 0 then
		return 1, retList
	else
		return 0,{}
	end
	
	
end

function MajiangHuUtils.ProcessCardList(cardList, guiLen)
	local retList = {}
	local ziHuList = {} --如果胡牌的时候，对字牌的胡牌列表
	local guiTempLen = guiLen
	local cardArrList = {}
	cardArrList[1] = {}
	cardArrList[2] = {}
	cardArrList[3] = {}

	local ziList = {}
	
	for k,v in ipairs(cardList) do
		local cardType = math.floor(v/10)
		if cardType > 3 then
			table.insert(ziList, v)
		else
			table.insert(cardArrList[cardType], v)
		end
	end
	
	local ziList555 = MajiangUtils.Get555CardSList(ziList)
	local ziList55 = MajiangUtils.Get55CardSList(ziList)
	--对于字来说，需要的鬼牌的长度是 #ziList55 + #ziListX2
	local ziGuiNeedLen = #ziList55 + #ziList*2
	if guiTempLen < ziGuiNeedLen then --如果鬼牌的长度刚不够给这里。那就悲剧了，只能返回了
		return 0,{}
	end
	guiTempLen = guiTempLen - ziGuiNeedLen  --这里需要减去
	for k,v in ipairs(ziList55) do
		table.insert(ziHuList, v) --只要有鬼牌参与的，就算听的牌
	end
	
	for k,v in ipairs(ziList) do
		table.insert(ziHuList, v) --只要有鬼参与的，就算听的牌
	end

	for i = 3,1,-1 do
		if #cardArrList[i] == 0 then  --在这里把没用的给删除
			table.remove(cardArrList,i)
		end
		
	end
	
	--剩下的鬼牌就按照需要分配到需要需要的列表里面
	--怎么分配呢，这里
	local arrGuiList = {}
	local cardGuiLenList = {}
	local needAll = 0
	for k,v in ipairs(cardArrList) do
		local needLen = math.mod(#v,3)
		needLen = 3 - needLen
		needLen = needLen == 3 and 0 or needLen
		table.insert(cardGuiLenList, needLen) --得到需要分配的鬼牌的长度，这个是初次分配鬼牌的长度
		needAll = needAll + needLen
	end

	if needAll > guiTempLen then
		return 0,{}  --鬼牌的长度不够，理论上是这个牌出错了。这里先不管，返回先
	end

	local guiPreList = {}
	guiPreList[1] = guiTempLen - needAll
	for i = 1,#cardArrList do
		
		local isHu, huList = MajiangHuUtils.ProcessDetail(cardArrList[i],cardGuiLenList[i],guiPreList)
		if isHu == 0 then  --只要有一个不糊，就是不会胡
			return 0,{}
		else
			if guiPreList[1] > 0 then
				table.append(retList, huList)
			else
				table.append(retList, huList)
			end
		end
	end
	
	if guiPreList[1] > 0 then
		return 1,{-1}
	end
	
	table.append(retList, ziHuList)
	
	--到这里，其实已经算是胡了，可能是不需要鬼牌的胡
	return 1, retList

end


--传入需要判断的cardList:传入到这里的时候，已经是单一色的列表了
--传入鬼牌的列表
--传入额外的鬼牌的列表，当分配的鬼牌列表不足的时候，从这里取，每次取三个，传入剩余的鬼牌的table,这个只要是为了修改里面的值
function MajiangHuUtils.ProcessDetail(cardUinList, guiLen, guiPreList)

	if #cardUinList == 0 then
		return 0,{}
	end
	
	local retList = {}
	--首先把可能听的牌取下来，听的牌一定是在cardUinList前后两个位置的
	local markTempList = {}
	for k,v in ipairs(cardUinList) do   --这种判断，在鬼牌比较长的时候，会几何倍的减低遍历的长度
		local cardNum = math.mod(v,10)  --求模，取得面值，
		if cardNum == 1 then --如果面值是1
			markTempList[v] = 1
			markTempList[v+1] = 1
			markTempList[v+2] = 1
		elseif cardNum == 2 then
			markTempList[v-1] = 1
			markTempList[v] = 1
			markTempList[v+1] = 1
			markTempList[v+2] = 1
		elseif cardNum == 8 then
			markTempList[v-2] = 1
			markTempList[v-1] = 1
			markTempList[v] = 1
			markTempList[v+1] = 1
		elseif cardNum == 9 then
			markTempList[v-2] = 1
			markTempList[v-1] = 1
			markTempList[v] = 1
		else
			markTempList[v-2] = 1
			markTempList[v-1] = 1
			markTempList[v] = 1
			markTempList[v+1] = 1
			markTempList[v+2] = 1										
		end
	end

	local markList = {}
	for k,v in pairs(markTempList) do
		--TMD,强迫症患者，还是把顺序给逆过来吧
		table.AscInsert(markList,k) --这里是K额，不是V
	end
	local markLen = #markList
	if guiLen == 0 then
		--如果鬼牌的长度是0
		--表示不用鬼牌
		--
		if true == MajiangHuUtils.PressCardLast(cardUinList) then
			return 1,{} --这种情况下，返回胡了，可以听牌的列表是空的
		end
	elseif guiLen == 1 then
		--有一个鬼牌的情况下
		for k,v in ipairs(markList) do
			local tempList = table.clone(cardUinList)
			table.AscInsert(tempList, v)
			if true == MajiangHuUtils.PressCardLast(tempList) then
				table.insert(retList,v)
			end
		end
	elseif guiLen == 2 then
		--出现两个鬼的情况
		for k1 = 1,markLen do
			for k2 = k1, markLen do   --这里一定要是从k1开始的，要不然，就会出现漏掉55 ** 77的6
				local tempList = table.clone(cardUinList)
				table.AscInsert(tempList,markList[k1])
				table.AscInsert(tempList,markList[k2])
				if true == MajiangHuUtils.PressCardLast(tempList) then
					table.insert(retList, markList[k1])
					table.insert(retList, markList[k2])
				end
			end
		end
	end
	--分配的鬼牌的长度不会超过三的
	
	if #retList > 0 then --如果这里已经胡牌了，就不用再分配鬼牌了
		return 1,retList  
	end
	
	--狗日的还没有胡牌的情况下
	if guiPreList[1] < 3 then
		--每次鬼牌的操作，必定是按照三的整数来的
		--理论上是出错的了。
		return 0,{}
	end
	
	--再分配三个鬼牌
	guiPreList[1] = guiPreList[1] - 3
	guiLen = guiLen + 3
	
	if guiLen == 3 then
		for k1 =1, markLen do
			for k2 = k1,markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					local tempList = table.clone(cardUinList)
					table.AscInsert(tempList,markList[k1])
					table.AscInsert(tempList,markList[k2])
					table.AscInsert(tempList,markList[k3])
					if true == MajiangHuUtils.PressCardLast(tempList) then
						table.insert(retList,markList[k1])
						table.insert(retList,markList[k2])
						table.insert(retList,markList[k3])
					end
				end
			end
		end
	elseif guiLen == 4 then
		for k1 =1, markLen do
			for k2 = k1,markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					for k4 = k3, markLen do
						local tempList = table.clone(cardUinList)
						table.AscInsert(tempList,markList[k1])
						table.AscInsert(tempList,markList[k2])
						table.AscInsert(tempList,markList[k3])
						table.AscInsert(tempList,markList[k4])
						if true == MajiangHuUtils.PressCardLast(tempList) then
							table.insert(retList,markList[k1])
							table.insert(retList,markList[k2])
							table.insert(retList,markList[k3])
							table.insert(retList,markList[k4])
						end
					end
				end
			end
		end
	elseif guiLen == 5 then
		for k1 =1, markLen do
			for k2 = k1,markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					for k4 = k3, markLen do
						for k5 = k4, markLen do
							local tempList = table.clone(cardUinList)
							table.AscInsert(tempList,markList[k1])
							table.AscInsert(tempList,markList[k2])
							table.AscInsert(tempList,markList[k3])
							table.AscInsert(tempList,markList[k4])
							table.AscInsert(tempList,markList[k5])
							if true == MajiangHuUtils.PressCardLast(tempList) then
								table.insert(retList,markList[k1])
								table.insert(retList,markList[k2])
								table.insert(retList,markList[k3])
								table.insert(retList,markList[k4])
								table.insert(retList,markList[k5])
							end
						end
					end
				end
			end
		end		
	end

	if #retList > 0 then --如果这里已经胡牌了，就不用再分配鬼牌了
		return 1,retList
	end
	
	--TMD,五个鬼牌都不糊，搞毛啊
	if guiPreList[1] < 3 then
		--每次鬼牌的操作，必定是按照三的整数来的
		--理论上是出错的了。
		return 0,{}
	end	
	
	--再分配三个鬼牌
	guiPreList[1] = guiPreList[1] - 3
	guiLen = guiLen + 3
	
	if guiLen == 6 then
		for k1 =1, markLen do
			for k2 = k1, markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					for k4 = k3, markLen do
						for k5 = k4, markLen do
							for k6 = k5,markLen do
								local tempList = table.clone(cardUinList)
								table.AscInsert(tempList,markList[k1])
								table.AscInsert(tempList,markList[k2])
								table.AscInsert(tempList,markList[k3])
								table.AscInsert(tempList,markList[k4])
								table.AscInsert(tempList,markList[k5])
								table.AscInsert(tempList,markList[k6])
								if true == MajiangHuUtils.PressCardLast(tempList) then
									table.insert(retList,markList[k1])
									table.insert(retList,markList[k2])
									table.insert(retList,markList[k3])
									table.insert(retList,markList[k4])
									table.insert(retList,markList[k5])
									table.insert(retList,markList[k6])
								end
							end
						end
					end
				end
			end
		end
	elseif guiLen == 7 then
		for k1 =1, markLen do
			for k2 = k1,markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					for k4 = k3, markLen do
						for k5 = k4, markLen do
							for k6 = k5, markLen do
								for k7 = k6, markLen do
									local tempList = table.clone(cardUinList)
									table.AscInsert(tempList,markList[k1])
									table.AscInsert(tempList,markList[k2])
									table.AscInsert(tempList,markList[k3])
									table.AscInsert(tempList,markList[k4])
									table.AscInsert(tempList,markList[k5])
									table.AscInsert(tempList,markList[k6])
									table.AscInsert(tempList,markList[k7])
									if true == MajiangHuUtils.PressCardLast(tempList) then
										table.insert(retList,markList[k1])
										table.insert(retList,markList[k2])
										table.insert(retList,markList[k3])
										table.insert(retList,markList[k4])
										table.insert(retList,markList[k5])
										table.insert(retList,markList[k6])
										table.insert(retList,markList[k7])
									end
								end
							end
						end
					end
				end
			end
		end
	elseif guiLen == 8 then   --理论上不会出现这种情况的
		for k1 =1, markLen do
			for k2 = k1,markLen do
				for k3 = k2, markLen do  --这样就避免了重复遍历计算的问题了
					for k4 = k3, markLen do
						for k5 = k4, markLen do
							for k6 = k5, markLen do
								for k7 = k6, markLen do
									for k8 = k7, markLen do
										local tempList = table.clone(cardUinList)
										table.AscInsert(tempList,markList[k1])
										table.AscInsert(tempList,markList[k2])
										table.AscInsert(tempList,markList[k3])
										table.AscInsert(tempList,markList[k4])
										table.AscInsert(tempList,markList[k5])
										table.AscInsert(tempList,markList[k6])
										table.AscInsert(tempList,markList[k7])
										table.AscInsert(tempList,markList[k8])
										if true == MajiangHuUtils.PressCardLast(tempList) then
											table.insert(retList,markList[k1])
											table.insert(retList,markList[k2])
											table.insert(retList,markList[k3])
											table.insert(retList,markList[k4])
											table.insert(retList,markList[k5])
											table.insert(retList,markList[k6])
											table.insert(retList,markList[k7])
											table.insert(retList,markList[k8])
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
	
	if #retList > 0 then --如果还是没有胡，就真的逆天了。
		return 1,retList
	else
		return 0,{}
	end
end

function MajiangHuUtils.PressCardLast(cardList)
	if 0 ~= math.mod(#cardList,3) then --不能被三整除，下面就没得完了
		return false
	end
	--从第一个数据开始取，如果第一个是三个,就取三个出来，如果不是三个，就取看数量，取几次
	
	local numList = {}
	
	for k,v in ipairs(cardList) do
		--local temp = math.mod(v,10)
		
		if #numList ~= 0 and numList[#numList][1] == v then
			numList[#numList][2] = numList[#numList][2] + 1
		else
			local addItem = {}
			addItem[1] = v
			addItem[2] = 1
			table.insert(numList, addItem)
		end
	end
	
	for cycle = 1,10 do
		
		if #numList == 0 then
			return true
		end
		
		if numList[1][2] == 3 then
			table.remove(numList,1)  --如果第一个是三个长度的，那就删除第一个，就结束了
		elseif numList[1][2] == 4 then
			--如果是四张的，就是四个了
			numList[1][2] = numList[1][2] - 3   --先扣除三个，然后在补充
		else   --如果第一个不是三个的，就会组成456的了
			
			if #numList < 3 then  --长度都不够三个，当然返回了
				return false
			end
			
			if (numList[1][1] + 1) ~= numList[2][1] or (numList[1][1] + 2) ~= numList[3][1] then
				--没连成456
				return false
			end
			
			local len = numList[1][2]
			
			if numList[2][2] < len or numList[3][2] < len then
				--长度不够
				return false
			end
			numList[3][2] = numList[3][2] - len
			numList[2][2] = numList[2][2] - len
			numList[1][2] = numList[1][2] - len
			
			if numList[3][2] == 0 then
				table.remove(numList,3)
			end
			if numList[2][2] == 0 then
				table.remove(numList,2)
			end
			table.remove(numList,1)  --1是一定要被删除的
		end
		
	end 
	return false
end


function MajiangHuUtils.PressCardKaWuXing(cardList,checkPokerID)
	if 0 ~= math.mod(#cardList,3) then --不能被三整除，下面就没得完了
		return false
	end
	--从第一个数据开始取，如果第一个是三个,就取三个出来，如果不是三个，就取看数量，取几次
	
	local numList = {}
	
	for k,v in ipairs(cardList) do
		--local temp = math.mod(v,10)
		
		if #numList ~= 0 and numList[#numList][1] == v then
			numList[#numList][2] = numList[#numList][2] + 1
		else
			local addItem = {}
			addItem[1] = v
			addItem[2] = 1
			table.insert(numList, addItem)
		end
	end

	local isAll = false
	local isKaWuXing = false
	for cycle = 1,10 do
		
		if #numList == 0 then
			isAll = true
			break
		end
		
		if numList[1][2] == 3 then
			table.remove(numList,1)  --如果第一个是三个长度的，那就删除第一个，就结束了
		elseif numList[1][2] == 4 then
			--如果是四张的，就是四个了
			numList[1][2] = numList[1][2] - 3   --先扣除三个，然后在补充
		else   --如果第一个不是三个的，就会组成456的了
			
			if #numList < 3 then  --长度都不够三个，当然返回了
				--return false
				break
			end
			
			if (numList[1][1] + 1) ~= numList[2][1] or (numList[1][1] + 2) ~= numList[3][1] then
				--没连成456
				--return false
				break
			end
			
			local len = numList[1][2]
			
			if numList[2][2] < len or numList[3][2] < len then
				--长度不够
				break
			end
			numList[3][2] = numList[3][2] - len
			numList[2][2] = numList[2][2] - len
			numList[1][2] = numList[1][2] - len

			if numList[2][1] == checkPokerID then
				isKaWuXing = true
			end
			
			if numList[3][2] == 0 then
				table.remove(numList,3)
			end
			if numList[2][2] == 0 then
				table.remove(numList,2)
			end
			table.remove(numList,1)  --1是一定要被删除的
		end
		
	end 

	if isAll == true and isKaWuXing == true then
		return true
	else
		return false
	end
end


function MajiangHuUtils.GetPairList(cardList)
	--先取得对子的列表，返回对子的列表以及剩下的可能的列表
	--从第一个开始，到倒数第二个开始遍历
	local pairList = {}
	
	local len = 1
	while len < #cardList do
		if cardList[len] == cardList[len + 1] then
			local isExist = false
			for k1,v1 in ipairs(pairList) do
				if v1[1][1] == cardList[i] then  --对于已经出现过的对子，不用处理
					isExist = true  --这个是判断四条的情况的，如果出现四条了
					break
				end
			end
			if isExist == false then
				local tempList = {}
				for i = 1, #cardList do
					if i ~= len and i ~= (len + 1) then
						table.insert(tempList, cardList[i])
					end
				end
				pairList[ cardList[len] ] = tempList
			end
			len = len + 2 --将会跳过两个
		else
			len = len + 1
		end
	end
	return pairList  --返回对应的列表
end

function MajiangHuUtils.IsHzzJihu(handList, pokerID, openList)
	--该函数是插入胡牌列表前做的判断	
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
	
	for k,v in ipairs(openList) do
		if v.cardtype == g_gdmjAction.type_peng or v.cardtype == g_gdmjAction.type_gang then
			if v.cardlist[1] < 40 then
				local typeTemp = math.floor(v.cardlist[1]/10)
				if #cardList[typeTemp] == 0 then
					table.insert(cardList[typeTemp],v.cardlist[1])
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



