
module("JulebuCreate", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebucreate()
	local gcmsg = msg_julebu_pb.gcjulebucreate()
	
	cgmsg:ParseFromString(buffer)

	--AgencyModel.ZsmjDivide(cgmsg.userid, 100, "test")
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuCreate", 0, "重复创建俱乐部，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "julebu", "JulebuCreate", ReturnCode["player_not_exist"], "人物信息不存在，userid="..cgmsg.userid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""		
	end
	
	--[[local agency = AgencyModel.GetAgencyInfo(cgmsg.userid)
	local isDayLi = false
	if agency ~= nil and agency.level > 90 then
		isDayLi = true
	end
	
	
	if isDayLi == false then
		gcmsg.result = ReturnCode["jlb_create_daili"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end]]
	
	if cgmsg.name == nil or cgmsg.name == '' then
		gcmsg.result = ReturnCode["jlb_name_nil"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jInfo = st_julebu_pb.julebuinfo()
	jInfo.name = cgmsg.name
	
	--这里开始创建
	local moduleslist = {}
	for k,v in ipairs(cgmsg.moduleslist) do
		table.insert(moduleslist, v)
		jInfo.gamemodules:append(v)
	end
	jInfo.adminlist:append(cgmsg.userid)
	jInfo.membernum = 1
	jInfo.maxmembernum = g_JulebuDefine.maxMemberNum   --
	jInfo.ownerid = cgmsg.userid
	jInfo.setroom = 0
	jInfo.autoroom = 0
	jInfo.gamecount = 2000
	jInfo.bankercount = 1000
	jInfo.ruleset = 2
	jInfo.faceaward_figure = 1
	jInfo.faceaward_num = 5
	jInfo.faceaward_count = 0
	jInfo.proportion = 0  
	jInfo.totaljifen = 0
	jInfo.yesterdaypump = 0
	jInfo.todaypump = 0
	jInfo.updateday = 0
	if nil == JulebuModel.CreateJulebu(jInfo) then
		gcmsg.result = ReturnCode["jlb_create_fail"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	jInfo.ownerid = cgmsg.userid
	JulebuModel.SetJulebuInfo(jInfo)
	JulebuModel.SetUserJulebuList(cgmsg.userid, jInfo.julebuid)
	gcmsg.julebuid = jInfo.julebuid
	gcmsg.name = jInfo.name
	gcmsg.membernum = jInfo.membernum
	gcmsg.maxmembernum = jInfo.maxmembernum
	
	--JulebuModel.AddOnline(jInfo.julebuid, cgmsg.userid)
	JulebuModel.LoadMemberList(jInfo.julebuid)
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end