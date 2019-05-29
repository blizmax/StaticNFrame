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
			GdmjWork.CheckJiFen(tItem)   --在这里检查积分
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
	

	--print
	--if tItem.m_tInfo.straction ~= '' then
	--	gcmsg.ParseFromString(tItem.m_tInfo.straction)
	--end
	
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


function RpmjService.DoPlay(tItem, cgmsg, gcmsg)
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

function RpmjService.DoGuo(tItem,cgmsg,gcmsg)
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


function RpmjService.DoPeng(tItem, cgmsg, gcmsg)
	
	GdmjWork.ActionPeng(tItem,cgmsg, gcmsg)
	--碰了后轮到自己出牌了 ，但是不用发牌的
	
	tItem.m_nextInfo.actchairid[cgmsg.actchairid] = 1
end
function RpmjService.DoGang(tItem, cgmsg, gcmsg)
	
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

function RpmjService.CheckLiuJu(tItem)
	if #tItem.m_tInfo.publicpoker > 0 then
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

function RpmjService.PlayCountWin(tItem)
	
	--这个是一个人赢的，不存在多个人赢
	
	local gcAccount = msg_gdmj_pb.gcgdmjaccount()
	
	local recoreItem = tItem.m_tInfo.recordlist:add()
	recoreItem.roundid = tItem.m_tInfo.usevipnum
	local doubleAccount = 0    --番倍类型 --tItem.m_tInfo.pourjetton
	
	local winChairID = tItem.m_tInfo.winchairlist[1]  --因为前面已经确定长度是1了
	local getMaNum = 0
	local otherMaNum = 0;
	local maList = {0,0,0,0}
	local desList = {}
	local fenList = {0,0,0,0}
	local tarChairID = tItem.m_tInfo.nextinfo.tarchairid   --这里还不会被清空,暂时先用这个来
	local pokerID = tItem.m_tInfo.nextinfo.actpokerid   --这里还不会被清空,暂时先用这个来
	
	--推倒胡不能吃胡
	
	if tItem.m_tInfo.wintype == g_gdmjAction.type_qiangganghu then
		--允许抢杠胡
		--如果是抢杠胡，看看是不是抢杠全包
		doubleAccount = doubleAccount + 2
		table.insert(desList,"抢杠胡")
		
		tItem.m_detailList[winChairID].hunum = tItem.m_detailList[winChairID].hunum + 1
		
	elseif tItem.m_tInfo.wintype == g_gdmjAction.type_hu then  --这个是自摸胡
		--这个看看是不是要杠爆全包，杠爆全包就是放杠后，杠上花，放杠者全包。
		table.insert(desList,"自摸")   --自摸
		doubleAccount = doubleAccount + 2
		if tItem.m_tInfo.prevpos ~= 0 then

		end

		--这里指的是豪华7对
		if tItem.m_vipRoomInfo.kehuqidui == 1 then
			--如果是七对加番的，就必须检查胡的是否是七对了
			if 0 ~= GdmjHuHelper.IsQiDui(tItem, winChairID) then
				table.insert(desList,"七对")   --自摸
				doubleAccount = 4
			end
		end

		if tItem.m_vipRoomInfo.qiduisibei == 1 then
			--如果是七对加番的，就必须检查胡的是否是七对了
			if 0 ~= GdmjHuHelper.IsQiDui(tItem, winChairID) then
				table.insert(desList,"豪华七对")   --自摸
				doubleAccount = 8
			end
		end
		
		if tItem.m_vipRoomInfo.qingyises == 1 then
			if 0 ~= GdmjHuHelper.IsQingYiSe(tItem,winChairID) then
				table.insert(desList, "清一色")
				doubleAccount = 4
			end				
		end

		if tItem.m_vipRoomInfo.shisanyao == 1 then
			if 0 ~= GdmjHuHelper.IsShiSanYao(tItem,winChairID) then
				table.insert(desList, "十三幺")
				doubleAccount = 16
			end				
		end
		
		if 0 ~= GdmjHuHelper.WuhzJiafen(tItem,winChairID) then
			tItem.m_detailList[winChairID].wuguizimo = tItem.m_detailList[winChairID].wuguizimo + 1	
		else
			tItem.m_detailList[winChairID].youguizimo = tItem.m_detailList[winChairID].youguizimo + 1	
		end		

		
		local maNum = tItem.m_vipRoomInfo.manum

		while maNum > 0 do
			if #tItem.m_tInfo.publicpoker == 0 then
				break
			end			
			if maNum == 99 then
				local getMaPoker = GdmjWork.GetRandPoker(tItem, 0)
				gcAccount.malist:append(getMaPoker)
				getMaNum = math.mod( getMaPoker, 10 )   --这时候中码的数量，就是码的面值
				break
			else	

				
				gcAccount.malist:append(GdmjWork.GetRandPoker(tItem, 0))
				maNum = maNum - 1
			end
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
				if maIndexList[index] == winChairID then
					gcAccount.magetlist:append(v)
					getMaNum = getMaNum + 1
				end				
			end

			for i = 1,tItem.m_maxUser do
				tItem.m_detailList[i].zhongmanum = tItem.m_detailList[i].zhongmanum + maList[i]
				if i ~= winChairID then
					otherMaNum = otherMaNum + maList[i]
				end
			end
		end
		if getMaNum > 0 then
			table.insert(desList,"中码+"..getMaNum)   --自摸
		end
		--tItem.m_detailList[winChairID].zhongmanum = tItem.m_detailList[winChairID].zhongmanum + getMaNum
		
		--稍后在看看要不要统计有鬼和无给自摸
	end

	doubleAccount = doubleAccount + getMaNum*2
	if tItem.m_detailList[winChairID].angangnum > 0 then
		doubleAccount = doubleAccount + tItem.m_detailList[winChairID].angangnum * 2
	end

	if tItem.m_detailList[winChairID].minggangnum > 0 then
		doubleAccount = doubleAccount + tItem.m_detailList[winChairID].minggangnum
	end

	local countJetton = doubleAccount*tItem.m_vipRoomInfo.difen
	--countJetton = countJetton*(1+getMaNum)   --有几个马就番几倍

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
			
			item.hunum = (tItem.m_maxUser - 1 + otherMaNum)*countJetton

			--减去暗杠的时候当初已经扣掉的
			if tItem.m_detailList[winChairID].angangnum > 0 then
				item.hunum = item.hunum - tItem.m_detailList[winChairID].angangnum * 2 * (tItem.m_maxUser - 1)
			end

			if tItem.m_detailList[winChairID].minggangnum > 0 then
				item.hunum = item.hunum - tItem.m_detailList[winChairID].minggangnum * (tItem.m_maxUser - 1)
			end
		else
			item.hunum = -countJetton*(1+maList[i])
			
			if tItem.m_detailList[winChairID].angangnum > 0 then
				item.hunum = item.hunum - countJetton*tItem.m_detailList[winChairID].angangnum * 2 + tItem.m_detailList[winChairID].angangnum * 2
			end
			
			if tItem.m_detailList[winChairID].minggangnum > 0 then
				--如果明杠不包杠的话，就全部扣一分
				if tItem.m_vipRoomInfo.gangbaoquanbao == 0 then
					item.hunum = item.hunum - countJetton*tItem.m_detailList[winChairID].minggangnum + tItem.m_detailList[winChairID].minggangnum
				else
					for kk, vv in ipairs(tItem.m_userList[winChairID].mjpokerlist) do
						if vv.pokertype == g_gdmjAction.type_gang and vv.typedetail == g_gdmjGangDetail.type_minggang then
							if i == vv.tarchairid then
								item.hunum = item.hunum - countJetton * (tItem.m_maxUser - 1) + (tItem.m_maxUser - 1)
							end
						end
					end
				end
			end
		end
		item.hunum = item.hunum + fenList[i]   --跟庄的也要顶上  跟庄的不包,
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
	
	--GdmjHistoryModel.SetTableAction(tItem.m_tInfo.frameid, PacketCode[2222].client, gcAccount:SerializeToString())
	
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
	
	tItem.m_tInfo.bankerpos = winChairID
	--tItem.m_tInfo.timemark = g_gdmjTime.end_time
end

function RpmjService.PlayCountWinMore(tItem)
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

