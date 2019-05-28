--进入小马快跑


module("EnterTable", package.seeall)

--总共六位数的房间号：100000 ~ 999999  最大数字数这个

--10W ~ 12W   是广东麻将的
--12W ~ 14W   是湖北麻将的
--14W ~ 15W   是牛牛的
--15W ~ 16W   是欢乐赢豆的
--16W ~ 17W   是火锅英雄的
--17W ~ 18W   是竞技场
--18W ~ 19W   是十点半
--19W ~ 20W   是斗牛

function execute(packetID, operateID, buffer)

	local cgmsg = msg_human_pb.cgentertable()
	local gcmsg = msg_human_pb.gcentertable()
	cgmsg:ParseFromString(buffer)
	
	local controller = nil
	local retPacketID = 0
	local tempPacketID = 0
	if cgmsg.tableid < 100000 then
		
	elseif cgmsg.tableid < 120000 then
		tempPacketID = 2201
	elseif cgmsg.tableid < 140000 then
		tempPacketID = 2801
	elseif cgmsg.tableid < 150000 then
	elseif cgmsg.tableid < 160000 then
		tempPacketID = 2001
	elseif cgmsg.tableid < 170000 then
		tempPacketID = 2701
	elseif cgmsg.tableid < 180000 then
	elseif cgmsg.tableid < 190000 then
		tempPacketID = 2901
	elseif cgmsg.tableid < 200000 then
		tempPacketID = 3010
	elseif cgmsg.tableid < 210000 then
		tempPacketID = 3101
	elseif cgmsg.tableid < 220000 then
		tempPacketID = 3310
	elseif cgmsg.tableid < 230000 then
		tempPacketID = 3906
	elseif cgmsg.tableid < 240000 then
		tempPacketID = 4010
	elseif cgmsg.tableid < 270000 then
		tempPacketID = 5106
	elseif cgmsg.tableid < 20200000 then
		
	elseif cgmsg.tableid < 20300000 then
		tempPacketID = 2201
	end
	
	retPacketID, controller = TcpManager.CreateController(tempPacketID)
	
	if controller ~= nil then
		
		local playerID, retCode, retBufferLen, retString, otString = controller.execute(tempPacketID, operateID, buffer, true)
		SendMessage(cgmsg.userid,retPacketID, retBufferLen, retString)
	else
		gcmsg.result = ReturnCode["table_not_exist"]
		return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end