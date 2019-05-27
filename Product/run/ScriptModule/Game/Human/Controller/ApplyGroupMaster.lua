module("ApplyGroupMaster", package.seeall)

-- 申请群主
local needMoney = 100
function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgapplygroupmaster()
	local gcmsg = msg_human_pb.gcapplygroupmaster()
	cgmsg:ParseFromString(buffer)
	
	repeat
		local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid, gcmsg.pinfo)
		if nil == pInfo then
			gcmsg.result = ReturnCode["account_not_exist"]
			break
		end

		if needMoney > pInfo.money then
			gcmsg.result = ReturnCode["money_not_enough"]
			break
		end
		
		local agyInfo = AgencyModel.GetAgencyInfo(cgmsg.userid, gcmsg.agyinfo)
		if agyInfo then
			if g_agencyLevel.groupMaster <= agyInfo.level then
				gcmsg.result = ReturnCode["already_have_access"]
				break
			end
			
			-- 更新代理等级
			local sqlCase = "update ag_player set level="..g_agencyLevel.groupMaster.." where userid="..pInfo.userid
			--SqlServer.rpush(sqlCase)
			mysqlItem:execute(sqlCase)
		else
			-- 创建代理
			local sqlCase = "insert into ag_player(userid, account, expandcode, state, level) values("
				..pInfo.userid..", "..pInfo.userid..", "..pInfo.userid..", 1, "..g_agencyLevel.groupMaster..")"
			--SqlServer.rpush(sqlCase)
			mysqlItem:execute(sqlCase)
		end
		
		-- 重新拉取代理信息
		agyInfo = AgencyModel.LoadAgencyInfo(pInfo.userid)
		if nil == agyInfo or g_agencyLevel.groupMaster > agyInfo.level then
			gcmsg.result = ReturnCode["apply_group_master_error"]
			break
		end
		
		-- 扣除钻石
		PlayerModel.DecMoney(pInfo, needMoney, "ApplyGroupMaster", "申请群主")
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo,{"money"})
		gcmsg.result = 0
	until(true)
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end