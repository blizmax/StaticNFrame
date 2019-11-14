FenFenCaiService = FenFenCaiService or {}

function FenFenCaiService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        cgmsg.tableid = 2271001
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

FenFenCaiEnter = FenFenCaiEnter or {}
function FenFenCaiEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_fenfencai_pb.gcfenfencaienter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.tableid
        local state = gcmsg.state
        local timemark = gcmsg.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)
    end
end

FenFenCaiSitDown = FenFenCaiSitDown or {}
function FenFenCaiSitDown.execute(nMsgId, clientId, strMsg)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)
    local rand = math.myrandom(1, 100)
    if rand < 20 then
        FenFenCaiService.LeaveGame(clientId, userid, tableid)
    end
end

FenFenCaiStandUp = FenFenCaiStandUp or {}
function FenFenCaiStandUp.execute(nMsgId, clientId, strMsg)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)
    local rand = math.myrandom(1, 100)
    if rand < 20 then
        FenFenCaiService.LeaveGame(clientId, userid, tableid)
    end
end

FenFenCaiLeave = FenFenCaiLeave or {}
function FenFenCaiLeave.execute(nMsgId, clientId, strMsg)

end

FenFenCaiPourJetton = FenFenCaiPourJetton or {}
function FenFenCaiPourJetton.execute(nMsgId, clientId, strMsg)

end

FenFenCaiGroupJetton = FenFenCaiGroupJetton or {}
function FenFenCaiGroupJetton.execute(nMsgId, clientId, strMsg)

end

FenFenCaiPourJettonXt = FenFenCaiPourJettonXt or {}
function FenFenCaiPourJettonXt.execute(nMsgId, clientId, strMsg)

end

FenFenCaiCount = FenFenCaiCount or {}
function FenFenCaiCount.execute(nMsgId, clientId, strMsg)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)
    local rand = math.myrandom(1, 100)
    if rand < 20 then
        FenFenCaiService.LeaveGame(clientId, userid, tableid)
    end
end

FenFenCaiBeingBanker = FenFenCaiBeingBanker or {}
function FenFenCaiBeingBanker.execute(nMsgId, clientId, strMsg)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)
    local rand = math.myrandom(1, 100)
    if rand < 20 then
        FenFenCaiService.LeaveGame(clientId, userid, tableid)
    end
end

FenFenCaiDropBanker = FenFenCaiDropBanker or {}
function FenFenCaiDropBanker.execute(nMsgId, clientId, strMsg)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local tableid = RebotModel.GetPlayerTableId(userid)
    local rand = math.myrandom(1, 100)
    if rand < 20 then
        FenFenCaiService.LeaveGame(clientId, userid, tableid)
    end
end

FenFenCaiStart = FenFenCaiStart or {}
function FenFenCaiStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_fenfencai_pb.gcfenfencaistart()
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

    for i = 1, tonumber(gcmsg.timemark) -1 do
        local rand = math.myrandom(1, 1000)
        local rand2 = math.myrandom(1, 100)
        if rand2 < 30 then
            processTimer(i*1000+rand, "FenFenCaiTimer", luajson.encode(timeData))
        end
    end
end

FenFenCaiTimer = FenFenCaiTimer or {}
function FenFenCaiTimer.execute(buffer)
	--通知下注的这里，不需要加入锁，这里只是一个通知的过程
    
    
    local jsonData = luajson.decode(buffer)   --这里只是做业务的分发就行
    local rand = math.myrandom(1, 100)
    if rand > 50 then
        FenFenCaiService.SendPourJetton(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
    else
        FenFenCaiService.LeaveGame(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
    end
end

function FenFenCaiService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_fenfencai_pb.cgfenfencaipourjetton()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.jetton = 1000
    local pourType = {1,2,3,4, 5, 6, 7}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourtype = pourType[index]

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 7209, cgmsg:ByteSize(), cgmsg:SerializeToString())

    RebotModel.RecordPlayerSendPour(userid)
    --LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

function FenFenCaiService.LeaveGame(clientId, userid, tableid)
    local cgmsg = msg_fenfencai_pb.cgfenfencaileave()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 7207, cgmsg:ByteSize(), cgmsg:SerializeToString())
    RebotModel.SetPlayerTableId(userid, 0)
end

FenFenCaiHistory = FenFenCaiHistory or {}
function FenFenCaiHistory.execute(nMsgId, clientId, strMsg)

end

FenFenCaiChat = FenFenCaiChat or {}
function FenFenCaiChat.execute(nMsgId, clientId, strMsg)

end

FenFenCaiStandList = FenFenCaiStandList or {}
function FenFenCaiStandList.execute(nMsgId, clientId, strMsg)

end

FenFenCaiBankerList = FenFenCaiBankerList or {}
function FenFenCaiBankerList.execute(nMsgId, clientId, strMsg)

end

FenFenCaiSendgetResult = FenFenCaiSendgetResult or {}
function FenFenCaiSendgetResult.execute(nMsgId, clientId, strMsg)

end

FenFenCaiUpdate = FenFenCaiUpdate or {}
function FenFenCaiUpdate.execute(nMsgId, clientId, strMsg)

end

FenFenCaiUserHistory = FenFenCaiUserHistory or {}
function FenFenCaiUserHistory.execute(nMsgId, clientId, strMsg)

end

FenFenCaiGetHangUpState = FenFenCaiGetHangUpState or {}
function FenFenCaiGetHangUpState.execute(nMsgId, clientId, strMsg)

end

FenFenCaiSetHangUpState = FenFenCaiSetHangUpState or {}
function FenFenCaiSetHangUpState.execute(nMsgId, clientId, strMsg)

end