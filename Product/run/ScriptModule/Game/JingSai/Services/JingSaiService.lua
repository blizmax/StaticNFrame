
JingSaiService = {}

function JingSaiService.Init()
	--在么每次启动前，需要检查是否暂停的时候，有竞赛没有开始的，如果存在有竞赛没有开始的，那就需要开始
	local startList = JingSaiModel.GetAllStart()
	local longList = JingSaiModel.GetAllLong()
	--两个都获取回来
	
	local currSec = TimeUtils.GetTime()
	for k,v in pairs(startList) do
		--开始做计算了
		--
		local isExist = true
		if longList[k] == nil then
			local jsInfo = JingSaiModel.GetJingSaiInfo(k)
			if nil == jsInfo then
				JingSaiModel.DelJingSai(k, 0) --这里是找不到俱乐部
				isExist = false
			else
				JingSaiModel.SetLongTime(jsInfo)
				longList[k] = jsInfo.longtime
			end
		end
		
		if true == isExist then
			if currSec > tonumber(v) then
				--在一每次
				local timeMark = currSec - tonumber(startList[k])
				
				if timeMark < tonumber(longList[k]) then
					--说明这里需要判断是否已经开始的了，如果还没有开始，就需要开始
					local jsInfo = JingSaiModel.GetJingSaiInfo(k)
					if jsInfo.state == g_jingSaiDefine.state_ready then
						jsInfo.state = g_jingSaiDefine.state_start
						JingSaiModel.SetJingSaiInfo(jsInfo)
						--竞赛设置为开始
					end
				end
				
			end
		end
		
	end	
	
end


--创建游戏的时候，
--这里是做结算的情况
function JingSaiService.ServerLoop()
	--在这里轮询中，做每天结算的工作
	--只有在晚上八点的时候，才对对应的玩家做统计
	--
	--这里开始轮询
	local startList = JingSaiModel.GetAllStart()
	local longList = JingSaiModel.GetAllLong()
	--两个都获取回来
	
	local currSec = TimeUtils.GetTime()
	local delList = {}
	for k,v in pairs(startList) do
		--开始做计算了
		--
		local tempStr = TimeUtils.GetTimeString(v)
		local isExist = true
		if longList[k] == nil then
			local jsInfo = JingSaiModel.GetJingSaiInfo(k)
			if nil == jsInfo then
				JingSaiModel.DelJingSai(k, 0) --这里是找不到俱乐部
				isExist = false
			else
				JingSaiModel.SetLongTime(jsInfo)
				longList[k] = jsInfo.longtime
			end
		end
		
		if true == isExist then
			if currSec > tonumber(v) then
				--在一每次
				local timeMark = currSec - tonumber(startList[k])
				
				if  timeMark >= 0 and timeMark < 10 then
					--只有10个轮询的机会来判断
					--说明这里需要判断是否已经开始的了，如果还没有开始，就需要开始
					local jsInfo = JingSaiModel.GetJingSaiInfo(k)
					if jsInfo.state == g_jingSaiDefine.state_ready then
						jsInfo.state = g_jingSaiDefine.state_start
						JingSaiModel.SetJingSaiInfo(jsInfo)
						JingSaiService.SendUpdate(jsInfo)
						--竞赛设置为开始
					end
				end
				
				timeMark = timeMark - tonumber(longList[k])
				
				if timeMark >= 0 then
					--这里是的需要结算的了
					local jsInfo = JingSaiModel.GetJingSaiInfo(k)

					if jsInfo ~= nil then
						if true == JingSaiService.ServerCount(jsInfo, currSec) then
							JingSaiService.SendDel(jsInfo.jingsaiid, jsInfo.julebuid)
							JingSaiModel.DelJingSai(k, jsInfo.julebuid)
						end
					else

						JingSaiModel.DelJingSai(k, 1)
					end
				end
				
			end
		end
		
	end	
		
end


--创建游戏的时候，
function JingSaiService.CreateGame(gameType, jingSaiID, tableID, maxUserNum, userNum, des)
	
	if false == JingSaiModel.IsExist(jingSaiID) then
		--因为有可能存在，竞赛已经结束了，但是比赛还存在的情况
		return
	end
	
	local jsGame = st_jingsai_pb.jingsaitable()
	
	jsGame.jingsaiid = jingSaiID
	jsGame.tableid = tableID
	jsGame.maxusernum = maxUserNum
	jsGame.usernum = userNum
	jsGame.state = g_jingSaiDefine.table_ready
	JingSaiModel.SetGameInfo(jsGame)
	JingSaiModel.PushGame(jingSaiID, tableID)
end

function JingSaiService.JulebuGameStart(jingSaiID, tableID)

	if false == JingSaiModel.IsExist(jingSaiID) then
		--因为有可能存在，竞赛已经结束了，但是比赛还存在的情况
		return
	end
	
	local strGet = JingSaiModel.GetGameInfo(jingSaiID, tableID)
	if strGet ~= nil then
		
		local jsGame = st_jingsai_pb.jingsaitable()
		jsGame:ParseFromString(strGet)
		jsGame.state = g_jingSaiDefine.table_start    --游戏开始了
		JingSaiModel.SetGameInfo(jsGame)
	end
end

function JingSaiService.GameEnd(jingSaiID, tableID)

	if false == JingSaiModel.IsExist(jingSaiID) then
		--因为有可能存在，竞赛已经结束了，但是比赛还存在的情况
		return
	end	
	
	JingSaiModel.DelGameInfo(jingSaiID, tableID)
end

function JingSaiService.GameUpdate(jingSaiID, tableID, userNum)
	

	if false == JingSaiModel.IsExist(jingSaiID) then
		--因为有可能存在，竞赛已经结束了，但是比赛还存在的情况
		return
	end	
	
	local strGet = JingSaiModel.GetGameInfo(jingSaiID, tableID)
	if strGet ~= nil then
		local jsGame = st_jingsai_pb.jingsaitable()
		jsGame:ParseFromString(strGet)
		
		jsGame.usernum = userNum
		JingSaiModel.SetGameInfo(jsGame)
	end
end

function JingSaiService.GameCount(jingSaiID, tableID, userArr, moduleType)


	if false == JingSaiModel.IsExist(jingSaiID) then
		--因为有可能存在，竞赛已经结束了，但是比赛还存在的情况
		return
	end

	for k,v in ipairs(userArr['userlist']) do
		JingSaiModel.AddUserJiFen(jingSaiID, v['userid'], v['jifen'], v['nick'], v['faceid'])  --把积分同步更新
	end

end



function JingSaiService.ServerCount(jsInfo, currSec)
	--这里是的需要结算的
	if jsInfo == nil then -- or jsInfo.state ~= g_jingSaiDefine.state_start then
		--如果状态不对的时候，就返回
		return false
	end

	if jsInfo.state == g_jingSaiDefine.state_start then
		jsInfo.state = g_jingSaiDefine.state_count   --正在结算中
		JingSaiModel.SetJingSaiInfo(jsInfo)
	end



	local gameList = JingSaiModel.GetGameList(jsInfo.jingsaiid, 1)  --这里只取第一页就够了
	if #gameList > 0 then
		
		for m,n in pairs(gameList) do
			local strGet = JingSaiModel.GetGameInfo(jsInfo.jingsaiid, n)
			if strGet ~= nil then
				local gItem = st_jingsai_pb.jingsaitable()
				gItem:ParseFromString(strGet)
				
				if gItem.state == g_jingSaiDefine.table_ready then
					--如果还是准备中的牌桌，就会被清除
					JingSaiModel.DelGameInfo(jsInfo.jingsaiid, n)
				end
				
			else
				JingSaiModel.DelGameInfo(jsInfo.jingsaiid, n)
			end
		end
		--如果还有牌桌正在玩的，还不能够结算
		return false
	end


	local htyInfo = st_jingsai_pb.jingsaihistory()
	htyInfo.jingsaiid = jsInfo.jingsaiid
	htyInfo.name = jsInfo.name
	htyInfo.moduletype = jsInfo.moduletype
	htyInfo.starttime = jsInfo.starttime
	htyInfo.endtime =  currSec --jsInfo.endtime
	htyInfo.des = jsInfo.des
	
	local tempList = JingSaiModel.GetAllUserJiFen(jsInfo.jingsaiid)
	local userList = {}
	--这里需要排序
	
	local function insertList(userID, jiFen)
		local addItem = {}
		addItem[1] = userID
		addItem[2] = tonumber(jiFen)
		
		table.insert(userList, addItem)
		
		for i = #userList, 2 , -1 do

			if userList[i][2] > userList[i-1][2] then
				local tempUserID = userList[i-1][1]
				local tempJiFen = userList[i-1][2]
				userList[i-1][1] = userList[i][1]
				userList[i-1][2] = userList[i][2]
				userList[i][1] = tempUserID
				userList[i][2] = tempJiFen
				--交换了他们两个的值
			else
				break
			end
			
		end
	end
	for k,v in pairs(tempList) do
		insertList(k,v)
	end
	local nickList = JingSaiModel.GetAllUserNick(jsInfo.jingsaiid)
	local faceList = JingSaiModel.GetAllUserFaceID(jsInfo.jingsaiid)



	for k,v in ipairs(userList) do
		
		if faceList[v[1] ] == nil then
			local pInfo = PlayerModel.GetPlayerInfo(v[1])
			if pInfo ~= nil then
				faceList[ v[1] ] = pInfo.face_1
			end
		end
		
		if nickList[ v[1] ] ~= nil then
			htyInfo.nicklist:append( nickList[ v[1] ] )
		else
			htyInfo.nicklist:append( '' )
		end
		
		if faceList[ v[1] ] ~= nil then
			htyInfo.faceidlist:append(faceList[ v[1] ])
		else
			htyInfo.faceidlist:append()
		end
		htyInfo.useridlist:append(tonumber(v[1]))
		htyInfo.jifenlist:append(v[2])
	end		
	
	--开始结算了
	JingSaiModel.PushHistory(jsInfo.julebuid, htyInfo)
	return true
end

function JingSaiService.SendDel(jingSaiID, julebuID)
	local gcDel = msg_jingsai_pb.gcjingsaidel()
	gcDel.result = 0
	gcDel.jingsaiid = jingSaiID
	local sendList = JulebuModel.GetOnlineList(julebuID)
	SendMessage(sendList, PacketCode[3406].client, gcDel:ByteSize(), gcDel:SerializeToString())   --同时广播给其他人
end

function JingSaiService.SendUpdate(jsInfo)
	local gcUpDate = msg_jingsai_pb.gcjingsaiupdate()
	gcUpDate.jsinfo:ParseFromString(jsInfo:SerializeToString())
	gcUpDate.result = 0
	local sendList = JulebuModel.GetOnlineList(jsInfo.julebuid)
	SendMessage(sendList, PacketCode[3420].client, gcUpDate:ByteSize(), gcUpDate:SerializeToString())
end
