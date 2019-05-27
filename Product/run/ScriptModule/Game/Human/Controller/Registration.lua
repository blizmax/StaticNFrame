module("Registration", package.seeall)

--签到

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgregistration()
	local gcmsg = msg_human_pb.gcregistration()
	cgmsg:ParseFromString(buffer)

	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		gcmsg.result = -1
	else
		local temptime = os.date("*t", os.time())
		--更新月份内的签到数据
		if pInfo.signinmonth ~= temptime.month then
			pInfo.registration1 = "1111111111"
			pInfo.registration2 = "1111111111"
			local month = temptime.month
			local year = temptime.year
			if month == 1 or month == 3 or month == 5 or month == 7 or month == 8 or month == 10 or month == 12 then
				pInfo.registration3 = "11111111111"
			elseif month == 4 or month == 6 or month == 9 or month == 11 then
				pInfo.registration3 = "1111111111"
			elseif month == 2 then
				if( year % 4 == 0 and year % 100 ~= 0) or ( year % 100 == 0 and year % 400 == 0) then
					pInfo.registration3 = "111111111"
				else
					pInfo.registration3 = "11111111"
				end
			end
			pInfo.bsignin = 0
			pInfo.signinmonth = temptime.month
		elseif pInfo.signinmonth == temptime.month then
			--更新日签到数据
			if pInfo.bsignin ~= 0 then
				local tempday = os.date("*t", bsignin).day
				if tempday ~= temptime.day then
					pInfo.bsignin = 0 
				end
			end
		end
		
		--查询
		if cgmsg.optype == 1 then
			gcmsg.result = 0 
			gcmsg.userid = cgmsg.userid 
			gcmsg.registration1 = pInfo.registration1
			gcmsg.registration2 = pInfo.registration2
			gcmsg.registration3 = pInfo.registration3
			--签到
		elseif cgmsg.optype == 2 then	
			if pInfo.bsignin == 0 then
				local day = temptime.day
				local num = day%10
				if day >= 1 and day <= 10 then
					local temp = tonumber(pInfo.registration1)
					temp = temp + math.pow(10,#pInfo.registration1 - num)
					pInfo.registration1 = tostring(temp)
				elseif day >= 11 and day <= 20 then
					local temp = tonumber(pInfo.registration2)
					temp = temp + math.pow(10,#pInfo.registration2 - num)
					pInfo.registration2 = tostring(temp)
				elseif day >= 21 then
					local temp = tonumber(pInfo.registration3)
					temp = temp + math.pow(10,#pInfo.registration3 - num)
					pInfo.registration3 = tostring(temp)
				end
				pInfo.turntablenum = pInfo.turntablenum + 1
				pInfo.bsignin = os.time()
				PlayerModel.AddMoney(pInfo, 1, "Registration", "签到")
				--推送给前端刷新钻石
				local gckit = msg_human_pb.gcupdateplayermoney()
				gckit.result = 0
				gckit.userid = cgmsg.userid
				gckit.money = pInfo.money
				SendMessage(cgmsg.userid, PacketCode[1050].client, gckit:ByteSize(), gckit:SerializeToString())
				
				gcmsg.result = 0 
				gcmsg.userid = cgmsg.userid
				gcmsg.registration1 = pInfo.registration1
				gcmsg.registration2 = pInfo.registration2
				gcmsg.registration3 = pInfo.registration3
			elseif pInfo.bsignin > 0 then
				gcmsg.result = ReturnCode["login_award_finsh"]
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		else
			gcmsg.result = 1011
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		local modMsg = {49,50,51,52,53,54,}
		PlayerModel.SetMysqlPlayerInfo(pInfo,modMsg)
		PlayerModel.SetPlayerInfo(pInfo)
	end
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end