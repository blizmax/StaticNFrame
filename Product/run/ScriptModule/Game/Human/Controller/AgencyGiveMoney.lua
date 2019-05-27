module("AgencyGiveMoney", package.seeall)

-- 代理赠送钻石
-- wyh 2018-3-13 21:09:44

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cggivemoney()
	local gcmsg = msg_human_pb.gcgivemoney()
	cgmsg:ParseFromString(buffer)
	
	-- 检查权限
	local pAgyInfo = PlayerModel.GetPlayerInfo(cgmsg.userid, gcmsg.pinfo)
	local agency = AgencyModel.GetAgencyInfo(cgmsg.userid, gcmsg.agyinfo)
	if nil == pAgyInfo or nil == agency then
		gcmsg.result = ReturnCode["not_agency"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if g_agencyLevel.groupMaster > agency.level then
		gcmsg.result = ReturnCode["inadequate_permissions"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	
	if 0 >= cgmsg.amount then
		-- 参数错误
		gcmsg.result = ReturnCode["givemoney_amount_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	-- 判断money是否足够	
	if cgmsg.amount > pAgyInfo.money then
		gcmsg.result = ReturnCode["money_not_enough"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	-- 判断目标玩家是否存在
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.touserid)
	if nil == pInfo then
		gcmsg.result = ReturnCode["player_not_exist"]
	elseif pInfo.userid == pAgyInfo.userid then
		gcmsg.result = ReturnCode["givemoney_not_self"]
	else
		gcmsg.result = 0
		AgencyModel.GiveMoney(pAgyInfo, pInfo, cgmsg.amount)
		PlayerModel.SendPlayerInfo(pAgyInfo,{"money"})
		PlayerModel.SendPlayerInfo(pInfo,{"money"})

		gcmsg.record.userid = cgmsg.touserid
		gcmsg.record.nickname = pInfo.nickname or ""
		gcmsg.record.amount = cgmsg.amount
		gcmsg.record.timemark = TimeUtils.GetTimeString()
		
	end

	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end