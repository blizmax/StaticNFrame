module("JulebuGetPartner", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebugetpartner()
	local gcmsg = msg_julebu2_pb.gcjulebugetpartner()
	
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
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--检查更新昨日数据
	--JulebuService.CheckoutUpdateYesterDayRecord(jInfo)
	
	local resultlist = {}
	if cgmsg.requesttype == 1 then
		if jInfo == nil then
			gcmsg.result = ReturnCode["jlb_enter_fail"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		if cgmsg.opttype == 1 then
			local count = 1
			for i = 1, #jInfo.partnerlist do 
				if count > (cgmsg.page - 1)*10 and count <= cgmsg.page*10 then
					table.insert(resultlist, jInfo.partnerlist[i])					
				end
				count = count + 1
			end
		else
			for i = 1, #jInfo.partnerlist do 
				if jInfo.partnerlist[i] == cgmsg.targetid then
					table.insert(resultlist, jInfo.partnerlist[i])
				end
				
			end
			if #resultlist == 0 then
				gcmsg.result = ReturnCode["jlb_partner_error2"]
				return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		end
	else 
		if cgmsg.opttype == 1 then
			local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, cgmsg.targetid)
			local PartnerInfo = st_julebu_pb.julebupartnerinfo()
			PartnerInfo:ParseFromString(tmp)
			gcmsg.partnerjifen = PartnerInfo.todayconsume
			gcmsg.isconfirm = (false == JulebuModel.IsJulebuDelPartnerid(cgmsg.julebuid, cgmsg.targetid)) and 0 or 1
			local sqlCase = "select userid from dy_julebu_member  where superiorid="..cgmsg.targetid.." and julebuid="..cgmsg.julebuid
			mysqlItem:executeQuery(sqlCase)
			local count = 1
			while true do
				local sqlData = mysqlItem:fetch()
				if sqlData == nil or tonumber(sqlData) == 0 then
					break
				end
				if cgmsg.page <= 1 then 
					table.insert(resultlist, tonumber(sqlData))
				elseif cgmsg.page > 1 then
					if count > (cgmsg.page - 1)*10 then
						table.insert(resultlist, tonumber(sqlData))
					end
				end
				if count > (cgmsg.page - 1)*10 and count >= cgmsg.page*10 then
					break
				end
				count = count + 1
			end
		else
			local sqlCase = "select * from dy_julebu_member  where superiorid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid.." and userid="..cgmsg.targetid
			mysqlItem:executeQuery(sqlCase)
			local sqlData = mysqlItem:fetch({})
			if sqlData ~= nil then
				table.insert(resultlist, tonumber(sqlData[2]))
			end
		end
	end
	
	for k, v in ipairs(resultlist) do 
		local tmp = JulebuModel.GetPartnerInfo(cgmsg.julebuid, v)
		local PartnerInfo = gcmsg.partnerlist:add()
		PartnerInfo:ParseFromString(tmp)
	end
	gcmsg.requesttype = cgmsg.requesttype	
	gcmsg.opttype = cgmsg.opttype	
	gcmsg.page = cgmsg.page	
	gcmsg.targetid = cgmsg.targetid	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()	
end
