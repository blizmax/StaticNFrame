
ChatModel = {}
ChatModel.redis_index = "redis_chat"
function ChatModel.GetRedisKey(userID)
	return "chat_"..userID
end

function ChatModel.Push(userID, cInfo)
	redisItem:lpush(ChatModel.GetRedisKey(userID), cInfo:SerializeToString(), ChatModel.redis_index)
end


function ChatModel.LoginCheck(userID)
	--登录的时候，检查有没有聊天信息
	local redisKey = ChatModel.GetRedisKey(userID)
	local len = redisItem:llen(redisKey, ChatModel.redis_index)

	local gcmsg = progame_pb.gcchat()
	
	for i = 1,len do
		local cInfo = proplayer_pb.chatinfo()
		local strGet = redisItem:lpop(redisKey, ChatModel.redis_index)
		cInfo:ParseFromString(strGet)
	end
	
	
	if len > 0 then
		gcmsg.result = 0
		SendMessage(userID, gcmsg,PacketCode[88].client,7777, gcmsg:ByteSize(),gcmsg:SerializeToString())
	end
end