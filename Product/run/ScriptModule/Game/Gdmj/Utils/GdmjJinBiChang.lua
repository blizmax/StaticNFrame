
GdmjJinBiChang = {}


function GdmjJinBiChang.Enter(pInfo,tInfo, mjType, tableType, tableID)
	
	if tInfo.playernum >= 4 then
		return false
	end
	--传入的是pInfo，tInfo, mjType：麻将类型,tableType:牌桌类型
	if tableType < g_gdmjTableType["junior"] or tableType > g_gdmjTableType["serior"] then
		--超过了，就默认选
		if pInfo.jetton < 20000 then
			tableType = g_gdmjTableType["junior"]
		elseif pInfo.jetton < 100000 then
			tableType = g_gdmjTableType["medium"]
		else
			tableType = g_gdmjTableType["serior"]
		end
	end
	if pInfo.jetton < 20000 then    --小于2W的，只能在初级场
		if tableType > g_gdmjTableType["junior"]  then
			tableType = g_gdmjTableType["junior"]
		end
	elseif pInfo.jetton < 100000 then   --大于2W，小于10W的，可以在中级场和初级场
		if tableType == g_gdmjTableType["serior"]  then
			tableType = g_gdmjTableType["junior"]
		end
	end	
	
	ThreadManager.GdmjLock(mjType..tableType)
	local IsNew = false
	if tableID == nil or tonumber(tableID) == 0 then
		tableID,IsNew = GdmjModel.GetJBCTableID(mjType, tableType)
	end
	
	if IsNew == false then
		--这个是进入一个旧的牌桌
		ThreadManager.GdmjLock(tableID)
		local gcSitDown = msg_gdmj_pb.gcgdmjsitdown()   --这个协议是发送给其他玩家的
		PlayerModel.ChangeSimpleInfo(pInfo, gcSitDown.mjuser.psinfo)
		gcSitDown.mjuser.userid = pInfo.userid			
		if tInfo.tableid == 0 then
			--有可能是的不用取了
			if nil == GdmjModel.GetTableInfo(tableID,tInfo) then
				
				return false
			end
		end
		
		--这里是坐下的协议,对于其他人来说
		--对于其他人就是坐下，对于自己，就是进入
		local emptyList = {}
		local userIDList = {}
		for i = 1,#tInfo.situser do
			if tInfo.situser[i] == 0 then
				table.insert(emptyList, i)
			else
				table.insert(userIDList, tInfo.situser[i])
			end
		end
		--随机取一个空值
		local indexNum = math.myrandom(1,#emptyList)
		
		
		GdmjModel.SitDown(tInfo, gcSitDown.mjuser, emptyList[indexNum], g_gdmjStatus.status_waiting)
		gcSitDown.result = 0
		SendMessage(userIDList,PacketCode[2216].client,gcSitDown:ByteSize(),gcSitDown:SerializeToString())

		
		GdmjModel.SetTableInfo(tInfo, 1)
		ThreadManager.GdmjUnLock(tableID)
	else
		--这是进入要给新的牌桌
		tInfo.mjtype = mjType
		tInfo.tableid = tableID
		tInfo.maxvipnum =  4 
		tInfo.pourjetton = g_gdmjPourList[tableType] 
		tInfo.viptable = 0    --设置成vip桌子
		--tInfo.tableuserid = pInfo.userid
		tInfo.status = g_gdmjStatus.status_ready 
		
		tInfo.fengid = g_gdmjFengQuan[1]  --初始化设定为东风东局
		tInfo.timemark = g_gdmjTime.over_time + g_gdmjTime.ready_time   --这里为射门么要相加呢。懒得写另外一个了
		tInfo.ownerid = pInfo.userid
		tInfo.tabletype = tableType
		for i = 1,4 do
			tInfo.situser:append(0)    --广东麻将有四个人的，把四个人的顺序都初始化出来
			tInfo.nextinfo.actchairid:append(0)   --初始化四个玩家
			tInfo.viprecord.score:append(0)
		end
		
		local mjUser = st_gdmj_pb.gdmjuser()
		
		PlayerModel.ChangeSimpleInfo(pInfo, mjUser.psinfo)
		mjUser.userid = pInfo.userid
		local indexNum = math.myrandom(1,4)
		
		GdmjModel.SitDown(tInfo, mjUser, indexNum, g_gdmjStatus.status_waiting)  --这里已经坐下来了
		GdmjModel.SetTableInfo(tInfo,1)
	end
	
	
	GdmjModel.SetUserTableID(pInfo.userid, tableID)    --自己肯定进入到那个房间了
	GdmjModel.SetJBCTableID(mjType, tableType, tableID, tInfo.playernum)  --在这里设置参数	
	ThreadManager.GdmjUnLock(mjType..tableType)
	return true
end


function GdmjJinBiChang.EnterNoneLock(pInfo,tInfo, mjType, tableType, tableID)
	
	if tInfo.playernum >= 4 then
		return false
	end
	--传入的是pInfo，tInfo, mjType：麻将类型,tableType:牌桌类型
	if tableType < g_gdmjTableType["junior"] or tableType > g_gdmjTableType["serior"] then
		--超过了，就默认选
		if pInfo.jetton < 20000 then
			tableType = g_gdmjTableType["junior"]
		elseif pInfo.jetton < 100000 then
			tableType = g_gdmjTableType["medium"]
		else
			tableType = g_gdmjTableType["serior"]
		end
	end
	if pInfo.jetton < 20000 then    --小于2W的，只能在初级场
		if tableType > g_gdmjTableType["junior"]  then
			tableType = g_gdmjTableType["junior"]
		end
	elseif pInfo.jetton < 100000 then   --大于2W，小于10W的，可以在中级场和初级场
		if tableType == g_gdmjTableType["serior"]  then
			tableType = g_gdmjTableType["junior"]
		end
	end	
	local IsNew = false
	if tableID == 0 or tableID == nil then
		tableID,IsNew = GdmjModel.GetJBCTableID(mjType,tableType)
	end
	
	if IsNew == false then
		--这个是进入一个旧的牌桌
		ThreadManager.GdmjLock(tableID)
		local gcSitDown = msg_gdmj_pb.gcgdmjsitdown()
		PlayerModel.ChangeSimpleInfo(pInfo, gcSitDown.mjuser.psinfo)
		gcSitDown.mjuser.userid = pInfo.userid			
		if tInfo.tableid == 0 then
			--有可能是的不用取了
			if nil == GdmjModel.GetTableInfo(tableID, tInfo) then
				
				return false
			end
		end
		
		--这里是坐下的协议,对于其他人来说
		--对于其他人就是坐下，对于自己，就是进入
		local emptyList = {}
		local userIDList = {}
		for i = 1,#tInfo.situser do
			if tInfo.situser[i] == 0 then
				table.insert(emptyList, i)
			else
				table.insert(userIDList, tInfo.situser[i])
			end
		end
		--随机取一个空值
		local indexNum = math.myrandom(1,#emptyList)
		
		GdmjModel.SitDown(tInfo, gcSitDown.mjuser, emptyList[indexNum], g_gdmjStatus.status_waiting)
		gcSitDown.result = 0
		SendMessage(userIDList,PacketCode[2216].client,gcSitDown:ByteSize(),gcSitDown:SerializeToString())

		GdmjModel.SetTableInfo(tInfo,1)
		ThreadManager.GdmjUnLock(tableID)
		
	else
		--这是进入要给新的牌桌
		tInfo.mjtype = mjType
		tInfo.tableid = tableID
		tInfo.maxvipnum =  4 
		tInfo.pourjetton = g_gdmjPourList[tableType] 
		tInfo.viptable = 0    --设置成vip桌子
		--tInfo.tableuserid = pInfo.userid
		tInfo.status = g_gdmjStatus.status_ready
		
		tInfo.fengid = g_gdmjFengQuan[1]  --初始化设定为东风东局
		tInfo.timemark = g_gdmjTime.over_time + g_gdmjTime.ready_time   --这里为射门么要相加呢。懒得写另外一个了
		tInfo.ownerid = pInfo.userid
		tInfo.tabletype = tableType
		for i = 1,4 do
			tInfo.situser:append(0)    --广东麻将有四个人的，把四个人的顺序都初始化出来
			tInfo.nextinfo.actchairid:append(0)   --初始化四个玩家
			tInfo.viprecord.score:append(0)
		end
		
		local mjUser = st_gdmj_pb.gdmjuser()
		PlayerModel.ChangeSimpleInfo(pInfo, mjUser.psinfo)
		mjUser.userid = pInfo.userid
		local indexNum = math.myrandom(1,4)
		GdmjModel.SitDown(tInfo, mjUser, indexNum, g_gdmjStatus.status_waiting)  --这里已经坐下来了
		GdmjModel.SetTableInfo(tInfo,1)
	end
	
	
	GdmjModel.SetUserTableID(pInfo.userid, tableID)    --自己肯定进入到那个房间了
	GdmjModel.SetJBCTableID(mjType, tableType, tableID, tInfo.playernum)  --在这里设置参数	
	return true
end


function GdmjJinBiChang.CouZhuo()
	local mjTypeList = {311}  --几个房间的类型
	local tableTypeList = {1,2,3}      --几个级别
	
	for i = 1,#mjTypeList do
		for j = 1,#tableTypeList do
			--
			ThreadManager.GdmjCLock(mjTypeList[i]..tableTypeList[j])  --金币场的加锁	
			
			local tableList = GdmjModel.GetJBCTableList(mjTypeList[i], tableTypeList[j])
			
			local tableArrList = {{},{},{}}    --另外再排序
			
			for k,v in pairs(tableList) do
				if tonumber(v) > 0 and tonumber(v) < 4 then
					table.insert(tableArrList[tonumber(v)], k)
				end
			end
			
			--开始补充
			--补充的原则是,优先补足给3个人的，再补给2个人的。
			
			--优先补充
			
			for k = 1,100 do
				--这里是为了防止死循环
				--这里是补充三个人桌子的
				if #tableArrList[3] == 0 then
					break
				end
				--开始补充
				local isBuChong = false
				local tPlayerNum = 0
				local sPlayerNum = 0
				if #tableArrList[1] > 0 then
					--local i
					isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[3][1], tableArrList[1][1], mjTypeList[i], tableTypeList[j])
					if isBuChong == true then
						--补充了一个了
						table.remove(tableArrList[3],1)
						table.remove(tableArrList[1],1)
					end
				end
				
				if isBuChong == false and #tableArrList[2] > 0 then
					isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[3][1], tableArrList[2][1], mjTypeList[i], tableTypeList[j])
					if isBuChong == true then
						table.remove(tableArrList[3],1)
						table.remove(tableArrList[2],1)  --把2列表中的移除掉
					end					
				end
				
				if isBuChong == false and #tableArrList[3] > 1 then
					--如果都没有补充到，就把两个三桌的人补充
					isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[3][1], tableArrList[3][2], mjTypeList[i], tableTypeList[j])
					
					table.remove(tableArrList[3],1) --这里需要移除两个了
					table.remove(tableArrList[3],1) --这里需要移除两个了
				end
				if isBuChong == false then
					--如果都没有补充掉，就break
					break
				end
			end
			
			--下面开始补充两个人的
			for k = 1,100 do
				if #tableArrList[2] == 0 then
					break
				end
				local isBuChong = false
				local tPlayerNum = 0
				local sPlayerNum = 0				
				if #tableArrList[1] > 0 then
					
					--local i
					isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[2][1], tableArrList[1][1], mjTypeList[i], tableTypeList[j])
					table.remove(tableArrList[1],1)
					table.remove(tableArrList[2],1)
				end				
				
				if isBuChong == false and #tableArrList[2] > 1 then
					--超过2个以上
					isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[2][1], tableArrList[2][2], mjTypeList[i], tableTypeList[j])
					table.remove(tableArrList[2],1)
					table.remove(tableArrList[2],1)
				end
				if isBuChong == false then
					break
				end
			end
			
			for i = 1,100 do
				if #tableArrList[1] <= 1 then
					break
				end
				local isBuChong = false
				local tPlayerNum = 0
				local sPlayerNum = 0
				
				isBuChong,tPlayerNum,sPlayerNum = GdmjJinBiChang.StartCouZhuo(tableArrList[1][1], tableArrList[1][2], mjTypeList[i], tableTypeList[j])
				table.remove(tableArrList[1],1)				
				table.remove(tableArrList[1],1)
				if isBuChong == false then
					break
				end
			end
			
			
			ThreadManager.GdmjUnLock(mjTypeList[i]..tableTypeList[j])
		end
	end
end


function GdmjJinBiChang.StartCouZhuo(targetID, sourceID, mjType, tableType)
	--开始凑桌,targetID:被加人桌的ID， sourceID:被减人的ID
	
	local gcEnter = msg_gdmj_pb.gcgdmjenter()
	
	GdmjModel.GetTableInfo(targetID,gcEnter.mjinfo)
	local sourceInfo = GdmjModel.GetTableInfo(sourceID)
	if gcEnter.mjinfo.tableid == 0 or sourceInfo == nil then
		return false
	end
	
	if gcEnter.mjinfo.playernum >= 4 then
		return false
	end
	--开始补人了
	local isEnter = false
	for i = gcEnter.mjinfo.playernum,4 do
		--需要补充几个人
		local isEnd = false
		for j = 1, 4 do
			if sourceInfo.situser[j] ~= 0 then
				local pInfo = PlayerModel.GetPlayerInfo(sourceInfo.situser[j])
				
				if true == GdmjJinBiChang.EnterNoneLock(pInfo,gcEnter.mjinfo, mjType, tableType, targetID) then					
					for i = 1,#gcEnter.mjinfo.situser do
						if gcEnter.mjinfo.situser[i] ~= 0 then
							local mjUser = gcEnter.userlist:add()
							
							GdmjModel.GetChairUser(gcEnter.mjinfo.tableid, gcEnter.mjinfo.situser[i], mjUser)
						end
					end
					gcEnter.result = 0
					local gcStandUp = msg_gdmj_pb.gcgdmjstandup()
					gcStandUp.chairid = j
					gcSitDown.tableid = sourceInfo.tableid
					local userIDList = GdmjWork.GetUserList(sourceInfo, pInfo.userid)
					
					SendMessage(userIDList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
					
					SendMessage(pInfo.userid, PacketCode[2202].client, gcEnter:ByteSize(), gcEnter:SerializeToString())
					
					
					GdmjModel.DelChairUserByUserID(sourceInfo.tableid, sourceInfo.situser[j])
					sourceInfo.situser[j] = 0
					sourceInfo.playernum = sourceInfo.playernum - 1
					
					isEnter = true
				else
					isEnd = true
				end
				break
			end
			if isEnd == true then
				break
			end
		end
	end
	if isEnter == false then
		return false
	end
	sourceInfo.tableid = tonumber(sourceID)
	
	
	GdmjModel.SetTableInfo(sourceInfo,1)
	
	GdmjModel.SetJBCTableID(mjType, tableType, gcEnter.mjinfo.tableid, gcEnter.mjinfo.playernum)  --在这里设置参数
	GdmjModel.SetJBCTableID(mjType, tableType, sourceInfo.tableid, sourceInfo.playernum)  --在这里设置参数
	return true,gcEnter.mjinfo.playernum,sourceInfo.playernum
end

function GdmjJinBiChang.GetTableType(pourJetton)
	for i =1,#g_gdmjPourList do
		if pourJetton == g_gdmjPourList[i] then
			return i
		end
	end
	return 1
end

function GdmjJinBiChang.ReadyCheck(tItem)
	--检查玩家的准备状态
	--print("tabletype="..tItem.m_tInfo.tabletype)
	if tItem.m_tInfo.viptable ~= 0 then
		return 
	end
	for i = 1,4 do
		if tItem.m_userList[i] ~= nil and tItem.m_userList[i] ~= 0 and tItem.m_userList[i].userid ~= 0 and tItem.m_userList[i].playstate == g_gdmjPlayerState.play_ready then
			tItem.m_userList[i].robottime = tItem.m_userList[i].robottime - 1
			
			if tItem.m_userList[i].robottime < -30 then
				--给30秒的准备时间，如果没准备，就剔除
				tItem.m_tInfo.playernum = tItem.m_tInfo.playernum - 1
				
				ThreadManager.GdmjLock(tItem.m_tInfo.mjtype..tItem.m_tInfo.tabletype)  --金币场的加锁	
				--这里一定要加锁F
				
				GdmjModel.DelChairUserByUserID(tItem.m_tInfo.tableid,tItem.m_userList[i].userid)    --删除在当前牌桌的信息
				GdmjModel.SetJBCTableID(tItem.m_tInfo.mjtype, tItem.m_tInfo.tabletype, tItem.m_tInfo.tableid, tItem.m_tInfo.playernum)  --在这里设置参数
				ThreadManager.GdmjUnLock(tItem.m_tInfo.mjtype..tItem.m_tInfo.tabletype)				
				
				
				GdmjModel.DelUserTableID(tItem.m_userList[i].userid)
	
				local gcStandUp = msg_gdmj_pb.gcgdmjstandup()
				gcStandUp.chairid = i
				gcSitDown.tableid = tItem.m_tInfo.tableid
				local userList = GdmjWork.GetUserList(tItem.m_tInfo, tItem.m_userList[i].userid)
				SendMessage(userList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
								
				local gcLeave = msg_gdmj_pb.gcgdmjleave()
				gcLeave.userid = tItem.m_userList[i].userid
				gcLeave.chairid = i
				gcLeave.leavetype = 1
				gcLeave.tableid = tItem.m_tInfo.tableid
				local msgList = {}
				msgList[1] = "由于长时间没开始，您已经被踢出房间！"
				msgList[2] = "休息几分钟，再过来玩吧！"
				msgList[3] = "不要走，决战到天亮！"
				local indexNum = math.myrandom(1,3)
				gcLeave.leavemsg = msgList[indexNum]
				gcLeave.result = 0
				SendMessage(tItem.m_userList[i].userid, PacketCode[2212].client, gcLeave:ByteSize(), gcLeave:SerializeToString())
				
				tItem.m_tInfo.situser[i] = 0
				tItem.m_isModify = true
			end
		end
	end
	if tItem.m_tInfo.playernum == 0 then
		
		tItem.m_tInfo.status = g_gdmjStatus.status_delete
	end		
end

function GdmjJinBiChang.Leave(tInfo, userID)
	
	for i = 1,4 do
		if tInfo.situser[i] == userID then
			tInfo.playernum = tInfo.playernum - 1
			
			ThreadManager.GdmjLock(tInfo.mjtype..tInfo.tabletype)

			--这里一定要加锁
			GdmjModel.DelChairUserByUserID(tInfo.tableid,userID)
			GdmjModel.SetJBCTableID(tInfo.mjtype, tInfo.tabletype, tInfo.tableid, tInfo.playernum)
			ThreadManager.GdmjUnLock(tInfo.mjtype..tInfo.tabletype)
			GdmjModel.DelUserTableID(userID)   --这里还需要把
			local gcStandUp = msg_gdmj_pb.gcgdmjstandup() --
			gcStandUp.chairid = i
			gcSitDown.tableid = tInfo.tableid
			local userList = GdmjWork.GetUserList(tInfo, userID)
			SendMessage(userList, PacketCode[2218].client, gcStandUp:ByteSize(), gcStandUp:SerializeToString())
			
			tInfo.situser[i] = 0
			break
		end		
	end
end

function GdmjJinBiChang.AutoAction(tItem, currPos)
	--如果金币场里面的玩家不出牌，就先默认发哪张就出哪张
	local cgmsg = msg_gdmj_pb.cggdmjaction()
	local gcmsg = msg_gdmj_pb.gcgdmjaction()
	
	cgmsg.userid = tItem.m_userList[currPos].userid
	cgmsg.tableid = tItem.m_tInfo.tableid

	cgmsg.actchairid = currPos
	cgmsg.tarchairid = tItem.m_tInfo.nextinfo.tarchairid
	
	
	
	cgmsg.actiontype,cgmsg.actpokerid = GdmjRobot.Play(tItem,currPos,tItem.m_tInfo.nextinfo.actpokerid)
	
	GdmjService.PlayerAction(tItem,cgmsg,gcmsg)
	--行动完后，开始CheckMoving
	if tItem.m_tInfo.status == g_gdmjStatus.status_playing and tItem.m_tInfo.userstate == g_gdmjUserState.state_waiting then
		--只有玩家的装修是在等待的时候，才是检查更新的时候
		--只有这时才checkmoving
		
		GdmjService.CheckMoving(tItem)
	end	
	
end