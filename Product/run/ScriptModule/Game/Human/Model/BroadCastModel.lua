BroadCastModel = {}
BroadCastModel.redis_index = "broad_cast"
BroadCastModel.redisKey = "broad_"

function BroadCastModel.Init()
	
	BroadCastModel.LoadBroadList()
	
end

function BroadCastModel.LoadBroadList()
	local timeStr = TimeUtils.GetTimeString()
	local sqlCase = "select * from dy_broadcast where isexist=1"
	mysqlItem:executeQuery(sqlCase)
	
	local delList = {}
	
	for i = 1, 100 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		
		local diffDay = TimeUtils.DifferentDay(sqlData[6],TimeUtils.GetTableTime())
		
		--这里先判断该系统广播的时效性
		if diffDay >= tonumber(sqlData[8]) then
			--先把对应的ID放到删除的列表中
			table.insert(delList, sqlData[1])
		else
			local addItem = st_human_pb.broadcastinfo()
			addItem.id = tonumber(sqlData[1])
			addItem.broadtype = tonumber(sqlData[4])
			addItem.senderid = tonumber(sqlData[2])
			addItem.sender = sqlData[3]
			addItem.msg = sqlData[5]
			addItem.broadtimes = tonumber(sqlData[7])
			addItem.validday = tonumber(sqlData[8])
			addItem.createtime = sqlData[6]
			addItem.cycletype = tonumber(sqlData[9])
			--保存到对应的循环类型里面
			redisItem:hset(BroadCastModel.redisKey..addItem.cycletype, addItem.id, addItem:SerializeToString(),BroadCastModel.redis_index)
		end
	end
	
	for k,v in ipairs(delList) do
		--把已经过期的推送删除
		local sqlCase = "update dy_broadcast set isexist=0 where id="..v
		mysqlItem:execute(sqlCase)
	end
	
end

function BroadCastModel.LoadBroadByID(ID)
	local sqlCase = "select * from dy_broadcast where isexist=1 and id="..ID
	mysqlItem:executeQuery(sqlCase)
	local sqlData = mysqlItem:fetch({})
	if sqlData == nil then
		return nil
	end	
	
	local addItem = st_human_pb.broadcastinfo()
	addItem.id = tonumber(sqlData[1])
	addItem.broadtype = tonumber(sqlData[4])
	addItem.senderid = tonumber(sqlData[2])
	addItem.sender = sqlData[3]
	addItem.msg = sqlData[5]
	addItem.broadtimes = tonumber(sqlData[7])
	addItem.validday = tonumber(sqlData[8])
	addItem.createtime = sqlData[6]
	addItem.cycletype = tonumber(sqlData[9])
	--保存到对应的循环类型里面
	redisItem:hset(BroadCastModel.redisKey..addItem.cycletype, addItem.id, addItem:SerializeToString(),BroadCastModel.redis_index)

end

function BroadCastModel.Delete(ID)
	
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_login,ID, BroadCastModel.redis_index) then
		return 0
	end
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_1,ID,BroadCastModel.redis_index) then
		return 0
	end
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_10,ID,BroadCastModel.redis_index) then
		return 0
	end
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_30,ID,BroadCastModel.redis_index) then
		return 0
	end
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_60,ID,BroadCastModel.redis_index) then
		return 0
	end
	if 0 ~= redisItem:hdel(BroadCastModel.redisKey..g_broadCast.cycle_24,ID,BroadCastModel.redis_index) then
		return 0
	end	
end

--玩家使用小喇叭发送的广播，需要插到数据库中
function BroadCastModel.PlayerBroad(bItem)
	
	local sqlCase = "insert into dy_broadcast(senderid, sender,broadtype,msg) values("..bItem.senderid..",'"..bItem.sender..","..bItem.broadtype..",'"..bItem.msg.."')"
	
	mysqlItem:execute(sqlCase)
	
end

function BroadCastModel.SendLogin(userID)
	--在玩家登陆后，一次性把登陆需要发送的公告发送给玩家
	
	local gcmsg = msg_human_pb.gcbroadcast()
	
	local strGetList = redisItem:hvals(BroadCastModel.redisKey..g_broadCast.cycle_login, BroadCastModel.redis_index)
	
	if strGetList == nil or #strGetList == 0 then
		return nil
	end
	
	for k,v in ipairs(strGetList) do
		local bItem = st_human_pb.broadcastinfo()
		bItem:ParseFromString(v)
		gcmsg.broadtypelist:append(bItem.broadtype)
		gcmsg.senderidlist:append(bItem.senderid)
		gcmsg.senderlist:append(bItem.sender)
		gcmsg.msglist:append(bItem.msg)
	end
	gcmsg.result = 0
	
	SendMessage(userID,PacketCode[1014].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
end

function BroadCastModel.LoopOne()
		
	local currMin = g_markTime.curr.min
	local lastMin = g_markTime.last.min
	
	local gcmsg = msg_human_pb.gcbroadcast()
	
	local function check(cycleType)
		
		local strGetList = redisItem:hvals(BroadCastModel.redisKey..cycleType, BroadCastModel.redis_index)
		for k,v in ipairs(strGetList) do
			local bItem = st_human_pb.broadcastinfo()
			bItem:ParseFromString(v)

			if bItem.broadtimes > 0 then
				bItem.broadtimes = bItem.broadtimes - 1;
				redisItem:hset(BroadCastModel.redisKey..cycleType, bItem.id, bItem:SerializeToString(),BroadCastModel.redis_index)
			elseif bItem.broadtimes == 0 then
				local sqlCase = "update dy_broadcast set broadtimes=0 and isexist=0 where id="..bItem.id
				mysqlItem:execute(sqlCase)
				--SqlServer.rpush(sqlCase)
				redisItem:hdel(BroadCastModel.redisKey..cycleType, bItem.id,BroadCastModel.redis_index)				
			end
			gcmsg.broadtypelist:append(bItem.broadtype)
			gcmsg.senderidlist:append(bItem.senderid)
			gcmsg.senderlist:append(bItem.sender)
			gcmsg.msglist:append(bItem.msg)
		end
	end
	
	
	
	if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里表示是十分钟循环的一个判断
		check(g_broadCast.cycle_1)
		
	end
	
	if 0 == math.mod(currMin, 10) and 9==math.mod(lastMin,10) then
	--if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里表示是十分钟循环的一个判断
		check(g_broadCast.cycle_10)
		
	end
	
	if 0 == math.mod(currMin,30) and 29 == math.mod(lastMin,29) then
	--if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里是半个小时一个循环的判断
		check(g_broadCast.cycle_30)
	end
	
	if 0 == currMin and 59 == lastMin then
		--这里是一个小时的判断
		check(g_broadCast.cycle_60)
	end
	
	if g_markTime.last.hour ~= g_markTime.curr.hour then
	--if lastMin ~= currMin then  --调试，暂时设置一分钟
		--这里是每天的判断
		check(g_broadCast.cycle_24)
	end
	

	if #gcmsg.senderlist <= 0 then
		return nil
	end
	
	BroadCastModel.SendToAll(gcmsg)
end

function BroadCastModel.InsertItem(bItem, retType)
	local sqlCase = "insert into dy_broadcast(senderid,sender,broadtype,msg,createdate,broadtimes,validday,cycletype) values("..
		bItem.senderid..",'"..bItem.sender.."',"..bItem.broadtype..",'"..bItem.msg.."','"..bItem.createtime.."',"..bItem.broadtimes..","..bItem.validday..","..bItem.cycletype..")"
		
	local ret = mysqlItem:execute(sqlCase)

	if retType == nil or retType == 0 then
		return ret
	end
	
	sqlCase = "select id from dy_broadcast where createdate='"..bItem.createtime.."' and broadtype="..bItem.broadtype
	
	mysqlItem:executeQuery(sqlCase)
	
	local sqlData = mysqlItem:fetch()
	return tonumber(sqlData)
	
end


function BroadCastModel.SendToAll(gcmsg)
	local onList = OnlineModel.GetOnlineList(-1)
	if #onList == 0 then
		return nil
	end
	
	SendMessage(onList,PacketCode[1014].client,gcmsg:ByteSize(),gcmsg:SerializeToString())
end

function BroadCastModel.FruitNotice(msg)
	local gcmsg = msg_human_pb.gcbroadcast()
	gcmsg.broadtypelist:append(g_broadCast.type_fruit)
	gcmsg.senderidlist:append(1)
	gcmsg.senderlist:append("水果小王子")
	gcmsg.msglist:append(msg)
	BroadCastModel.SendToAll(gcmsg)
end