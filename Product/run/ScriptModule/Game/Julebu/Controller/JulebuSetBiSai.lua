
module("JulebuSetBiSai", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebusetbisai()
	local gcmsg = msg_julebu2_pb.gcjulebusetbisai()
	
	cgmsg:ParseFromString(buffer)
	local jinfo = JulebuModel.GetJulebuInfo(cgmsg.julebuid)
	if nil == jinfo then
		--俱乐部不存在
		--只有群主才能设置管理员
		jinfo.julebuid = 0
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	--查看玩家是否是管理员
	local len = #jinfo.adminlist
	local isAdmin = false
	for i = len,1,-1 do
		--从后面往回看
		if jinfo.adminlist[i] == cgmsg.userid then
			isAdmin = true
		end
	end
	
	if isAdmin == false then
		gcmsg.result = ReturnCode["jlb_setbisai_error1"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	if cgmsg.requesttype == 1 then
		jinfo.gamecount = cgmsg.gamecount
		jinfo.bankercount = cgmsg.bankercount
		jinfo.ruleset = cgmsg.ruleset
		jinfo.faceaward_figure = cgmsg.faceaward_figure
		jinfo.faceaward_num = cgmsg.faceaward_num
		jinfo.faceaward_count = cgmsg.faceaward_count
	else
		gcmsg.gamecount = jinfo.gamecount
		gcmsg.bankercount = jinfo.bankercount
		gcmsg.ruleset = jinfo.ruleset
		gcmsg.faceaward_figure = jinfo.faceaward_figure
		gcmsg.faceaward_num = jinfo.faceaward_num
		gcmsg.faceaward_count = jinfo.faceaward_count
	end
	
	JulebuModel.SetJulebuInfo(jinfo)
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end