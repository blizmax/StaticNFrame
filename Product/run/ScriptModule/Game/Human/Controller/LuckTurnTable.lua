module("LuckTurnTable", package.seeall)

--幸运转盘

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgluckturntable()
	local gcmsg = msg_human_pb.gcluckturntable()
	cgmsg:ParseFromString(buffer)

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		gcmsg.result = -1
	else
		--查询
		if cgmsg.optype == 1 then
			gcmsg.result = 0 
			gcmsg.userid = cgmsg.userid 
			gcmsg.turnresult = 0
			gcmsg.bsignin = pInfo.bsignin
			gcmsg.turntablenum = pInfo.turntablenum
			gcmsg.tengames = pInfo.tengames
			gcmsg.todayshare = pInfo.todayshare
			--转盘
		elseif cgmsg.optype == 2 then	
			if pInfo.turntablenum >= 1 then
				local num = turntable() --随机结果
				pInfo.turntablenum = pInfo.turntablenum -1  --转盘次数减一
				PlayerModel.AddMoney(pInfo, num, "LuckTurnTable", "幸运转盘")
				--通知前端刷新钻石
				local gckit = msg_human_pb.gcupdateplayermoney()
				gckit.result = 0
				gckit.userid = cgmsg.userid
				gckit.money = pInfo.money
				SendMessage(cgmsg.userid, PacketCode[1050].client, gckit:ByteSize(), gckit:SerializeToString())
				
				num = arearandom(num)		--随机结果所对应的区域
				gcmsg.turnresult = num
				gcmsg.result = 0 
				gcmsg.userid = cgmsg.userid
				gcmsg.bsignin = pInfo.bsignin
				gcmsg.turntablenum = pInfo.turntablenum
				gcmsg.tengames = pInfo.tengames
				gcmsg.todayshare = pInfo.todayshare
			elseif pInfo.turntablenum <= 0 then
				gcmsg.result = ReturnCode["login_award_finsh"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		else
			gcmsg.result = 1011
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		local modMsg = {54,}
		PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
		PlayerModel.SetPlayerInfo(pInfo)
	end
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end

function turntable()
	local num = math.random(1,100)
	if num > 0 and num <= 10 then
		return 3
	elseif num > 10 and num <= 40 then
		return 2
	elseif num > 40 and num <= 100 then
		return 1
	end
end

function arearandom(count)
	local num = math.random(1,100)
	if count == 1 then
		if num >= 50 then
			return 2
		end
		return 5
	elseif count == 2 then
		if num >= 50 then
			return 3
		end
		return 6
	elseif count == 3 then
		if num >= 50 then
			return 1
		end
		return 4
	end
end