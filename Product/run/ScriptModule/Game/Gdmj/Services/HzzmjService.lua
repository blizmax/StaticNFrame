
--推倒胡


HzzmjService = {}


function HzzmjService.DoWork(tItem) --传入的是TdhStruct类型的Item
	if tItem.m_tInfo.status == g_gdmjStatus.status_ready then
		--牌桌处于等待的状态，如果人满了就开始
		HzzmjService.TableReady(tItem)
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_waiting then
		
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		
		if tItem.m_tInfo.timemark <= 0 then
			tItem.m_tInfo.status = g_gdmjStatus.status_playing
		end
		tItem.m_isModify = true
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_playing then
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		HzzmjService.TablePlay(tItem,v)
		
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_counting then
		tItem.m_isModify = true
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		if tItem.m_tInfo.timemark < 0 then
			HzzmjService.PlayCount(tItem)
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
			GdmjWork.CheckJiFen(tItem, 0)
		end
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_dissolve then
		
		if tItem.m_tInfo.timemark > 0 then
			tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		end
		
		if tItem.m_tInfo.timemark <= 0 then
			--这里开始下发数据
			HzzmjService.PlayCountDissolve(tItem)
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

function HzzmjService.TableReady(tItem)  --牌桌处于等待状态的函数
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
		HzzmjService.GameStart(tItem)
		--开始的时候，强制要求非座位上的玩家退出
		
		
	else
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
	end
	
end

function HzzmjService.InitPublicPoker(tItem)

	while #tItem.m_tInfo.winchairlist > 0 do
		tItem.m_tInfo.winchairlist:remove(1)
	end
	while #tItem.m_tInfo.guilist > 0 do
		tItem.m_tInfo.guilist:remove(1)
	end
	while #tItem.m_tInfo.chihulist > 0 do
		tItem.m_tInfo.chihulist:remove(1)
	end
	GdmjWork.NextInfoInit(tItem.m_nextInfo)  --这个也需要初始化
	GdmjWork.NextInfoInit(tItem.m_tInfo.nextinfo)
	
	local pokerList = {}
	

	for i = 1,4 do
		for j = 1,5 do
			for k,v in ipairs(g_gdmjPokerList[j]) do
				table.insert(pokerList, v)
			end
		end
	end		
	for k,v in ipairs(g_gdmjPokerList[6]) do
		--有花牌
		table.insert(pokerList, v)
	end

	for k,v in ipairs(g_gdmjPokerList[7]) do
		--有花牌
		table.insert(pokerList, v)
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
	
	GdmjWork.InitPubGuiPai(tItem)
	--在这里设置鬼牌
	--在这里还需要确定鬼牌的长度
end

function HzzmjService.StartTest(tItem)

	local function InsertTest( tItem, pos, pokerID)
		for k, v in ipairs(tItem.m_tInfo.publicpoker) do
			if v == pokerID then
				tItem.m_tInfo.publicpoker:remove(k)
				break
			end
		end
		tItem.m_userList[pos].handpoker:append(pokerID)
	end
	
	
	InsertTest(tItem, 1, 11)
	InsertTest(tItem, 1, 11)
	InsertTest(tItem, 1, 11)
	InsertTest(tItem, 1, 12)
	InsertTest(tItem, 1, 13)
	InsertTest(tItem, 1, 31)
	InsertTest(tItem, 1, 31)
	InsertTest(tItem, 1, 32)
	InsertTest(tItem, 1, 32)
	InsertTest(tItem, 1, 33)
	InsertTest(tItem, 1, 35)
	InsertTest(tItem, 1, 37)
	InsertTest(tItem, 1, 39)
	
	InsertTest(tItem, 2, 14)
	InsertTest(tItem, 2, 14)
	InsertTest(tItem, 2, 14)
	InsertTest(tItem, 2, 15)
	
	
	InsertTest(tItem, 2, 15)
	InsertTest(tItem, 2, 15)
	InsertTest(tItem, 2, 17)
	InsertTest(tItem, 2, 17)
	InsertTest(tItem, 2, 17)
	InsertTest(tItem, 2, 18)
	InsertTest(tItem, 2, 18)
	InsertTest(tItem, 2, 19)
	InsertTest(tItem, 2, 19)
	
	InsertTest(tItem, 3, 17)
	InsertTest(tItem, 3, 31)
	InsertTest(tItem, 3, 39)
	InsertTest(tItem, 3, 39)
	InsertTest(tItem, 3, 41)
	InsertTest(tItem, 3, 41)
	InsertTest(tItem, 3, 42)
	InsertTest(tItem, 3, 42)
	InsertTest(tItem, 3, 51)
	InsertTest(tItem, 3, 52)
	InsertTest(tItem, 3, 52)
	InsertTest(tItem, 3, 53)
	InsertTest(tItem, 3, 53)

	if tItem.m_maxUser == 4 then
		InsertTest(tItem, 4, 24)	
		InsertTest(tItem, 4, 24)
		InsertTest(tItem, 4, 26)
		InsertTest(tItem, 4, 27)
		InsertTest(tItem, 4, 28)
		InsertTest(tItem, 4, 36)
		InsertTest(tItem, 4, 36)

		InsertTest(tItem, 4, 43)
		InsertTest(tItem, 4, 43)
		InsertTest(tItem, 4, 44)
		InsertTest(tItem, 4, 44)
		InsertTest(tItem, 4, 52)
		InsertTest(tItem, 4, 51)
	end
end

function HzzmjService.GameStart(tItem)

	--初始化公共牌
	HzzmjService.InitPublicPoker(tItem)
	GdmjWork.VipPay(tItem.m_tInfo)
	
	--GdmjWork.SendStartInfo(tItem)
	for i = 1,tItem.m_maxUser do
		GdmjWork.UserStartInit(tItem, tItem.m_userList[i])   --初始化
	end
	
	if g_isDebug == 1 then
		--HzzmjService.StartTest(tItem)
		for i = 1,13 do

			for k = 1,tItem.m_maxUser do
				GdmjWork.HandPokerInsert(tItem, k)
			end
		end
		
	else
	
		for i = 1,13 do
			for k = 1,tItem.m_maxUser do
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
	
	--tItem.m_tInfo.nextinfo.actchairid[tItem.m_tInfo.bankerpos] = 1 --这里不能置为1，这里不能置为1
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
	
	local buHuaList = {}
	
	local function HzzBuHua(tItem, pos)
		--开始补花了
		local gcBuHua = msg_gdmj_pb.gcgdmjaction()
		local huaNum = 0
		
		for k = #tItem.m_userList[pos].handpoker,1,-1 do
			if tItem.m_userList[pos].handpoker[k] > 60 then
				huaNum = huaNum + 1

				GdmjWork.AddPokerToOutList(tItem.m_userList[pos], tItem.m_userList[pos].handpoker[k])
				GdmjWork.DelPokerFromHand(tItem.m_userList[pos], tItem.m_userList[pos].handpoker[k],1)
				--扣了一个
			end
		end
		while #tItem.m_userList[pos].handpoker < 13 do
			--一定会补够13个
			local pokerID = GdmjWork.GetRandPoker(tItem, pos)
			if pokerID > 60 then
				GdmjWork.AddPokerToOutList(tItem.m_userList[pos], pokerID)
				huaNum = huaNum + 1
			else
				GdmjWork.HandPokerInsert(tItem, pos, pokerID)
			end
			
		end
		
		if huaNum > 0 then
			--需要补花
			gcBuHua.actiontype:append(g_gdmjAction.type_buhua)
			
			local addItem = gcBuHua.tarplayer:add()
			addItem:ParseFromString(tItem.m_userList[pos]:SerializeToString())
			
			local addItem2 = gcBuHua.actplayer:add()
			addItem2:ParseFromString(tItem.m_userList[pos]:SerializeToString())
			gcBuHua.result = 0
			
			--SendMessage(userIDList, PacketCode[2204].client, gcBuHua:ByteSize(), gcBuHua:SerializeToString())
			table.insert(buHuaList, gcBuHua)
		end
		
	end
	
	for i = tItem.m_tInfo.bankerpos, tItem.m_maxUser do
		HzzBuHua(tItem, i)
	end	
	for i = 1, (tItem.m_tInfo.bankerpos - 1) do
		HzzBuHua(tItem, i)
	end
		
	GdmjWork.SendStartInfo(tItem)
	
	--在这里开始的补花
	--从庄家开始
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	for k,v in ipairs(buHuaList) do
		--因为补花出问题，所以，开始的时候，不发补花的协议
		--SendMessage(userIDList, PacketCode[2204].client, v:ByteSize(), v:SerializeToString())
	end
	
end

function HzzmjService.TablePlay(tItem,currPos)
	
	if tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		tItem.m_isModify = true
		
		HzzmjService.CheckMoving(tItem, true)
		
	else
		local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
		for k,currPos in ipairs(currPosList) do
			if tItem.m_userList[currPos].isrobot == 1 then
				--如果是机器人

				tItem.m_userList[currPos].robottime = tItem.m_userList[currPos].robottime - 1
				if tItem.m_userList[currPos].robottime < 0 then
					HzzmjService.AutoAction(tItem)
				end
			else
				if tItem.m_userList[currPos].autoplayer == 1 then
					HzzmjService.AutoAction(tItem)
				else
					if tItem.m_tInfo.actiontime > 0 then
						tItem.m_tInfo.actiontime = tItem.m_tInfo.actiontime - 1
					end
				end
			end
		end
	end
end

function HzzmjService.CheckMoving(tItem,isLog)
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..", moving start")
	end	
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
	
	if #currPosList == 0 then
		LogFile("error", "HzzmjService.CheckMoving actchairid is nil")
		return nil
	end
	local currPos = currPosList[1] --取第一个就行了。
	local gcUpdate = msg_gdmj_pb.gcgdmjupdate()
	
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..", currPos="..currPos..",beingpoker="..tItem.m_tInfo.beingpoker)
	end
	
	if tItem.m_tInfo.beingpoker == 1 then
		--这个是需要发牌的
		local pokerID = GdmjWork.HandPokerInsert(tItem, currPos)
		
		tItem.m_userList[currPos].guohu = 0   --这里改为摸牌，只有摸牌才算，出牌的不算，过胡的时候
		
		local checkHu = GdmjWork.CheckIsHu(tItem, currPos, pokerID) --为什么是humark呢
		local checkGang = GdmjWork.CheckIsGang(tItem.m_userList[currPos], pokerID)
		tItem.m_tInfo.nextinfo.actpokerid = pokerID
		tItem.m_tInfo.nextinfo.actchairid[currPos] = 1
		tItem.m_tInfo.nextinfo.tarchairid = 0   --设置
		tItem.m_userList[currPos].humark = pokerID    --设置成这里
		
		if checkHu > 0 or checkGang > 0 then
			--已经可以胡了
			--
			tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_guo)
			
			if checkHu > 0 then
				tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_hu)
			end
			
			if checkGang > 0 then
				tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_gang)
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
			tItem.m_tInfo.nextinfo.actiontype:append(g_gdmjAction.type_play)    --这里需要加一个play，因为在恢复的时候没办法
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


	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..", canplay="..gcUpdate.canplay ..",tarchairid="..gcUpdate.tarchairid )
	end

	
	GdmjWork.SendUpdate(tItem,gcUpdate, currPos, isLog)
	
	if true == isLog then
		LogFile("moving","tableid="..tItem.m_tInfo.tableid..", moving end")
	end	
	tItem.m_isModify = true
	for k,v in ipairs(currPosList) do
		tItem.m_userModify[v] = 1
	end
	
	
end


function HzzmjService.PlayingAction(tItem,cgmsg, gcmsg)
	
	local isWin = false
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
	GdmjWork.NextInfoInit(tItem.m_nextInfo)
	
	tItem.m_tInfo.beingpoker = 0   --置为不能发牌
	if cgmsg.actiontype == g_gdmjAction.type_play then
		
		HzzmjService.DoPlay(tItem,cgmsg,gcmsg)
		huMark = cgmsg.actpokerid
		--if tItem.m_userList[cgmsg.actchairid].humark ~= cgmsg.actpokerid then
			--如果发了这个牌，立刻打出这个牌，则不用处理
			GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
		--end
	elseif cgmsg.actiontype == g_gdmjAction.type_guo then
		isWin = HzzmjService.DoGuo(tItem, cgmsg, gcmsg)  --这里过的时候，必须要在这里返回
	elseif cgmsg.actiontype == g_gdmjAction.type_chi then
		HzzmjService.DoChi(tItem, cgmsg, gcmsg)
	elseif cgmsg.actiontype == g_gdmjAction.type_peng then
		HzzmjService.DoPeng(tItem, cgmsg, gcmsg)
		
	elseif cgmsg.actiontype == g_gdmjAction.type_gang then
		HzzmjService.DoGang(tItem, cgmsg, gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
	elseif cgmsg.actiontype == g_gdmjAction.type_hu then
		isWin = true
		tItem.m_tInfo.wintype = g_gdmjAction.type_hu
		tItem.m_tInfo.winchairlist:append(cgmsg.actchairid)
	elseif cgmsg.actiontype == g_gdmjAction.type_qiangganghu then
	
		if #currPosList == 1 then
			isWin = true			
		end
		
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
	elseif cgmsg.actiontype == g_gdmjAction.type_genzhuang then
	
	elseif cgmsg.actiontype == g_gdmjAction.type_buhua then
		--这个是补花
		HzzmjService.DoBuHua(tItem, cgmsg, gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
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
	tItem.m_userList[cgmsg.actchairid].humark = 0   --设置为0，记住一定要设置为0
	if cgmsg.tarchairid ~= 0 then
		tItem.m_userModify[cgmsg.tarchairid] = 1
	end
	
	local isEnd = false
	
	if isWin == true then
		HzzmjService.DoWin(tItem, cgmsg.actchairid, cgmsg.actchairid)
		isEnd = true
	else
		tItem.m_userList[cgmsg.actchairid].playstate = g_gdmjPlayerState.play_waiting
		if tItem.m_tInfo.beingpoker == 1 then
			--只有发牌的情况下会去检查流局
			isEnd = HzzmjService.CheckLiuJu(tItem)
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


function HzzmjService.DoPlay(tItem, cgmsg, gcmsg)
	--这个是打牌的操作
	--在打牌的操作中，只有一个人的行动，不会存在多个人的
	--local chiHuList = 
	--推倒胡中，不能够吃胡，所以这里不用检查吃胡
	--首先，在手牌中去掉
	tItem.m_tInfo.prevpos = 0  --首先，把这个设置为0，为什么要把这个设置为0呢，这个是杠爆全包的,只要是出牌了，这个就不算了
	
	
	while #tItem.m_userList[cgmsg.actchairid].guopeng > 0 do   --过碰过碰
		tItem.m_userList[cgmsg.actchairid].guopeng:remove(1)
	end
	
	GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	GdmjWork.AddPokerToOutList(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)

	local chiHu = HzzmjService.NewCheckChiHu(tItem,cgmsg.actpokerid, cgmsg.actchairid,false)  --这里检查吃胡的时候，在自己本地检查	
	
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
			local nextPos,actList = GdmjWork.CheckNewPengGang(tItem,cgmsg.actpokerid, cgmsg.actchairid)
			if nextPos == chiHu then
				for k1,v1 in ipairs(actList) do
					tItem.m_nextInfo.actiontype:append(v1)
					if v1 == g_gdmjAction.type_gang then
						tItem.m_tInfo.prevpos = cgmsg.actchairid
					end				
				end
			end
			--在这里对惠东庄的判断
			local chiPos = 0
			if tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
				--如果是惠东庄，就行看看是不是能吃
				chiPos = GdmjWork.CheckChi(tItem, cgmsg.actchairid,cgmsg.actpokerid)
			end
			if chiPos == chiHu then
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chi)
			end
		end
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chihu)
	else
		local nextPos,actList = GdmjWork.CheckNewPengGang(tItem,cgmsg.actpokerid, cgmsg.actchairid)
		--在这里对惠东庄的判断
		local chiPos = 0
		if tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
			--如果是惠东庄，就行看看是不是能吃
			chiPos = GdmjWork.CheckChi(tItem, cgmsg.actchairid,cgmsg.actpokerid)
		end
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
			if chiPos == nextPos then
				--这里也要加过去
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chi)
			end
		else
			--如果没有碰或者杠，表示轮到下一个人了
			

			if chiPos ~= 0 then
				--这里是的需要吃的
				--可以吃了
				tItem.m_nextInfo.actchairid[chiPos] = 1
				tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chi)
				tItem.m_nextInfo.tarchairid = cgmsg.actchairid   --设置目标ID是当前目标				
			else
				tItem.m_tInfo.beingpoker = 1
				nextPos = cgmsg.actchairid == tItem.m_maxUser and 1 or cgmsg.actchairid + 1
				tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
			end
		end
		HzzmjService.CheckGenZhuang(tItem,cgmsg)   --检查跟庄
	end
end

function HzzmjService.DoGuo(tItem,cgmsg,gcmsg)
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
			end
		end
		
		if prevType == g_gdmjAction.type_qiangganghu then
			--抢杠胡是不用检查吃的
			--如果是吃胡的过，检查有没有下一个吃胡
			tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 0  --首先需要把自己的置为空
			--过了抢杠胡了
			tItem.m_tInfo.chihulist:remove(1)
			local chiHu = HzzmjService.NewCheckChiHu2(tItem)
			
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
				GdmjWork.ActionGang(tItem, cgGang, gcmsg)
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
			tItem.m_tInfo.chihulist:remove(1)
			local chiHu = HzzmjService.NewCheckChiHu2(tItem)
			
			if chiHu > 0 then
				tItem.m_nextInfo.actchairid[chiHu] = 1
				tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
				tItem.m_tInfo.beingpoker = 0
				tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
				tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chihu)
			else
				
				
				local nextPos,actList = GdmjWork.CheckNewPengGang(tItem,tItem.m_tInfo.nextinfo.actpokerid, tItem.m_tInfo.nextinfo.tarchairid)

				if nextPos ~= 0 then
					local tempList = HzzmjService.GetChiHuList(tItem, cgmsg.actpokerid, cgmsg.tarchairid)
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
					
					local chiPos = 0
					if tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
						--如果是惠东庄，就行看看是不是能吃
						chiPos = GdmjWork.CheckChi(tItem, tItem.m_tInfo.nextinfo.tarchairid, tItem.m_tInfo.nextinfo.actpokerid)
					end
					if chiPos ~= 0 then
						--这里是的需要吃的
						--可以吃了
						--看一看有没有吃啊
						tItem.m_nextInfo.actchairid[chiPos] = 1
						tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
						tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
						tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chi)
						tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid   --设置目标ID是当前目标				
					else					

						tItem.m_tInfo.beingpoker = 1
						nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
						tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
						tItem.m_nextInfo.canplay = 1
					end
				end
				--GdmjWork.GenZhuangCheck(tItem,cgmsg)   --这里不检查跟庄了，如果TMD有这种情况出现，我就认命了			
			end
		elseif prevType == g_gdmjAction.type_peng or prevType == g_gdmjAction.type_gang then
			--这里是的需要检查
				local chiPos = 0
				if tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
					--如果是惠东庄，就行看看是不是能吃
					chiPos = GdmjWork.CheckChi(tItem, tItem.m_tInfo.nextinfo.tarchairid, tItem.m_tInfo.nextinfo.actpokerid)
				end
				if chiPos ~= 0 then
					--这里是的需要吃的
					--可以吃了
					--看一看有没有吃啊
					tItem.m_nextInfo.actchairid[chiPos] = 1
					tItem.m_nextInfo.actpokerid = tItem.m_tInfo.nextinfo.actpokerid
					tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
					tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_chi)
					tItem.m_nextInfo.tarchairid = tItem.m_tInfo.nextinfo.tarchairid   --设置目标ID是当前目标				
				else					

					tItem.m_tInfo.beingpoker = 1
					nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
					tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
					tItem.m_nextInfo.canplay = 1
				end			
		else
			local nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
			tItem.m_nextInfo.actchairid[nextPos] = 1
			tItem.m_tInfo.beingpoker = 1
			tItem.m_nextInfo.canplay = 1
		end
	end
	
	return false
end


function HzzmjService.DoPeng(tItem, cgmsg, gcmsg)
	
	GdmjWork.ActionPeng(tItem,cgmsg, gcmsg)
	--碰了后轮到自己出牌了 ，但是不用发牌的
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
	tItem.m_nextInfo.canplay = 1
	
	if #tItem.m_userList[cgmsg.actchairid].handpoker == 5 then
		--这里是的需要报大哥的
		if true == HzzmjHuHelper.BaoDaGeNotice(tItem.m_userList[cgmsg.actchairid]) then
			gcmsg.noticetype = g_gdmjActionNotice.type_baodage
		end
	elseif #tItem.m_userList[cgmsg.actchairid].handpoker == 2 then
		--这里是的需要包大哥的，真的是包的。
		--tItem.m_userList[cgmsg]
		tItem.m_userList[cgmsg.actchairid].prop1 = cgmsg.tarchairid
	end
end
function HzzmjService.DoGang(tItem, cgmsg, gcmsg)
	
	--杠牌，要检查抢杠胡
	--先检查抢杠杠胡
	local qgHuChair = 0
	
	
	if cgmsg.tarchairid == 0 then
		
		local isBuGang = false
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].mjpokerlist) do
			if v.pokerlist[1] == cgmsg.actpokerid and v.pokertype == g_gdmjAction.type_peng then
				isBuGang = true
				break
			end
		end
		
		if isBuGang == false then
			--只有暗杠才抢十三幺
			qgHuChair = HzzmjService.ShiSanYaoQiangAnGang(tItem, cgmsg.actpokerid, cgmsg.actchairid)
		end
		if isBuGang == true and qgHuChair == 0 then
			--只有补杠才抢杠胡
			qgHuChair = HzzmjService.NewCheckChiHu(tItem, cgmsg.actpokerid, cgmsg.actchairid, true)
		end
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
	
		GdmjWork.ActionGang2(tItem,cgmsg,gcmsg)
		tItem.m_tInfo.beingpoker = 1   --杠完后需要发牌的
		tItem.m_nextInfo.actchairid[cgmsg.actchairid] = cgmsg.actchairid
		tItem.m_nextInfo.canplay = 1
		if #tItem.m_userList[cgmsg.actchairid].handpoker == 4 then
			--这里是的需要报大哥的
			if true == HzzmjHuHelper.BaoDaGeNotice(tItem.m_userList[cgmsg.actchairid]) then
				gcmsg.noticetype = g_gdmjActionNotice.type_baodage
			end			
			
		elseif #tItem.m_userList[cgmsg.actchairid].handpoker == 1 and cgmsg.tarchairid ~= 0 then
			--这里是的需要包大哥的。
			--这里是杠的包大哥
			tItem.m_userList[cgmsg.actchairid].prop1 = cgmsg.tarchairid  
		end

	end
	
end

function HzzmjService.DoGenZhuang(tItem, cgmsg, gcmsg)
	
end

function HzzmjService.DoBuHua(tItem, cgmsg, gcmsg)
	--已经有人赢了
	--tItem.m_tInfo.status = g_gdmjStatus.status_counting   --进入结算的时间
	--tItem.m_tInfo.timemark = g_gdmjTime.counting_time
	GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)	
	tItem.m_tInfo.beingpoker = 1   --杠完后需要发牌的
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = cgmsg.actchairid
	--GdmjWork.AddBuHuaPokerID(tItem, cgmsg.actchairid, cgmsg.actpokerid)
	GdmjWork.AddPokerToOutList(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	gcmsg.actiontype:append(cgmsg.actiontype)
	local addItem = gcmsg.tarplayer:add()
	addItem:ParseFromString(tItem.m_userList[cgmsg.actchairid]:SerializeToString())	
end

function HzzmjService.DoWin(tItem, pos, pokerID)
	--已经有人赢了
	tItem.m_tInfo.status = g_gdmjStatus.status_counting   --进入结算的时间
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
end

function HzzmjService.DoChi(tItem, cgmsg, gcmsg)
	
	local pokerID = cgmsg.actpokerid
	local count = 0
	
	local chiPokerID = tItem.m_tInfo.nextinfo.actpokerid
	if pokerID == chiPokerID then
		pokerID = pokerID + 1
		count = count + 1
	end			
	for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
		

		if pokerID == v then
			pokerID = pokerID + 1
			count = count + 1
		end
		if pokerID == chiPokerID then
			pokerID = pokerID + 1
			count = count + 1
		end		
		if count >= 3 then
			break
		end
	end
	
	if count < 3 then
		--这里是有问题的了,不够三个长度，不能吃啊
		LogFile("error","JphService.DoChi actchairid="..cgmsg.actchairid..",actpokerid="..cgmsg.actpokerid)
		return 
	end
	
	local mjPoker = tItem.m_userList[cgmsg.actchairid].mjpokerlist:add()
	mjPoker.pokertype = cgmsg.actiontype	
	gcmsg.showpoker.pokertype = cgmsg.actiontype
	
	--够了三个长度
	pokerID = cgmsg.actpokerid
	for i = 1,3 do
		for k,v in ipairs(tItem.m_userList[cgmsg.actchairid].handpoker) do
			
			if pokerID == chiPokerID then
				mjPoker.pokerlist:append(pokerID)
				gcmsg.showpoker.pokerlist:append(pokerID)
				pokerID = pokerID + 1
				break
			elseif v == pokerID then
				mjPoker.pokerlist:append(pokerID)
				gcmsg.showpoker.pokerlist:append(pokerID)
				pokerID = pokerID + 1
				tItem.m_userList[cgmsg.actchairid].handpoker:remove(k)  --记得要从这删除
				break
			end
		end
	end
	
	local len = #tItem.m_userList[cgmsg.tarchairid].outpoker
	tItem.m_userList[cgmsg.tarchairid].outpoker:remove(len)

	while #tItem.m_userList[cgmsg.actchairid].tinglist > 0 do
		tItem.m_userList[cgmsg.actchairid].tinglist:remove(1)
	end
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
end


function HzzmjService.CheckLiuJu(tItem)
	if #tItem.m_tInfo.publicpoker > 0 then
		return nil
	end
	
	tItem.m_tInfo.status = g_gdmjStatus.status_counting
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
	
	--在惠州庄中。流局的情况下。庄家不变
	--tItem.m_tInfo.bankerpos = tItem.m_tInfo.bankerpos >= tItem.m_maxUser and 1 or tItem.m_tInfo.bankerpos + 1
	tItem.m_tInfo.bankercount = 0
	return true
end

function HzzmjService.PlayCount(tItem)

	local len = #tItem.m_tInfo.winchairlist
	
	if #tItem.m_tInfo.winchairlist == 0 then
		HzzmjService.PlayCountLiuJu(tItem)
	elseif #tItem.m_tInfo.winchairlist == 1 then
		HzzmjService.PlayCountWin(tItem)
	else
		HzzmjService.PlayCountWinMore(tItem)
	end
	
	for i = 1,tItem.m_maxUser do      --设置进入等待的状态
		tItem.m_userList[i].playstate = g_gdmjPlayerState.play_ready
		tItem.m_userModify[i] = 1     --在这里也需要设置
	end

	
	if tItem.m_tInfo.viptable == 2 then
		tItem.m_tInfo.timemark = 1
		tItem.m_tInfo.status = g_gdmjStatus.status_delete		
	else  
		--如果是0，表示本次牌局已经结束了
		tItem.m_tInfo.timemark = g_gdmjTime.end_time
		tItem.m_tInfo.status = g_gdmjStatus.status_end	
	end
	
end


function HzzmjService.PlayCountLiuJu(tItem)
	
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

function HzzmjService.PlayCountDissolve(tItem)
	--申请解散后，发送的结算，在广东麻将推倒胡中，流局不算分
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	gcAccount.result = 0
	
	tItem.m_tInfo.viptable = 2
	gcAccount.counttype = 1
	
	for i = 1,tItem.m_maxUser do
		if tItem.m_tInfo.situser[i] ~= 0 then
			local pCountItem = gcAccount.detaillist:add()
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
			pCountItem.chairid = i
		end
	end

	--玩家手牌
	for i = 1,tItem.m_maxUser do
		if tItem.m_tInfo.situser[i] ~= 0 then
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
	end	
	
	
	GdmjHistoryModel.SetHistory(tItem, 1, gcAccount)
	gcAccount.result = 0


	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	
	tItem.m_tInfo.timemark = 1
	tItem.m_tInfo.status = g_gdmjStatus.status_delete   --这里是的需要删除这个牌桌的，已经发送结算
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
end

function HzzmjService.PlayCountWin(tItem)
	
	--这个是一个人赢的，不存在多个人赢
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	local doubleAccount = 0    --番倍类型 --tItem.m_tInfo.pourjetton
	
	
	local winChairID = tItem.m_tInfo.winchairlist[1]  --因为前面已经确定长度是1了
	local fenList = {0,0,0,0}
	local desList = {{},{},{},{}}
	local gangList = {0,0,0,0}
	local anGangList = {0,0,0,0}   --记录暗杠的个数
	local mingGangList = {0,0,0,0}  --记录明杠的个数
	local buGangList = {0,0,0,0}    --不杠的个数
	local dianGangList = {0,0,0,0}  --点杠的个数（放杠被被人杠）
	local maList = {0,0,0,0}        --这个是计算马跟杠专用的
	
	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来
	
	--推倒胡不能吃胡
	
	local isQiangGangQuanBao = 0
	local isGangBaoQuanBao = 0   --杠爆全包

	local isMenQing = false

	
	if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		--如果是抢杠胡，看看是不是抢杠全包
		table.insert(desList[winChairID],"抢杠胡")
		table.insert(desList[tarChairID],"点炮")
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1
		
	elseif 	tItem.m_tInfo.wintype == g_gdmjAction.type_chihu then
		table.insert(desList[winChairID],"胡")
		table.insert(desList[tarChairID],"点炮")
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1		
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_hu then  --这个是自摸胡
		--这个看看是不是要杠爆全包，杠爆全包就是放杠后，杠上花，放杠者全包。
		table.insert(desList[winChairID],"自摸")   --自摸	
		--稍后在看看要不要统计有鬼和无给自摸

		tItem.m_detailList[winChairID].zimonum = tItem.m_detailList[winChairID].zimonum + 1	
	end	

	local winType = 0
	for k,v in ipairs(tItem.m_userList[winChairID].tinglist) do
		if v == pokerID then
			winType = tItem.m_userList[winChairID].tingtype[k]
			break
		end
	end
	
	local isHuaHu = false  --这是多花胡牌
	if winType == 0 then
		--在这里检查多花胡牌
		local huaNum = 0
		for k,v in ipairs(tItem.m_userList[winChairID].outpoker) do
			if v > 60 then
				huaNum = huaNum + 1
			end
		end
		if huaNum == 7 then
			winType = g_gdmjWinType.type_hzz_jihu
			table.insert(desList[winChairID], "七花胡牌")
			isHuaHu = true
		elseif huaNum == 8 then
			winType = g_gdmjWinType.type_hzz_qingyise
			table.insert(desList[winChairID], "八花胡牌")
			isHuaHu = true
		end
	else
		if tItem.m_tInfo.wintype == g_gdmjAction.type_hu then
			if tItem.m_vipRoomInfo.menqingjiafen == 1 and #tItem.m_userList[winChairID].mjpokerlist == 0 then
				isMenQing = true
				table.insert(desList[winChairID], "门清")
			end
		end
	end
	
	--看看有没有全包的
	local quanBaoChairID = 0
	if winType > g_gdmjWinType.type_hzz_qingyise and tItem.m_tInfo.wintype == g_gdmjAction.type_hu then
		quanBaoChairID = tItem.m_userList[winChairID].prop1
	end
	
	if winType == 0 then
		LogFile("winerror", "winType is 0 ,userid="..winChairID..",pokerID="..pokerID)
	else
		table.insert(desList[winChairID], g_gdmjHzzWinJetton[winType][2])
		if isMenQing == true then
			doubleAccount = g_gdmjHzzWinJetton[winType][3]
		else
			doubleAccount = g_gdmjHzzWinJetton[winType][1]
		end
	end
	
	local maNum = tItem.m_vipRoomInfo.manum

	while maNum > #gcAccount.malist do
		if #tItem.m_tInfo.publicpoker == 0 then
			break
		end
		gcAccount.malist:append(GdmjWork.GetRandPoker(tItem, 0)) --这里是取的码牌的
	end
	if maNum > #gcAccount.malist then
		--说明买马的数量还不够，需要从一个新的牌里面再买剩下的马
		local pokerAllList = {11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53,
	11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53,
	11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53,
	11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53,
	61,62,63,64,71,72,73,74}

		for i = #gcAccount.malist + 1,maNum do  --因为这里是的一定够的
			local numIndex = math.myrandom(1,#pokerAllList)
			gcAccount.malist:append( pokerAllList[numIndex] )
			table.remove(pokerAllList, numIndex)
		end
	end
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

	local huaNum = 0
	for k,v in ipairs(tItem.m_userList[winChairID].outpoker) do
		if v > 60 then
			huaNum = huaNum + 1
		--else
			--break
		end
	end
	if huaNum > 0 and winType > g_gdmjWinType.type_hzz_jihu and isHuaHu == false then
		--鸡胡不能补花
		doubleAccount = doubleAccount + huaNum
		table.insert(desList[winChairID], "花"..huaNum)
	end
	
	for k,v in ipairs(gcAccount.malist) do  --这个index是针对庄家来说的，记住，是庄家啊，是庄家
		local index = math.mod(v,10)
		index = math.mod(index,tItem.m_maxUser)
		index = index == 0 and tItem.m_maxUser or index
		
		maList[ maIndexList[index] ] = maList[ maIndexList[index] ] + 1
	
		
	
		if maIndexList[index] == winChairID then
			gcAccount.magetlist:append(v)
		end
	end

	if maList[winChairID] > 0 then
		table.insert(desList[winChairID],"中马"..maList[winChairID])
		--doubleAccount = doubleAccount*(1 + maList[winChairID])  --每加一个码，就番一番
	end
	tItem.m_detailList[winChairID].zhongmanum = tItem.m_detailList[winChairID].zhongmanum + maList[winChairID]
	--这里开始算杠的分数
	for i = 1,tItem.m_maxUser do
		for k,v in ipairs(tItem.m_userList[i].mjpokerlist) do
			if v.pokertype == g_gdmjAction.type_gang then
				--如果是杠的类型
				if v.typedetail == g_gdmjGangDetail.type_angang then
					local gangJetton = tItem.m_tInfo.pourjetton*2  --暗杠是两倍
					anGangList[i] = anGangList[i] + 1
					for j = 1,tItem.m_maxUser do
						if i == j then
							gangList[j] = gangList[j] + (tItem.m_maxUser - 1)*gangJetton
						else
							gangList[j] = gangList[j]  - gangJetton
						end
					end
					
					if tItem.m_vipRoomInfo.ganggenma == 1 then
						--表示庄家买中了马，其他所有的人都得给这个庄家同样倍数的钱
						
						if maList[i] > 0 then   --买中了马
							local maGangJetton = 2*tItem.m_tInfo.pourjetton*maList[i]  --这个是没家需要赔付的钱
							--中了几个，剩下的人就得陪几个
							for j = 1,tItem.m_maxUser do
								if j ~= i then
									gangList[j] = gangList[j] - maGangJetton
								end
							end
							--这里中的马是给庄家的，因为庄家买中了
							gangList[winChairID] = gangList[winChairID] + (tItem.m_maxUser - 1)*maGangJetton  --这个是给胜利玩家的
						end
						--还有一个是没买中的，没买中的，庄家赔付
						local maGangJetton = 2*tItem.m_tInfo.pourjetton*(maNum - maList[i])  --剩下的就是中了赔付的
						gangList[i] = gangList[i] + maGangJetton
						gangList[winChairID] = gangList[winChairID] - maGangJetton
						--这里是庄家买马的,首先看看买中了没,买中了的给庄家
					end
				elseif v.typedetail == g_gdmjGangDetail.type_bugang then
					local gangJetton = tItem.m_tInfo.pourjetton  
					buGangList[i] = buGangList[i] + 1
					
					for j = 1,tItem.m_maxUser do
						if i == j then
							gangList[j] = gangList[j] + (tItem.m_maxUser - 1)*gangJetton
						else
							gangList[j] = gangList[j]  - gangJetton
						end
					end	
					if tItem.m_vipRoomInfo.ganggenma == 1 then
						--表示庄家买中了马，其他所有的人都得给这个庄家同样倍数的钱
						
						if maList[i] > 0 then   --买中了马
							local maGangJetton = tItem.m_tInfo.pourjetton*maList[i]  --这个是没家需要赔付的钱
							--中了几个，剩下的人就得陪几个
							for j = 1,tItem.m_maxUser do
								if j ~= i then
									gangList[j] = gangList[j] - maGangJetton
								end
							end
							--这里中的马是给庄家的，因为庄家买中了
							gangList[winChairID] = gangList[winChairID] + (tItem.m_maxUser - 1)*maGangJetton  --这个是给胜利玩家的
						end
						--还有一个是没买中的，没买中的，庄家赔付
						local maGangJetton = tItem.m_tInfo.pourjetton*(maNum - maList[i])  --剩下的就是中了赔付的
						gangList[i] = gangList[i] + maGangJetton
						gangList[winChairID] = gangList[winChairID] - maGangJetton
						--这里是庄家买马的,首先看看买中了没,买中了的给庄家
					end
				elseif v.typedetail == g_gdmjGangDetail.type_minggang then
					local gangJetton = tItem.m_tInfo.pourjetton*(tItem.m_maxUser - 1)
					mingGangList[i] = mingGangList[i] + 1
					dianGangList[v.tarchairid] = dianGangList[v.tarchairid] + 1
					gangList[i] = gangList[i] + gangJetton
					gangList[v.tarchairid] = gangList[v.tarchairid] - gangJetton
					if tItem.m_vipRoomInfo.ganggenma == 1 then
						--表示庄家买中了马，其他所有的人都得给这个庄家同样倍数的钱
						if maList[i] > 0 then   --买中了马,这里的赔付需要注意了，是点杠的玩家出的
							local maGangJetton = (tItem.m_maxUser - 1)*tItem.m_tInfo.pourjetton*maList[i]  --这个是没家需要赔付的钱
							--这里中的马是给庄家的，因为庄家买中了
							gangList[winChairID] = gangList[winChairID] + maGangJetton  --这个是给胜利玩家的
							gangList[v.tarchairid] = gangList[v.tarchairid] - maGangJetton
						end
						--还有一个是没买中的，没买中的，庄家赔付
						if maList[v.tarchairid] > 0 then
							--这里悲剧了，庄家需要赔钱了
							local maGangJetton = (tItem.m_maxUser - 1)*tItem.m_tInfo.pourjetton*maList[v.tarchairid]  --这个是没家需要赔付的钱
							--这里中的马是给庄家的，因为庄家买中了
							gangList[winChairID] = gangList[winChairID] - maGangJetton  --这个是给胜利玩家的
							gangList[i] = gangList[i] + maGangJetton							
						end
					end
				end
			end
		end
	end
	
	if tItem.m_tInfo.genzhuang ~= 0 then
		--跟庄
		for i =1,tItem.m_maxUser do
			if i == tItem.m_tInfo.bankerpos then
				fenList[i] = fenList[i] -3*(tItem.m_maxUser - 1)
				table.insert(desList[i],"跟庄")
			else
				fenList[i] = fenList[i] + 3   --鸡胡的分数
			end
		end
	end
	
	
	doubleAccount = doubleAccount*tItem.m_tInfo.pourjetton
	local countJetton = doubleAccount*(1+maList[winChairID])
	
	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist:add()
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = gangList[i]
		item.iswin = 0
		for k2,v2 in ipairs(desList[i]) do
			item.des:append(v2)
		end
		
		if anGangList[i] ~= 0 then
			item.des:append("暗杠"..anGangList[i])
		end
		if buGangList[i] ~= 0 then
			item.des:append("明杠"..buGangList[i])
		end
		if mingGangList[i] ~= 0 then
			item.des:append("杠"..mingGangList[i])
		end
		if dianGangList[i] ~= 0 then
			item.des:append("点杠"..dianGangList[i])
		end
		
		if i == winChairID then
			item.iswin = 1
			gcAccount.winid:append(item.userid)
			gcAccount.winchairid:append(i)
			gcAccount.wintypelist:append(tItem.m_tInfo.wintype)
			
			if tItem.m_tInfo.wintype == g_gdmjAction.type_hu then
				item.hunum = (tItem.m_maxUser - 1)*countJetton
				if quanBaoChairID ~= 0 then
					item.hunum = item.hunum + (maNum - maList[quanBaoChairID] - maList[i])*doubleAccount
				end
			elseif tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
				item.hunum = item.hunum + (tItem.m_maxUser - 1)*countJetton   --这是包赔三家的
				--这里还需要算上买马的分
				--这里有几分就赔几份
				item.hunum = item.hunum + (maNum - maList[i] - maList[tarChairID])*doubleAccount  --其他的每人陪一倍

			else
				item.hunum = countJetton
			end
			
		else
			if tItem.m_tInfo.wintype == g_gdmjAction.type_hu then
				if quanBaoChairID == 0 then
					item.hunum = -countJetton
				elseif quanBaoChairID == i then
					--这里就是全包了
					item.hunum = -(tItem.m_maxUser - 1)*countJetton	
					item.hunum = item.hunum - (maNum - maList[winChairID] - maList[i])*doubleAccount				
				end
			elseif tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
				--抢杠胡，通赔三家
				if  tarChairID == i then
					--抢杠胡好惨的
					item.hunum = -(tItem.m_maxUser - 1)*countJetton
					item.hunum = item.hunum - (maNum - maList[winChairID] - maList[i])*doubleAccount  --其他的每人陪一倍
					
				end			
			else
				if  tarChairID == i then
					item.hunum = -countJetton
				end
			end
		end
		item.hunum = item.hunum + fenList[i]   --
		item.winjetton = item.gangjetton + item.hunum
		tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
		tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
		
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
		
		for k,v in ipairs(tItem.m_userList[i].outpoker) do
			if v > 60 then
				local addItem = addPoker.mjpokerlist:add()
				addItem.pokertype = g_gdmjAction.type_buhua
				addItem.pokerlist:append(v)
			end
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
		
		gcAccount.countlist[i].carryjetton = tItem.m_userList[i].carryjetton
	end
	
	
	
	GdmjHistoryModel.SetHistory(tItem, 0, gcAccount)
	gcAccount.result = 0
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())

	--tItem.m_tInfo.status = g_gdmjStatus.status_end
	--print("tItem.m_tInfo.bankercount="..tItem.m_tInfo.bankercount)
	if tItem.m_tInfo.bankerpos == winChairID then
		--连庄的设置,这里首先要判断节节高
		tItem.m_tInfo.bankercount = tItem.m_tInfo.bankercount + 1
	else
		tItem.m_tInfo.bankercount = 0
	end
	if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		tItem.m_tInfo.bankerpos = tarChairID
	else
		tItem.m_tInfo.bankerpos = quanBaoChairID == 0 and winChairID or quanBaoChairID
	end
	GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
	--tItem.m_tInfo.timemark = g_gdmjTime.end_time
end

function HzzmjService.PlayCountWinMore(tItem)
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	local doubleList = {}    --番倍类型 --tItem.m_tInfo.pourjetton
	
	local winList = {}  
	local desList = {}
	for k,v in ipairs(tItem.m_tInfo.winchairlist) do
		table.insert(winList,v)
		doubleList[k] = 0
		desList[k] = {}
	end
	
	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来
	
	
	--推倒胡不能吃胡
	local isQiangGangQuanBao = 0
	if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		for k,v in ipairs(winList) do
			doubleList[k] = doubleList[k] + 2    --抢杠胡算两番
			table.insert(desList[k],"抢杠胡")
			tItem.m_detailList[v].hunum = tItem.m_detailList[v].hunum + 1
		end
		--这里还需要看看是否是抢杠全包,如果是抢杠全包
		isQiangGangQuanBao = GdmjWork.IsQiangGangQuanBao(tItem)
	else
		
	end
	
	
	--抢杠胡不能摸码
	--所以在这里直接统计最后赢的钱
	
	if isQiangGangQuanBao == 1 then
		--如果是抢杠全包，被抢杠者就悲剧了
		--赢分的得到赢分，被抢杠的一个全输了，这里先计算赢的分
		
		

		
		local loseHuNum = 0
		for k1,v1 in ipairs(winList) do
			local countJetton = (tItem.m_maxUser - 1)*doubleList[k1]*tItem.m_tInfo.pourjetton
			loseHuNum = loseHuNum - countJetton
		end
		
		local countJetton = 0
		for i = 1,tItem.m_maxUser do
			local item = gcAccount.countlist:add()
			item.userid = tItem.m_userList[i].userid
			item.gangjetton = tItem.m_userList[i].carryjetton - tItem.m_userList[i].psinfo.jetton
			item.iswin = 0	
			
			
			
			if i == tarChairID then
				countJetton = loseHuNum
			else
				for k1,v1 in ipairs(winList) do
					if i == v1 then
						countJetton = (tItem.m_maxUser - 1)*doubleList[k1]*tItem.m_tInfo.pourjetton
						item.iswin = 1
						gcAccount.winid:append(item.userid)
						gcAccount.winchairid:append(i)
						gcAccount.wintypelist:append(tItem.m_tInfo.wintype)
						for k2,v2 in ipairs(desList[k1]) do
							item.des:append(v2)
						end
						break
					end
				end				
			end
			
			item.winjetton = item.gangjetton + countJetton
			item.hunum = countJetton
			tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
			recoreItem.score:append(item.winjetton)
			tItem.m_tInfo.viprecord.score[i] = tItem.m_userList[i].carryjetton
			gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
			tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + item.winjetton
			item.carryjetton = tItem.m_userList[i].carryjetton
		end		

	else
		--如果没有抢杠全包的，就复杂了
		for i = 1,tItem.m_maxUser do
			local item = gcAccount.countlist:add()
			item.userid = tItem.m_userList[i].userid
			item.gangjetton = tItem.m_userList[i].carryjetton - tItem.m_userList[i].psinfo.jetton  --先把杠的分算出来
			item.iswin = 0
		end
		
		for k1,v1 in ipairs(winList) do
			
			for i = 1,tItem.m_maxUser do
				local countJetton = doubleList[k1]*tItem.m_tInfo.pourjetton
				
				if v1 == i then
					--这个是赢的人
					gcAccount.countlist[i].iswin = 1
					gcAccount.countlist[i].hunum = (tItem.m_maxUser - 1)*countJetton
					gcAccount.winid:append(tItem.m_userList[i].userid)
					gcAccount.winchairid:append(i)
					gcAccount.wintypelist:append(tItem.m_tInfo.wintype)	
					
				else
					gcAccount.countlist[i].hunum = 0 - countJetton
				end
			end
		end
		
		for i = 1, tItem.m_maxUser do
			
			gcAccount.countlist[i].winjetton = gcAccount.countlist[i].gangjetton + gcAccount.countlist[i].hunum
			tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + gcAccount.countlist[i].winjetton
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
			recoreItem.score:append(gcAccount.countlist[i].winjetton)
			tItem.m_tInfo.viprecord.score[i] = tItem.m_userList[i].carryjetton
			gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
			
			tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + gcAccount.countlist[i].winjetton
			gcAccount.countlist[i].carryjetton = tItem.m_userList[i].carryjetton
		end
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
	
	GdmjHistoryModel.SetHistory(tItem, 0, gcAccount)
	gcAccount.result = 0
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())


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


function HzzmjService.NewCheckChiHu(tItem,actPokerID,actChairID,isQiangGang)

	while #tItem.m_tInfo.chihulist > 0 do
		tItem.m_tInfo.chihulist:remove(1)
	end

	local userIDList = GdmjWork.GetCheckList(actChairID, tItem.m_maxUser) --默认是四个玩家
	--先把需要检查的玩家找出来
	
	local huChairList = {}
	
	for i = 1,#userIDList do
		--在这里必须要判断鸡胡，鸡胡是不允许吃胡的
		for k,v in ipairs(tItem.m_userList[ userIDList[i] ].tinglist) do
			if tItem.m_userList[ userIDList[i] ].guohu == 0 and v == actPokerID then
				--同时必须要满足不是鸡胡才可以,过胡不胡
				--抢杠胡是可以鸡胡的
				--这里再加一个，惠东庄也是允许吃鸡胡的,但是惠州庄，是不允许吃鸡胡的,所以最后一个是判断惠州庄吃胡的
				if isQiangGang == true or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz or tItem.m_userList[ userIDList[i] ].tingtype[k] ~= g_gdmjWinType.type_hzz_jihu then
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
		tItem.m_userList[huChairList[1][2]].guohu = 1
		return huChairList[1][2]
	end
	
	return 0
	
end

function HzzmjService.NewCheckChiHu2(tItem)
	if #tItem.m_tInfo.chihulist > 0 then
		local chairID = tItem.m_tInfo.chihulist[1]
		tItem.m_userList[chairID].guohu = chairID
		return chairID
	end
	return 0
end

function HzzmjService.GetChiHuList(tItem,actPokerID,actChairID)
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


function HzzmjService.ShiSanYaoQiangAnGang(tItem,actPokerID,actChairID,startPos)
	local userIDList = GdmjWork.GetCheckList(actChairID, tItem.m_maxUser) --默认是四个玩家
	--先把需要检查的玩家找出来
	local beginPos = 5
	if startPos ~= nil then
		startPos = startPos == tItem.m_maxUser and 1 or startPos + 1
		for i = 1, #userIDList do
			if startPos == userIDList[i] then
				beginPos = i
				break
			end
		end
	else
		beginPos = 1
	end
	for i = beginPos,#userIDList do

		--在这里必须要判断鸡胡，鸡胡是不允许吃胡的
		for k,v in ipairs(tItem.m_userList[ userIDList[i] ].tinglist) do
			if tItem.m_userList[ userIDList[i] ].guohu == 0 and v == actPokerID and tItem.m_userList[ userIDList[i] ].tingtype[k] == g_gdmjWinType.type_hzz_shisanyao then
				--必须是真正的十三幺才可以
				local handList = GdmjWork.GetHandList(tItem.m_userList[ userIDList[i] ])
				table.insert(handList, actPokerID)
				if true == HzzmjHuHelper.IsShiSanYao(handList) then
					tItem.m_userList[userIDList[i]].guohu = 1   --过胡不胡
					return userIDList[i]
				end
			end
		end

	end
	return 0	
end

function HzzmjService.CheckGenZhuang(tItem, cgmsg)
	
	if tItem.m_tInfo.viptable == 0 then
		return nil
	end
	
	if tItem.m_vipRoomInfo.genzhuang == 0 then
		--如果本来就不知道跟庄的
		return nil
	end
	
	--取到庄家的上家检查跟庄
	local checkPos = tItem.m_tInfo.bankerpos == 1 and 4 or (tItem.m_tInfo.bankerpos - 1)
	if cgmsg.actchairid ~= checkPos then
		return nil
	end

	for i = 1,tItem.m_maxUser do
		if #tItem.m_userList[i].handpoker ~= 13 then
			--在这里不能够检查mjpokerlist 的长度
			return nil
		end
		--if #tItem.m_userList[i].
		if #tItem.m_userList[i].mjpokerlist > 0  then
			--除了满足上面四个条件，还要满足这个条件
			return nil
		end
		
		local maxLen = #tItem.m_userList[i].outpoker
		if maxLen == 0 then
			return nil
		end
		if tItem.m_userList[i].outpoker[maxLen] ~= cgmsg.actpokerid then
			return nil
		end
		local realLen = 0
		for k = maxLen,1,-1 do
			if tItem.m_userList[i].outpoker[k] > 0 and tItem.m_userList[i].outpoker[k] < 60 then
				realLen = realLen + 1
			end
			if realLen > 1 then
				return nil
			end
		end
	end
	tItem.m_tInfo.genzhuang = 1
end
