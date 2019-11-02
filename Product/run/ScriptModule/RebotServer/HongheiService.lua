HongheiService = HongheiService or {}

function HongheiService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        local tabletype = math.myrandom(1,2)
        cgmsg.tableid = g_hongheiDefine.init_data[tabletype]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

HongHeiEnter = HongHeiEnter or {}

function HongHeiEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_honghei_pb.gchongheienter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.tableid
        local state = gcmsg.state
        local timemark = gcmsg.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)
    end
end

HongHeiLeave = HongHeiLeave or {}
function HongHeiLeave.execute(nMsgId, clientId, strMsg)

end

HongHeiSitDown = HongHeiSitDown or {}
function HongHeiSitDown.execute(nMsgId, clientId, strMsg)

end

HongHeiPourJetton = HongHeiPourJetton or {}
function HongHeiPourJetton.execute(nMsgId, clientId, strMsg)

end

HongHeiCount = HongHeiCount or {}
function HongHeiCount.execute(nMsgId, clientId, strMsg)

end

HongHeiTableList = HongHeiTableList or {}
function HongHeiTableList.execute(nMsgId, clientId, strMsg)

end

HongHeiBeingBanker = HongHeiBeingBanker or {}
function HongHeiBeingBanker.execute(nMsgId, clientId, strMsg)

end

HongHeiDropBanker = HongHeiDropBanker or {}
function HongHeiDropBanker.execute(nMsgId, clientId, strMsg)

end

HongHeiStart = HongHeiStart or {}
function HongHeiStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_honghei_pb.gchongheistart()
    gcmsg:ParseFromString(strMsg)

    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)

    if tableid == nil then
        return
    end
    
    local timeData= {}
    timeData['tableid'] = tostring(tableid)
    timeData['userid'] = tostring(userid)
    timeData['clientId'] = tostring(clientId)

    for i = 1, 15 do
        local rand = math.myrandom(1, 1000)
        local rand2 = math.myrandom(1, 100)
        if rand2 < 30 then
            processTimer(i*1000+rand, "HongHeiTimer", luajson.encode(timeData))
        end
    end
end

HongHeiTimer = HongHeiTimer or {}
function HongHeiTimer.execute(buffer)
	--通知下注的这里，不需要加入锁，这里只是一个通知的过程
	
    local jsonData = luajson.decode(buffer)   --这里只是做业务的分发就行
    HongheiService.SendPourJetton(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
end

function HongheiService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_honghei_pb.cghongheipourjetton()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.jetton = 100
    local pourType = {1,2,3}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourtype = pourType[index]

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 3707, cgmsg:ByteSize(), cgmsg:SerializeToString())

    RebotModel.RecordPlayerSendPour(userid)
    --LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

HongHeiUpdate = HongHeiUpdate or {}
function HongHeiUpdate.execute(nMsgId, clientId, strMsg)

end

HongHeiStandUp = HongHeiStandUp or {}
function HongHeiStandUp.execute(nMsgId, clientId, strMsg)

end

HongHeiGroupJetton = HongHeiGroupJetton or {}
function HongHeiGroupJetton.execute(nMsgId, clientId, strMsg)

end

HongHeiHistory = HongHeiHistory or {}
function HongHeiHistory.execute(nMsgId, clientId, strMsg)

end

HongHeiChat = HongHeiChat or {}
function HongHeiChat.execute(nMsgId, clientId, strMsg)

end

HongHeiStandList = HongHeiStandList or {}
function HongHeiStandList.execute(nMsgId, clientId, strMsg)

end

HongHeiBankerList = HongHeiBankerList or {}
function HongHeiBankerList.execute(nMsgId, clientId, strMsg)

end

HongHeiStats = HongHeiStats or {}
function HongHeiStats.execute(nMsgId, clientId, strMsg)

end

HongHeiAllGameHistory = HongHeiAllGameHistory or {}
function HongHeiAllGameHistory.execute(nMsgId, clientId, strMsg)

end

HongHeiGameHistoryUpdata = HongHeiGameHistoryUpdata or {}
function HongHeiGameHistoryUpdata.execute(nMsgId, clientId, strMsg)

end

HongHeiPourJettonXt = HongHeiPourJettonXt or {}
function HongHeiPourJettonXt.execute(nMsgId, clientId, strMsg)

end