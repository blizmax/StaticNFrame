module("PszAction", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszaction()
	local gcmsg = msg_psz_pb.gcpszaction()
	
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszAction", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"psz","PszAction", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	ThreadManager.PszLock(cgmsg.tableid)
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "psz", "PszAction", 0,"牌桌不存在")
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	if tInfo.state ~= g_PszDefine.state_play then
		gcmsg.result = 0
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--除了看牌操作和弃牌，其他操作必须在轮到轮到玩家才能
	if cgmsg.userid ~= tInfo.actionuser  then
		if cgmsg.actiontype ~= g_PszDefine.abandonpoker and cgmsg.actiontype ~= g_PszDefine.lookpoker then
			gcmsg.result = ReturnCode["psz_action_no"]
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
	end
	
	if cgmsg.actiontype ~= g_PszDefine.lookpoker then
		for k, v in ipairs(tInfo.situser) do
			if cgmsg.userid == v.userid then
				if v.pokerstate_discard == 2 or v.compare == 1 or #tInfo.actionlist < 2  then
					gcmsg.result = 0
					ThreadManager.PszUnLock(cgmsg.tableid)
					return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
				end
			end
		end
	end
	
	if tInfo.julebuid ~= 0 and tInfo.julebutype == 2 then
		local indexPos = 0
		for k, v in ipairs(tInfo.situser)do
			if v.userid == cgmsg.userid then
				indexPos = k
			end
		end
		if cgmsg.actiontype == g_PszDefine.lookpoker then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].callNum * 2 then
				gcmsg.result = ReturnCode["psz_jetton_error_1"]
				ThreadManager.PszUnLock(cgmsg.tableid)
				return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
		end
		if cgmsg.actiontype == g_PszDefine.upjetton then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < cgmsg.addjetton then
				gcmsg.result = ReturnCode["psz_jetton_error_1"]
				ThreadManager.PszUnLock(cgmsg.tableid)
				return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
		end
		
		if cgmsg.actiontype == g_PszDefine.comparepoker then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].thanNum then
				gcmsg.result = ReturnCode["psz_jetton_error_1"]
				ThreadManager.PszUnLock(cgmsg.tableid)
				return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
		end
		
		if cgmsg.actiontype == g_PszDefine.followjetton then
			if (tInfo.situser[indexPos].jifen - tInfo.situser[indexPos].jetton) < tInfo.situser[indexPos].callNum then
				gcmsg.result = ReturnCode["psz_jetton_error_2"]
				ThreadManager.PszUnLock(cgmsg.tableid)
				return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
			end
		end
		
	end
	
	local deluaction = 0
	--弃牌操作
	if cgmsg.actiontype == g_PszDefine.abandonpoker then
		local userid = PszService.abandonpoker(tInfo,cgmsg.userid)
		gcmsg.actiontype = g_PszDefine.abandonpoker
		deluaction = userid
	--看牌操作
	elseif cgmsg.actiontype == g_PszDefine.lookpoker then
		--print("tInfo.presentbout"..tInfo.presentbout.."********".."tInfo.menpai"..tInfo.menpai)
		if tInfo.presentbout <  tInfo.menpai then
			gcmsg.result = ReturnCode["psz_menpai_no"]
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		
		local pokerlist, psztype = PszService.lookpoker(tInfo,cgmsg.userid)
		gcmsg.ptype = psztype
		gcmsg.actiontype = g_PszDefine.lookpoker
		for k,v in ipairs(pokerlist) do
			gcmsg.pokerlist:append(v)
		end

	--比牌操作
	elseif cgmsg.actiontype == g_PszDefine.comparepoker then
		if tInfo.presentbout <  tInfo.mincomp then
			gcmsg.result = ReturnCode["psz_mincomp_no"]
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		local isok = false
		for k,v in ipairs(tInfo.actionlist)do 
			if v == cgmsg.rivalID then
				isok = true
			end
		end
		
		if isok == false then
			gcmsg.status = 1
			gcmsg.result = 0
			gcmsg.actiontype = cgmsg.actiontype
			for k, v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					gcmsg.callNum = v.callNum 	--跟注的值
					gcmsg.thanNum = v.thanNum 	--比牌的值
					for i = 1, #v.filList do
						gcmsg.filList:append(v.filList[i])
					end
				end
			end
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0 ,gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		local winner, loser,jetton = PszService.comparepoker(tInfo, cgmsg.rivalID)
		gcmsg.actiontype = g_PszDefine.comparepoker
		gcmsg.winnerid = winner
		tmp = gcmsg.bipailist:add()
		tmp.winnerid = winner
		tmp.loserid = loser
		deluaction = loser
		gcmsg.addjetton = jetton
		
	--加注操作
	elseif cgmsg.actiontype == g_PszDefine.upjetton then
		local isEx = false 
		for k,v in ipairs(tInfo.situser)do
			if cgmsg.userid == v.userid then
				for i = 1, #g_PszDefine_difenlist[tInfo.jifen] do
					local tmp = g_PszDefine_difenlist[tInfo.jifen][i]
					if v.pokerstate == 1 then
						tmp = tmp * 2
					end
					if tmp == cgmsg.addjetton then
						if cgmsg.addjetton > v.callNum then
							isEx = true
						end
					end
				end
				break
			end
		end	 
		if isEx == false then
			--查看这个错误的是否是存在玩家的列表中
			local tmp = false
			for k,v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					for z, j in ipairs(v.filList)do 
						if z == cgmsg.addjetton then
							tmp = true
						end
					end
				end
			end
			--如果是true就说明服务器出错了， 重新刷新一下玩家的下注列表
			if tmp == true then
				LogFile("error","tableID:"..tInfo.tableid.."usevipnum:"..tInfo.usevipnum.."addjetton: "..cgmsg.addjetton )
				for k,v in ipairs(tInfo.situser) do
					if v.userid == cgmsg.userid then
						while #tInfo.situser[k].filList > 0 do
							tInfo.situser[k].filList:remove(1)
						end
						for z, y in ipairs(g_PszDefine_difenlist[tInfo.difen]) do
							local tmp = y 
							if v.pokerstate == g_PszPokerStatus.poker_looked then
								tmp = tmp * 2
							end
							tInfo.situser[k].filList:append(tmp)
						end
					end
				end
			end
			--gcmsg.status = 1   landy：190109 先注释掉。搞不懂这里干嘛的。
			gcmsg.result = 0
			gcmsg.actiontype = cgmsg.actiontype
			for k, v in ipairs(tInfo.situser) do
				if v.userid == cgmsg.userid then
					gcmsg.callNum = v.callNum 	--跟注的值
					gcmsg.thanNum = v.thanNum 	--比牌的值
					for i = 1, #v.filList do
						gcmsg.filList:append(v.filList[i])
					end
				end
			end
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0 ,gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	
		local jetton = PszService.upjetton(tInfo, cgmsg.addjetton)
		gcmsg.actiontype = g_PszDefine.upjetton
		gcmsg.addjetton = jetton
		
	--跟注操作
	elseif cgmsg.actiontype == g_PszDefine.followjetton then
		local jetton = PszService.followjetton(tInfo)
		gcmsg.actiontype = g_PszDefine.followjetton
		gcmsg.addjetton = jetton
	end
		
	--金币场：扣除下注的金币
	if tInfo.tabletype ~= g_PszDefine.table_srf then
		if false == PszService.FeeDeductions(pInfo, gcmsg.addjetton, g_PszDefine.jetton_pay, "PszPourJetton") then
			gcmsg.result = ReturnCode["psz_jeeton_short"]
			ThreadManager.PszUnLock(cgmsg.tableid)
			return cgmsg.userid, 0, 0,gcmsg:SerializeToString()
		end
	end
	
	--只有在弃牌和比牌才会产生淘汰的玩家
	if cgmsg.actiontype == g_PszDefine.abandonpoker or cgmsg.actiontype == g_PszDefine.comparepoker then
		tInfo.deluser =  deluaction
	end
	--如果弃牌的不是当前行动，就不把时间置为0	
	if cgmsg.actiontype == g_PszDefine.abandonpoker and tInfo.actionuser == cgmsg.userid then
		tInfo.timemark = -1
	elseif  cgmsg.actiontype == g_PszDefine.abandonpoker then
		PszService.NextAction(tInfo, 1)
	end
	
	--看牌不结束该玩家的行动时间 
	if cgmsg.actiontype ~= g_PszDefine.lookpoker and cgmsg.actiontype ~= g_PszDefine.abandonpoker then
		tInfo.timemark = -1
	end
	--
	if cgmsg.actiontype ~= g_PszDefine.lookpoker then
		for i = 1, #tInfo.situser do 
			if cgmsg.userid ==  tInfo.situser[i].userid then
				tInfo.situser[i].state = g_PszPlayStatus.status_wait
				gcmsg.jetton = tInfo.situser[i].jetton
				gcmsg.oncepour = tInfo.oncepour
				if tInfo.situser[i].pokerstate == 1 then
					gcmsg.oncepour = gcmsg.oncepour * 2
				end
			end
		end
	end
	
	for k, v in ipairs(tInfo.situser) do
		if v.userid == cgmsg.userid then
			gcmsg.callNum = v.callNum 	--跟注的值
			gcmsg.thanNum = v.thanNum 	--比牌的值
			for i = 1, #v.filList do
				gcmsg.filList:append(v.filList[i])
			end
		end
	end
	gcmsg.guodi = tInfo.guodi
	gcmsg.result = 0
	gcmsg.userid = cgmsg.userid
	local userIDList = PszService.GetUserIDList(tInfo)
	if cgmsg.actiontype == g_PszDefine.lookpoker then
		userIDList = PszService.GetUserIDList(tInfo, cgmsg.userid)
		SendMessage(cgmsg.userid, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		gcmsg.ptype = 0
		while #gcmsg.pokerlist > 0 do
			gcmsg.pokerlist:remove(1)
		end
	end
	--print("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++")
	--print(gcmsg)
	SendMessage(userIDList, PacketCode[3320].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	if  cgmsg.actiontype == g_PszDefine.abandonpoker then
		if #tInfo.actionlist == 1 or  (#tInfo.actionlist == 2 and tInfo.deluser == tInfo.actionuser)then
			local gcmsgb = msg_psz_pb.gcpszconcealbutton()
			gcmsgb.result = 0
			gcmsgb.flag = 1
			local userIDList = PszService.GetUserIDList(tInfo)
			SendMessage(userIDList, PacketCode[3336].client, gcmsgb:ByteSize(), gcmsgb:SerializeToString())
		end
	end
	
	if tInfo.timemark == - 1 then
		if cgmsg.actiontype ~= g_PszDefine.lookpoker then
			PszService.NextAction(tInfo)
			if #tInfo.actionlist == 1 or tInfo.presentbout > tInfo.boutcapp then
				tInfo.state = g_PszDefine.state_play
				tInfo.timemark = -2
				if cgmsg.actiontype == g_PszDefine.comparepoker then
					tInfo.flag = 1
				end
			else
				if cgmsg.actiontype == g_PszDefine.comparepoker then
					tInfo.timemark = tInfo.timemark +  4
				end
			end
		end
	end
	PszModel.SetTableInfo(tInfo, 1)   
	ThreadManager.PszUnLock(cgmsg.tableid)
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end

function ContinuousComparepoker(tInfo)
	local gcA = msg_psz_pb.gcpszaction()
	
	local tmpJetton = 0
	local Act = PszService.returnSub(tInfo, tInfo.actionuser)
	tmpJetton = tonumber(tInfo.situser[Act].jifen) < tInfo.situser[Act].callNum and tonumber(tInfo.situser[Act].jifen) or tInfo.situser[Act].callNum
	tInfo.guodi = tInfo.guodi + tmpJetton
	tInfo.situser[Act].jetton = tInfo.situser[Act].jetton + tmpJetton
	tInfo.situser[Act].state = g_PszPlayStatus.status_wait
	tInfo.situser[Act].jifen = tInfo.situser[Act].jifen - tmpJetton
	gcA.addjetton = tmpJetton
	local pos = 0
	for k, v in ipairs(tInfo.actionlist) do
		if v == tInfo.actionuser then
			pos = k 
			break
		end
	end
	gcA.userid = tInfo.situser[Act].userid
	gcA.jetton = tInfo.situser[Act].jetton
	local ComparepokerList = {}
	local isF = false
	for i = 1, 2 do
		for j = 1, #tInfo.actionlist do
			if isF == false then
				if pos < j then
					table.insert(ComparepokerList, tInfo.actionlist[j])
				end
			else
				if pos > j then
					table.insert(ComparepokerList, tInfo.actionlist[j])
				end
			end
		end
		isF = true
	end
	
	local action = tInfo.actionuser

	local loseNum = 0
	for i = 1, #ComparepokerList do
		local ord = PszService.returnSub(tInfo, ComparepokerList[i])
		local TmpA = PszService.returnSub(tInfo, action)
		
		if false == PszService.whoWin(TmpA,ord, tInfo) then
			tmp = gcA.bipailist:add()
			tmp.winnerid = tInfo.situser[ord].userid
			tmp.loserid = tInfo.situser[TmpA].userid
			tInfo.situser[TmpA].compare = 1
			tInfo.actionuser = tInfo.situser[ord].userid
			loseNum = loseNum + 1
			for i = #tInfo.actionlist, 1, -1 do
				if tInfo.actionlist[i] == tmp.loserid then
					tInfo.actionlist:remove(i)
				end
			end
			break
		else
			tmp = gcA.bipailist:add()
			tmp.winnerid = tInfo.situser[TmpA].userid
			tmp.loserid = tInfo.situser[ord].userid
			tInfo.situser[ord].compare = 1
			loseNum = loseNum + 1
			for i = #tInfo.actionlist, 1, -1 do
				if tInfo.actionlist[i] == tmp.loserid then
					tInfo.actionlist:remove(i)
				end
			end
		end
	end
	
	for k,v in ipairs(tInfo.situser)do
		if tInfo.actionuser == v.userid then
			tInfo.situser[k].state = g_PszPlayStatus.status_play
			break
		end
	end

	gcA.actiontype = g_PszDefine.comparepoker
	gcA.winnerid = tInfo.actionuser
	gcA.guodi = tInfo.guodi
	gcA.oncepour = tInfo.oncepour
	gcA.result = 0
	local userIDList = PszService.GetUserIDList(tInfo)
	SendMessage(userIDList, PacketCode[3320].client, gcA:ByteSize(), gcA:SerializeToString())

	if #tInfo.actionlist == 1 then
		tInfo.state = g_PszDefine.state_count
		tInfo.timemark = g_PszTime.count_time
		tInfo.timemark = ArtoonTime(tInfo,loseNum + 1) + 2
	else
		
		local tmpT
	
		if loseNum == 2 then
			tmpT = 3
		elseif loseNum == 3 then
			tmpT = 6
		elseif loseNum == 4 then
			tmpT = 9
		elseif loseNum == 5 then
			tmpT = 12
		elseif loseNum == 6 then
			tmpT = 16
		elseif loseNum == 7 then
			tmpT = 19
		elseif loseNum == 8 then
			tmpT = 23
		else 
			tmpT = 3
		end
		tInfo.timemark = tInfo.qipai + tmpT
	end
end		
