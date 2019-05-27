
module("JulebuGetJimitJiFenHistory", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebugetjimitjifenhistory()
	local gcmsg = msg_julebu2_pb.gcjulebugetjimitjifenhistory()
	
	cgmsg:ParseFromString(buffer)
	if nil == JulebuModel.GetJulebuInfo(cgmsg.julebuid) then
		--俱乐部不存在
		--只有群主才能设置管理员
		gcmsg.result = ReturnCode["jlb_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()		
	end
	
	local historyList = JulebuModel.GetPlayerHistory(cgmsg.julebuid, cgmsg.recordtype)
	for k,v in ipairs(historyList) do
		local tmpInfo = st_julebu_pb.julebujimitjifeninfo()
		tmpInfo:ParseFromString(v)
		local tmptime = TimeUtils.GetTime()
		if 259200 > tmptime - tmpInfo.timemark then
			if cgmsg.requesttype == 1 then 
				local Item = gcmsg.infolist:add()
				Item.executorid = tmpInfo.executorid
				Item.targetid = tmpInfo.targetid
				Item.count = tmpInfo.count
				Item.timemark = tmpInfo.timemark
				local pInfo = PlayerModel.GetPlayerInfo(tmpInfo.executorid)
				Item.executornic = pInfo.nickname
			elseif cgmsg.requesttype == 2 then	
				if tmpInfo.executorid == cgmsg.targetid then
					local Item = gcmsg.infolist:add()
					Item.executorid = tmpInfo.executorid
					Item.targetid = tmpInfo.targetid
					Item.count = tmpInfo.count
					Item.timemark = tmpInfo.timemark
					local pInfo = PlayerModel.GetPlayerInfo(tmpInfo.executorid)
					Item.executornic = pInfo.nickname
				end
			end
		end
	end	
	
	gcmsg.recordtype = cgmsg.recordtype
	gcmsg.requesttype = cgmsg.requesttype
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end