BarccatatService = BarccatatService or {}

function BarccatatService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        cgmsg.tableid = g_barccatatDefine.init_data[1]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToWorld(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function BarccatatService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_barccatat_pb.cgbarccatatpourjetton()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.jetton = 100
    local pourType = {1,2,3}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourtype = pourType[index]

    LuaNFrame.SendMsgToWorld(tonumber(clientId), 0, 4411, cgmsg:ByteSize(), cgmsg:SerializeToString())

    LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

BarccatatEnter = BarccatatEnter or {}

function BarccatatEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_barccatat_pb.gcbarccatatenter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.tableid
        local state = gcmsg.state
        local timemark = gcmsg.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)

        if state == g_barccatatDefine.state_pour then
            BarccatatService.SendPourJetton(clientId, userid, tableid)
        end
    end
end

BarccatatLeave = BarccatatLeave or {}
function BarccatatLeave.execute(nMsgId, clientId, strMsg)

end

BarccatatSitDown= BarccatatSitDown or {}
function BarccatatSitDown.execute(nMsgId, clientId, strMsg)

end

BarccatatStandUp = BarccatatStandUp or {}
function BarccatatStandUp.execute(nMsgId, clientId, strMsg)

end

BarccatatStart = BarccatatStart or {}
function BarccatatStart.execute(nMsgId, clientId, strMsg)

end

BarccatatPourJetton = BarccatatPourJetton or {}
function BarccatatPourJetton.execute(nMsgId, clientId, strMsg)

end

BarccatatGroupJetton = BarccatatGroupJetton or {}
function BarccatatGroupJetton.execute(nMsgId, clientId, strMsg)

end

BarccatatSendCard = BarccatatSendCard or {}
function BarccatatSendCard.execute(nMsgId, clientId, strMsg)

end

BarccatatCount = BarccatatCount or {}
function BarccatatCount.execute(nMsgId, clientId, strMsg)

end

BarccatatNext = BarccatatNext or {}
function BarccatatNext.execute(nMsgId, clientId, strMsg)

end

BarccatatHistory = BarccatatHistory or {}
function BarccatatHistory.execute(nMsgId, clientId, strMsg)

end

BarccatatBeingBanker = BarccatatBeingBanker or {}
function BarccatatBeingBanker.execute(nMsgId, clientId, strMsg)

end

BarccatatDropBanker = BarccatatDropBanker or {}
function BarccatatDropBanker.execute(nMsgId, clientId, strMsg)

end