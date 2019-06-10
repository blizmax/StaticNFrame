
module("JulebuGetAward", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cggetjulebuaward()
	local gcmsg = msg_julebu2_pb.gcgetjulebuaward()

	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local jifen = JulebuModel.GetAwardJiFen(cgmsg.julebuid, cgmsg.userid)
	
	if jifen ~= cgmsg.awardcount then
		--LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, cgmsg.userid,  "jifen not match julebuid="..cgmsg.julebuid..", userid="..cgmsg.userid)
	end
	--������Ҫ���ӻ��ֵ���ҵĻ�������
	JulebuModel.AddUserJiFen( cgmsg.julebuid, cgmsg.userid, jifen, -1 )  --��-1����ʾ����ȡ�Ļ���
	
	redisItem:hincrby( JulebuModel.JulebuGetJifen..cgmsg.julebuid, cgmsg.userid, -jifen, JulebuModel.redis_index)
	redisItem:del(JulebuModel.JulebuJifenHistory..cgmsg.julebuid.."_"..cgmsg.userid, JulebuModel.redis_index)

	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end