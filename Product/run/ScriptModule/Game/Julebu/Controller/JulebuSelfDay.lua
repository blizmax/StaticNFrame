
module("JulebuSelfDay", package.seeall)
--获取天的战绩

function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebuselfday()
	local gcmsg = msg_julebu_pb.gcjulebuselfday()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "julebu", "JulebuSelfDay", 0, "重复获取，userid="..cgmsg.userid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	if cgmsg.daynum == 1 then
		--如果是第一天的时候，需要获取的是实时的
		local getList = JulebuModel.GetdayUserZhanJi(cgmsg.julebuid, cgmsg.userid, cgmsg.pagenum)
		for k,v in ipairs(getList) do
			local addItem = gcmsg.zjlist:add()
			addItem:ParseFromString(v)
		end
	else
		local getList = JulebuModel.GetdayUserZhanJi2(cgmsg.julebuid, cgmsg.userid, cgmsg.timemark, cgmsg.pagenum)
		for k,v in ipairs(getList) do
			local addItem = gcmsg.zjlist:add()
			addItem:ParseFromString(v)
		end		
	end
	
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end