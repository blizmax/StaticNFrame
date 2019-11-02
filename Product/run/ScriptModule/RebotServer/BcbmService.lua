BcbmService = BcbmService or {}

function BcbmService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        cgmsg.tableid = g_bcbmDefine.init_data[1]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function BcbmService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_bcbm_pb.cgbcbmpour()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.pourjetton = 100
    local pourType = {1,2,3,4,5, 6, 7, 8}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourid = pourType[index]

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 4205, cgmsg:ByteSize(), cgmsg:SerializeToString())

    RebotModel.RecordPlayerSendPour(userid)
    --LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

BcbmEnter = BcbmEnter or {}

function BcbmEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_bcbm_pb.gcbcbmenter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.finfo.tableid
        local state = gcmsg.finfo.state
        local timemark = gcmsg.finfo.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)
    end
end

BcbmLeave = BcbmLeave or {}
function BcbmLeave.execute(nMsgId, clientId, strMsg)

end

BcbmCount= BcbmCount or {}
function BcbmCount.execute(nMsgId, clientId, strMsg)

end

BcbmUpdatePour = BcbmUpdatePour or {}
function BcbmUpdatePour.execute(nMsgId, clientId, strMsg)

end

BcbmStart = BcbmStart or {}
function BcbmStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_bcbm_pb.gcbcbmstart()
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
            processTimer(i*1000+rand, "BcbmTimer", luajson.encode(timeData))
        end
    end
end

BcbmTimer = BcbmTimer or {}
function BcbmTimer.execute(buffer)
	--通知下注的这里，不需要加入锁，这里只是一个通知的过程
	
    local jsonData = luajson.decode(buffer)   --这里只是做业务的分发就行
    BcbmService.SendPourJetton(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
end 

BcbmPour = BcbmPour or {}
function BcbmPour.execute(nMsgId, clientId, strMsg)

end

BcbmPlayerList = BcbmPlayerList or {}
function BcbmPlayerList.execute(nMsgId, clientId, strMsg)

end

BcbmBeBanker = BcbmBeBanker or {}
function BcbmBeBanker.execute(nMsgId, clientId, strMsg)

end

BcbmDropBanker = BcbmDropBanker or {}
function BcbmDropBanker.execute(nMsgId, clientId, strMsg)

end

BcbmUpdate = BcbmUpdate or {}
function BcbmUpdate.execute(nMsgId, clientId, strMsg)

end

BcbmChat = BcbmChat or {}
function BcbmChat.execute(nMsgId, clientId, strMsg)

end

BcbmEnd = BcbmEnd or {}
function BcbmEnd.execute(nMsgId, clientId, strMsg)

end

BcbmAllgameHistory = BcbmAllgameHistory or {}
function BcbmAllgameHistory.execute(nMsgId, clientId, strMsg)

end

BcbmGameHistoryUpdata = BcbmGameHistoryUpdata or {}
function BcbmGameHistoryUpdata.execute(nMsgId, clientId, strMsg)

end

BcbmStandList = BcbmStandList or {}
function BcbmStandList.execute(nMsgId, clientId, strMsg)

end