
--金币场，玩法是推倒胡的玩法


JbcService = {}


function JbcService.DoWork(tItem) --传入的是TdhStruct类型的Item

	if tItem.m_tInfo.status == g_gdmjStatus.status_ready then
		--牌桌处于等待的状态，如果人满了就开始
		JbcService.TableReady(tItem)
		
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_waiting then
		
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		
		if tItem.m_tInfo.timemark <= 0 then
			tItem.m_tInfo.status = g_gdmjStatus.status_playing
		end
		tItem.m_isModify = true
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_playing then
	
		JbcService.TablePlay(tItem,v)
		
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_counting then
		tItem.m_isModify = true
		tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		if tItem.m_tInfo.timemark < 0 then
			JbcService.PlayCount(tItem)
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
			
		end
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_dissolve then
		
		if tItem.m_tInfo.timemark > 0 then
			tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		end
		
		if tItem.m_tInfo.timemark <= 0 then
			--这里开始下发数据
			JbcService.PlayCountDissolve(tItem)
		end
	elseif tItem.m_tInfo.status == g_gdmjStatus.status_delete then
		--这里是的需要清空删除的
		table.insert(GdmjService.dismissList, tItem.m_tInfo.tableid)
		
		--
		--在这里是不能发离开协议的，不管是打牌结束还是中途申请解散的，都必须在各自的地方做发送离开的协议
		--
	end
	
end

function JbcService.TableReady(tItem)  --牌桌处于等待状态的函数
	local isReady = true
	local function AutoCheck(tItem)
		for k,v in ipairs(tItem.m_tInfo.situser) do
			
			if v ~= 0 then
				if true == RobotService.IsRobot(v) then
					
					
					if tItem.m_userList[k].psinfo.jetton < g_gdmjNeedJetton[ tItem.m_tInfo.tabletype ] then
						JbcService.RobotLeave(tItem,k)
					else
						if true == JbcService.RobotAutoLeave(tItem, k) then
						end
					end
					
				end
			end
			
		end
	end
	if tItem.m_tInfo.playernum < tItem.m_maxUser then
		--小于三个人的情况下，看看需不需要加入机器人
		--tItem.m_tInfo.timemark = tItem.m_tInfo.timemark - 1
		--if tItem.m_tInfo.timemark < 0 then
		--tItem.m_tInfo.timemark = g_gdmjTime.autostart_time
			--这里表示是的进入自动开始时间了
		--end
		JbcService.RobotAutoEnter(tItem)
		AutoCheck(tItem)
		isReady = false
	else   --如果是已经满四个人了
	
		AutoCheck(tItem)
		for i = 1,#tItem.m_tInfo.situser do
			--如果全部人都是准备的状态，说明可以开始了			
			if tItem.m_tInfo.situser[i] ~= 0 then
				if tItem.m_userList[i].playstate == g_gdmjPlayerState.play_ready then
					if true == RobotService.IsRobot(tItem.m_userList[i].userid) then
						JbcService.RototReady(tItem, i)
					end
					if tItem.m_userList[i].playstate == g_gdmjPlayerState.play_ready then
						isReady = false
					end
					
				end
			else
				isReady = false
			end
		end
		
		
		
	end
	
	if isReady == true then
		--说明已经准备好了
		
		tItem.m_isModify = true
		
		for i = 1,tItem.m_maxUser do
			tItem.m_userModify[i] = 1
		end
		
		JbcService.GameStart(tItem)
	end
	GdmjJinBiChang.ReadyCheck(tItem)
end

function JbcService.InitPublicPoker(tItem)

	while #tItem.m_tInfo.winchairlist > 0 do
		tItem.m_tInfo.winchairlist:remove(1)
	end
	while #tItem.m_tInfo.guilist > 0 do
		tItem.m_tInfo.guilist:remove(1)
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
	
	GdmjWork.InitPubGuiPai(tItem)
	--在这里设置鬼牌
	--在这里还需要确定鬼牌的长度
end

function JbcService.StartTest(tItem)

	local function InsertTest( tItem, pos, pokerID)
		for k, v in ipairs(tItem.m_tInfo.publicpoker) do
			if v == pokerID then
				tItem.m_tInfo.publicpoker:remove(k)
				break
			end
		end
		tItem.m_userList[pos].handpoker:append(pokerID)
	end
	InsertTest(tItem, 1, 36)
	InsertTest(tItem, 1, 36)
	InsertTest(tItem, 1, 14)
	InsertTest(tItem, 1, 14)
	InsertTest(tItem, 1, 14)
	InsertTest(tItem, 1, 15)
	InsertTest(tItem, 1, 16)
	InsertTest(tItem, 1, 21)
	InsertTest(tItem, 1, 23)
	InsertTest(tItem, 1, 23)
	InsertTest(tItem, 1, 24)
	InsertTest(tItem, 1, 27)
	InsertTest(tItem, 1, 27)


--[[	InsertTest(tItem, 2, 21)
	InsertTest(tItem, 2, 21)
	InsertTest(tItem, 2, 21)
	InsertTest(tItem, 2, 22)
	InsertTest(tItem, 2, 22)
	InsertTest(tItem, 2, 22)
	InsertTest(tItem, 2, 38)
	InsertTest(tItem, 2, 38)
	InsertTest(tItem, 2, 38)
	InsertTest(tItem, 2, 39)
	InsertTest(tItem, 2, 39)
	InsertTest(tItem, 2, 23)
	InsertTest(tItem, 2, 24)

	InsertTest(tItem, 3, 26)
	InsertTest(tItem, 3, 26)
	InsertTest(tItem, 3, 26)
	InsertTest(tItem, 3, 27)
	InsertTest(tItem, 3, 27)
	InsertTest(tItem, 3, 27)
	InsertTest(tItem, 3, 28)
	InsertTest(tItem, 3, 28)
	InsertTest(tItem, 3, 28)
	InsertTest(tItem, 3, 29)
	InsertTest(tItem, 3, 29)
	InsertTest(tItem, 3, 23)
	InsertTest(tItem, 3, 24)

	InsertTest(tItem, 4, 31)
	InsertTest(tItem, 4, 31)
	InsertTest(tItem, 4, 31)
	InsertTest(tItem, 4, 32)
	InsertTest(tItem, 4, 32)
	InsertTest(tItem, 4, 32)
	InsertTest(tItem, 4, 33)
	InsertTest(tItem, 4, 33)
	InsertTest(tItem, 4, 33)
	InsertTest(tItem, 4, 25)
	InsertTest(tItem, 4, 25)
	InsertTest(tItem, 4, 25)
	InsertTest(tItem, 4, 23)--]]

end

function JbcService.GameStart(tItem)

	--初始化公共牌
	JbcService.InitPublicPoker(tItem)
	GdmjWork.VipPay(tItem.m_tInfo)
	
	--GdmjWork.SendStartInfo(tItem)
	for i = 1,tItem.m_maxUser do
		GdmjWork.UserStartInit(tItem, tItem.m_userList[i])   --初始化
	end
	
	if g_isDebug == 1 then
		--JbcService.StartTest(tItem)
		for i = 1,13 do
			GdmjWork.HandPokerInsert(tItem, 1)
			GdmjWork.HandPokerInsert(tItem, 2)
			GdmjWork.HandPokerInsert(tItem, 3)
			GdmjWork.HandPokerInsert(tItem, 4)
		end
		
	else
	
		for i = 1,13 do
			GdmjWork.HandPokerInsert(tItem, 1)
			GdmjWork.HandPokerInsert(tItem, 2)
			GdmjWork.HandPokerInsert(tItem, 3)
			GdmjWork.HandPokerInsert(tItem, 4)
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
	--tItem.m_tInfo.viprecord.score:append(0)
	--tItem.m_tInfo.viprecord.score:append(0)
	--tItem.m_tInfo.viprecord.score:append(0)
	--tItem.m_tInfo.viprecord.score:append(0)
	GdmjWork.SendStartInfo(tItem)
end

function JbcService.TablePlay(tItem,currPos)
	
	if tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		tItem.m_isModify = true
		JbcService.CheckMoving(tItem)
	else
		local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
		for k,currPos in ipairs(currPosList) do

			tItem.m_isModify = true
			if   RobotService.IsRobot(tItem.m_tInfo.situser[currPos])  --[[tItem.m_userList[currPos].isrobot == 1--]] then
				--如果是机器人
				tItem.m_userList[currPos].robottime = tItem.m_userList[currPos].robottime - 1
				if tItem.m_userList[currPos].robottime < 0 then
					
					GdmjJinBiChang.AutoAction(tItem,currPos)
				end
				tItem.m_userModify[currPos] = 1
			else
				if tItem.m_userList[currPos].autoplayer == 1 then
					GdmjJinBiChang.AutoAction(tItem,currPos)
				else
					if tItem.m_tInfo.actiontime > 0 then
						tItem.m_tInfo.actiontime = tItem.m_tInfo.actiontime - 1
					else
						tItem.m_userList[currPos].autoplayer = 1
						tItem.m_userModify[currPos] = 1
					end
				end
			end
		end
	end
	

end

function JbcService.CheckMoving(tItem)
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
	
	if #currPosList == 0 then
		LogFile("error", "JbcService.CheckMoving actchairid is nil")
		return nil
	end
	local currPos = currPosList[1] --取第一个就行了。
	local gcUpdate = msg_gdmj_pb.gcgdmjupdate()
	tItem.m_tInfo.nextinfo.canplay = 0
	if tItem.m_tInfo.beingpoker == 1 then
		--这个是需要发牌的
		
		local pokerID = GdmjWork.HandPokerInsert(tItem, currPos)
		
		local checkHu = GdmjWork.CheckIsHu(tItem, currPos, pokerID) --为什么是humark呢
		local checkGang = GdmjWork.CheckIsGang(tItem.m_userList[currPos], pokerID)
		tItem.m_tInfo.nextinfo.actpokerid = pokerID
		tItem.m_tInfo.nextinfo.actchairid[currPos] = 1
		tItem.m_tInfo.nextinfo.tarchairid = 0   --设置

		
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
		end
		
	end
	
	for i = 1,tItem.m_maxUser do
		gcUpdate.actchairid:append(0)
	end
	
	for k,v in ipairs(currPosList) do  --这里是为了一炮多响的，记住，一炮多响，抢杠胡的时候
		tItem.m_userList[v].playstate = g_gdmjPlayerState.play_action  --设置对应的状态
		tItem.m_userList[v].robottime = math.myrandom(2,5)
		if g_isDebug ~= 0 then
			tItem.m_userList[v].robottime = math.myrandom(1,2)
		end
		gcUpdate.actchairid[v] = 1
		tItem.m_userModify[v] = 1
	end
	
	gcUpdate.lastpokerlen = #tItem.m_tInfo.publicpoker
	gcUpdate.actiontime = g_gdmjTime.play_robot_time  --先定为5吧
	gcUpdate.canplay = tItem.m_tInfo.nextinfo.canplay
	gcUpdate.result = 0
	gcUpdate.tarchairid = tItem.m_tInfo.nextinfo.tarchairid  
	tItem.m_tInfo.userstate = g_gdmjUserState.state_playing  --轮到玩家开始打牌了
	tItem.m_tInfo.actiontime = g_gdmjTime.play_robot_time
	GdmjWork.SendUpdate(tItem,gcUpdate, currPos)
	
	
	
	tItem.m_isModify = true
	for k,v in ipairs(currPosList) do
		tItem.m_userModify[v] = 1
	end
	
	
end

function JbcService.AutoAction(tItem, currPos)
	--跟之前一样，取到自动打的牌型
	--
	local cgmsg = msg_gdmj_pb.cggdmjaction()
	local gcmsg = msg_gdmj_pb.gcgdmjaction()
	
	cgmsg.userid = tItem.m_userList[currPos].userid
	cgmsg.tableid = tItem.m_tInfo.tableid

	cgmsg.actchairid = currPos
	cgmsg.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
	
	
	
	cgmsg.actiontype,cgmsg.actpokerid = GdmjRobot.Play(tItem,currPos,tItem.m_tInfo.nextinfo.actpokerid)
	
	JbcService.PlayingAction(tItem,cgmsg, gcmsg)
	
	if tItem.m_tInfo.status == g_gdmjStatus.status_playing and tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		--只有玩家的装修是在等待的时候，才是检查更新的时候
		--只有这时才checkmoving
		GdmjService.CheckMoving(tItem)
	end	
end

function JbcService.PlayingAction(tItem,cgmsg, gcmsg)
		
	local isWin = false
	
	local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)

	GdmjWork.NextInfoInit(tItem.m_nextInfo)
	tItem.m_tInfo.beingpoker = 0   --置为不能发牌
	if cgmsg.actiontype == g_gdmjAction.type_play then
		JbcService.DoPlay(tItem,cgmsg,gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
	elseif cgmsg.actiontype == g_gdmjAction.type_guo then
		isWin = JbcService.DoGuo(tItem, cgmsg, gcmsg)  --这里过的时候，必须要在这里返回
	elseif cgmsg.actiontype == g_gdmjAction.type_chi then
		
	elseif cgmsg.actiontype == g_gdmjAction.type_peng then
		JbcService.DoPeng(tItem, cgmsg, gcmsg)
		GdmjWork.CheckHuList(tItem, cgmsg.actchairid)   --检查听牌列表
	elseif cgmsg.actiontype == g_gdmjAction.type_gang then
		JbcService.DoGang(tItem, cgmsg, gcmsg)
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
	elseif cgmsg.actiontype == g_gdmjAction.type_chihu then
		
	elseif cgmsg.actiontype == g_gdmjAction.type_genzhuang then
	
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
		JbcService.DoWin(tItem, cgmsg.actchairid, cgmsg.actchairid)
		isEnd = true
	else
		tItem.m_userList[cgmsg.actchairid].playstate = g_gdmjPlayerState.play_waiting
		if tItem.m_tInfo.beingpoker == 1 then
			--只有发牌的情况下会去检查流局
			isEnd = JbcService.CheckLiuJu(tItem)
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


function JbcService.DoPlay(tItem, cgmsg, gcmsg)
	--这个是打牌的操作
	--在打牌的操作中，只有一个人的行动，不会存在多个人的
	--local chiHuList = 
	--推倒胡中，不能够吃胡，所以这里不用检查吃胡
	
	--首先，在手牌中去掉
	
	tItem.m_tInfo.prevpos = 0  --首先，把这个设置为0，为什么要把这个设置为0呢，这个是杠爆全包的,只要是出牌了，这个就不算了
	
	GdmjWork.DelPokerFromHand(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	GdmjWork.AddPokerToOutList(tItem.m_userList[cgmsg.actchairid],cgmsg.actpokerid)
	local nextPos,num = GdmjWork.CheckPengGang(tItem,cgmsg.actpokerid, cgmsg.actchairid)
	
	if nextPos ~= 0 then
		--表示是可以碰和或者杠的
		tItem.m_nextInfo.actchairid[nextPos] = 1
		tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
		
		tItem.m_nextInfo.tarchairid = cgmsg.actchairid   --设置目标ID是当前目标
		
		if num ==  2 then
			--表示是碰的
			tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_peng)  --如果等于2，只有碰的情况
		else
			tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_peng)
			tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_gang)
					
			tItem.m_tInfo.prevpos = cgmsg.actchairid   --这里是记录放杠者的位置
		end
		
		
	else
		--如果没有碰或者杠，表示轮到下一个人了
		tItem.m_tInfo.beingpoker = 1
		nextPos = cgmsg.actchairid == tItem.m_maxUser and 1 or cgmsg.actchairid + 1
		tItem.m_nextInfo.actchairid[nextPos] = 1   --轮到他行动了
	end
	GdmjWork.GenZhuangCheck(tItem,cgmsg)   --检查跟庄
end

function JbcService.DoGuo(tItem,cgmsg,gcmsg)
	--这里是过牌的操作，过牌的操作，可能存在几个人同时操作，必须等到最后的人操作完成了，才发送行动的类型
	--如果是抢杠胡，而且是同时抢杠胡，或者同时吃胡
	--
	tItem.m_tInfo.prevpos = 0   --在这里也要对这个设置
	if tItem.m_tInfo.nextinfo.tarchairid == 0 then
		--如果目标ID是0，表示是自摸的杠牌的过，或者是胡牌的过，这里的牌权依然在自己手上，只是
		--是出牌，现在这种情况已经忽略了。
		tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
	else

		local prevType = 0   --上一个玩家的出牌类型
		
		for k,v in ipairs(tItem.m_tInfo.nextinfo.actiontype) do
			if v == g_gdmjAction.type_qiangganghu then
				--广东麻将只有抢杠胡才会出现胡
				prevType = v
			end
		end
		
		if prevType == g_gdmjAction.type_qiangganghu then
			--就是TMD的抢杠胡也不胡
			--这里开始胡的操作
			--为了实现胡的操作，这里需要构造出cgmsg
			
			local currPosList = GdmjWork.GetCurrPosList(tItem.m_tInfo)
			
			if #currPosList == 1 then
				
				if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
					--如果赢牌类型有变化，表示已经结束了
					return true
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
			else

			end
						
		else
			local nextPos = tItem.m_tInfo.nextinfo.tarchairid == tItem.m_maxUser and 1 or tItem.m_tInfo.nextinfo.tarchairid + 1
			tItem.m_nextInfo.actchairid[nextPos] = 1
			tItem.m_tInfo.beingpoker = 1	
		end
	end
	
	return false
end


function JbcService.DoPeng(tItem, cgmsg, gcmsg)
	
	GdmjWork.ActionPeng(tItem,cgmsg, gcmsg)
	--碰了后轮到自己出牌了 ，但是不用发牌的
	
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
end
function JbcService.DoGang(tItem, cgmsg, gcmsg)
	
	--杠牌，要检查抢杠胡
	--先检查抢杠杠胡
	local qghList = GdmjWork.CheckQiangGangHu(tItem, cgmsg)
	
	if #qghList > 0 then
		--如果抢杠胡的列表大于0
		--
		--开始吹抢杠胡的流程
		--先处理一个的
		
		for k,v in ipairs(qghList) do
			tItem.m_nextInfo.actchairid[v] = 1
		end
		tItem.m_nextInfo.tarchairid = cgmsg.actchairid
		tItem.m_tInfo.beingpoker = 0
		tItem.m_nextInfo.actpokerid = cgmsg.actpokerid
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_guo)
		tItem.m_nextInfo.actiontype:append(g_gdmjAction.type_qiangganghu)

	else
		--如果没有抢杠胡，表示开始杠牌了
	
		GdmjWork.ActionGang(tItem,cgmsg,gcmsg)
		tItem.m_tInfo.beingpoker = 1   --杠完后需要发牌的
		tItem.m_nextInfo.actchairid[cgmsg.actchairid] = cgmsg.actchairid

	end
	
end
function JbcService.DoHu(tItem, cgmsg, gcmsg)
	--胡牌
end

function JbcService.DoQiangGangHu(tItem, cgmsg, gcmsg)
	
end

function JbcService.DoChiHu(tItem, cgmsg, gcmsg)
	
end

function JbcService.DoGenZhuang(tItem, cgmsg, gcmsg)
	
end

function JbcService.DoWin(tItem, pos, pokerID)
	--已经有人赢了
	tItem.m_tInfo.status = g_gdmjStatus.status_counting   --进入结算的时间
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
end

function JbcService.CheckLiuJu(tItem)
	if #tItem.m_tInfo.publicpoker > 0 then
		return nil
	end
	
	tItem.m_tInfo.status = g_gdmjStatus.status_counting
	tItem.m_tInfo.timemark = g_gdmjTime.counting_time
	tItem.m_tInfo.bankerpos = tItem.m_tInfo.bankerpos >= tItem.m_maxUser and 1 or tItem.m_tInfo.bankerpos + 1
	tItem.m_tInfo.bankercount = 0
	return true
end

function JbcService.PlayCount(tItem)

	local len = #tItem.m_tInfo.winchairlist
	
	if #tItem.m_tInfo.winchairlist == 0 then
		JbcService.PlayCountLiuJu(tItem)
	elseif #tItem.m_tInfo.winchairlist == 1 then
		JbcService.PlayCountWin(tItem)
	else
		JbcService.PlayCountWinMore(tItem)
	end
	
	for i = 1,tItem.m_maxUser do      --设置进入等待的状态
		tItem.m_userList[i].playstate = g_gdmjPlayerState.play_ready
		tItem.m_userModify[i] = 1     --在这里也需要设置
	end
	

	tItem.m_tInfo.timemark = g_gdmjTime.end_time
	tItem.m_tInfo.status = g_gdmjStatus.status_end		

	
end

function JbcService.PlayCountLiuJu(tItem)
	
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()

	--玩家手牌
	for i = 1,tItem.m_maxUser do

		local item = gcAccount.countlist:add()
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = 0
		item.iswin = 0
		item.winjetton = 0

		--gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
		
	end	
	
	--玩家手牌
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
			tItem.m_userList[i].carryjetton = 0
			tItem.m_userList[i].psinfo.jetton = 0
		end
		
	end

	gcAccount.result = 0
	
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	
	--tItem.m_tInfo.timemark = 1
	--tItem.m_tInfo.status = g_gdmjStatus.status_end
	
end

function JbcService.PlayCountDissolve(tItem)
	--申请解散后，发送的结算，在广东麻将推倒胡中，流局不算分
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	
	gcAccount.result = 0
	
	tItem.m_tInfo.viptable = 2
	gcAccount.counttype = 1
	
	for i = 1,tItem.m_maxUser do
		local pCountItem = gcAccount.detaillist:add()
		pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
		pCountItem.chairid = i		
		
		tItem.m_userList[i].carryjetton = 0
		tItem.m_userList[i].psinfo.jetton = 0
	end

	--玩家手牌
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

	gcAccount.result = 0


	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())
	
	tItem.m_tInfo.timemark = 1
	tItem.m_tInfo.status = g_gdmjStatus.status_delete   --这里是的需要删除这个牌桌的，已经发送结算
end

function JbcService.PlayCountWin(tItem)
	
	--这个是一个人赢的，不存在多个人赢
	
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	

	
	local doubleAccount = 0    --番倍类型 --tItem.m_tInfo.pourjetton
	
	local winChairID = tItem.m_tInfo.winchairlist[1]  --因为前面已经确定长度是1了
	
	local desList = {}

	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来
	
	--推倒胡不能吃胡
	
	local isQiangGangQuanBao = 0
	local isGangBaoQuanBao = 0   --杠爆全包
	
	if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		--如果是抢杠胡，看看是不是抢杠全包
		doubleAccount = doubleAccount + 2
		table.insert(desList,"抢杠胡")
		
		--isQiangGangQuanBao = GdmjWork.IsQiangGangQuanBao(tItem)
		
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1
		
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_hu then  --这个是自摸胡
		--这个看看是不是要杠爆全包，杠爆全包就是放杠后，杠上花，放杠者全包。
		table.insert(desList,"自摸")   --自摸
		doubleAccount = doubleAccount + 2
		if tItem.m_tInfo.prevpos ~= 0 then
			--表示是杠爆的，在检查是否选择了杠包全包
			--isGangBaoQuanBao = GdmjWork.IsGangBaoQuanBao(tItem)
			--if isGangBaoQuanBao == 1 then
			--	tarChairID = tItem.m_tInfo.prevpos  --这个是认定的ID
			--	table.insert(desList,"杠爆全包")   --自摸
			--end
		end
		
		local guiPubLen,guiPriLen = GdmjWork.HuGuiCheck(tItem, winChairID)
		
		if guiPubLen ~= 0 then
			if guiPriLen ~= 0 then
				--有鬼自摸
				tItem.m_detailList[winChairID].youguizimo = tItem.m_detailList[winChairID].youguizimo + 1
			else
				--有公共鬼牌的情况下，但是手上没有鬼牌，而且又符合无鬼加倍的
				tItem.m_detailList[winChairID].wuguizimo = tItem.m_detailList[winChairID].wuguizimo + 1				
			end
		else
			tItem.m_detailList[winChairID].wuguizimo = tItem.m_detailList[winChairID].wuguizimo + 1
		end
		
		
		if tItem.m_tInfo.bankercount > 0 then
			table.insert(desList,"节节高+"..tItem.m_tInfo.bankercount)   --自摸
			doubleAccount = doubleAccount + tItem.m_tInfo.bankercount*2
		end

		
		local maNum = tItem.m_tInfo.manum

		while maNum > 0 do
			if #tItem.m_tInfo.publicpoker == 0 then
				break
			end
			
			gcAccount.malist:append(GdmjWork.GetRandPoker(tItem, 0))
			maNum = maNum - 1
		end
		
		local maIndex = 0
		if winChairID >= tItem.m_tInfo.bankerpos then
			maIndex = winChairID - tItem.m_tInfo.bankerpos + 1
		else
			maIndex = 4 - tItem.m_tInfo.bankerpos + winChairID + 1
		end

		local getMaNum = 0
		for k1,v1 in ipairs(g_gdmjWinMa[maIndex]) do
			
			for k2,v2 in ipairs(gcAccount.malist) do
				local isMaGet = false
				if v2 > 40 then   
					if v1 == v2 then
						isMaGet = true
					end
				else
					if v1 == math.mod(v2,10) then  --一定要注意这里
						isMaGet = true
					end
				end
				
				if isMaGet == true then
					getMaNum = getMaNum + 1
					doubleAccount = doubleAccount + 2  --每加一个码，就番一番
					gcAccount.magetlist:append(v2)
				end
			end
			
		end
		
		if getMaNum > 0 then
			table.insert(desList,"中码+"..getMaNum)   --自摸
		end
		tItem.m_detailList[winChairID].zhongmanum = tItem.m_detailList[winChairID].zhongmanum + getMaNum
		
		--稍后在看看要不要统计有鬼和无给自摸
	end
	
	local countJetton = doubleAccount*tItem.m_tInfo.pourjetton
	
	for i = 1,tItem.m_maxUser do
		
		local item = gcAccount.countlist:add()
		
		item.userid = tItem.m_userList[i].userid
		item.gangjetton = tItem.m_userList[i].carryjetton - tItem.m_userList[i].psinfo.jetton
		item.iswin = 0
		
		if i == winChairID then
			item.iswin = 1
			gcAccount.winid:append(item.userid)
			gcAccount.winchairid:append(i)
			gcAccount.wintypelist:append(tItem.m_tInfo.wintype)
			
			for k2,v2 in ipairs(desList) do
				item.des:append(v2)
			end
			
			item.hunum = (tItem.m_maxUser - 1)*countJetton
		else
			
			if isQiangGangQuanBao == 1 or isGangBaoQuanBao == 1 then
				--如果是抢杠全包
				if i == tarChairID then
					--如果是输的人
					item.hunum = -(tItem.m_maxUser - 1)*countJetton
				end
			else
				item.hunum = -countJetton
			end
			
		end
		item.winjetton = item.gangjetton + item.hunum
		
		if (item.winjetton + tItem.m_userList[i].psinfo.jetton) < 0 then
			--这里需要做出判断
			item.winjetton = -tItem.m_userList[i].psinfo.jetton
		end
		
		tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
			
		tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton
		
		local pInfo = PlayerModel.GetPlayerInfo(tItem.m_userList[i].userid)
		
		PlayerModel.AddJetton(pInfo, item.winjetton, "gdmj", "结算")

		--gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
		tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + item.winjetton
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendJetton(pInfo)
	end

	if tItem.m_tInfo.viptable ~= 0 and tItem.m_tInfo.usevipnum == tItem.m_tInfo.maxvipnum then
		--打完八局或者16局了
		tItem.m_tInfo.viptable = 2
		gcAccount.counttype = 1
		
		for i = 1,tItem.m_maxUser do
			local pCountItem = gcAccount.detaillist:add()
			pCountItem:ParseFromString(tItem.m_detailList[i]:SerializeToString())
			pCountItem.chairid = i
			
			
			tItem.m_userList[i].carryjetton = 0
			tItem.m_userList[i].psinfo.jetton = 0
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

	gcAccount.result = 0
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	SendMessage(userIDList, PacketCode[2222].client, gcAccount:ByteSize(), gcAccount:SerializeToString())

	
		
	--tItem.m_tInfo.status = g_gdmjStatus.status_end

	if tItem.m_tInfo.bankerpos == winChairID then
		--连庄的设置,这里首先要判断节节高
		tItem.m_tInfo.bankercount = tItem.m_tInfo.bankercount + 1
	else
		tItem.m_tInfo.bankercount = 0
	end	
	
	tItem.m_tInfo.bankerpos = winChairID
	--tItem.m_tInfo.timemark = g_gdmjTime.end_time
end

function JbcService.PlayCountWinMore(tItem)
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()

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
			
			if (item.winjetton + tItem.m_userList[i].psinfo.jetton) < 0 then
				item.winjetton = -tItem.m_userList[i].psinfo.jetton
			end
			
			tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + item.winjetton
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton

			--gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
			tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + item.winjetton
			
			local pInfo = PlayerModel.GetPlayerInfo(tItem.m_userList[i].userid)
			PlayerModel.AddJetton(pInfo, item.winjetton, "gdmj", "结算")
			
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendJetton(pInfo)
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
			

			if (gcAccount.countlist[i].winjetton + tItem.m_userList[i].psinfo.jetton) < 0 then
				gcAccount.countlist[i].winjetton = -tItem.m_userList[i].psinfo.jetton
			end

			tItem.m_userList[i].psinfo.jetton = tItem.m_userList[i].psinfo.jetton + gcAccount.countlist[i].winjetton
			tItem.m_userList[i].carryjetton = tItem.m_userList[i].psinfo.jetton

			--tItem.m_tInfo.viprecord.score[i] = tItem.m_userList[i].carryjetton
			--gcAccount.viprecord.score:append(tItem.m_userList[i].carryjetton)
			
			tItem.m_detailList[i].winnum = tItem.m_detailList[i].winnum + gcAccount.countlist[i].winjetton
			
			local pInfo = PlayerModel.GetPlayerInfo(tItem.m_userList[i].userid)
			PlayerModel.AddJetton(pInfo, gcAccount.countlist[i].winjetton, "gdmj", "结算")
			PlayerModel.SetPlayerInfo(pInfo)
			PlayerModel.SendJetton(pInfo)
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


function JbcService.RobotAutoEnter(tItem)
	--在结算的时候，是自动调配真人玩家最好的时候。
	if tItem.m_tInfo.viptable ~= 0 then
		return nil
	end
	local random = math.myrandom(1,100)
	if random < 40 then  --有20%的概率分派不到机器人
		return nil
	end
	
	local emptyList = {}
	local userNum = 0
	
	for k,v in ipairs(tItem.m_tInfo.situser) do
		if v == 0 then
			--如果是空座位
			table.insert(emptyList, k)
		elseif RobotService.IsRobot(v) == false then
			--如果不是机器人
			userNum = userNum + 1
		end
		
	end
	
	if userNum == 0 then
		--如果全部都是机器人，则让机器人全部离开，不能够让机器人在里面等
		for k,v in ipairs(tItem.m_tInfo.situser) do
			if v ~= 0 then
				JbcService.RobotLeave(tItem, k)
			end
		end
		tItem.m_tInfo.status = g_gdmjStatus.status_delete
	else
		
		if #emptyList > 0 then
			
			local randNum = math.myrandom(1, #emptyList)
			local pos = emptyList[randNum]
			JbcService.RobotAdd(tItem, pos)
		end
		
	end
end

function JbcService.RobotLeave(tItem, pos)
	
	if tItem.m_tInfo.status ~= g_gdmjStatus.status_ready then
		return nil
	end
	
	if pos < 1 or pos > 4 then
		return nil
	end
	
	local userID = tItem.m_tInfo.situser[pos]
	
	if userID ~= 0 then
		
		tItem.m_tInfo.playernum = tItem.m_tInfo.playernum - 1
		tItem.m_tInfo.situser[pos] = 0
		GdmjModel.DelUserTableID(userID) 
		RobotService.GdmjReleaseRobot(userID)
		
	end
	local userList =  GdmjWork.GetUserList(tItem.m_tInfo)
	local gcStandUp = msg_gdmj_pb.gcgdmjstandup()
	gcStandUp.chairid = pos
	gcStandUp.tableid = ctItem.m_tInfo.tableid
	SendMessage(userList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
	--GdmjModel.SetTableInfo(tItem.m_tInfo, 1)
	tItem.m_isModify = true
	tItem.m_detailList[pos] = 0
	tItem.m_userList[pos] = 0
	tItem.m_userModify[pos] = 0
end

function JbcService.RobotAdd(tItem, pos)
	
	if pos == nil or pos == 0 then
		for k,v in ipairs(tItem.m_tInfo.situser) do
			if v == 0 then
				pos = v
			end
		end
	end
	
	if pos == nil or pos == 0 then
		return nil
	end
	
	if tItem.m_tInfo.situser[pos] ~= 0 then
		return nil
	end
	
	local robotItem = RobotService.GdmjGetRobot(tItem.m_tInfo.tabletype)
	
	
	if robotItem == nil then
		return nil
	end
	
	local gcSitDown = msg_gdmj_pb.gcgdmjsitdown()
	
	local pInfo = PlayerModel.GetPlayerInfo(robotItem.m_userID)
	
	if pInfo == nil then
		LogFile("error", "robotID="..robotItem.m_userID.." is not exist")
		return nil
	end
	
	PlayerModel.ChangeSimpleInfo(pInfo, gcSitDown.mjuser.psinfo)
	gcSitDown.mjuser.userid = pInfo.userid
	GdmjModel.SitDown(tItem.m_tInfo, gcSitDown.mjuser, pos)  --玩家坐下来了
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo)
	gcSitDown.result = 0
	
	SendMessage(userIDList,PacketCode[2216].client,gcSitDown:ByteSize(),gcSitDown:SerializeToString())
	
	--GdmjModel.SetTableInfo(tItem.m_tInfo, 1)
	tItem.m_isModify = true
	local addUser = GdmjModel.GetChairUser(tItem.m_tInfo.tableid,pInfo.userid)
	tItem.m_userList[pos] = addUser
	local detailInfo = GdmjModel.GetPlayDetail(tItem.m_tInfo.tableid, pInfo.userid)
	tItem.m_detailList[pos] = detailInfo
	
	
	local gcReady = msg_gdmj_pb.gcgdmjready()
	gcReady.mjuser:ParseFromString(gcSitDown.mjuser:SerializeToString())
	gcReady.result = 0
	SendMessage(userIDList, PacketCode[2214].client, gcReady:ByteSize(), gcReady:SerializeToString())
	tItem.m_isModify = true	
	GdmjModel.SetJBCTableID(tItem.m_tInfo.mjtype, tItem.m_tInfo.tabletype, tItem.m_tInfo.tableid, tItem.m_tInfo.playernum)
end

function JbcService.RototReady(tItem, pos)
	
	local randNum = math.myrandom(1,10000)
	
	if randNum < 5000 then
		return nil
	end
	
	tItem.m_userList[pos].playstate = g_gdmjPlayerState.play_waiting
	local gcReady = msg_gdmj_pb.gcgdmjready()
	--gcReady.mjuser:ParseFromString(tItem.m_userList[i]:SerializeToString())
	GdmjModel.SetChairUser(tItem.m_tInfo.tableid, tItem.m_userList[pos])
	GdmjModel.GetChairUser(tItem.m_tInfo.tableid, tItem.m_tInfo.situser[pos], gcReady.mjuser)
	gcReady.mjuser.playstate = g_gdmjPlayerState.play_waiting
	gcReady.result = 0
	local userIDList = GdmjWork.GetUserList(tItem.m_tInfo, tItem.m_userList[pos].userid)
	SendMessage(userIDList, PacketCode[2214].client, gcReady:ByteSize(), gcReady:SerializeToString())
	tItem.m_isModify = true
end


function JbcService.RobotAutoLeave(tItem, pos)
	
	local randNum = math.myrandom(1,10000)
	if true then
		return nil
	end
	
	JbcService.RobotLeave(tItem, pos)
	return true
end