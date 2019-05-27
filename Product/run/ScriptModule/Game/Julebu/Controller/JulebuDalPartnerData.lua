module("JulebuDalPartnerData", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebudalpartnerdata()
	local gcmsg = msg_julebu2_pb.gcjulebudalpartnerdata()
	
	cgmsg:ParseFromString(buffer)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--查看partnerid 是不是合伙人
	local ispartner = false
	for i = 1, #jInfo.partnerlist do
		if jInfo.partnerlist[i] == cgmsg.partnerid then
			ispartner = true
			break
		end	
	end
	
	if ispartner == false then
		gcmsg.result = ReturnCode["jlb_partner_error2"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
	end
	
	if cgmsg.requesttype == 1 then
		local isadmin = false
		for i = 1, #jInfo.adminlist do 
			if jInfo.adminlist[i] == cgmsg.userid then
				isadmin = true
				break
			end
		end
		if isadmin == false then
			gcmsg.result = ReturnCode["jlb_is_admin2"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
		if false == JulebuModel.IsJulebuDelPartnerid(cgmsg.julebuid, cgmsg.partnerid) then
			JulebuModel.SetJulebuDelPartnerid(cgmsg.julebuid, cgmsg.partnerid)
		end
	else
		if cgmsg.partnerid ~= cgmsg.userid then
			gcmsg.result = ReturnCode["jlb_del_partnerdate_1"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
		end
		
		if false == JulebuModel.IsJulebuDelPartnerid(cgmsg.julebuid, cgmsg.partnerid) then
			gcmsg.result = ReturnCode["jlb_del_partnerdate_1"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		JulebuModel.DelJulebuDelPartnerid(cgmsg.julebuid, cgmsg.partnerid)
		
		local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, cgmsg.partnerid)
		local PartnerInfo = st_julebu_pb.julebupartnerinfo()
		PartnerInfo:ParseFromString(tmp)
		PartnerInfo.yesterdayscore = 0
		PartnerInfo.yesterdaydayingjia = 0
		PartnerInfo.yesterdaygamenum = 0
		PartnerInfo.yesterdayconsume = 0
		PartnerInfo.gamenum = 0
		PartnerInfo.todayscore = 0
		PartnerInfo.todaydayingjia = 0
		PartnerInfo.todaygamenum = 0
		PartnerInfo.todayconsume = 0
		JulebuModel.SetPartnerInfo(jInfo.julebuid, cgmsg.partnerid, PartnerInfo)
		
		local sqlCase = "select userid from dy_julebu_member  where superiorid="..cgmsg.partnerid.." and julebuid="..cgmsg.julebuid
		mysqlItem:executeQuery(sqlCase)
		local count = 1
		while true do
			local sqlData = mysqlItem:fetch()
			if sqlData == nil then
				break
			end
			local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, tonumber(sqlData))
			local PartnerInfo = st_julebu_pb.julebupartnerinfo()
			PartnerInfo:ParseFromString(tmp)
			PartnerInfo.yesterdayscore = 0
			PartnerInfo.yesterdaydayingjia = 0
			PartnerInfo.yesterdaygamenum = 0
			PartnerInfo.yesterdayconsume = 0
			PartnerInfo.gamenum = 0
			PartnerInfo.todayscore = 0
			PartnerInfo.todaydayingjia = 0
			PartnerInfo.todaygamenum = 0
			PartnerInfo.todayconsume = 0
			JulebuModel.SetPartnerInfo(cgmsg.julebuid, tonumber(sqlData), PartnerInfo)
			
			if count < 0 or count >= 10 then
				local addPartnerInfo = gcmsg.partnerlist:add()
				addPartnerInfo:ParseFromString(PartnerInfo:SerializeToString())
			end
			count = count + 1
		end
	end
	
	gcmsg.userid = cgmsg.userid
	gcmsg.julebuid = cgmsg.julebuid
	gcmsg.partnerid = cgmsg.partnerid
	gcmsg.requesttype = cgmsg.requesttype
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
end
