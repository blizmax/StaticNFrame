
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
		--ֻ����Ҫ�����ѵģ�����Ҫ�����︶��
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	if jsInfo.paytype == g_goodsID.jetton then
		--������ý�Ҹ��ѵ�
		if pInfo.jetton < jsInfo.paynum then
			--gcmsg.result = 
			gcmsg.result = ReturnCode["jetton_not_enough"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		PlayerModel.DecJetton(pInfo, jsInfo.paynum, "jingsai", "������(���)")
		
	elseif jsInfo.paytype == g_goodsID.money then
--[[		if g_servername == "run_zsmj" or g_servername == "run_xwpk" then
			--������Ҫ���С�����Ƿ��㹻
			if pInfo.gold > jsInfo.paynum*10 then
				--������С���񸶷�
				PlayerModel.DecGold(pInfo, jsInfo.paynum*10, "jingsai", "������(��)")
			else
				if pInfo.money < jsInfo.paynum then
					gcmsg.result = ReturnCode["money_not_enough"]
					return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
				end
				PlayerModel.DecMoney(pInfo, jsInfo.paynum, "jingsai", "������")
			end
			
		else--]]
		if pInfo.money < jsInfo.paynum then
			gcmsg.result = ReturnCode["money_not_enough"]
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		PlayerModel.DecMoney(pInfo, jsInfo.paynum, "jingsai", "������")			
		--end		
	end
	JingSaiModel.SetBaoMingFei(cgmsg.jingsaiid, cgmsg.userid, jsInfo.paynum)  --������
	PlayerModel.SetPlayerInfo(pInfo)   --�ǵ���Ҫ����
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end