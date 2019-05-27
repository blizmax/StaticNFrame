
module("JulebuApply", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuapplylist()
	local gcmsg = msg_julebu_pb.gcjulebuapplylist()
	
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
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() 
	end
	
	if true == JulebuModel.IsJoinIn(cgmsg.userid, cgmsg.julebuid) then
		gcmsg.result = ReturnCode["jlb_apply_joinin"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() 
	end
	
	local jInfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if jInfo == nil then
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString() 
	end
	
	local sqlCase = "select id,status from dy_julebu_member where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
	local aItem = st_julebu_pb.julebumember()
	aItem.userid = cgmsg.userid
	aItem.nickname = pInfo.nickname
	aItem.faceid = pInfo.face_1
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData ~= nil then
		if g_JulebuDefine.mstatus_apply == tonumber(sqlData[2]) then
			gcmsg.result = ReturnCode["jlb_apply_exist"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		elseif g_JulebuDefine.mstatus_normal == tonumber(sqlData[2]) then
			gcmsg.result = ReturnCode["jlb_apply_joinin"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		else
			sqlCase = "update dy_julebu_member set status="..g_JulebuDefine.mstatus_apply.." where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
			mysqlItem:execute(sqlCase)
			aItem.id = tonumber(sqlData[1])
		end
	else
		local sqlCase="insert into dy_julebu_member(userid,julebuid,status) values("..cgmsg.userid..","..cgmsg.julebuid..","..g_JulebuDefine.mstatus_apply..")"
		mysqlItem:execute(sqlCase)
		local sqlCase = "select id from dy_julebu_member where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch()
		if sqlData ~= nil then
			aItem.id = tonumber(sqlData)
		end
	end
	PushApplyList(cgmsg.julebuid, jInfo)
	JulebuModel.AddToApplyList(cgmsg.julebuid, aItem)
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end


function PushApplyList(julebuid, jInfo)
	local pushAppl = msg_julebu_pb.gcjulebuapplylist()
	local appList = JulebuModel.GetApplyList(julebuid)
	if appList ~= nil then
		local count = 1
		for k,v in pairs(appList) do
			if count > (1 - 1)*10 and count <= 1*10 then
				local addItem = pushAppl.jalist:add()
				addItem:ParseFromString(v)
			end
			count = count + 1
			if count >1*10 then
				break
			end
		end
	end
	pushAppl.julebuid = julebuid
	pushAppl.result = 0
	local userIDList = {}
	for k, v in ipairs(jInfo.adminlist) do 
		table.insert(userIDList, tonumber(v))
	end
	SendMessage(userIDList, PacketCode[3214].client,pushAppl:ByteSize(), pushAppl:SerializeToString())
end