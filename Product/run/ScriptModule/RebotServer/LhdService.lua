LhdService = LhdService or {}

function LhdService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        local tabletype = math.myrandom(1,2)
        cgmsg.tableid = g_lhdDefine.init_data[tabletype]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

LhdEnter = LhdEnter or {}

function LhdEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_lhd_pb.gclhdenter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.tableid
        local state = gcmsg.state
        local timemark = gcmsg.timemark
    
        RebotModel.SetPlayerTableId(userid, tableid)
    end
end

LhdLeave = LhdLeave or {}
function LhdLeave.execute(nMsgId, clientId, strMsg)

end

LhdSitDown = LhdSitDown or {}
function LhdSitDown.execute(nMsgId, clientId, strMsg)

end

LhdPourJetton = LhdPourJetton or {}
function LhdPourJetton.execute(nMsgId, clientId, strMsg)

end

LhdCount = LhdCount or {}
function LhdCount.execute(nMsgId, clientId, strMsg)

end

LhdTableList = LhdTableList or {}
function LhdTableList.execute(nMsgId, clientId, strMsg)

end

LhdBeingBanker = LhdBeingBanker or {}
function LhdBeingBanker.execute(nMsgId, clientId, strMsg)

end

LhdDropBanker = LhdDropBanker or {}
function LhdDropBanker.execute(nMsgId, clientId, strMsg)

end

LhdStart = LhdStart or {}
function LhdStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_lhd_pb.gclhdstart()
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
            processTimer(i*1000+rand, "LhdTimer", luajson.encode(timeData))
        end
    end
end

LhdTimer = LhdTimer or {}
function LhdTimer.execute(buffer)
	--通知下注的这里，不需要加入锁，这里只是一个通知的过程
	
    local jsonData = luajson.decode(buffer)   --这里只是做业务的分发就行
    LhdService.SendPourJetton(jsonData['clientId'], jsonData['userid'], jsonData['tableid'])
end

function LhdService.SendPourJetton(clientId, userid, tableid)
    local cgmsg = msg_lhd_pb.cglhdpourjetton()
    cgmsg.userid = tonumber(userid)
    cgmsg.tableid = tonumber(tableid)
    cgmsg.jetton = 100
    local pourType = {1,2,3}
    local index = math.myrandom(1,#pourType)
    cgmsg.pourtype = pourType[index]

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 5307, cgmsg:ByteSize(), cgmsg:SerializeToString())

    RebotModel.RecordPlayerSendPour(userid)
    --LogFile("error", "player:"..userid.." send pour:"..cgmsg.jetton)
end

LhdUpdate = LhdUpdate or {}
function LhdUpdate.execute(nMsgId, clientId, strMsg)

end

LhdStandUp = LhdStandUp or {}
function LhdStandUp.execute(nMsgId, clientId, strMsg)

end

LhdGroupJetton = LhdGroupJetton or {}
function LhdGroupJetton.execute(nMsgId, clientId, strMsg)

end

LhdHistory = LhdHistory or {}
function LhdHistory.execute(nMsgId, clientId, strMsg)

end

LhdChat = LhdChat or {}
function LhdChat.execute(nMsgId, clientId, strMsg)

end

LhdStandList = LhdStandList or {}
function LhdStandList.execute(nMsgId, clientId, strMsg)

end

LhdBankerList = LhdBankerList or {}
function LhdBankerList.execute(nMsgId, clientId, strMsg)

end

LhdStats = LhdStats or {}
function LhdStats.execute(nMsgId, clientId, strMsg)

end

LhdAllGameHistory = LhdAllGameHistory or {}
function LhdAllGameHistory.execute(nMsgId, clientId, strMsg)

end

LhdGameHistoryUpdata = LhdGameHistoryUpdata or {}
function LhdGameHistoryUpdata.execute(nMsgId, clientId, strMsg)

end

LhdPourJettonXt = LhdPourJettonXt or {}
function LhdPourJettonXt.execute(nMsgId, clientId, strMsg)

end