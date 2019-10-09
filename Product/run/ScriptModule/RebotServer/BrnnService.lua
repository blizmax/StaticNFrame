BrnnService = BrnnService or {}

function BrnnService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        local tabletype = math.myrandom(1,2)
        cgmsg.tableid = g_brnnDefine.init_data[tabletype]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

BrnnEnter = BrnnEnter or {}

function BrnnEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_brnn_pb.gcbrnnenter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.tableid
        local state = gcmsg.state
        local timemark = gcmsg.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)
    end
end

BrnnLeave = BrnnLeave or {}
function BrnnLeave.execute(nMsgId, clientId, strMsg)

end

BrnnSitDown = BrnnSitDown or {}
function BrnnSitDown.execute(nMsgId, clientId, strMsg)

end

BrnnPourJetton = BrnnPourJetton or {}
function BrnnPourJetton.execute(nMsgId, clientId, strMsg)

end

BrnnCount = BrnnCount or {}
function BrnnCount.execute(nMsgId, clientId, strMsg)

end

BrnnTableList = BrnnTableList or {}
function BrnnTableList.execute(nMsgId, clientId, strMsg)

end

BrnnBeingBanker = BrnnBeingBanker or {}
function BrnnBeingBanker.execute(nMsgId, clientId, strMsg)

end

BrnnDropBanker = BrnnDropBanker or {}
function BrnnDropBanker.execute(nMsgId, clientId, strMsg)

end

BrnnStart = BrnnStart or {}
function BrnnStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_brnn_pb.gcbrnnstart()
    gcmsg:ParseFromString(strMsg)

    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)

    local timeData= {}
    timeData['tableid'] = tostring(tableid)
    timeData['userid'] = tostring(userid)
    timeData['clientId'] = tostring(clientId)

    for i = 1, 15 do
        local rand = math.myrandom(1, 1000)
        local rand2 = math.myrandom(1, 100)
        if rand2 < 30 then
            processTimer(i*1000+rand, "BrnnTimer", luajson.encode(timeData))
        end
    end
end

BrnnTimer = BrnnTimer or {}
function BrnnTimer.execute(buffer)
	--通知下注的这里，不需要加入锁，这里只是一个通知的过程
	
    local jsonData = luajson.decode(buffer)   --这里只是做业务的分发就行
    BrnnService.SendPourJetton(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
end

function BrnnService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_brnn_pb.cgbrnnpourjetton()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.jetton = 100
    local pourType = {1,2,3,4}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourtype = pourType[index]

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 2507, cgmsg:ByteSize(), cgmsg:SerializeToString())

    RebotModel.RecordPlayerSendPour(userid)
    --LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

BrnnUpdate = BrnnUpdate or {}
function BrnnUpdate.execute(nMsgId, clientId, strMsg)

end

BrnnStandUp = BrnnStandUp or {}
function BrnnStandUp.execute(nMsgId, clientId, strMsg)

end

BrnnGroupJetton = BrnnGroupJetton or {}
function BrnnGroupJetton.execute(nMsgId, clientId, strMsg)

end

BrnnHistory = BrnnHistory or {}
function BrnnHistory.execute(nMsgId, clientId, strMsg)

end

BrnnChat = BrnnChat or {}
function BrnnChat.execute(nMsgId, clientId, strMsg)

end

BrnnStandList = BrnnStandList or {}
function BrnnStandList.execute(nMsgId, clientId, strMsg)

end

BrnnBankerList = BrnnBankerList or {}
function BrnnBankerList.execute(nMsgId, clientId, strMsg)

end

BrnnStats = BrnnStats or {}
function BrnnStats.execute(nMsgId, clientId, strMsg)

end

BrnnAllGameHistory = BrnnAllGameHistory or {}
function BrnnAllGameHistory.execute(nMsgId, clientId, strMsg)

end

BrnnGameHistoryUpdata = BrnnGameHistoryUpdata or {}
function BrnnGameHistoryUpdata.execute(nMsgId, clientId, strMsg)

end