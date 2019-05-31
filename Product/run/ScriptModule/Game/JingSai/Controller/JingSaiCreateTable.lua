
module("JingSaiCreateTable", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_jingsai_pb.cgjingsaicreatetable()
	local gcmsg = msg_jingsai_pb.gcjingsaicreatetable()
	
	cgmsg:ParseFromString(buffer)
	
	local jsInfo = JingSaiModel.GetJingSaiInfo(cgmsg.jingsaiid)
	
	if jsInfo == nil then
		gcmsg.result = ReturnCode["jingsai_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
		
	if jsInfo.state == g_jingSaiDefine.state_ready then
		gcmsg.result = ReturnCode["jingsai_is_ready"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	if jsInfo.state ~= g_jingSaiDefine.state_start then
		gcmsg.result = ReturnCode["jingsai_is_counting"] --比赛已经结束
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--在这里检查是否需要付费
	if jsInfo.payway == g_gamePayWay.pay_baoming then
		--这里是的需要报名费的
		--检修一下是否已经付费
		if false == JingSaiModel.IsBaoMing(cgmsg.jingsaiid, cgmsg.userid) then
			--就是还没有交报名费,就需要提交报名费
			gcmsg.payway = jsInfo.payway
			gcmsg.paytype = jsInfo.paytype
			gcmsg.paynum = jsInfo.paynum
			gcmsg.result = 0
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
	end
	
	
	
	local jlbInfo = JulebuModel.GetJulebuInfo(jsInfo.julebuid)
	if jlbInfo == nil then
		gcmsg.result = ReturnCode["jingsai_del_fail"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	local tableList = redisItem:lrange(JingSaiModel.game_list..cgmsg.jingsaiid, 0, -1, JingSaiModel.redis_index)

	--看看有那个牌桌是空余的牌桌
	local numCount = 0
	local inList = {}               --不管怎么样，先填满之前的牌桌先
	for k,v in ipairs(tableList) do
		local strGet = JingSaiModel.GetGameInfo(cgmsg.jingsaiid, v)
		
		if strGet ~= nil then
			local jsGame = st_jingsai_pb.jingsaitable()
			jsGame:ParseFromString(strGet)
			if jsGame.state == g_jingSaiDefine.table_ready then
				numCount = numCount + 1
			end
			if jsGame.maxusernum > jsGame.usernum then
				--取到没满桌的列表
				table.insert(inList, jsGame.tableid)
			end
		end
	end

	local enterTableID = false
	if #inList > 2 then
		--先安排进入新的房间
		--
		--一定不会开新房
		enterTableID = true
	elseif #inList == 2 then
		local random = math.myrandom(1,10000)
		if random < 8000 then
			enterTableID = true
		end
	elseif #inList == 1 then
		local random = math.myrandom(1,10000)
		if random < 6000 then
			enterTableID = true
		end
	end
	
	if enterTableID == true then
		--在这里就是发送进入牌桌的协议，
		local random = math.myrandom(1,#inList)
		local gcEnter = msg_human_pb.cgentertable()
		gcEnter.userid = cgmsg.userid
		gcEnter.tableid = tonumber(inList[random])
		
		local retPacketID, controller = tcpManager:createController(1033)
		
		if controller~= nil then
			local playerID, retCode, retBufferLen, retString, otString = controller.execute(1033, 1033, gcEnter:SerializeToString())
			if retCode == 0 then
				return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
			end
		end
	end
	if numCount >= 3 then
		gcmsg.result = ReturnCode["jingsai_table_max"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--开始创建房间
	local cgCreate = nil
	local tempPacketID = 0
	if jsInfo.moduletype == g_JulebuDefine.modules_gdmj then
		tempPacketID = 2205
		cgCreate = msg_gdmj_pb.cggdmjcreate()


	elseif jsInfo.moduletype == g_JulebuDefine.modules_douniu then
		tempPacketID = 3003
		cgCreate = msg_douniu_pb.cgdouniucreate()
	elseif jsInfo.moduletype == g_JulebuDefine.modules_sangong then
		tempPacketID = 3119
		cgCreate = msg_sangong_pb.cgsangongcreatetable()
	elseif jsInfo.moduletype == g_JulebuDefine.modules_psz then
		tempPacketID = 3303
		cgCreate = msg_psz_pb.cgpszcreate()
	elseif jsInfo.moduletype == g_JulebuDefine.modules_doudizhu then
		tempPacketID = 3903
		cgCreate = msg_ddz_pb.cgddzcreate()
	elseif jsInfo.moduletype == g_JulebuDefine.modules_paijiu then
		tempPacketID = 3903
		cgCreate = msg_paijiu_2_pb.cgpaijiucreate()
		
	end


	local retPacketID, controller = tcpManager:createController(tempPacketID)
	if nil ~= controller then
		cgCreate:ParseFromString(jsInfo.strcreate)
		cgCreate.julebuid = jsInfo.jingsaiid
		cgCreate.userid = cgmsg.userid
		cgCreate.payway = jsInfo.payway
		--if jsInfo.
		cgCreate.payway = jsInfo.payway  --支付类型
		local playerID, retCode, retBufferLen, retString, otString = controller.execute(tempPacketID, tempPacketID, cgCreate:SerializeToString())
		
	end
	gcmsg.result = 0
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end


