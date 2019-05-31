
module("JingSaiPay", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaipay()
	local gcmsg = msg_jingsai_pb.gcjingsaipay()
	
	cgmsg:ParseFromString(buffer)
	
	local jsInfo = JingSaiModel.GetJingSaiInfo(cgmsg.jingsaiid)
	gcmsg.result = 0
	if jsInfo == nil then
		gcmsg.result = ReturnCode["jingsai_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	if jsInfo.payway ~= g_gamePayWay.pay_baoming then
		--只有需要报名费的，才需要在这里付费
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	if jsInfo.paytype == g_goodsID.jetton then
		--这个是用金币付费的
		if pInfo.jetton < jsInfo.paynum then
			--gcmsg.result = 
			gcmsg.result = ReturnCode["jetton_not_enough"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		PlayerModel.DecJetton(pInfo, jsInfo.paynum, "jingsai", "报名费(金币)")
		
	elseif jsInfo.paytype == g_goodsID.money then
--[[		if g_servername == "run_zsmj" or g_servername == "run_xwpk" then
			--这里需要检查小蛙玉是否足够
			if pInfo.gold > jsInfo.paynum*10 then
				--这里用小蛙玉付费
				PlayerModel.DecGold(pInfo, jsInfo.paynum*10, "jingsai", "报名费(玉)")
			else
				if pInfo.money < jsInfo.paynum then
					gcmsg.result = ReturnCode["money_not_enough"]
					return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
				end
				PlayerModel.DecMoney(pInfo, jsInfo.paynum, "jingsai", "报名费")
			end
			
		else--]]
		if pInfo.money < jsInfo.paynum then
			gcmsg.result = ReturnCode["money_not_enough"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		PlayerModel.DecMoney(pInfo, jsInfo.paynum, "jingsai", "报名费")			
		--end		
	end
	JingSaiModel.SetBaoMingFei(cgmsg.jingsaiid, cgmsg.userid, jsInfo.paynum)  --报名费
	PlayerModel.SetPlayerInfo(pInfo)   --记得需要设置
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end