
--该类是通用与

GdmjWork = {}


function GdmjWork.VipPay(tInfo) --传过来的是gdmjinfo的结构体
	
	if tInfo.viptable == 0 then
		return nil
	end
	--这里开始扣钻石了
end


function GdmjWork.UserStartInit(tItem, mjUser)
	--初始化四个玩家
	while #mjUser.mjpokerlist > 0 do
		mjUser.mjpokerlist:remove(1)
	end

	mjUser.jinhu = 0
	mjUser.guohu = 0
	mjUser.zimoguohu = 0

	while #mjUser.guopeng > 0 do
		mjUser.guopeng:remove(1)
	end

	while #mjUser.guogang > 0 do
		mjUser.guogang:remove(1)
	end
	
	while #mjUser.handpoker > 0 do
		mjUser.handpoker:remove(1)
	end
	
	while #mjUser.outpoker > 0 do
		mjUser.outpoker:remove(1)
	end

	while #mjUser.tinglist > 0 do
		mjUser.tinglist:remove(1)
	end
	
	mjUser.autoplayer = 0
	mjUser.prop1 = 0
	
	--抽水，后面再加统计
	if tItem.m_tInfo.viptable == 0 then
		local pInfo = PlayerModel.GetPlayerInfo(mjUser.userid)
		PlayerModel.DecJetton(pInfo, tItem.m_tInfo.pourjetton, "MajiangStart", "每局麻将开始")
		PlayerModel.SendJetton(pInfo)
		mjUser.psinfo.jetton = pInfo.jetton
		mjUser.carryjetton = mjUser.psinfo.jetton
		PlayerModel.SetPlayerInfo(pInfo)
	else
		mjUser.julebu_jetton = JulebuService.GetJiFen(tItem.m_tInfo.julebuid, mjUser.userid) + mjUser.carryjetton
	end
end

function GdmjWork.GetRandPoker(tItem, pos)
		
	local getPoker = GdmjModel.GetNextCard(tItem.m_tInfo.tableid)
	
	if getPoker ~= 0 then
		
		for i = 1,#tItem.m_tInfo.publicpoker do
			if tItem.m_tInfo.publicpoker[i] == getPoker then
				tItem.m_tInfo.publicpoker:remove(i)
				break
			end
		end
		
		return getPoker
	end
	

	--local pokerID = GdmjWork.AllocateCard(tItem,pos)
	
	--if pokerID ~= 0 then
	--	return pokerID
	--end	
	
	local random = math.myrandom(1, #tItem.m_tInfo.publicpoker)
	
	local poker = tItem.m_tInfo.publicpoker[random]
	
	tItem.m_tInfo.publicpoker:remove(random)
	return poker
end

function GdmjWork.HandPokerInsert(tItem, pos, pokerID)
	
	if pokerID == nil then
		pokerID = GdmjWork.GetRandPoker(tItem, pos)
	end
	if pokerID == nil then
		LogFile("error","GdmjWork.HandPokerInsert poker is nil")
		return nil
	end
	
	local len = #tItem.m_userList[pos].handpoker    --取得长度
	if len == 0 then
		tItem.m_userList[pos].handpoker:append(pokerID)
		return pokerID
	end

	for k,v in ipairs(tItem.m_tInfo.guilist) do
		if v == pokerID then
			--如果是鬼牌，就插到第一位
			tItem.m_userList[pos].handpoker:append(0)
			local isExist = false
			for i = #tItem.m_userList[pos].handpoker,2,-1 do
				if tItem.m_userList[pos].handpoker[i] == pokerID then
					isExist = true  --表示这里已经有了一个了
				else
					tItem.m_userList[pos].handpoker[i] = tItem.m_userList[pos].handpoker[i-1]
				end
				
			end
			if isExist == false then
				tItem.m_userList[pos].handpoker[1] = pokerID
			end
			return pokerID
		end
	end



	local mark = len + 1
	for i = len,1,-1 do
		local isExist = false
		for k,v in ipairs(tItem.m_tInfo.guilist) do
			if v == tItem.m_userList[pos].handpoker[i] then
				mark = i + 1   --到了鬼牌也会停住
				isExist = true
				break
			end
		end
		
		if isExist == true then
			break
		end
		
		
		
		if pokerID <= tItem.m_userList[pos].handpoker[i] then
			mark = i
		else
			break
		end
	end
	tItem.m_userList[pos].handpoker:append(0)
	
	len = len + 1
	for i = len, mark + 1,-1 do
		tItem.m_userList[pos].handpoker[i] = tItem.m_userList[pos].handpoker[i - 1]
	end
	tItem.m_userList[pos].handpoker[mark] = pokerID
	tItem.m_userList[pos].humark = pokerID  --记录最新发的牌
	return pokerID
end

function GdmjWork.CheckIsHu(tItem,pos,pokerID)
	
	for k,v in ipairs(tItem.m_tInfo.guilist) do
		if v == pokerID then
			if #tItem.m_userList[pos].tinglist > 0 then
				return g_gdmjAction.type_hu
			end
		end
	end
	
	for k,v in ipairs(tItem.m_userList[pos].tinglist) do
		
		if v == pokerID or v == -1 then
			--已经胡了,如果是-1，表示也是全部都胡了
			return g_gdmjAction.type_hu
		end
	end
	

	if tItem.m_tInfo.viptable ~= 0 and tItem.m_vipRoomInfo.duohuahupai == 1 then
		--看看符不符合多花胡牌
		local huaNum = 0
		for k,v in ipairs(tItem.m_userList[pos].outpoker) do
			if v > 60 then
				huaNum = huaNum + 1
			end
		end
		if huaNum >= 7 then
			return g_gdmjAction.type_hu   --多花胡牌
		end
	end
	
	
	return g_gdmjAction.type_play
end

function GdmjWork.CheckIsGang(mjUser, pokerID)
	
	if pokerID > 60 then
		return 0
	end
	
	--这个是在发牌的时候
	for k,v in ipairs(mjUser.mjpokerlist) do
		--先检查碰中是否有杠
		if v.pokertype == g_gdmjAction.type_peng then
			if v.pokerlist[1] == pokerID then
				return pokerID
			end
		end
	end


	local count = 0
	
	local gangList = {}
	
	for k,v in ipairs(mjUser.handpoker) do
		if v == pokerID then
			count = count + 1
		end
		if gangList[v] == nil then
			gangList[v] = 1 
		else
			gangList[v] = gangList[v] + 1
		end

		for k2,v2 in ipairs(mjUser.mjpokerlist) do
			--先检查碰中是否有杠
			if v2.pokertype == g_gdmjAction.type_peng then
				if v2.pokerlist[1] == v then
					--直接返回对应,如果
					return v
				end
			end
		end
	end
	if count == 4 then
		return pokerID
	end
	--在检查剩下的
	for k,v in pairs(gangList) do
		if v == 4 then
			return k
		end
	end
	--这里还需要见见其他四个的问题
	
	
	
	return 0
end

function GdmjWork.CheckHuList(tItem, pos)
		
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	--local pengList = GdmjWork.GetPengList(tItem.m_userList[pos])
	--local gangList = GdmjWork.GetGangList(tItem.m_userList[pos])
	local guiList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	local huList = GdmjHuHelper.GetHuList(handList, guiList, tItem.m_tInfo.mjtype, tItem.m_vipRoomInfo)
	
	while #tItem.m_userList[pos].tinglist > 0 do
		tItem.m_userList[pos].tinglist:remove(1)
		--碰了后就不是听牌的状态了
	end	

	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz or tItem.m_tInfo.mjtype == g_gdmjType.type_hbz then
		while #tItem.m_userList[pos].tingtype > 0 do
			tItem.m_userList[pos].tingtype:remove(1)
		end

		if #huList > 0 then
			table.sort( huList, function(a,b) return a < b end )
			for k,v in ipairs(huList) do
				local huType = HzzmjHuHelper.CheckHuType(tItem, pos, v)
				if huType ~= 0 then
					tItem.m_userList[pos].tinglist:append(v)
					tItem.m_userList[pos].tingtype:append(huType)
				end
			end
		end
	else
		if #huList > 0 then
			table.sort( huList, function(a,b) return a < b end )
			for k,v in ipairs(huList) do
				tItem.m_userList[pos].tinglist:append(v)   --加入到听牌列表中
			end
		end		
	end
end

function GdmjWork.GetGuiList(tInfo)
	--获取鬼牌列表
	local guiList = {}
	for k,v in ipairs(tInfo.guilist) do
		table.insert(guiList, v)
	end
	return guiList
end


function GdmjWork.GetHandList(mjUser)
	--获取手牌列表
	local handList = {}
	for k,v in ipairs(mjUser.handpoker) do
		table.insert(handList,v)
	end
	return handList
end

function GdmjWork.GetPengList(mjUser)
	--获取碰牌列表
	local pengList = {}
	
	for k,v in ipairs(mjUser.mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_peng then
			table.insert(pengList, v.pokerlist[1])
		end
	end
	return pengList
end

function GdmjWork.GetGangList(mjUser)
	--获取杠牌列表
	local gangList = {}
	for k,v in ipairs(mjUser.mjpokerlist) do
		if v.pokertype == g_gdmjAction.type_gang then
			table.insert(gangList, v.pokerlist[1])
		end
	end
	return gangList	
end

function GdmjWork.SendStartInfo(tItem) --这里记得要传这个过来
	
	local gcStart = msg_gdmj_pb.gcgdmjstart()
	local str = tItem.m_tInfo:SerializeToString()
	gcStart.mjinfo:ParseFromString(tItem.m_tInfo:SerializeToString())
	for i = 1,tItem.m_maxUser do
		local addUser = gcStart.userlist:add()
		addUser:ParseFromString(tItem.m_userList[i]:SerializeToString())
	end
	gcStart.result = 0
	
	--先保存到缓存中
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2208].client,gcStart:SerializeToString())
	
	--下面开始下发，针对每个人，为了防止作弊，下发的内容是不一样的
	
	for i = 1,tItem.m_maxUser do
		--首先把全部设置为0
		for j = 1,#gcStart.userlist[i].handpoker do
			gcStart.userlist[i].handpoker[j] = 0
		end
		
	end
	
	local standList = GdmjWork.GetUserStandList(tItem.m_tInfo)
	
	--旁观的玩家也需要发送，只是发送的是空白的值
	SendMessage(standList,PacketCode[2208].client,gcStart:ByteSize(), gcStart:SerializeToString())
	
	for i = 1,tItem.m_maxUser do
		
		--发送给第一个
		for j = 1,#tItem.m_userList[i].handpoker do
			gcStart.userlist[i].handpoker[j] = tItem.m_userList[i].handpoker[j]
		end
		gcStart.result = 0   --为了防止作弊只能够一个一个发了
		SendMessage(tItem.m_userList[i].userid,PacketCode[2208].client,gcStart:ByteSize(), gcStart:SerializeToString())
		
		for j = 1,#tItem.m_userList[i].handpoker do   --这里记得把他设置为0
			gcStart.userlist[i].handpoker[j] = 0 --tItem.m_userList[i].handpoker[j]
		end		
	end
end

function GdmjWork.SendUpdate(tItem,gcUpdate, currPos, isLog)
	
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..", currPos="..currPos)
	end
	
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2210].client, gcUpdate:SerializeToString()) --先保存到缓存中
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..",set action end")
	end

	MajiangUtils.SendUpdate(gcUpdate, tItem)
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..",set log end")
	end
	if tItem.m_tInfo.beingpoker == 0 then
		--这种表示碰或者杠的，可以发送给全部人
		
		local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
		
		SendMessage(userIDList, PacketCode[2210].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
		
		if true == isLog then
			LogFile("moving","tableid="..tItem.m_tInfo.tableid..",send tItem.m_tInfo.beingpoker is 0 ")
		end
	else --如果需要发牌的
	
		--[[
		SendMessage(tItem.m_tInfo.situser[currPos], PacketCode[2210].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
		gcUpdate.actpokerid = 0 --如果是发牌的情况下，把牌的ID设置为0后再下发给其他玩家
		gcUpdate.pokerid = 1    --发给其他玩家，发送1就好了。
		local userIDList = GdmjWork.GetUserList(tItem.m_tInfo, tItem.m_tInfo.situser[currPos])
		SendMessage(userIDList, PacketCode[2210].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
		]]
		--先全部下发
		local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
		SendMessage(userIDList, PacketCode[2210].client, gcUpdate:ByteSize(), gcUpdate:SerializeToString())
		if true == isLog then
			LogFile("moving","tableid="..tItem.m_tInfo.tableid..",send tItem.m_tInfo.beingpoker is 1 ")
		end		
	end

end

function GdmjWork.SendAction(tItem, gcAction, isEnd)
	
	gcAction.result = 0
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2204].client, gcAction:SerializeToString()) --先保存到缓存中
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	MajiangUtils.SendAction(gcAction, tItem)
	if isEnd == true then
		--表示牌局结束了，可以下发全部数据
			
			for i = 1,tItem.m_maxUser do
				
				local isExist = false
				for j = 1,#gcAction.actplayer do
					if i == gcAction.actplayer[j].chairid then
						isExist = true
						break
					end
				end
				if isExist == false then
					for j = 1,#gcAction.tarplayer do
						if i == gcAction.tarplayer[j].chairid then
							isExist = true
							break
						end
					end
				end
				if isExist == false then
					local addTar = gcAction.tarplayer:add()
					addTar:ParseFromString(tItem.m_userList[i]:SerializeToString())					
				end
			end
		SendMessage(userIDList, PacketCode[2204].client, gcAction:ByteSize(),gcAction:SerializeToString())
	else
		
		SendMessage(userIDList, PacketCode[2204].client, gcAction:ByteSize(),gcAction:SerializeToString())
		--因为出现了乱牌。所以这里先全部下发。监控是否有是这里导致的乱牌
		--[[
		for i = 1,#gcAction.actplayer do
			for j = 1,#gcAction.actplayer[i].handpoker do
				gcAction.actplayer[i].handpoker[j] = 0
			end
			for k,v in ipairs(userIDList) do
				if v == gcAction.actplayer[i].userid then
					table.remove(userIDList,k)
					break
				end
			end
		end
		--首先发给没有值的玩家
		SendMessage(userIDList, PacketCode[2204].client, gcAction:ByteSize(),gcAction:SerializeToString())
		
		for i = 1,#gcAction.actplayer do
			
			for k,v in ipairs(tItem.m_userList[gcAction.actplayer[i].chairid].handpoker) do
				gcAction.actplayer[i].handpoker[k] = v
			end
			
			SendMessage(gcAction.actplayer[i].userid, PacketCode[2204].client, gcAction:ByteSize(),gcAction:SerializeToString())
			
			for j = 1,#gcAction.actplayer[i].handpoker do   --发送后记得置为空
				gcAction.actplayer[i].handpoker[j] = 0
			end	
		end
		]]
	end
	
end

function GdmjWork.CheckPengGang(tItem, pokerID, actChairid)

	--检查是否有碰以及杠
	if #tItem.m_tInfo.publicpoker == 0 then
		return 0,0  --剩下最后一张牌了，不能碰和杠，因为碰和杠后就不能够出牌了
	end
	
	local checkList = GdmjWork.GetCheckList(actChairid, tItem.m_maxUser)   --
	for k1,v1 in ipairs(checkList) do
		--开始检查碰，杠
		local num = 0
		for k2,v2 in ipairs(tItem.m_userList[v1].handpoker) do
			
			if pokerID == v2 then
				num = num + 1
			end
		end 
		
		if num >= 2 then
			--在这里，表示就可以碰或者杠了
			return v1,num     --返回所在的位置，以及数量
		end
		
	end
	return 0,0
end

function GdmjWork.CheckNewPengGang(tItem, pokerID, actChairid)
	--该函数是检查新型的碰和杠的，碰的时候需要检查是否存在过碰不碰的情况
	--返回的时候，是返回财政的列表
	if #tItem.m_tInfo.publicpoker == 0 then
		return 0,0  --剩下最后一张牌了，不能碰和杠，因为碰和杠后就不能够出牌了
	end
	local retList = {}
	local checkList = GdmjWork.GetCheckList(actChairid, tItem.m_maxUser)   --
	for k1,v1 in ipairs(checkList) do
		--开始检查碰，杠
		local num = 0
		for k2,v2 in ipairs(tItem.m_userList[v1].handpoker) do
			
			if pokerID == v2 then
				num = num + 1
			end
		end 
		if num >= 2 then
			--在这里，表示就可以碰或者杠了
			local isExist = false
			for k2,v2 in ipairs(tItem.m_userList[v1].guopeng) do
				if v2 == pokerID then
					isExist = true
					break
				end
			end
			if isExist == false then
				table.insert(retList, g_gdmjAction.type_peng)
				if num > 2 then
					table.insert(retList, g_gdmjAction.type_gang)
				end
			elseif num > 2 then
				table.insert(retList, g_gdmjAction.type_gang)
			
			end				
			return #retList == 0 and 0 or v1,retList
		end
	end
	return 0,{}
end

function GdmjWork.GetCheckList(startPos,maxUser)
	--取出除了startPos外，剩下的其他的几个数
	--默认长度是4个
	--例如，传入 2，取得：3,4,1
	if maxUser == nil then
		maxUser = 4
	end
	
	local retList = {}
	
	for i = startPos + 1,maxUser do
		table.insert(retList, tonumber(i))
	end
	for i = 1,startPos - 1 do
		table.insert(retList, tonumber(i))
	end
	return retList
end

function GdmjWork.NextInfoInit(nextInfo)
	--初始化牌桌信息中的nextInfo的信息
	for i = 1,#nextInfo.actchairid do    --把每个玩家的人初始化为0
		nextInfo.actchairid[i] = 0
	end
	
	nextInfo.tarchairid = 0
	nextInfo.actpokerid = 0
	nextInfo.typedetail = 0
	nextInfo.canplay = 0
	
	while #nextInfo.actiontype > 0 do
		nextInfo.actiontype:remove(1)
	end
	
end


function GdmjWork.ActionPeng(tItem,cgmsg, gcmsg)
	
	local amount = 2  --有可能存在三个一样的情况下
	local len = 1
	
	local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
	mjPoker.pokerlist:append(cgmsg.actpokerid)
	mjPoker.pokerlist:append(cgmsg.actpokerid)
	mjPoker.pokerlist:append(cgmsg.actpokerid)
	mjPoker.pokertype = cgmsg.actiontype

	GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 2) --删除两个
	
	gcmsg.showpoker.pokerlist:append(cgmsg.actpokerid)
	gcmsg.showpoker.pokerlist:append(cgmsg.actpokerid)
	gcmsg.showpoker.pokerlist:append(cgmsg.actpokerid)
	gcmsg.showpoker.pokertype = cgmsg.actiontype
	
	len = #tItem.m_userList[cgmsg.tarchairid].outpoker
	tItem.m_userList[cgmsg.tarchairid].outpoker:remove(len)  --还要把最后一个牌删掉
	local len2 = #tItem.m_userList[cgmsg.tarchairid].outpoker
	
	while #tItem.m_userList[cgmsg.actchairid].tinglist > 0 do
		tItem.m_userList[cgmsg.actchairid].tinglist:remove(1)
		--碰了后就不是听牌的状态了
	end	
end

function GdmjWork.GetCurrPos(tInfo)
	for k,v in ipairs(tInfo.nextinfo.actchairid) do
		if v ~= 0 then
			return v
		end
	end
	return 0
end

function GdmjWork.GetCurrPosList(tInfo)
	local retList = {}
	for k,v in ipairs(tInfo.nextinfo.actchairid) do
		if v ~= 0 then
			table.insert(retList, k)  --这里不是insert v，是insert k 
		end
	end
	if #retList == 0 then
		table.insert(retList,  tInfo.bankerpos )
	end
	return retList
end

function GdmjWork.GetUserList(tInfo, execptID)
	local retList = {}
	for k,v in ipairs(tInfo.situser) do
		if v ~= 0 and v ~= execptID then
			table.insert(retList,v)
		end
	end
	for k,v in ipairs(tInfo.standuser) do
		if v ~= execptID then
			table.insert(retList, v)
		end
	end
	return retList
end

function GdmjWork.GetUserSitList(tInfo, execptID)
	--获取座位上的玩家列表
	local retList = {}
	for k,v in ipairs(tInfo.situser) do
		if v ~= 0 and v ~= execptID then
			table.insert(retList,v)
		end
	end
	return retList	
end

function GdmjWork.GetUserStandList(tInfo, execptID)
	--获取站立的列表
	local retList = {}
	for k,v in ipairs(tInfo.standuser) do
		if v ~= execptID then
			table.insert(retList, v)
		end
	end
	return retList
end

function GdmjWork.DelPokerFromHand(mjUser,pokerID,num)
	--在手牌中删除已经打出去的牌型
	num = num == nil and 1 or num
	local len = 1
	
	while len <= #mjUser.handpoker do
		if num <= 0 then
			break
		end
		if mjUser.handpoker[len] == pokerID then
			mjUser.handpoker:remove(len)
			num = num - 1
		else
			len = len + 1
		end
		
	end
	return 0
end

function GdmjWork.AddPokerToOutList(mjUser, pokerID)
	--把打出去的牌型，添加到出牌的列表中
	mjUser.outpoker:append(pokerID)
end

function GdmjWork.DelPokerFromOutList(mjUser, pokerID)
	--从手牌中，删除最后一张牌
	local len = #mjUser.outpoker
	
	if len > 0 and mjUser.outpoker[len] == pokerID then
		mjUser.outpoker:remove(len)
	end
	
end

function GdmjWork.CheckQiangGangHu(tItem, cgmsg)
	--抢杠胡是可以存在一炮多响的
	--在这里需要先检查是否符合抢杠胡的条件	
	local gangType = 0
	
	if cgmsg.tarchairid ~= 0 then
		gangType = g_gdmjGangDetail.type_minggang
		--先设置为明杠
	end
	
	
	for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
		if v.pokerlist[1] == cgmsg.actpokerid and v.pokertype == g_gdmjAction.type_peng then
			if cgmsg.tarchairid == 0 then
				gangType = g_gdmjGangDetail.type_bugang
				--这里在设置为补杠
			end
		end
	end
	
	if gangType == 0 then
		return {}
	end

	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh or tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		--如果类型是推倒胡或者做牌推倒胡
		if tItem.m_vipRoomInfo.keqiangganghu == 0 then
			--如果不能抢杠胡，就直接返回了
			return {}
		else
			--如果可以抢抢杠胡
			if tItem.m_vipRoomInfo.minggangkeqiang == 0 and gangType == g_gdmjGangDetail.type_minggang then
				--如果是明杠可不抢，但是类型又是明杠的，就直接返回了
				return {}
			end
		end
		--到这里才开始走到下面	
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jph then
		--鸡平胡是可以抢杠胡
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
		--金币场是可以抢杠胡的
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzz then
		--这个也是可以抢杠胡的
	end

	local userIDList = GdmjWork.GetCheckList(cgmsg.actchairid, tItem.m_maxUser) --默认是四个玩家
	
	local retList = {}
	
	for k1,v1 in ipairs(userIDList) do
		
		for k2,v2 in ipairs(tItem.m_userList[v1].tinglist) do
			--检查听牌列表，如果行动者杠的牌在玩家的听牌列表中，表示这个是可以抢杠胡的
			--把对应的位置加入到返回列表中
			if v2 == -1 or v2 == cgmsg.actpokerid then
				table.insert(retList, v1)
				break
			end
		end 
		
	end
	return retList
end


function GdmjWork.ActionGang(tItem, cgmsg, gcmsg)
	
	local temp = cgmsg.tarchairid
	local temp2 = cgmsg.actchairid
	
	gcmsg.showpoker.pokertype = g_gdmjAction.type_gang   --杠的行动
	gcmsg.showpoker.typedetail = 0   --先初始化为0
	for i = 1,4 do
		gcmsg.changejetton:append(0)   --先补充好四个0
		gcmsg.showpoker.pokerlist:append(cgmsg.actpokerid)		
	end
	local diFen = 1
	if tItem.m_tInfo.mjtype == g_gdmjType.type_hzmj or tItem.m_tInfo.mjtype == g_gdmjType.type_rpmj then
		diFen = tItem.m_vipRoomInfo.difen
	end
	
	if cgmsg.tarchairid ~= 0 then
		--是明杠,从自己的手上的牌拿出来,然后被杠的人扣掉3倍的低分,自己加上3倍的分数
		gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_minggang
		local len = 1
		
		local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
		
		for i = 1, 4 do
			mjPoker.pokerlist:append(cgmsg.actpokerid)
		end
		
		mjPoker.pokertype = g_gdmjAction.type_gang
		mjPoker.typedetail = g_gdmjGangDetail.type_minggang
		
		local changeJetton = (tItem.m_maxUser - 1)*diFen   --需要加和扣去的底分
		
		tItem.m_userList[cgmsg.actchairid].carryjetton = tItem.m_userList[cgmsg.actchairid].carryjetton + changeJetton
		gcmsg.changejetton[cgmsg.actchairid] = changeJetton
		
		--如果被杠玩家的筹码不足了，就补到0,这里要区分对待贵宾房和普通房
		
		if tItem.m_tInfo.viptable ~= 0 then
			--如果是贵宾房 --
			if tItem.m_tInfo.mjtype == g_gdmjType.type_rpmj and tItem.m_vipRoomInfo.gangbaoquanbao == 1 then
				for i = 1, #tItem.m_userList do
					gcmsg.changejetton[i] = 0 - 1
					tItem.m_userList[i].carryjetton = tItem.m_userList[cgmsg.tarchairid].carryjetton - 1
				end
			else
				gcmsg.changejetton[cgmsg.tarchairid] = 0 - changeJetton
				tItem.m_userList[cgmsg.tarchairid].carryjetton = tItem.m_userList[cgmsg.tarchairid].carryjetton - changeJetton
			end

		else --如果不是贵宾房，则要考虑到为0的情况
			changeJetton = changeJetton > tItem.m_userList[cgmsg.tarchairid].carryjetton and tItem.m_userList[cgmsg.tarchairid].carryjetton or changeJetton
			gcmsg.changejetton[cgmsg.tarchairid] = 0 - changeJetton
			tItem.m_userList[cgmsg.tarchairid].carryjetton = tItem.m_userList[cgmsg.tarchairid].carryjetton - changeJetton
		end
		
		--最后把被杠玩家已经出的牌扣掉
		
		GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 3)
		
		local addPlayer = gcmsg.tarplayer:add()   --在这里就把需要更新的人给加进去了，其他的就不用操作了
		addPlayer:ParseFromString(tItem.m_userList[cgmsg.tarchairid]:SerializeToString())
		tItem.m_detailList[cgmsg.actchairid].minggangnum = tItem.m_detailList[cgmsg.actchairid].minggangnum + 1
		
		mark = #tItem.m_userList[cgmsg.tarchairid].outpoker
		tItem.m_userList[cgmsg.tarchairid].outpoker:remove(mark)
		
		tItem.m_userModify[cgmsg.tarchairid] = 1
	else --这个是暗杠，分两种，一种是从自己碰杠，一种是暗杠
		
		local changeJetton = 0
		
		gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_angang
		
		
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
			
			if v.pokertype == g_gdmjAction.type_peng and v.pokerlist[1] == cgmsg.actpokerid then
				--这个是补杠
				gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_bugang
				v.pokertype = g_gdmjAction.type_gang
				v.typedetail = g_gdmjGangDetail.type_bugang
				v.pokerlist:append(cgmsg.actpokerid)
				
				changeJetton = diFen   --设置底分
				GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 1)
				break
			end
			
		end
		
		if changeJetton == 0 then
			--说明是暗杠
			local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
			for i = 1, 4 do
				mjPoker.pokerlist:append(cgmsg.actpokerid)
			end
			
			mjPoker.pokertype = g_gdmjAction.type_gang
			mjPoker.typedetail = g_gdmjGangDetail.type_angang
			changeJetton = 2*diFen   --设置底分
			GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 4)
			tItem.m_detailList[cgmsg.actchairid].angangnum = tItem.m_detailList[cgmsg.actchairid].angangnum + 1
		else
			tItem.m_detailList[cgmsg.actchairid].minggangnum = tItem.m_detailList[cgmsg.actchairid].minggangnum + 1
		end
		
		for i = 1,tItem.m_maxUser do
			if i == cgmsg.actchairid then
				--这里是增加的
				tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton + (tItem.m_maxUser - 1)*changeJetton
				
				gcmsg.changejetton[i] =  (tItem.m_maxUser - 1)*changeJetton
			else				
				tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton - changeJetton
				
				if tItem.m_tInfo.viptable == 0 and tItem.m_userList[i].carryjetton == 0 then
					--如果牌桌不是私人房，并且玩家的筹码是0的情况下
					tItem.m_userList[i].carryjetton = 0
				end
				gcmsg.changejetton[i] = 0 - changeJetton
				
				local addPlayer = gcmsg.tarplayer:add()   --在这里就把需要更新的人给加进去了，其他的就不用操作了
				addPlayer:ParseFromString(tItem.m_userList[i]:SerializeToString())
				
			end
			tItem.m_userModify[i] = 1   --在这里，需要把数据保存到缓存中
		end
		
	end
	
end


function GdmjWork.ActionGang2(tItem, cgmsg, gcmsg)
	--这个杠是不扣分的，结算的时候再扣分
	local temp = cgmsg.tarchairid
	local temp2 = cgmsg.actchairid
	
	gcmsg.showpoker.pokertype = g_gdmjAction.type_gang   --杠的行动
	gcmsg.showpoker.typedetail = 0   --先初始化为0
	for i = 1,4 do
		gcmsg.changejetton:append(0)   --先补充好四个0
		gcmsg.showpoker.pokerlist:append(cgmsg.actpokerid)		
	end
	
	if cgmsg.tarchairid ~= 0 then
		--是明杠,从自己的手上的牌拿出来,然后被杠的人扣掉3倍的低分,自己加上3倍的分数
		gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_minggang
		local len = 1
		
		local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
		
		for i = 1, 4 do
			mjPoker.pokerlist:append(cgmsg.actpokerid)
		end
		
		mjPoker.pokertype = g_gdmjAction.type_gang
		mjPoker.typedetail = g_gdmjGangDetail.type_minggang
		mjPoker.tarchairid = cgmsg.tarchairid
		--local changeJetton = 3*tItem.m_tInfo.pourjetton   --需要加和扣去的底分
		--tItem.m_userList[cgmsg.actchairid].carryjetton = tItem.m_userList[cgmsg.actchairid].carryjetton + changeJetton
		--gcmsg.changejetton[cgmsg.actchairid] = changeJetton
		
		--如果被杠玩家的筹码不足了，就补到0,这里要区分对待贵宾房和普通房
		
		if tItem.m_tInfo.viptable ~= 0 then
			--如果是贵宾房
			
			--gcmsg.changejetton[cgmsg.tarchairid] = 0 - changeJetton
			--tItem.m_userList[cgmsg.tarchairid].carryjetton = tItem.m_userList[cgmsg.tarchairid].carryjetton - changeJetton

		else --如果不是贵宾房，则要考虑到为0的情况
			--changeJetton = changeJetton > tItem.m_userList[cgmsg.tarchairid].carryjetton and tItem.m_userList[cgmsg.tarchairid].carryjetton or changeJetton
			--gcmsg.changejetton[cgmsg.tarchairid] = 0 - changeJetton
			--tItem.m_userList[cgmsg.tarchairid].carryjetton = tItem.m_userList[cgmsg.tarchairid].carryjetton - changeJetton
		end
		
		--最后把被杠玩家已经出的牌扣掉
		
		GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 3)
		
		local addPlayer = gcmsg.tarplayer:add()   --在这里就把需要更新的人给加进去了，其他的就不用操作了
		addPlayer:ParseFromString(tItem.m_userList[cgmsg.tarchairid]:SerializeToString())
		
		tItem.m_detailList[cgmsg.actchairid].minggangnum = tItem.m_detailList[cgmsg.actchairid].minggangnum + 1
		
		mark = #tItem.m_userList[cgmsg.tarchairid].outpoker
		tItem.m_userList[cgmsg.tarchairid].outpoker:remove(mark)
		
		tItem.m_userModify[cgmsg.tarchairid] = 1
	else --这个是暗杠，分两种，一种是从自己碰杠，一种是暗杠
		
		local changeJetton = 0
		
		gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_angang
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
			
			if v.pokertype == g_gdmjAction.type_peng and v.pokerlist[1] == cgmsg.actpokerid then
				--这个是补杠
				gcmsg.showpoker.typedetail = g_gdmjGangDetail.type_bugang
				v.pokertype = g_gdmjAction.type_gang
				v.typedetail = g_gdmjGangDetail.type_bugang
				v.pokerlist:append(cgmsg.actpokerid)
				
				changeJetton = tItem.m_tInfo.pourjetton   --设置底分
				GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 1)
				break
			end
			
		end
		
		if changeJetton == 0 then
			--说明是暗杠
			local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
			for i = 1, 4 do
				mjPoker.pokerlist:append(cgmsg.actpokerid)
			end
			
			mjPoker.pokertype = g_gdmjAction.type_gang
			mjPoker.typedetail = g_gdmjGangDetail.type_angang
			--changeJetton = 2*tItem.m_tInfo.pourjetton   --设置底分
			GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid], cgmsg.actpokerid, 4)
			tItem.m_detailList[cgmsg.actchairid].angangnum = tItem.m_detailList[cgmsg.actchairid].angangnum + 1
		else
			tItem.m_detailList[cgmsg.actchairid].minggangnum = tItem.m_detailList[cgmsg.actchairid].minggangnum + 1
		end
		
		for i = 1,tItem.m_maxUser do
			if i == cgmsg.actchairid then
				--这里是增加的
				--tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton + 3*changeJetton
				
				--gcmsg.changejetton[i] =  3*changeJetton
			else				
				--tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton - changeJetton
				
				--if tItem.m_tInfo.viptable == 0 and tItem.m_userList[i].carryjetton == 0 then
					--如果牌桌不是私人房，并且玩家的筹码是0的情况下
					--tItem.m_userList[i].carryjetton = 0
				--end
				--gcmsg.changejetton[i] = 0 - changeJetton
				local addPlayer = gcmsg.tarplayer:add()   --在这里就把需要更新的人给加进去了，其他的就不用操作了
				addPlayer:ParseFromString(tItem.m_userList[i]:SerializeToString())
				
			end
			tItem.m_userModify[i] = 1   --在这里，需要把数据保存到缓存中
		end
		
	end
	
end


function GdmjWork.InitPubGuiPai(tItem)
	--初始化公共玩家的鬼牌
	--先把鬼牌跳出来
	local index = 0
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh or tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		--如果是推倒胡
		
		if tItem.m_vipRoomInfo.typegui == g_gdmjGuiType.type_baiban then
			--白板当鬼
			index = 53
			tItem.m_tInfo.guilist:append(index)
		elseif tItem.m_vipRoomInfo.typegui == g_gdmjGuiType.type_fangui then

			local random = 0 
			if tItem.m_vipRoomInfo.budaifeng == 1 then
				--如果是不带风的情况下
				random = math.myrandom(1,#g_gdmjDefine.mark_alllist - 7)
			else
				random = math.myrandom(1,#g_gdmjDefine.mark_alllist)
			end


			index = g_gdmjDefine.mark_alllist[random]
			
			if index < 40 then
				local num = math.mod(index,10)
				
				if tItem.m_vipRoomInfo.shuanggui == 1 then
					--如果是双鬼的
					if num == 9 then
						--如果是等于9
						tItem.m_tInfo.guilist:append(index - 8)
						tItem.m_tInfo.guilist:append(index - 7)
					elseif num == 8 then
						tItem.m_tInfo.guilist:append(index + 1)
						tItem.m_tInfo.guilist:append(index - 7)
					else
						tItem.m_tInfo.guilist:append(index + 1)
						tItem.m_tInfo.guilist:append(index + 2)
					end
				else
					if num == 9 then
						tItem.m_tInfo.guilist:append(index - 8)
					else
						tItem.m_tInfo.guilist:append(index + 1)
					end
				end
			else
				if tItem.m_vipRoomInfo.shuanggui == 1 then
					if index == 43 then
						tItem.m_tInfo.guilist:append(44)
						tItem.m_tInfo.guilist:append(51)
					elseif index == 44 then
						tItem.m_tInfo.guilist:append(51)
						tItem.m_tInfo.guilist:append(52)
					elseif index == 52 then
						tItem.m_tInfo.guilist:append(53)
						tItem.m_tInfo.guilist:append(41)
					elseif index == 53 then
						tItem.m_tInfo.guilist:append(41)
						tItem.m_tInfo.guilist:append(42)				
					else
						tItem.m_tInfo.guilist:append(index + 1)
						tItem.m_tInfo.guilist:append(index + 2)
					end
				else
					if index == 44 then
						tItem.m_tInfo.guilist:append(51)
					elseif index == 53 then
						tItem.m_tInfo.guilist:append(41)
					else
						tItem.m_tInfo.guilist:append(index + 1)
					end
				end
				

			end
			
		end
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
		--如果是金币场，就看场次的大小,稍后在弄鬼牌
		if tItem.m_tInfo.tabletype == g_gdmjTableType["medium"] or tItem.m_tInfo.tabletype == g_gdmjTableType["serior"] then
			local random = math.myrandom(1,#g_gdmjDefine.mark_alllist)
			index = g_gdmjDefine.mark_alllist[random]

			if index < 40 then
				local num = math.mod(index,10)
				if num == 9 then
					tItem.m_tInfo.guilist:append(index - 8)
				else
					tItem.m_tInfo.guilist:append(index + 1)
				end
			else
				if index == 44 then
					tItem.m_tInfo.guilist:append(51)
				elseif index == 53 then
					tItem.m_tInfo.guilist:append(41)
				else
					tItem.m_tInfo.guilist:append(index + 1)
				end
			end
		end
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzmj then
		tItem.m_tInfo.guilist:append(51)
	end
	
	
	if index ~= 0 then
		for k,v in ipairs(tItem.m_tInfo.publicpoker) do
			if v == index then
				--把其中的一个鬼牌删除
				tItem.m_tInfo.publicpoker:remove(k)
				break
			end
		end
	end
	
end

function GdmjWork.IsQiangGangQuanBao(tItem)
	--抢杠是否抢杠全包的
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh or tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		return tItem.m_vipRoomInfo.qianggangquanbao
	else
		return 0
	end
	
	return 0
end

function GdmjWork.IsGangBaoQuanBao(tItem)
	--抢杠是否抢杠全包的
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh or tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		return tItem.m_vipRoomInfo.gangbaoquanbao
	else
		return 0
	end
	
	return 0
end

function GdmjWork.GenZhuangCheck(tItem,cgmsg)
	--检查该游戏是否符合跟庄
	--只要在玩家出牌的时候会检查
	--这个是另外的新的协议
	for i = 1,tItem.m_maxUser do
		if #tItem.m_userList[i].outpoker ~= 1 then
			--就是长度不等于1
			return false
		end
		
		if #tItem.m_userList[i].handpoker ~= 13 then
			--在这里不能够检查mjpokerlist 的长度
			return false
		end
		
		if tItem.m_userList[i].outpoker[1] ~= cgmsg.actpokerid then
			--除了满足上面四个条件，还要满足这个条件
			return false
		end
		
		--这里先放在杠分中
	end
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh or tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		if tItem.m_vipRoomInfo.gengzhuang ~= 1 then
			return false
		end
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jph or tItem.m_tInfo.mjtype == g_gdmjType.type_hzmj or tItem.m_tInfo.mjtype == g_gdmjType.type_rpmj then
		--鸡平胡不用检查跟庄
		return false
		
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
		return false
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz or tItem.m_tInfo.mjtype == g_gdmjType.type_hbz then
		--跟庄
		if tItem.m_vipRoomInfo.gengzhuang ~= 1 then
			return false
		end
	end
	tItem.m_tInfo.genzhuang = 1	
--[[	local changeJetton = 2*tItem.m_tInfo.pourjetton
	local gcAction = msg_gdmj_pb.gcgdmjaction()
	gcAction.showpoker.pokertype = g_gdmjAction.type_genzhuang   --杠的行动
	for i = 1,tItem.m_maxUser do
		
		if i == tItem.m_tInfo.bankerpos then 
			--这个是庄家
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton - (tItem.m_maxUser - 1)*changeJetton
			
			local addPlayer = gcAction.tarplayer:add()
			gcAction.changejetton:append(-(tItem.m_maxUser - 1)*changeJetton)
			addPlayer:ParseFromString(tItem.m_userList[i]:SerializeToString())
		else
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton + changeJetton
			gcAction.changejetton:append(changeJetton)
			local addPlayer = gcAction.actplayer:add()
			addPlayer:ParseFromString(tItem.m_userList[i]:SerializeToString())
		end
		tItem.m_userModify[i] = 1
	end
	gcAction.actiontype:append(g_gdmjAction.type_genzhuang)
	gcAction.result = 0
	GdmjWork.SendAction(tItem, gcAction)--]]
end

function GdmjWork.HuGuiCheck(tItem, pos)
	--胡牌的时候对鬼牌的检查
	--返回两个参数，第一个是公共鬼牌的长度，第二个是自己手上鬼牌的长度
	local guiList = GdmjWork.GetGuiList(tItem.m_tInfo)
	
	if #guiList == 0 then
		return 0,0
	end
	
	local priLen = 0
	for k1,v1 in ipairs(tItem.m_userList[pos].handpoker) do
		for k2,v2 in ipairs(guiList) do
			if v1 == v2 then
				priLen = priLen + 1
				break
			end
		end
	end
	return #guiList,priLen
end

function GdmjWork.CheckChi(tItem, actChairid, actPokerID)
	if actPokerID > 40 then
		--只有万筒条能吃
		return 0
	end
	--cgmsg
	local checkPos = actChairid == tItem.m_maxUser and 1 or (actChairid + 1)
	local handList = GdmjWork.GetHandList(tItem.m_userList[checkPos])
	if #tItem.m_tInfo.guilist > 0 then
		local maxLen = 4*#tItem.m_tInfo.guilist
		maxLen = maxLen > #handList and #handList or maxLen
		for i = 1,#tItem.m_tInfo.guilist do
			if actPokerID == tItem.m_tInfo.guilist[i] then
				return 0
			end
			
			for m = maxLen,1,-1 do
				if handList[m] == tItem.m_tInfo.guilist[i] then
					table.remove(handList,m)
				end
			end
			
		end
		
		
	end
	
	local checkList = {}
	for k,v in ipairs(handList) do
		if math.floor(v/10) == math.floor(actPokerID/10) and v >= (actPokerID - 2) and v <= (actPokerID + 2) then
			table.AscInsert(checkList, v)
		end
	end
	
	if #checkList < 2 then --长度不够2
		return 0
	end
	
	table.AscInsert(checkList, actPokerID)   --这里需要插入这个
	
	local checkLen = 1
	local checkValue = checkList[1]
	for i = 2,#checkList do
		
		if checkValue == checkList[i] then
		
		elseif checkList[i] == (checkValue + 1) then
			checkLen = checkLen + 1
			checkValue = checkList[i]
		else
			checkLen = 1
			checkValue = checkList[i]
		end
		if checkLen >= 3 then
			--break
			return checkPos
		end
	end
	
	
	return 0
end




function GdmjWork.AddBuHuaPokerID(tItem, pos, pokerID)
	--把杠的牌放到公共列表的第一位
	
	local isExist = false
	tItem.m_userList[pos].outpoker:append(0)
	
	if tItem.m_userList[pos].outpoker[1] > 60 or tItem.m_userList[pos].outpoker[1] == tItem.m_tInfo.guilist[1] then
		isExist = true
	end
	
	
	if isExist == false then
		tItem.m_userList[pos].outpoker:append(0)
		for i = #tItem.m_userList[pos].outpoker, 3, -1 do
			tItem.m_userList[pos].outpoker[i] = tItem.m_userList[pos].outpoker[i - 2]
		end
		tItem.m_userList[pos].outpoker[2] = -1
		tItem.m_userList[pos].outpoker[1] = pokerID
	else
		for i = #tItem.m_userList[pos].outpoker, 2, -1 do
			tItem.m_userList[pos].outpoker[i] = tItem.m_userList[pos].outpoker[i - 1]
		end
		tItem.m_userList[pos].outpoker[1] = pokerID
	end
end

function GdmjWork.CheckFangFei(tItem)
	--在每次牌局第一次开始的时候，检查是否是均摊房费，然后扣除
	GdmjEvent.JulebuGameStart(tItem.m_tInfo)
	if tItem.m_tInfo.payway == 1 then
		--房主付费
		return nil 
	end
	local needGold = 0
	if tItem.m_tInfo.paytype == g_gdmjDefine.pay_gold then
		--先看看掌上币够不够扣除
		needGold = tItem.m_tInfo.paynum
	else
		needGold = tItem.m_tInfo.paynum*10
	end	
	for i = 1,tItem.m_maxUser do
		local pInfo = PlayerModel.GetPlayerInfo(tItem.m_userList[i].userid)
		
		if g_servername == "run_zsmj" or g_servername == "run_xwpk" then

			if tItem.m_userList[i].userid == tItem.m_tInfo.ownerid then
				if tItem.m_tInfo.paytype == g_gdmjDefine.pay_gold then
					if pInfo.gold >= needGold then
						PlayerModel.DecGold(pInfo, needGold, "gdmj", "AA cost gold")
					else
						PlayerModel.DecMoney(pInfo, needGold/10, "gdmj", "AA cost money") --掌上币不够的时候，钻石顶上
					end
				else
					if pInfo.money >= needGold/10 then
						PlayerModel.DecMoney(pInfo, needGold/10, "gdmj", "AA cost money") --掌上币不够的时候，钻石顶上
					else
						PlayerModel.DecGold(pInfo, needGold, "gdmj", "AA cost gold")
					end
				end
				
			else	
				if pInfo.gold >= needGold then
					--有玉先扣除玉
					PlayerModel.DecGold(pInfo, needGold, "gdmj", "AA cost gold")
				else
					PlayerModel.DecMoney(pInfo, needGold/10, "gdmj", "AA cost money") --掌上币不够的时候，钻石顶上
				end
			end
		else
			if tItem.m_tInfo.paytype == g_gdmjDefine.pay_gold then
				if pInfo.gold >= tItem.m_tInfo.paynum then
					PlayerModel.DecGold(pInfo, tItem.m_tInfo.paynum, "gdmj", "AA cost gold")
				else --就需要用钻石去抵扣了
					PlayerModel.DecMoney(pInfo, tItem.m_tInfo.paynum/10, "gdmj", "AA cost money") --掌上币不够的时候，钻石顶上
				end
			else
				PlayerModel.DecMoney(pInfo, tItem.m_tInfo.paynum, "gdmj", "AA cost money") --掌上币不够的时候，钻石顶上
			end			
		end
		
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo, {"money","gold"})
	end
	
end


function GdmjWork.CheckOver(tItem)
	--超过一定的时间，还没有开始或者没有行动，房间就会解散
	if tItem.m_tInfo.status == g_gdmjStatus.status_delete or tItem.m_tInfo.status == g_gdmjStatus.status_dissolve then
		return nil
	end
	
	if tItem.m_tInfo.timemark < -7200 then  --两个小时
		--一个小时如果还没有开始，就需要解散了
		if tItem.m_tInfo.usevipnum == 0 then
			--表示整个牌局还没有开始，需要退还钻石
			
			if tItem.m_tInfo.payway == 1 then
				--房主支付的
				local pInfo = PlayerModel.GetPlayerInfo(tItem.m_tInfo.tableuserid)
				if tItem.m_tInfo.paytype == g_gdmjDefine.pay_gold then
					PlayerModel.AddGold(pInfo, tItem.m_tInfo.paynum, "gdmj", "dissolve gold")
					PlayerModel.SendPlayerInfo(pInfo, {"gold"})
				else
					PlayerModel.AddMoney(pInfo, tItem.m_tInfo.paynum, "gdmj", "dissolve money")
					PlayerModel.SendPlayerInfo(pInfo, {"money"})
				end
				PlayerModel.SetPlayerInfo(pInfo)
			end
			local gcLeave = msg_gdmj_pb.gcgdmjleave()
			gcLeave.result = 0
			gcLeave.leavemsg = "房间超时未开始，已经解散!!!"
			gcLeave.tableid = tItem.m_tInfo.tableid
			for k,v in ipairs(tItem.m_tInfo.standuser) do
				gcLeave.userid = v
				SendMessage(v, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
			end
			for k,v in ipairs(tItem.m_tInfo.situser) do
				if v ~= 0 then
					gcLeave.userid = v
					gcLeave.chairid = k
					SendMessage(v, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				end
			end				
			tItem.m_tInfo.status = g_gdmjStatus.status_delete
		else
			tItem.m_tInfo.status = g_gdmjStatus.status_dissolve
		end
		tItem.m_isModify = true
	end	
end

function GdmjWork.CheckLeave(tItem)
	
	local standLen = #tItem.m_tInfo.standuser
	if standLen > 0 then
		local gcLeave = msg_gdmj_pb.gcgdmjleave()
		gcLeave.result = 0
		gcLeave.leavemsg = "牌局已经开始，无座玩家请离开牌桌"
		gcLeave.tableid = tItem.m_tInfo.tableid
		for i = standLen, 1,-1 do
			local isExist = false
			for k = 1,tItem.m_maxUser  do
				if tItem.m_tInfo.standuser[i] == tItem.m_userList[k].userid then
					isExist = true
				end
			end
			
			if isExist == false then
				--房主的另外发
				gcLeave.userid = tItem.m_tInfo.standuser[i]
				SendMessage(tItem.m_tInfo.standuser[i], PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				GdmjModel.DelUserTableID(tItem.m_tInfo.standuser[i]) 
			end
			tItem.m_tInfo.standuser:remove(1)
		end
	end	
end

--在这个函数中，对玩家的分数进行分配，然后对分数比较低的玩家进行配牌
function GdmjWork.AllocateCard(tItem,pos)

	--if true then
	--	return 0
	--end
	if pos == nil or pos == 0 then
		return 0
	end
	if type(pos) ~= 'number' then
		return 0
	end
	
	if  tItem.m_userList[pos] == nil then
		return 0
	end
	if  tItem.m_userList[pos].userid == nil then
		return 0
	end
	
	
	local isSpecial = SpecialModel.MajiangDianCard( tItem.m_userList[pos].userid )
	if SpecialModel == nil or 0 == isSpecial then
		--特殊的情况配牌
		return 0
	end

	if #tItem.m_userList[pos].tinglist > 0 then
		if isSpecial == g_logintable.majiang_win then
			
			local tingListTep = {}
			for k,v in ipairs( tItem.m_userList[pos].tinglist ) do
				table.insert(tingListTep, v)
			end
			for i = 1,50 do
				--这里洗牌100次
				local index1 = math.myrandom(1, #tingListTep)
				local index2 = math.myrandom(1, #tingListTep)
				local numTemp = tingListTep[index1]
				tingListTep[index1] = tingListTep[index2]
				tingListTep[index2] = numTemp
			end				
			
			for k1,v1 in ipairs(tItem.m_tInfo.publicpoker) do
				for k2,v2 in ipairs( tingListTep ) do
					if v1 == v2 then
						tItem.m_tInfo.publicpoker:remove(k1)
						return v2
					end
				end
			end			
			
		else
			return 0
		
		end
	end
	

	if #tItem.m_userList[pos].handpoker < 13 then
		return 0
	end
	
	
	
	local peiRate = 0  --发生配牌的概率，随分数越低，概率越大，低于-30,分，一定发生配牌
	
	peiRate  = 0 - tItem.m_userList[pos].carryjetton    ---- 50
	
	if peiRate > math.myrandom(0,100) then
		--不在配牌的范围内
		--
		return 0
	end
	


	
	--取得需要的牌的列表，总共分5个权重列表，100,80,60,40,20
	--100表示，5555的列表
	--80表示凑成555的列表
	--60表示凑成456的列表
	--40表示凑成44或是45的列表
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])
	
	local pei_100 = MajiangUtils.Get555CardSList(handList)
	local list456 = MajiangUtils.Get456CardSArrList(handList)
	local pei_80 = MajiangUtils.Get55CardSList(handList)
	local pei_60 = {}
	local listTong = {}  --筒
	local listTiao = {}  --条
	local listWan = {}   --万
	for k,v in ipairs(handList) do
		if v < 20 then
			table.insert(listWan, v)
		elseif v < 30 then
			table.insert(listTong,v)
		elseif v < 40 then
			table.insert(listTiao,v)
		end
	end

	table.insertto(pei_60,pei_80)
	
	if #pei_60 <= 1 then
		--如果只有一对。
		if #listWan >= #listTiao then
			
			if #listWan >= #listTong then
				table.insertto(pei_60,listWan)
			else
				table.insertto(pei_60,listTong)
			end
		else
			if #listTiao >= #listTong then
				table.insertto(pei_60,listTiao)
			else
				table.insertto(pei_60,listTong)
			end			
		end
		
	end

	
	while true do
		
		if #pei_60 <= 0 then
			break
		end
		
		local indexID = math.myrandom(1,#pei_60)
		
		for k,v in ipairs(tItem.m_tInfo.publicpoker) do
			if v == pei_60[indexID] then
				tItem.m_tInfo.publicpoker:remove(k)
				luaPrint("hu pei pai="..pei_60[indexID])
				return pei_60[indexID]
			end
		end
		table.remove(pei_60,indexID)
	end
	
	return 0
end



function GdmjWork.CheckJiFen(tItem)
	--这个的函数放在这里了。
	--检查每个玩家的积分是否足够
	
	local gcHistory = nil
	for i = 1,tItem.m_maxUser do
		if false == GdmjEvent.CheckJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid, tItem.m_userList[i].carryjetton) then
			--需要把人清出房间
			local gcStandUp = msg_gdmj_pb.gcgdmjstandup()
			gcStandUp.chairid = i
			gcStandUp.tableid = tItem.m_tInfo.tableid
			local userList = GdmjWork.GetUserList(tItem.m_tInfo, tItem.m_userList[i].userid)
			
			SendMessage(userList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())

			tItem.m_tInfo.situser[i] = 0    --这里设置为0
			tItem.m_tInfo.playernum = tItem.m_tInfo.playernum - 1
			GdmjEvent.JulebuGameUpdate(tItem.m_tInfo)

			local gcLeave = msg_gdmj_pb.gcgdmjleave()
			gcLeave.userid = tItem.m_userList[i].userid
			gcLeave.chairid = i
			gcLeave.tableid = tItem.m_tInfo.tableid
			gcLeave.leavemsg = "你积分不足，已被移出房间！"


			SendMessage(tItem.m_userList[i].userid, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())

			GdmjModel.DelUserTableID(tItem.m_userList[i].userid)   --这里还需要把
						
			--在这里需要把积分同步到俱乐部
			--
			--GdmjEvent.AddJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid, tItem.m_userList[i].carryjetton, tItem.m_tInfo.tableid,tItem.m_tInfo.julebutype)
			tItem.m_tInfo.situser[i] = 0
		end
	end
end