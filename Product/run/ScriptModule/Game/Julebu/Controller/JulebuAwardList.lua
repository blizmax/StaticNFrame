
module("JulebuAwardList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu2_pb.cgjulebuawardlist()
	local gcmsg = msg_julebu2_pb.gcjulebuawardlist() 
	
	cgmsg:ParseFromString(buffer)
	
	
	local getList = redisItem:hgetall( JulebuModel.JulebuJifenHistory..cgmsg.julebuid.."_"..cgmsg.userid,  JulebuModel.redis_index)
	
	local count = 0
	for k,v in pairs( getList ) do
		count = count + 1
		
		if count > (cgmsg.pagenum - 1)*10 and  count <= cgmsg.pagenum*10 then
			local pInfo = PlayerModel.GetPlayerInfo( tonumber(k) )
			gcmsg.userid:append(tonumber(k))
			gcmsg.nickname:append(pInfo.nickname)
			gcmsg.awardcount:append(tonumber(v))
		elseif count > (cgmsg.pagenum + 1)*10 then
			break
		end
	end
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
	
end