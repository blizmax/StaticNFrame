module("JulebuAddPartner", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebuaddpartner()
	local gcmsg = msg_julebu2_pb.gcjulebuaddpartner()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuApply", 0, "重复申请，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuApply", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""	
	end
	
	local tpInfo = PlayerModel.GetPlayerInfo(cgmsg.targetid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuApply", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""	
	end
	
	if true ~= JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_not_member"]
		return cgmsg.userid, 0, gcmsg:SerializeToString(), gcmsg:ByteSize()
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_enter_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local isPartner = false
	local Partnerlist = {}
	for i = 1, #jInfo.partnerlist do 
		if jInfo.partnerlist[i] == cgmsg.targetid then
			isPartner = true	
		end
	end
		
	if cgmsg.requesttype == 1 then
		if true ~= JulebuModel.IsJoinIn(cgmsg.targetid, cgmsg.julebuid) then
			gcmsg.result = ReturnCode["jlb_partner_error4"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		if 0 ~= JulebuService.checkoutsuperiorid(cgmsg.julebuid, cgmsg.targetid) then
			gcmsg.result = ReturnCode["jlb_partner_error1"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		if isPartner == true then
			gcmsg.result = ReturnCode["jlb_partner_error1"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		jInfo.partnerlist:append(cgmsg.targetid)
		for i = 1, #jInfo.partnerlist do 
			table.insert(Partnerlist, jInfo.partnerlist[i])
		end
		local PartnerInfo = st_julebu_pb.julebupartnerinfo()
		PartnerInfo.userid = tpInfo.userid
		PartnerInfo.nickname = tpInfo.nickname
		PartnerInfo.yesterdayscore = 0
		PartnerInfo.yesterdaydayingjia = 0
		PartnerInfo.yesterdaygamenum = 0
		PartnerInfo.yesterdayconsume = 0
		PartnerInfo.gamenum = 0
		PartnerInfo.todayscore = 0
		PartnerInfo.todaydayingjia = 0
		PartnerInfo.todaygamenum = 0
		PartnerInfo.todayconsume = 0
		
		JulebuModel.SetPartnerInfo(cgmsg.julebuid, tpInfo.userid, PartnerInfo)
	else 
		if isPartner == false then
			gcmsg.result = ReturnCode["jlb_partner_error2"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		for i = #jInfo.partnerlist, 1, -1 do 
			if jInfo.partnerlist[i] == cgmsg.targetid then
				jInfo.partnerlist:remove(i)	
			else
				table.insert(Partnerlist, jInfo.partnerlist[i])
			end
		end
		JulebuService.DelPartner(cgmsg.julebuid, tpInfo.userid)
	end
		
	local sqlCase = "update dy_julebu set partnerlist='"..luajson.encode(Partnerlist).."' where id="..cgmsg.julebuid
	mysqlItem:execute(sqlCase) 
	JulebuModel.SetJulebuInfo(jInfo)
	
	local count = 0
	for k, v in ipairs(jInfo.partnerlist) do 
		local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, v)
		local PartnerInfo = gcmsg.partnerlist:add()
		PartnerInfo:ParseFromString(tmp)
		if count > 9 then
			break
		end
		count = count + 1
	end
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
	
end
