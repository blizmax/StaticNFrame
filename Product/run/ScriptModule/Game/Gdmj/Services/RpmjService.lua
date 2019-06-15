--饶平麻将

RpmjService = {}


function RpmjService.DoWork(tItem) --传入的是TdhStruct类型的Item
    if tItem.m_tInfo.status == g_gdmjStatus.status_ready then
        RpmjService.TableReady(tItem)
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_waiting then
			
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		
		if tItem.m_tInfo.timemark <= 0 then
			tItem.m_tInfo.status = g_gdmjStatus.status_playing
		end
		tItem.m_isModify = true
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_playing then

		RpmjService.TablePlay(tItem,v)
		
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_counting then
		tItem.m_isModify = true
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		if tItem.m_tInfo.timemark < 0 then
			RpmjService.PlayCount(tItem)
		end
		
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_end then
		tItem.m_isModify = true
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		if tItem.m_tInfo.timemark <= 0 then
			tItem.m_tInfo.status = g_gdmjStatus.status_ready  -- 开始打牌了
			
			tItem.m_tInfo.timemark = g_gdmjTime.autostart_time
			--这里需要检查钱不够的问题。
			--如果是在高场次不够，字自动调剂到低场次。
			--for k,v in ipairs(mjTable.mjplist) do
			--v.robottime = g_majiangVal.autostart_time  --如果是真实玩家，20秒还没开始，就自动踢出
			--end
			RpmjService.CheckJiFen(tItem)   --在这里检查积分
		end
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_dissolve then
		
		if tItem.m_tInfo.timemark > 0 then
			tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		end
		
		if tItem.m_tInfo.timemark <= 0 then
			--这里开始下发数据
			RpmjService.PlayCountDissolve(tItem)
		end
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_delete then
		--这里是的需要清空删除的
		table.insert(GdmjService.dismissList, tItem.m_tInfo.tableid)
		
		--
		--在这里是不能发离开协议的，不管是打牌结束还是中途申请解散的，都必须在各自的地方做发送离开的协议
		--
	end

	GdmjWork.CheckOver(tItem)
end

function RpmjService.CheckJiFen2(julebuID, userID, carryjetton, strVipInfo)
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end

	if julebuID == nil or julebuID == 0 then
		return true
	end
	
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if  member == nil or member == 0 then
		return false
	end
	
	if member.limitjifen + carryjetton < strVipInfo.limitjifen then
		return false
	end	
	return true
end

function RpmjService.CheckJiFenLiuju(julebuID, userID, carryjetton)
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end

	if julebuID == nil or julebuID == 0 then
		return true
	end
	
	local member = JulebuModel.GetUserMemberInfo(julebuID, userID)
	if  member == nil or member == 0 then
		return false
	end
	
	if member.limitjifen + carryjetton <= 0 then
		return false
	end	
	return true
end

function RpmjService.CheckJiFen(tItem)
	--这个的函数放在这里了。
	--检查每个玩家的积分是否足够
	local jInfo	 = JulebuModel.GetJulebuInfo(tItem.m_tInfo.julebuid)
	if jInfo == nil then
		return 
	end
	
	for i = 1,tItem.m_maxUser do
		if tItem.m_tInfo.julebutype == 2 and false == RpmjService.CheckJiFenLiuju(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid, tItem.m_userList[i].carryjetton) and tItem.m_tInfo.usevipnum ~= tItem.m_tInfo.maxvipnum then
			tItem.m_tInfo.timemark = g_gdmjTime.end_time
			tItem.m_tInfo.status = g_gdmjStatus.status_dissolve

			GdmjModel.SetTableInfo(tItem.m_tInfo, 1)
			return
		end
	end
end

function RpmjService.TableReady(tItem)
	local isReady = true

	if tItem.m_tInfo.playernum < tItem.m_maxUser then
		--小于三个人的情况下，看看需不需要加入机器人
		--tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		--if tItem.m_tInfo.timemark < 0 then
		--tItem.m_tInfo.timemark = g_gdmjTime.autostart_time
			--这里表示是的进入自动开始时间了
		--end
		isReady = false
	else   --如果是已经满四个人了
		for i = 1,#tItem.m_userList do
			--如果全部人都是准备的状态，说明可以开始了
			if tItem.m_userList[i].playstate == g_gdmjPlayerState.play_ready then
				isReady = false
				break
			end
		end
	end
	
	if isReady == true then
		--说明已经准备好了
		
		tItem.m_isModify = true
		
		for i = 1,tItem.m_maxUser do
			tItem.m_userModify[i] = 1
		end
		GdmjWork.CheckLeave(tItem)
		RpmjService.GameStart(tItem)

		if tItem.m_tInfo.usevipnum == 1 then
			if tItem.m_tInfo.julebuid ~= 0 then
				JulebuService.CreatTable(tItem.m_tInfo, g_JulebuDefine.modules_gdmj)
			end
		end
	else
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
	end
end

function RpmjService.InitPublicPoker(tItem)

	while #tItem.m_tInfo.winchairlist > 0 do
		tItem.m_tInfo.winchairlist:remove(1)
	end
	while #tItem.m_tInfo.guilist > 0 do
		tItem.m_tInfo.guilist:remove(1)
	end
	GdmjWork.NextInfoInit(tItem.m_nextInfo)  --这个也需要初始化
	GdmjWork.NextInfoInit(tItem.m_tInfo.nextinfo)
	
	local pokerList = {}
	
	if (tItem.m_maxUser == 3) or (tItem.m_maxUser == 2) then
		--二人，三人情况下108张牌
		for i = 1,4 do
			for j = 1,3 do
				for k,v in ipairs(g_gdmjPokerList[j]) do
					table.insert(pokerList, v)
				end
			end
		end	
    else
        --四人麻将东南西北中发白
		for i = 1,4 do
			for j = 1,5 do
				for k,v in ipairs(g_gdmjPokerList[j]) do
					table.insert(pokerList, v)
				end
			end
		end
	end

	for j = 1, 100 do
		for i = 1,#pokerList do
			--这里洗牌100次
			local index1 = math.myrandom(1, #pokerList)
			local numTemp = pokerList[index1]
			pokerList[index1] = pokerList[i]
			pokerList[i] = numTemp
		end
	end

	local len = #tItem.m_tInfo.publicpoker
	local mark = 1
	while #pokerList > 0 do
		local randNum = math.myrandom(1,#pokerList)
		if mark > len then
			tItem.m_tInfo.publicpoker:append(pokerList[randNum])
		else
			tItem.m_tInfo.publicpoker[mark] = pokerList[randNum]
			mark = mark + 1
		end
		table.remove(pokerList,randNum)
	end
	tItem.m_tInfo.genzhuang = 0
end

function RpmjService.GameStart(tItem)

	--初始化公共牌
	RpmjService.InitPublicPoker(tItem)
	GdmjWork.VipPay(tItem.m_tInfo)
	
	--GdmjWork.SendStartInfo(tItem)
	for i = 1,tItem.m_maxUser do
		GdmjWork.UserStartInit(tItem, tItem.m_userList[i])   --初始化
	end
	
	if g_isDebug == 1 then
		--RpmjService.StartTest(tItem)
		for i = 1,13 do
			for k =1,tItem.m_maxUser do
				GdmjWork.HandPokerInsert(tItem, k)
			end
		end
	else
		
		for i = 1,13 do
			for k =1,tItem.m_maxUser do
				GdmjWork.HandPokerInsert(tItem, k)
			end
		end
		
	end
	
	for i = 1,tItem.m_maxUser do  --检查胡牌列表
		GdmjWork.CheckHuList(tItem, i)
	end
	
	
	tItem.m_tInfo.status = g_gdmjStatus.status_waiting     --设置状态
	tItem.m_tInfo.timemark = g_gdmjTime.waiting_time       --设置前端发牌的时间
	tItem.m_tInfo.wintype = 0
	tItem.m_tInfo.bankercount = 0
	--这里在刚开始的时候可能需要随机给一个庄家
	
	if tItem.m_tInfo.bankerpos == 0 then
		tItem.m_tInfo.bankerpos = math.myrandom(1,tItem.m_maxUser)
	end
	
	--tItem.m_tInfo.nextinfo.actchairid[tItem.m_tInfo.bankerpos] = 1 --把这个设置为1
	tItem.m_tInfo.beingpoker = 1
	
	while #tItem.m_tInfo.winchairlist > 0 do
		tItem.m_tInfo.winchairlist:remove(1)
	end
	
	
	tItem.m_tInfo.userstate = g_gdmjUserState.state_waiting  --把牌桌的玩家状态设置为waiting
	--while #tItem.m_tInfo.nextinfo.actiontype > 0 do
	--	tItem.m_tInfo.nextinfo.actiontype:remove(1)
	--end
	
	--tItem.m_tInfo.nextinfo.actchairid = 
	
	local count = GdmjHistoryModel.GetDayCount()  --一定要弄清楚这里为什么要这么做，是为了取得一个唯一的ID啊

	if tItem.m_tInfo.viptable ~=0 then
		tItem.m_tInfo.usevipnum = tItem.m_tInfo.usevipnum + 1
		if tItem.m_tInfo.usevipnum == 1 then
			tItem.m_tInfo.viprecord.score:append(0)
			tItem.m_tInfo.viprecord.score:append(0)
			tItem.m_tInfo.viprecord.score:append(0)
			tItem.m_tInfo.viprecord.score:append(0)
			GdmjWork.CheckFangFei(tItem)
		end
		
		if tItem.m_tInfo.frameid == 0 then    --在私人房中，这个是用于保存历史记录的
			local tm = TimeUtils.GetTableTime()
			tItem.m_tInfo.frameid = tm.day*10000000 + count*100 + 1  --这里就是ID的妙用了
		else		
			tItem.m_tInfo.frameid = tItem.m_tInfo.frameid + 1  --取得了唯一的ID
		end
	end
	GdmjWork.SendStartInfo(tItem)
end

function RpmjService.TablePlay(tItem,currPos)
	
	if tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		tItem.m_isModify = true
		RpmjService.CheckMoving(tItem)
	else
		local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
		for k,currPos in ipairs(currPosList) do
			if tItem.m_userList[currPos].isrobot == 1 then
				--如果是机器人
				
				tItem.m_userList[currPos].robottime = tItem.m_userList[currPos].robottime - 1
				if tItem.m_userList[currPos].robottime < 0 then
					RpmjService.AutoAction(tItem)
				end
			else
				if tItem.m_userList[currPos].autoplayer == 1 then
					RpmjService.AutoAction(tItem)
				else
					if tItem.m_tInfo.actiontime > 0 then
						tItem.m_tInfo.actiontime = tItem.m_tInfo.actiontime - 1
					end
				end
			end
		end
	end
end

function RpmjService.CheckIsGang(mjUser, pokerID)
	
	if pokerID > 60 then
		return 0
	end
	
	--这个是在发牌的时候
	for k,v in ipairs(mjUser.mjpokerlist) do
		--先检查碰中是否有杠
		if v.pokertype == g_gdmjAction.type_peng then
			if v.pokerlist[1] == pokerID then
				--这个是在发牌的时候

				local exist = false
				for k1, v1 in ipairs(mjUser.guogang) do
					--先检查碰中是否有杠
					if v1 == pokerID then
						exist = true
					end
				end
				if exist == false then
					return pokerID
				end
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
					--检查过杠
					local exist = false
					for k1, v1 in ipairs(mjUser.guogang) do
						if v1 == v then
							exist = true
						end
					end
					if exist == false then
						return v
					end
				end
			end
		end
	end
	if count == 4 then
		--检查过杠
		local exist = false
		for k1, v1 in ipairs(mjUser.guogang) do
			if v1 == pokerID then
				exist = true
			end
		end
		if exist == false then
			return pokerID
		end
	end
	--在检查剩下的
	for k,v in pairs(gangList) do
		if v == 4 then
			--检查过杠
			local exist = false
			for k1, v1 in ipairs(mjUser.guogang) do
				if v1 == k then
					exist = true
				end
			end
			if exist == false then
				return k
			end
		end
	end
	--这里还需要见见其他四个的问题
	return 0
end

function RpmjService.CheckMoving(tItem)
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
	
	if #currPosList == 0 then
		LogFile("error", "RpmjService.CheckMoving actchairid is nil")
		return nil
	end
	local currPos = currPosList[1] --取第一个就行了。
	local gcUpdate = msg_gdmj_pb.gcgdmjupdate()
	
	if tItem.m_tInfo.beingpoker == 1 then
		--这个是需要发牌的
		--local pokerID = GdmjWork.GetRandPoker(tItem.m_tInfo)
		local pokerID = GdmjWork.HandPokerInsert(tItem, currPos)
		
		local checkHu = GdmjWork.CheckIsHu(tItem, currPos, pokerID) --为什么是humark呢
		local checkGang = RpmjService.CheckIsGang(tItem.m_userList[currPos], pokerID)
		tItem.m_tInfo.nextinfo.actpokerid = pokerID
		tItem.m_tInfo.nextinfo.actchairid[currPos] = 1
		tItem.m_tInfo.nextinfo.tarchairid = 0   --设置
		tItem.m_userList[currPos].humark = pokerID    --设置成这里
		
		if checkHu > 0 or checkGang > 0 then
			--已经可以胡了
			--
			
			if checkHu > 0 then
				if tItem.m_userList[currPos].guohu == 0 then
					if tItem.m_vipRoomInfo.bishu1 ~= 0 or tItem.m_vipRoomInfo.bishu2 ~= 0 then
						tItem.m_userList[currPos].zimoguohu = 1
						tItem.m_userList[currPos].guohu = 1
					end

					tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_guo)
					tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_hu)
				end
			end
			
			if checkGang > 0 then
				if #tItem.m_tInfo.publicpoker > tItem.m_vipRoomInfo.manum + 4 then
					tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_guo)
					tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_gang)
				else
					checkGang = 0
				end
			end
		else
			tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_play)
		end
		
		for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
			gcUpdate.actiontype:append(v)
		end
		tItem.m_tInfo.nextinfo.canplay = 1
		gcUpdate.pokerid = pokerID
		gcUpdate.actpokerid = checkGang == 0 and pokerID or checkGang --pokerID
	else
		--如果是不需要发牌的情况下，表示是碰了之后出牌
		if #tItem.m_tInfo.nextinfo.actiontype > 0 then
			for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
				gcUpdate.actiontype:append(v)
			end
			gcUpdate.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
		else
			gcUpdate.actiontype:append(g_gdmjAction.type_play)
			gcUpdate.pokerid = 0
			gcUpdate.actpokerid = 0
		end
		
	end
	
	for i = 1,tItem.m_maxUser do
		gcUpdate.actchairid:append(0)
	end
	
	for k,v in ipairs(currPosList) do  --这里是为了一炮多响的，记住，一炮多响，抢杠胡的时候
		tItem.m_userList[v].playstate = g_gdmjPlayerState.play_action  --设置对应的状态
		gcUpdate.actchairid[v] = 1
	end
	
	gcUpdate.lastpokerlen = #tItem.m_tInfo.publicpoker
	gcUpdate.actiontime = 5  --先定为5吧
	gcUpdate.canplay = tItem.m_tInfo.nextinfo.canplay
	gcUpdate.result = 0
	gcUpdate.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
	tItem.m_tInfo.userstate = g_gdmjUserState.state_playing  --轮到玩家开始打牌了
	
	GdmjWork.SendUpdate(tItem,gcUpdate, currPos)
	
	
	tItem.m_isModify = true
	for k,v in ipairs(currPosList) do
		tItem.m_userModify[v] = 1
	end
	
	
end


function RpmjService.PlayingAction(tItem,cgmsg, gcmsg)
	
	local isWin = false
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)

	GdmjWork.NextInfoInit(tItem.m_nextInfo)
	tItem.m_tInfo.beingpoker = 0   --置为不能发牌
	if cgmsg.actiontype == g_gdmjAction.type_play then
		RpmjService.DoPlay(tItem,cgmsg,gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
	elseif cgmsg.actiontype == g_gdmjAction.type_guo then
		isWin = RpmjService.DoGuo(tItem, cgmsg, gcmsg)  --这里过的时候，必须要在这里返回
	elseif cgmsg.actiontype == g_gdmjAction.type_chi then
		
	elseif cgmsg.actiontype == g_gdmjAction.type_peng then
		RpmjService.DoPeng(tItem, cgmsg, gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
	elseif cgmsg.actiontype == g_gdmjAction.type_gang then
		RpmjService.DoGang(tItem, cgmsg, gcmsg)
	elseif cgmsg.actiontype == g_gdmjAction.type_hu then
		isWin = true
		tItem.m_tInfo.wintype = g_gdmjAction.type_hu
		tItem.m_tInfo.winchairlist:append(cgmsg.actchairid)
	elseif cgmsg.actiontype == g_gdmjAction.type_qiangganghu then
	
		if #currPosList == 1 then
			isWin = true			
		end
		tItem.m_tInfo.nextinfo.tarchairid = cgmsg.tarchairid     --modyfy 1121  landy 抢杠胡算谁输的问题
		tItem.m_tInfo.wintype = g_gdmjAction.type_qiangganghu
		tItem.m_tInfo.winchairlist:append(cgmsg.actchairid)
		GdmjWork.HandPokerInsert(tItem, cgmsg.actchairid, cgmsg.actpokerid)
	elseif cgmsg.actiontype == g_gdmjAction.type_chihu then
		if #currPosList == 1 then
			isWin = true
		end
		tItem.m_tInfo.wintype = g_gdmjAction.type_chihu
		tItem.m_tInfo.winchairlist:append(cgmsg.actchairid)
		GdmjWork.HandPokerInsert(tItem, cgmsg.actchairid, cgmsg.actpokerid)	
	end
	
	
	--在这里把action的内容下发
	
	local addUser = gcmsg.actplayer:add()
	addUser:ParseFromString(tItem.m_userList[cgmsg.actchairid]:SerializeToString())
	--目标的ID不放在这里
	
	gcmsg.actpokerid:append(cgmsg.actpokerid)
	gcmsg.actiontype:append(cgmsg.actiontype)
	
	if cgmsg.tarchairid ~= 0 then
		--如果目标ID不是0，表示需要更新目标ID
		--同时把目标ID也加入
		local addTar = gcmsg.tarplayer:add()
		addTar:ParseFromString(tItem.m_userList[cgmsg.tarchairid]:SerializeToString())
		
	end

	tItem.m_isModify = true
	tItem.m_userModify[cgmsg.actchairid] = 1
	if cgmsg.tarchairid ~= 0 then
		tItem.m_userModify[cgmsg.tarchairid] = 1
	end
	
	local isEnd = false
	
	if isWin == true then
		RpmjService.DoWin(tItem, cgmsg.actchairid, cgmsg.actchairid)
		isEnd = true
	else
		tItem.m_userList[cgmsg.actchairid].playstate = g_gdmjPlayerState.play_waiting
		if tItem.m_tInfo.beingpoker == 1 then
			--只有发牌的情况下会去检查流局
			isEnd = RpmjService.CheckLiuJu(tItem)
		end
	end

	if isEnd == true then
		--把所有玩家的数据统一发送
		for i = 1,tItem.m_maxUser do
			local isExist = false
			
			for k,v in ipairs(gcmsg.actplayer) do
				if tItem.m_userList[i].userid == v.userid then
					isExist = true
				end
			end
			for k,v in ipairs(gcmsg.tarplayer) do
				if tItem.m_userList[i].userid == v.userid then
					isExist = true
				end				
			end
			if isExist == false then
				
				local addUser = gcmsg.tarplayer:add()
				addUser:ParseFromString(tItem.m_userList[i]:SerializeToString())
			end
		end
		
	end


	if #currPosList == 1 then
		--表示是最后一个行动这，这里下发行动的结果
		--这里表示需要发送给全部人了
		GdmjWork.SendAction(tItem, gcmsg, isEnd)
		tItem.m_tInfo.userstate = g_gdmjUserState.state_waiting
		if isEnd ~= true then 
			--如果是牌局结束了，这里不能更新了
			tItem.m_tInfo.nextinfo:ParseFromString(tItem.m_nextInfo:SerializeToString())  --初始化下一个行动的目标
		end
	else
		--先保存下来
		tItem.m_tInfo.nextinfo.actchairid[cgmsg.actchairid] = 0 --已经有人操作了
		tItem.m_tInfo.straction = gcmsg:SerializeToString()
		
	end
	
end

function RpmjService.CheckNewPengGang(tItem, pokerID, actChairid)
	--该函数是检查新型的碰和杠的，碰的时候需要检查是否存在过碰不碰的情况
	--返回的时候，是返回财政的列表
	if #tItem.m_tInfo.publicpoker <= tItem.m_vipRoomInfo.manum then
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
			local isExistGang = false

			for k2,v2 in ipairs(tItem.m_userList[v1].guopeng) do
				if v2 == pokerID then
					isExist = true
					break
				end
			end

			for k2,v2 in ipairs(tItem.m_userList[v1].guogang) do
				if v2 == pokerID then
					isExistGang = true
					break
				end
			end
			
			if isExist == false then
				table.insert(retList, g_gdmjAction.type_peng)
				if num > 2 and isExistGang == false then
					if #tItem.m_tInfo.publicpoker > tItem.m_vipRoomInfo.manum + 4 then
						table.insert(retList, g_gdmjAction.type_gang)  --剩下最后一张牌了，不能碰和杠，因为碰和杠后就不能够出牌了
					end
				end
			elseif num == 2 then
				table.insert(retList, g_gdmjAction.type_peng)
			end				
			return #retList == 0 and 0 or v1,retList
		end
	end
	return 0,{}
end

function RpmjService.NewCheckChiHu(tItem,actPokerID,actChairID,isQiangGang)

	while #tItem.m_tInfo.chihulist > 0 do
		tItem.m_tInfo.chihulist:remove(1)
	end

	local userIDList = GdmjWork.GetCheckList(actChairID, tItem.m_maxUser) --默认是四个玩家
	--先把需要检查的玩家找出来
	
	local huChairList = {}
	
	for i = 1,#userIDList do
		--在这里必须要判断鸡胡，鸡胡是不允许吃胡的
		for k,v in ipairs(tItem.m_userList[ userIDList[i] ].tinglist) do
			if tItem.m_userList[ userIDList[i] ].guohu == 0 and tItem.m_userList[ userIDList[i] ].jinhu == 0 and v == actPokerID then
				--同时必须要满足不是鸡胡才可以,过胡不胡
				--抢杠胡是可以鸡胡的
				--这里再加一个，惠东庄也是允许吃鸡胡的,但是惠州庄，是不允许吃鸡胡的,所以最后一个是判断惠州庄吃胡的
				

				if isQiangGang == true then
					--tItem.m_userList[userIDList[i]].guohu = 1   --过胡不胡
					--return userIDList[i]
					if tItem.m_userList[ userIDList[i] ].tingtype[k] == g_gdmjWinType.type_hzz_shisanyao then
						--这里是的为了优先给十三幺胡牌
						local isInsert = #huChairList + 1
						for j = 1,#huChairList do
							if huChairList[j][1] ~= g_gdmjWinType.type_hzz_shisanyao then
								isInsert = j
								break
							end
						end
						local addItem = {}
						addItem[1] = tItem.m_userList[ userIDList[i] ].tingtype[k]
						addItem[2] = userIDList[i]
						table.insert(huChairList,isInsert, addItem)
					else
						local addItem = {}
						addItem[1] = tItem.m_userList[ userIDList[i] ].tingtype[k]
						addItem[2] = userIDList[i]
						table.insert(huChairList,addItem)
					end
				end
			end
		end
	end
	
	if #huChairList > 0 then
		for k,v in ipairs(huChairList) do
			tItem.m_tInfo.chihulist:append(v[2])			
		end

		if tItem.m_vipRoomInfo.bishu1 ~= 0 or tItem.m_vipRoomInfo.bishu2 ~= 0 then
			tItem.m_userList[huChairList[1][2]].guohu = 1
		end
		return huChairList[1][2]
	end
	
	return 0
	
end

function RpmjService.NewCheckChiHu2(tItem)
	if #tItem.m_tInfo.chihulist > 0 then
		local chairID = tItem.m_tInfo.chihulist[1]
		if tItem.m_vipRoomInfo.bishu1 ~= 0 or tItem.m_vipRoomInfo.bishu2 ~= 0 then
			tItem.m_userList[chairID].guohu = chairID
		end
		return chairID
	end
	return 0
end

function RpmjService.GetChiHuList(tItem,actPokerID,actChairID)
	--获取胡牌的列表，这个是发生在过吃胡之后，用来检查是否需要检查碰的情况的。
	--如果是单一个胡的，则检查这个胡是不是已经检查过了
	local userIDList = GdmjWork.GetCheckList(actChairID, tItem.m_maxUser) --默认是四个玩家
	
	
	local huChairList = {}
	
	for i = 1,#userIDList do
		--在这里必须要判断鸡胡，鸡胡是不允许吃胡的
		for k,v in ipairs(tItem.m_userList[ userIDList[i] ].tinglist) do
			if v == actPokerID and tItem.m_userList[ userIDList[i] ].tingtype[k] ~= g_gdmjWinType.type_hzz_jihu then
				table.insert(huChairList,userIDList[i])
			end
		end
	end	
	return huChairList
end


function RpmjService.DoPlay(tItem, cgmsg, gcmsg)
	--这个是打牌的操作
	--在打牌的操作中，只有一个人的行动，不会存在多个人的
	--首先，在手牌中去掉
	
	tItem.m_tInfo.prevpos = 0  --首先，把这个设置为0，为什么要把这个设置为0呢，这个是杠爆全包的,只要是出牌了，这个就不算了
	
	if tItem.m_userList[cgmsg.actchairid].zimoguohu == 0 then
		tItem.m_userList[cgmsg.actchairid].guohu = 0   --出牌，过胡的时候
	else
		local exist = false
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].tinglist) do
			if v == cgmsg.actpokerid then
				exist = true
			end
		end
		
		if exist == false then
			tItem.m_userList[cgmsg.actchairid].zimoguohu = 0
			tItem.m_userList[cgmsg.actchairid].guohu = 0   --出牌，过胡的时候
		end
	end
	
	while #tItem.m_userList[cgmsg.actchairid].guopeng > 0 do   --过碰过碰
		tItem.m_userList[cgmsg.actchairid].guopeng:remove(1)
	end
	
	GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	GdmjWork.AddPokerToOutList(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	
	local chiHu = RpmjService.NewCheckChiHu(tItem,cgmsg.actpokerid, cgmsg.actchairid, true)  --这里检查吃胡的时候，在自己本地检查	

	if chiHu > 0 then
		--表示可以吃胡
		--for k,v in ipairs(ChiHuList) do
			
		tItem.m_nextInfo.actchairid[chiHu] = 1
		--end
		tItem.m_nextInfo.tarchairid = cgmsg.actchairid
		tItem.m_tInfo.beingpoker = 0
		tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
		
		
		if #tItem.m_tInfo.chihulist == 1 then
			--这里加上过，碰，杠
			local nextPos,actList = RpmjService.CheckNewPengGang(tItem,cgmsg.actpokerid, cgmsg.actchairid)
			if nextPos == chiHu then
				for k1,v1 in ipairs(actList) do
					tItem.m_nextInfo.actiontype:append(v1)
					if v1 == g_gdmjAction.type_gang then
						tItem.m_tInfo.prevpos = cgmsg.actchairid
					end				
				end
			end
		end
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chihu)
	else
		local nextPos,actList = GdmjWork.CheckNewPengGang(tItem,cgmsg.actpokerid, cgmsg.actchairid)

		if nextPos ~= 0 then
			--表示是可以碰和或者杠的
			tItem.m_nextInfo.actchairid[nextPos] = 1
			tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
			tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
			tItem.m_nextInfo.tarchairid = cgmsg.actchairid   --设置目标ID是当前目标
			for k1,v1 in ipairs(actList) do
				tItem.m_nextInfo.actiontype:append(v1)
				if v1 == g_gdmjAction.type_gang then
					tItem.m_tInfo.prevpos = cgmsg.actchairid
				end				
			end
			tItem.m_userList[nextPos].guopeng:append(cgmsg.actpokerid)   --在这里就需要把过碰的加进去了
			
		else
			--如果没有碰或者杠，表示轮到下一个人了
			
			tItem.m_tInfo.beingpoker = 1
			nextPos = cgmsg.actchairid == tItem.m_maxUser and 1 or cgmsg.actchairid + 1
			tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
		end
		
	end
end

function RpmjService.DoGuo(tItem,cgmsg,gcmsg)
	--这里是过牌的操作，过牌的操作，可能存在几个人同时操作，必须等到最后的人操作完成了，才发送行动的类型
	--如果是抢杠胡，而且是同时抢杠胡，或者同时吃胡
	--
	--tItem.m_nextInfo:ParseFromString(tItem.m_tInfo.nextinfo:SerializeToString())
	tItem.m_tInfo.prevpos = 0   --在这里也要对这个设置
	if tItem.m_tInfo.nextinfo.tarchairid == 0 then
		--如果目标ID是0，表示是自摸的杠牌的过，或者是胡牌的过，这里的牌权依然在自己手上，只是
		--是出牌，现在这种情况已经忽略了。
		tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
		tItem.m_nextInfo.canplay = 1

		for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
			if v == g_gdmjAction.type_gang then
				tItem.m_userList[cgmsg.actchairid].guogang:append(cgmsg.actpokerid)   --在这里就需要把过碰的加进去了
			end
		end
	else

		local prevType = 0   --上一个玩家的出牌类型
		
		for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
			if v == g_gdmjAction.type_qiangganghu then
				--广东麻将只有抢杠胡才会出现胡
				prevType = v    
			elseif v == g_gdmjAction.type_chihu then
				prevType = v
			elseif v == g_gdmjAction.type_peng then
				prevType = v
			elseif v == g_gdmjAction.type_gang then
				prevType = v
				tItem.m_userList[cgmsg.actchairid].guogang:append(cgmsg.actpokerid)   --在这里就需要把过碰的加进去了
			end
		end
		
		if prevType == g_gdmjAction.type_qiangganghu then
			--抢杠胡是不用检查吃的
			--如果是吃胡的过，检查有没有下一个吃胡
			tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 0  --首先需要把自己的置为空
			--过了抢杠胡了
			if tItem.m_vipRoomInfo.bishu1 ~= 0 then
				tItem.m_userList[chairID].jinhu = 1
			end
			tItem.m_tInfo.chihulist:remove(1)
			local chiHu = RpmjService.NewCheckChiHu2(tItem)
			
			if chiHu > 0 then
				tItem.m_nextInfo.actchairid[chiHu] = 1
				tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
				tItem.m_tInfo.beingpoker = 0
				tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_qiangganghu)
			else
				
				local cgGang = msg_gdmj_pb.cggdmjaction()
				cgGang.actchairid = cgmsg.tarchairid                  --tItem.m_tInfo.nextinfo.actchairid
				cgGang.tarchairid = tItem.m_tInfo.prevpos             --这个是记录在前面放杠的位置
				cgGang.actpokerid = cgmsg.actpokerid
				RpmjService.ActionGang(tItem, cgGang, gcmsg)
				tItem.m_tInfo.beingpoker = 1   --杠完后需要发牌的
				tItem.m_nextInfo.actchairid[cgGang.actchairid] = cgGang.actchairid
				local addUser = gcmsg.actplayer:add()
				addUser:ParseFromString(tItem.m_userList[cgGang.actchairid]:SerializeToString())
				
				gcmsg.actpokerid:append(cgmsg.actpokerid)
				gcmsg.actiontype:append(g_gdmjAction.type_gang)
			end				
		elseif prevType == g_gdmjAction.type_chihu then
			--如果是吃胡的过，检查有没有下一个吃胡
			--吃胡是的需要检查吃的
			tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 0  --首先需要把自己的置为空
			if tItem.m_vipRoomInfo.bishu1 ~= 0 then
				tItem.m_userList[cgmsg.actchairid].jinhu = 1
			end
			tItem.m_tInfo.chihulist:remove(1)
			local chiHu = RpmjService.NewCheckChiHu2(tItem)
			
			if chiHu > 0 then
				tItem.m_nextInfo.actchairid[chiHu] = 1
				tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
				tItem.m_tInfo.beingpoker = 0
				tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chihu)
			else
				
				
				local nextPos,actList = RpmjService.CheckNewPengGang(tItem,tItem.m_tInfo.nextinfo.actpokerid, tItem.m_tInfo.nextinfo.tarchairid)

				if nextPos ~= 0 then
					local tempList = RpmjService.GetChiHuList(tItem, cgmsg.actpokerid, cgmsg.tarchairid)
					if #tempList == 1 and tempList[1] == nextPos then
						--只有一个吃胡列表的时候，已经被检查过了
						nextPos = 0
					end
				end
				
				if nextPos ~= 0 then
					--表示是可以碰和或者杠的
					tItem.m_nextInfo.actchairid[nextPos] = 1
					tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
					tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
					tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)

					for k1,v1 in ipairs(actList) do
						tItem.m_nextInfo.actiontype:append(v1)
						if v1 == g_gdmjAction.type_gang then
							tItem.m_tInfo.prevpos = tItem.m_tInfo.nextinfo.tarchairid
						end
					end
					
					tItem.m_userList[nextPos].guopeng:append(tItem.m_tInfo.nextinfo.actpokerid)   --在这里就需要把过碰的加进去了					
				else
					--如果没有碰或者杠，表示轮到下一个人了					

					tItem.m_tInfo.beingpoker = 1
					nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
					tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
					tItem.m_nextInfo.canplay = 1
					
				end
				--GdmjWork.GenZhuangCheck(tItem,cgmsg)   --这里不检查跟庄了，如果TMD有这种情况出现，我就认命了			
			end
		elseif prevType == g_gdmjAction.type_peng or prevType == g_gdmjAction.type_gang then					
				tItem.m_tInfo.beingpoker = 1
				nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
				tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
				tItem.m_nextInfo.canplay = 1
		else
			local nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
			tItem.m_nextInfo.actchairid[nextPos] = 1
			tItem.m_tInfo.beingpoker = 1
			tItem.m_nextInfo.canplay = 1
		end
	end
	
	return false
end


function RpmjService.DoPeng(tItem, cgmsg, gcmsg)
	
	for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
		if v == g_gdmjAction.type_gang then
			tItem.m_userList[cgmsg.actchairid].guogang:append(cgmsg.actpokerid)   --在这里就需要把过碰的加进去了
		end
	end

	GdmjWork.ActionPeng(tItem,cgmsg, gcmsg)
	--碰了后轮到自己出牌了 ，但是不用发牌的
	
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
end

function RpmjService.DoGang(tItem, cgmsg, gcmsg)
	
	--杠牌，要检查抢杠胡
	--先检查抢杠杠胡
	local qgHuChair = 0
	
	
	if cgmsg.tarchairid == 0 then
		
		local isBuGang = false
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
			if v.pokerlist[1] == cgmsg.actpokerid and v.pokertype == g_gdmjAction.type_peng then
				qgHuChair = RpmjService.NewCheckChiHu(tItem, cgmsg.actpokerid, cgmsg.actchairid, true)
				break
			end
		end
	else
		--明杠可以抢，而且还是翻倍
		qgHuChair = RpmjService.NewCheckChiHu(tItem, cgmsg.actpokerid, cgmsg.actchairid, true)
	end

	if qgHuChair > 0 then
		--如果抢杠胡的列表大于0
		--
		--开始吹抢杠胡的流程
		--先处理一个的
		tItem.m_nextInfo.actchairid[qgHuChair] = 1
		tItem.m_nextInfo.tarchairid = cgmsg.actchairid
		tItem.m_tInfo.beingpoker = 0
		tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_qiangganghu)

	else
		--如果没有抢杠胡，表示开始杠牌了
	
		RpmjService.ActionGang2(tItem,cgmsg,gcmsg)
		tItem.m_tInfo.beingpoker = 1   --杠完后需要发牌的
		tItem.m_nextInfo.actchairid[cgmsg.actchairid] = cgmsg.actchairid
		tItem.m_nextInfo.canplay = 1

		tItem.m_tInfo.prevpos = cgmsg.actpokerid       --这里是一个坑，这个的身份比较复杂，后面看看会不会出现坑的地方。先用这个来表示杠爆的
	end
end

function RpmjService.ActionGang(tItem, cgmsg, gcmsg)
	
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
		
		--tItem.m_userList[cgmsg.actchairid].carryjetton = tItem.m_userList[cgmsg.actchairid].carryjetton + changeJetton
		--gcmsg.changejetton[cgmsg.actchairid] = changeJetton
		
		--如果被杠玩家的筹码不足了，就补到0,这里要区分对待贵宾房和普通房
		
		if tItem.m_tInfo.viptable ~= 0 then
			--如果是贵宾房 --
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
				--tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton + (tItem.m_maxUser - 1)*changeJetton
				
				--gcmsg.changejetton[i] =  (tItem.m_maxUser - 1)*changeJetton
			else				
				--tItem.m_userList[i].carryjetton = tItem.m_userList[i].carryjetton - changeJetton
				
				if tItem.m_tInfo.viptable == 0 and tItem.m_userList[i].carryjetton == 0 then
					--如果牌桌不是私人房，并且玩家的筹码是0的情况下
					tItem.m_userList[i].carryjetton = 0
				end
				--gcmsg.changejetton[i] = 0 - changeJetton
				
				local addPlayer = gcmsg.tarplayer:add()   --在这里就把需要更新的人给加进去了，其他的就不用操作了
				addPlayer:ParseFromString(tItem.m_userList[i]:SerializeToString())
				
			end
			tItem.m_userModify[i] = 1   --在这里，需要把数据保存到缓存中
		end
		
	end
	
end

function RpmjService.ActionGang2(tItem, cgmsg, gcmsg)
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

function RpmjService.DoHu(tItem, cgmsg, gcmsg)
	--胡牌
end

function RpmjService.DoQiangGangHu(tItem, cgmsg, gcmsg)
	
end

function RpmjService.DoChiHu(tItem, cgmsg, gcmsg)
	
end

function RpmjService.DoGenZhuang(tItem, cgmsg, gcmsg)
	
end

function RpmjService.DoWin(tItem, pos, pokerID)
	--已经有人赢了
	tItem.m_tInfo.status = g_gdmjStatus.status_counting   --进入结算的时间
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
end

--饶平麻将， 码数不能打
function RpmjService.CheckLiuJu(tItem)
	if #tItem.m_tInfo.publicpoker > tItem.m_vipRoomInfo.manum then
		return nil
	end
	
	tItem.m_tInfo.status = g_gdmjStatus.status_counting
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
	tItem.m_tInfo.bankerpos = tItem.m_tInfo.bankerpos >= tItem.m_maxUser and 1 or tItem.m_tInfo.bankerpos + 1
	tItem.m_tInfo.bankercount = 0
	return true
end

function RpmjService.PlayCount(tItem)

	local len = #tItem.m_tInfo.winchairlist
	
	if #tItem.m_tInfo.winchairlist == 0 then
		RpmjService.PlayCountLiuJu(tItem)
	elseif #tItem.m_tInfo.winchairlist == 1 then
		RpmjService.PlayCountWin(tItem)
	else
		RpmjService.PlayCountWinMore(tItem)
	end
	
	for i = 1,tItem.m_maxUser do      --设置进入等待的状态
		tItem.m_userList[i].playstate = g_gdmjPlayerState.play_ready
		tItem.m_userModify[i] = 1     --在这里也需要设置
	end

	
	if tItem.m_tInfo.viptable == 2 then
		--如果是0，表示本次牌局已经结束了
		tItem.m_tInfo.timemark = 1
		tItem.m_tInfo.status = g_gdmjStatus.status_delete	
	else  
		tItem.m_tInfo.timemark = g_gdmjTime.end_time
		tItem.m_tInfo.status = g_gdmjStatus.status_end			
	end
	
	
end


function RpmjService.PlayCountLiuJu(tItem)
	
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	--玩家手牌
	for i = 1,tItem.m_maxUser do

		local item = gcAccount.countlist:add()
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = 0
		item.iswin = 0
		item.winjetton = 0
		recoreItem.score:append(0) --这里是安装顺序去的
		gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
		
	end

	for i = 1,tItem.m_maxUser do
		
		local addPoker = gcAccount.hpokerlist:add()
		
		for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
			
			local addItem = addPoker.mjpokerlist:add()
			addItem:ParseFromString(v:SerializeToString())
			
		end
		
		for k,v in ipairs(tItem.m_userList[i].handpoker) do
			addPoker.handpoker:append(v)
		end
		addPoker.userid = tItem.m_userList[i].userid
		tItem.m_userModify[i] = 1
	end	



	if tItem.m_tInfo.viptable ~= 0 and tItem.m_tInfo.usevipnum == tItem.m_tInfo.maxvipnum then
		--打完八局或者16局了
		tItem.m_tInfo.viptable = 2
		gcAccount.counttype = 1
		
		for i = 1,tItem.m_maxUser do
			local pCountItem = gcAccount.detaillist:add()
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
			pCountItem.chairid = i
		end
		
	end

	
	GdmjHistoryModel.SetHistory(tItem, 0, gcAccount)
	gcAccount.result = 0
	
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	
	--tItem.m_tInfo.timemark = 1
	--tItem.m_tInfo.status = g_gdmjStatus.status_end
	
end

function RpmjService.PlayCountDissolve(tItem)
	--申请解散后，发送的结算，在广东麻将推倒胡中，流局不算分
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	
	gcAccount.result = 0
	
	tItem.m_tInfo.viptable = 2
	gcAccount.counttype = 1
	
	for i = 1,tItem.m_maxUser do
		
		if tItem.m_tInfo.situser[i] ~= 0 then
			--这里是因为中途可能有人退出的
			local pCountItem = gcAccount.detaillist:add()
			pCountItem.chairid = i			
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
		end
		
	end

	--玩家手牌
	for i = 1,tItem.m_maxUser do
		if tItem.m_tInfo.situser[i] ~= 0 then
			--这里是因为中途可能有人退出的
			local addPoker = gcAccount.hpokerlist:add()
			for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
				local addItem = addPoker.mjpokerlist:add()
				addItem:ParseFromString(v:SerializeToString())
			end
			for k,v in ipairs(tItem.m_userList[i].handpoker) do
				addPoker.handpoker:append(v)
			end
			addPoker.userid = tItem.m_userList[i].userid
		end
		tItem.m_userModify[i] = 1
		
	end	
	
	--GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
	GdmjHistoryModel.SetHistory(tItem, 1, gcAccount)
	gcAccount.result = 0


	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	
	tItem.m_tInfo.timemark = 1
	tItem.m_tInfo.status = g_gdmjStatus.status_delete   --这里是的需要删除这个牌桌的，已经发送结算
end

function RpmjService.IsQiDui(tItem, pos)
	
	if #tItem.m_userList[pos].handpoker ~= 14 then
		--如果长度不对，肯定就不行了
		return 0
	end
	
	local handList = GdmjWork.GetHandList(tItem.m_userList[pos])

	table.sort(handList)

	local pair = 0
	local pairList = {}

	for i = 1, #handList, 2 do
		if handList[i] == handList[i+1] then
			pair = pair + 1
			if pairList[handList[i]] == nil then
				pairList[handList[i]] = 0
			end
			pairList[handList[i]] = pairList[handList[i]] + 1
		end
	end

	local haohua_pair = 0

	for k, v in pairs(pairList) do
		if v > 1 then
			haohua_pair = haohua_pair +1
		end
	end

	if pair == 7 then
		if haohua_pair == 0 then
			return g_gdmjWinType.type_qixiaodui
		else
			return g_gdmjWinType.type_longqidui
		end
	end

	return 0
end

function RpmjService.PlayCountWin(tItem)
	
	--这个是一个人赢的，不存在多个人赢

	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	local doubleAccount = 0    --番倍类型 --tItem.m_tInfo.pourjetton
	
	local winChairID = tItem.m_tInfo.winchairlist[1]  --因为前面已经确定长度是1了
	local maList = {0,0,0,0}
	local desList = {{},{},{},{}}
	local userMaList = {{},{},{},{}}
	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来
	
	--推倒胡不能吃胡
	if tItem.m_tInfo.wintype == g_gdmjAction.type_chihu then
		table.insert(desList[winChairID],"胡")
		table.insert(desList[tarChairID],"点炮")
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1
		tItem.m_detailList[tarChairID].dianpaonum = tItem.m_detailList[tarChairID].dianpaonum + 1
		tItem.m_detailList[winChairID].jiepaonum = tItem.m_detailList[winChairID].jiepaonum + 1
		doubleAccount = 2
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		--如果是抢杠胡，看看是不是抢杠全包
		doubleAccount = 4
		table.insert(desList[winChairID],"抢杠胡")
		table.insert(desList[tarChairID],"点炮")
		
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1
		tItem.m_detailList[tarChairID].dianpaonum = tItem.m_detailList[tarChairID].dianpaonum + 1
		tItem.m_detailList[winChairID].jiepaonum = tItem.m_detailList[winChairID].jiepaonum + 1
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_hu then  --这个是自摸胡
		--这个看看是不是要杠爆全包，杠爆全包就是放杠后，杠上花，放杠者全包。
		table.insert(desList[winChairID],"自摸")   --自摸
		doubleAccount = 2
	end

	local qidui = RpmjService.IsQiDui(tItem, winChairID)

	if 0 ~= qidui then
		if qidui == g_gdmjWinType.type_qixiaodui then
			table.insert(desList[winChairID],"七对")   --自摸
			doubleAccount = doubleAccount * 2
		elseif qidui == g_gdmjWinType.type_longqidui then
			table.insert(desList[winChairID],"豪华七对")   --自摸
			doubleAccount = doubleAccount * 4			
		end
	end

	if tItem.m_vipRoomInfo.qingyises == 1 then
		if 0 ~= GdmjHuHelper.IsQingYiSe(tItem,winChairID) then
			table.insert(desList[winChairID], "清一色")
			doubleAccount = doubleAccount * 2
		end				
	end

	if tItem.m_vipRoomInfo.shisanyao == 1 then
		if 0 ~= GdmjHuHelper.IsShiSanYao(tItem,winChairID) then
			table.insert(desList[winChairID], "十三幺")
			doubleAccount = doubleAccount * 8
		end				
	end
	
	tItem.m_detailList[winChairID].zimonum = tItem.m_detailList[winChairID].zimonum + 1
	
	if doubleAccount > tItem.m_detailList[winChairID].maxMultiple then
		tItem.m_detailList[winChairID].maxMultiple = doubleAccount
	end

	local maNum = tItem.m_vipRoomInfo.manum

	while maNum > 0 do
		if #tItem.m_tInfo.publicpoker == 0 then
			break
		end			
	
		gcAccount.malist:append(GdmjWork.GetRandPoker(tItem, 0))
		maNum = maNum - 1
	end

	if tItem.m_vipRoomInfo.manum ~= 99 then
		local maIndexList = {0,0,0,0}
		local maIndex = 1
		for i = tItem.m_tInfo.bankerpos,tItem.m_maxUser do
			maIndexList[maIndex] = i
			maIndex = maIndex + 1
		end
		for i = 1,tItem.m_tInfo.bankerpos - 1 do
			maIndexList[maIndex] = i
			maIndex = maIndex + 1
		end

		for k,v in ipairs(gcAccount.malist) do
			local index = math.mod(v,10)
			index = math.mod(index,tItem.m_maxUser)
			index = index == 0 and tItem.m_maxUser or index
			
			maList[ maIndexList[index] ] = maList[ maIndexList[index] ] + 1
			table.insert(userMaList[maIndexList[index]], v)			
		end

		for i = 1,tItem.m_maxUser do
			tItem.m_detailList[i].zhongmanum = tItem.m_detailList[i].zhongmanum + maList[i]
			table.insert(desList[i],"中马"..maList[i])
		end
	end

	local winItemList = {}
	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist:add()
		
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = 0
		item.iswin = 0
		item.hunum = 0
		item.winjetton = 0
		winItemList[i] = {}
		for j = 1, tItem.m_maxUser do
			winItemList[i][j] = 0
		end
	end

	if tItem.m_vipRoomInfo.gangbaoquanbao == 0 then
		for i = 1,tItem.m_maxUser do
			local srcItem = gcAccount.countlist[i]
			if tItem.m_detailList[i].minggangnum > 0 then
				local minggangnum = 0
				for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
					if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_minggang then
						minggangnum = minggangnum + 1
					elseif vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_bugang then
						minggangnum = minggangnum + 1
					end
				end
				table.insert(desList[i], "明杠"..minggangnum)

				for j = 1, tItem.m_maxUser do
					local dstItem = gcAccount.countlist[j]
					if j ~= i then
						local xxjetton = (minggangnum * (1 + maList[i]) * (1 + maList[j])) * tItem.m_vipRoomInfo.difen
						srcItem.gangjetton = srcItem.gangjetton + xxjetton
						dstItem.gangjetton = dstItem.gangjetton - xxjetton
						winItemList[i][j] = winItemList[i][j] - xxjetton
						winItemList[j][i] = winItemList[j][i] + xxjetton
					end
				end
			end
		end
	else
		for i = 1,tItem.m_maxUser do
			local srcItem = gcAccount.countlist[i]
			if tItem.m_detailList[i].minggangnum > 0 then
				local minggangnum = 0
				for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
					if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_minggang then
						minggangnum = minggangnum + 1
						local xxjetton = (tItem.m_maxUser - 1) * (1 + maList[i]) * (1 + maList[vv.tarchairid]) * tItem.m_vipRoomInfo.difen
						srcItem.gangjetton = srcItem.gangjetton + xxjetton
						local dstItem = gcAccount.countlist[vv.tarchairid]
						dstItem.gangjetton = dstItem.gangjetton - xxjetton
						winItemList[i][vv.tarchairid] = winItemList[i][vv.tarchairid] - xxjetton
						winItemList[vv.tarchairid][i] = winItemList[vv.tarchairid][i] + xxjetton
					elseif vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_bugang then
						minggangnum = minggangnum + 1
						for j = 1, tItem.m_maxUser do
							local dstItem = gcAccount.countlist[j]
							if j ~= i then
								local xxjetton = (1 * (1 + maList[i]) * (1 + maList[j])) * tItem.m_vipRoomInfo.difen
								srcItem.gangjetton = srcItem.gangjetton + xxjetton
								dstItem.gangjetton = dstItem.gangjetton - xxjetton
								winItemList[i][j] = winItemList[i][j] - xxjetton
								winItemList[j][i] = winItemList[j][i] + xxjetton
							end
						end
					end
				end
				table.insert(desList[i], "明杠"..minggangnum)
			end
		end
	end

	--再计算明杠，暗杠
	for i = 1,tItem.m_maxUser do
		local srcItem = gcAccount.countlist[i]
		if tItem.m_detailList[i].angangnum > 0 then
			
			local angangnum = 0
			for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
				if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_angang then
					angangnum = angangnum + 1
				end
			end
			table.insert(desList[i], "暗杠"..angangnum)
			for j = 1, tItem.m_maxUser do
				local dstItem = gcAccount.countlist[j]
				if j ~= i then
					local xxjetton = (2 * angangnum * (1 + maList[i]) * (1 + maList[j])) * tItem.m_vipRoomInfo.difen
					srcItem.gangjetton = srcItem.gangjetton + xxjetton
					dstItem.gangjetton = dstItem.gangjetton - xxjetton
					winItemList[i][j] = winItemList[i][j] - xxjetton
					winItemList[j][i] = winItemList[j][i] + xxjetton
				end
			end
		end
	end

	--先算胡牌获得分数
	if tItem.m_tInfo.wintype == g_gdmjAction.type_chihu or tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		if tarChairID >= 1 and tarChairID <= tItem.m_maxUser then
			--如果是吃胡的话， 点炮的人输钱
			local winItem = gcAccount.countlist[winChairID]
			local costItem = gcAccount.countlist[tarChairID]
			local xxjetton = (doubleAccount * (1 + maList[winChairID]) * (1 + maList[tarChairID])) * tItem.m_vipRoomInfo.difen
			winItem.hunum = winItem.hunum + xxjetton
			costItem.hunum = costItem.hunum - xxjetton
			winItemList[winChairID][tarChairID] = winItemList[winChairID][tarChairID] - xxjetton
			winItemList[tarChairID][winChairID] = winItemList[tarChairID][winChairID] + xxjetton
		end
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_hu then
		local winItem = gcAccount.countlist[winChairID]
		--如果是自摸的话， 其他人都要输钱
		for i = 1,tItem.m_maxUser do
		
			local item = gcAccount.countlist[i]

			if i ~= winChairID then
				local xxjetton = (doubleAccount * (1 + maList[winChairID]) * (1 + maList[i])) * tItem.m_vipRoomInfo.difen
				winItem.hunum = winItem.hunum + xxjetton
				item.hunum = item.hunum - xxjetton
				winItemList[winChairID][i] = winItemList[winChairID][i] - xxjetton
				winItemList[i][winChairID] = winItemList[i][winChairID] + xxjetton
			end
		end
	end

	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist[i]

		item.winjetton = item.gangjetton + item.hunum
	end

	--如果玩家分数不够， 这里回退
	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist[i]

		item.carryjetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
		
		if tItem.m_tInfo.julebutype == 2 then
			if item.winjetton < 0 then
				item.julebu_jetton = JulebuService.GetJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid) + item.carryjetton
				if item.julebu_jetton < 0 then
					local leftJetton = JulebuService.GetJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid) + tItem.m_userList[i].carryjetton
					item.winjetton = -leftJetton
					local allJetton = 0
					for j = 1, tItem.m_maxUser do
						if i ~= j then
							if winItemList[i][j] > 0 then
								local dstItem = gcAccount.countlist[j]
								dstItem.winjetton = dstItem.winjetton - winItemList[i][j]
								allJetton = allJetton + winItemList[i][j]
							end
						end
					end
					for j = 1, tItem.m_maxUser do
						if i ~= j then
							if winItemList[i][j] > 0 then
								local dstItem = gcAccount.countlist[j]
								dstItem.winjetton = dstItem.winjetton + math.floor(leftJetton * winItemList[i][j] / allJetton)
							end
						end
					end
				end
			end
		end
	end

	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist[i]
		
		if i == winChairID then
			item.iswin = 1
			gcAccount.winid:append(item.userid)
			gcAccount.winchairid:append(i)
			gcAccount.wintypelist:append(tItem.m_tInfo.wintype)
		end

		for k2,v2 in ipairs(desList[i]) do
			item.des:append(v2)
		end

		tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
		tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
		item.carryjetton = tItem.m_userList[i].carryjetton
		
		recoreItem.score:append(item.winjetton) --这里是安装顺序去的
		tItem.m_tInfo.viprecord.score[i] = tItem.m_userList[i].carryjetton
		
		gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
		tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + item.winjetton
	end

	if tItem.m_tInfo.viptable ~= 0 and tItem.m_tInfo.usevipnum == tItem.m_tInfo.maxvipnum then
		--打完八局或者16局了
		tItem.m_tInfo.viptable = 2
		gcAccount.counttype = 1
		
		for i = 1,tItem.m_maxUser do
			local pCountItem = gcAccount.detaillist:add()
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
			pCountItem.chairid = i
		end
		
	end

	--玩家手牌
	for i = 1,tItem.m_maxUser do
		
		local addPoker = gcAccount.hpokerlist:add()
		
		for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
			
			local addItem = addPoker.mjpokerlist:add()
			addItem:ParseFromString(v:SerializeToString())
			
		end

		if i == winChairID then
			--如果是胜利这，这里需要注意了
			local isExist = false
			
			for k,v in ipairs(tItem.m_userList[i].handpoker) do
				if v == pokerID then
					if isExist == false then
						isExist = true
					else
						addPoker.handpoker:append(v)
					end
				else
					addPoker.handpoker:append(v)
				end
			end
			addPoker.handpoker:append(pokerID)  --这是为了把这个值放到最后
		else
			for k,v in ipairs(tItem.m_userList[i].handpoker) do
				addPoker.handpoker:append(v)
			end			
		end

		addPoker.userid = tItem.m_userList[i].userid
		
		tItem.m_userModify[i] = 1
	end
	
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
	
	GdmjHistoryModel.SetHistory(tItem, 0, gcAccount)
	gcAccount.result = 0
	--local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	--SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())

	for i = 1,tItem.m_maxUser do
		local userid = tItem.m_userList[i].userid
		while #gcAccount.magetlist > 0 do
			gcAccount.magetlist:remove(1)
		end
		
		for k, v in ipairs(userMaList[i]) do
			gcAccount.magetlist:append(v)
		end
	
		SendMessage(userid, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	end

		
	--tItem.m_tInfo.status = g_gdmjStatus.status_end
	--print("tItem.m_tInfo.bankercount="..tItem.m_tInfo.bankercount)
	if tItem.m_tInfo.bankerpos == winChairID then
		--连庄的设置,这里首先要判断节节高
		tItem.m_tInfo.bankercount = tItem.m_tInfo.bankercount + 1
	else
		tItem.m_tInfo.bankercount = 0
	end	
	
	tItem.m_tInfo.bankerpos = winChairID
	--tItem.m_tInfo.timemark = g_gdmjTime.end_time
end

function RpmjService.PlayCountWinMore(tItem)
	--这个是一个人赢的，不存在多个人赢

	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	local doubleAccount = 0    --番倍类型 --tItem.m_tInfo.pourjetton
	
	local winList = {}  

	local doubleAccountList = {0,0,0,0}
	local maList = {0,0,0,0}
	local desList = {{},{},{},{}}
	local userMaList = {{},{},{},{}}
	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来

	for k,v in ipairs(tItem.m_tInfo.winchairlist) do
		table.insert(winList,v)
	end
	
	--推倒胡不能吃胡
	if tItem.m_tInfo.wintype == g_gdmjAction.type_chihu then
		for k,v in ipairs(winList) do
			doubleAccount = 2
			table.insert(desList[k],"吃胡")
			tItem.m_detailList[v].hunum = tItem.m_detailList[v].hunum + 1
			tItem.m_detailList[v].jiepaonum = tItem.m_detailList[v].jiepaonum + 1

			local qidui = RpmjService.IsQiDui(tItem, v)

			if 0 ~= qidui then
				if qidui == g_gdmjWinType.type_qixiaodui then
					table.insert(desList[v],"七对")   --自摸
					doubleAccount = doubleAccount * 2
				elseif qidui == g_gdmjWinType.type_longqidui then
					table.insert(desList[v],"豪华七对")   --自摸
					doubleAccount = doubleAccount * 4			
				end
			end

			if tItem.m_vipRoomInfo.qingyises == 1 then
				if 0 ~= GdmjHuHelper.IsQingYiSe(tItem,v) then
					table.insert(desList[v], "清一色")
					doubleAccount = doubleAccount * 2
				end				
			end

			if tItem.m_vipRoomInfo.shisanyao == 1 then
				if 0 ~= GdmjHuHelper.IsShiSanYao(tItem,v) then
					table.insert(desList[v], "十三幺")
					doubleAccount = doubleAccount * 8
				end				
			end

			doubleAccountList[v] = doubleAccount

			if doubleAccount > tItem.m_detailList[v].maxMultiple then
				tItem.m_detailList[v].maxMultiple = doubleAccount
			end
		end
		table.insert(desList[tarChairID],"点炮")
		tItem.m_detailList[tarChairID].dianpaonum = tItem.m_detailList[tarChairID].dianpaonum + 1
		
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		--如果是抢杠胡，看看是不是抢杠全包
		
		for k,v in ipairs(winList) do
			doubleAccount = 4
			table.insert(desList[k],"抢杠胡")
			tItem.m_detailList[v].hunum = tItem.m_detailList[v].hunum + 1
			tItem.m_detailList[v].jiepaonum = tItem.m_detailList[v].jiepaonum + 1

			if 0 ~= RpmjService.IsQiDui(tItem, v) then
				table.insert(desList[v],"七对")   --自摸
				doubleAccount = doubleAccount * 2
			end

			if tItem.m_vipRoomInfo.qingyises == 1 then
				if 0 ~= GdmjHuHelper.IsQingYiSe(tItem,v) then
					table.insert(desList[v], "清一色")
					doubleAccount = doubleAccount * 2
				end				
			end

			if tItem.m_vipRoomInfo.shisanyao == 1 then
				if 0 ~= GdmjHuHelper.IsShiSanYao(tItem,v) then
					table.insert(desList[v], "十三幺")
					doubleAccount = doubleAccount * 8
				end				
			end

			doubleAccountList[v] = doubleAccount

			if doubleAccount > tItem.m_detailList[v].maxMultiple then
				tItem.m_detailList[v].maxMultiple = doubleAccount
			end
		end
		table.insert(desList[tarChairID],"点炮")
		tItem.m_detailList[tarChairID].dianpaonum = tItem.m_detailList[tarChairID].dianpaonum + 1
	end
	
	local maNum = tItem.m_vipRoomInfo.manum

	while maNum > 0 do
		if #tItem.m_tInfo.publicpoker == 0 then
			break
		end			
	
		gcAccount.malist:append(GdmjWork.GetRandPoker(tItem, 0))
		maNum = maNum - 1
	end

	if tItem.m_vipRoomInfo.manum ~= 99 then
		local maIndexList = {0,0,0,0}
		local maIndex = 1
		for i = tItem.m_tInfo.bankerpos,tItem.m_maxUser do
			maIndexList[maIndex] = i
			maIndex = maIndex + 1
		end
		for i = 1,tItem.m_tInfo.bankerpos - 1 do
			maIndexList[maIndex] = i
			maIndex = maIndex + 1
		end


		for k,v in ipairs(gcAccount.malist) do
			local index = math.mod(v,10)
			index = math.mod(index,tItem.m_maxUser)
			index = index == 0 and tItem.m_maxUser or index
			
			maList[ maIndexList[index] ] = maList[ maIndexList[index] ] + 1
			table.insert(userMaList[maIndexList[index]], v)				
		end

		for i = 1,tItem.m_maxUser do
			tItem.m_detailList[i].zhongmanum = tItem.m_detailList[i].zhongmanum + maList[i]
			table.insert(desList[i],"中马"..maList[i])
		end
	end

	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist:add()
		
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = tItem.m_userList[i].carryjetton - tItem.m_userList[i].psinfo.jetton
		item.iswin = 0
	end

	--先算胡牌获得分数
	if tItem.m_tInfo.wintype == g_gdmjAction.type_chihu or tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		if tarChairID >= 1 and tarChairID <= tItem.m_maxUser then
			--如果是吃胡的话， 点炮的人输钱
			for k,v in ipairs(winList) do
				local winChairID = v
				local winItem = gcAccount.countlist[winChairID]
				local costItem = gcAccount.countlist[tarChairID]
				winItem.hunum = winItem.hunum + doubleAccountList[winChairID] * (1 + maList[winChairID]) * (1 + maList[tarChairID])
				costItem.hunum = costItem.hunum - doubleAccountList[winChairID] * (1 + maList[winChairID]) * (1 + maList[tarChairID])
			end
		end
	end

	--再计算明杠，暗杠
	for i = 1,tItem.m_maxUser do
		local srcItem = gcAccount.countlist[i]
		if tItem.m_detailList[i].angangnum > 0 then
			local angangnum = 0
			for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
				if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_angang then
					angangnum = angangnum + 1
				end
			end
			table.insert(desList[i], "暗杠"..angangnum)

			for j = 1, tItem.m_maxUser do
				local dstItem = gcAccount.countlist[j]
				if j ~= i then
					srcItem.gangjetton = srcItem.gangjetton + 2 * angangnum * (1 + maList[i]) * (1 + maList[j])
					dstItem.gangjetton = dstItem.gangjetton - 2 * angangnum * (1 + maList[i]) * (1 + maList[j])
				end
			end
		end
	end

	if tItem.m_vipRoomInfo.gangbaoquanbao == 0 then
		for i = 1,tItem.m_maxUser do
			local srcItem = gcAccount.countlist[i]
			if tItem.m_detailList[i].minggangnum > 0 then
				local minggangnum = 0
				for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
					if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_minggang then
						minggangnum = minggangnum + 1
					elseif vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_bugang then
						minggangnum = minggangnum + 1
					end
				end
				table.insert(desList[i], "明杠"..minggangnum)

				for j = 1, tItem.m_maxUser do
					local dstItem = gcAccount.countlist[j]
					if j ~= i then
						srcItem.gangjetton = srcItem.gangjetton + minggangnum * (1 + maList[i]) * (1 +  maList[j])
						dstItem.gangjetton = dstItem.gangjetton - minggangnum * (1 + maList[i]) * (1 +  maList[j])
					end
				end
			end
		end
	else
		for i = 1,tItem.m_maxUser do
			local srcItem = gcAccount.countlist[i]
			if tItem.m_detailList[i].minggangnum > 0 then
				local minggangnum = 0
				for kk, vv in ipairs(tItem.m_userList[i].mjpokerlist) do
					if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_minggang then
						minggangnum = minggangnum + 1
						srcItem.gangjetton = srcItem.gangjetton + (tItem.m_maxUser - 1) * (1 + maList[i]) * (1 + maList[vv.tarchairid])
						local dstItem = gcAccount.countlist[vv.tarchairid]
						dstItem.gangjetton = dstItem.gangjetton - (tItem.m_maxUser - 1) * (1 + maList[i]) * (1 + maList[vv.tarchairid])
					elseif vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_bugang then
						minggangnum = minggangnum + 1
						for j = 1, tItem.m_maxUser do
							local dstItem = gcAccount.countlist[j]
							if j ~= i then
								srcItem.gangjetton = srcItem.gangjetton + 1 * (1 + maList[i]) * (1 + maList[j])
								dstItem.gangjetton = dstItem.gangjetton - 1 * (1 + maList[i]) * (1 + maList[j])
							end
						end
					end
				end
				table.insert(desList[i], "明杠"..minggangnum)
			end
		end
	end

	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist[i]
		
		for k,v in ipairs(winList) do
			if i == v then
				item.iswin = 1
				gcAccount.winid:append(item.userid)
				gcAccount.winchairid:append(i)
				gcAccount.wintypelist:append(tItem.m_tInfo.wintype)
			end
		end

		for k2,v2 in ipairs(desList[i]) do
			item.des:append(v2)
		end

		item.gangjetton = item.gangjetton * tItem.m_vipRoomInfo.difen
		item.hunum = item.hunum * tItem.m_vipRoomInfo.difen
		item.winjetton = item.gangjetton + item.hunum
		tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
		tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
		item.carryjetton = tItem.m_userList[i].carryjetton
		item.julebu_jetton = JulebuService.GetJiFen(tItem.m_tInfo.julebuid, tItem.m_userList[i].userid) + item.carryjetton
		
		recoreItem.score:append(item.winjetton) --这里是安装顺序去的
		tItem.m_tInfo.viprecord.score[i] = tItem.m_userList[i].carryjetton
		
		gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
		tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + item.winjetton
	end

	if tItem.m_tInfo.viptable ~= 0 and tItem.m_tInfo.usevipnum == tItem.m_tInfo.maxvipnum then
		--打完八局或者16局了
		tItem.m_tInfo.viptable = 2
		gcAccount.counttype = 1
		
		for i = 1,tItem.m_maxUser do
			local pCountItem = gcAccount.detaillist:add()
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
			pCountItem.chairid = i
		end
		
	end

	--玩家手牌
	for i = 1,tItem.m_maxUser do
		
		local addPoker = gcAccount.hpokerlist:add()
		
		for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
			
			local addItem = addPoker.mjpokerlist:add()
			addItem:ParseFromString(v:SerializeToString())
			
		end

		if i == tarChairID then
			--如果是胜利这，这里需要注意了
			for k,v in ipairs(tItem.m_userList[i].handpoker) do
				addPoker.handpoker:append(v)
			end
			addPoker.handpoker:append(pokerID)  --这是为了把这个值放到最后
		else
			for k,v in ipairs(tItem.m_userList[i].handpoker) do
				addPoker.handpoker:append(v)
			end
		end
		addPoker.userid = tItem.m_userList[i].userid
		tItem.m_userModify[i] = 1
	end
	
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
	
	GdmjHistoryModel.SetHistory(tItem, 0, gcAccount)
	gcAccount.result = 0
	--local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	--SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())

	for i = 1,tItem.m_maxUser do
		local userid = tItem.m_userList[i].userid
		while #gcAccount.magetlist > 0 do
			gcAccount.magetlist:remove(1)
		end
		
		for k, v in ipairs(userMaList[i]) do
			gcAccount.magetlist:append(v)
		end
	
		SendMessage(userid, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	end

	if tItem.m_tInfo.bankerpos == winList[1] then
		--连庄的设置
		tItem.m_tInfo.bankercount = tItem.m_tInfo.bankercount + 1
	else
		tItem.m_tInfo.bankercount = 0
	end
	
		
	--tItem.m_tInfo.status = g_gdmjStatus.status_end
	tItem.m_tInfo.bankerpos = winList[1]
	--tItem.m_tInfo.timemark = g_gdmjTime.end_time	
end

