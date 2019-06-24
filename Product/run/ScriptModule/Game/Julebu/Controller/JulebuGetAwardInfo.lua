
module("JulebuGetAwardInfo", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cggetjulebuawardinfo()
	local gcmsg = msg_julebu2_pb.gcgetjulebuawardinfo()
	
	cgmsg:ParseFromString(buffer)
	--luaDump(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	

	local sqlCase = "select qrcodeurl from dy_julebu_member where userid="..cgmsg.userid.." and julebuid="..cgmsg.julebuid
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch()
	
	if sqlData == nil or #sqlData == 0 then

		local ret = HttpPost( "http://www.ya76r.cn/index.php/"..g_gamename.."/client/makecode", "userid="..cgmsg.userid.."&julebuid="..cgmsg.julebuid )
	end
	gcmsg.qrcodeurl = "http://www.ya76r.cn/Uploads/"..g_gamename.."/julebu/"..cgmsg.julebuid.."_"..cgmsg.userid..".jpg"
	gcmsg.awardcount = JulebuModel.GetAwardJiFen(cgmsg.julebuid, cgmsg.userid)

	sqlCase = "select count(*) from dy_julebu_member where julebuid="..cgmsg.julebuid.." and higherid="..cgmsg.userid
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch()
	if sqlData ~= nil then
		gcmsg.usernum = tonumber(sqlData)
	end

	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end