ShzBatService = ShzBatService or {}

function ShzBatService.EnterGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_human_pb.cgentertable()
    if tableid == nil then
        cgmsg.tableid = g_shzBatDefine.init_data[1]['tableid'][1]
    else
        cgmsg.tableid = tonumber(tableid)
    end
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1033, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function ShzBatService.LeaveGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_shzbat_pb.cgshzbatleave()
    cgmsg.tableid = tonumber(tableid)
    cgmsg.userid = tonumber(userid)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 9203, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function ShzBatService.StartGame(clientId, tableid)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_shzbat_pb.cgshzbatstart()
    cgmsg.tableid = tonumber(tableid)
    cgmsg.userid = tonumber(userid)
    cgmsg.bet = 1

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 9205, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function ShzBatService.ShzBatBetScore(clientId, tableid, betscore)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_shzbat_pb.cgshzbatbetscore()
    cgmsg.tableid = tonumber(tableid)
    cgmsg.userid = tonumber(userid)
    cgmsg.betscore = betscore
    local bettype = math.myrandom(1, 3)
    cgmsg.bettype = bettype

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 9207, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

function ShzBatService.ShzBatMaliya(clientId, tableid, betscore)
    local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
    local cgmsg = msg_shzbat_pb.cgshzbatmaliya()
    cgmsg.tableid = tonumber(tableid)
    cgmsg.userid = tonumber(userid)
    cgmsg.betscore = betscore

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 9209, cgmsg:ByteSize(), cgmsg:SerializeToString())
end

ShzBatEnter = ShzBatEnter or {}

function ShzBatEnter.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_shzbat_pb.gcshzbatenter()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = gcmsg.finfo.tableid
    
        RebotModel.SetPlayerTableId(userid, tableid)
        LogFile("error", "enter shz game sucess")

        ShzBatService.StartGame(clientId, tableid)
        ShzBatService.LeaveGame(clientId, tableid)
        return
    end
    LogFile("error", "enter shz game failed, result:"..ReturnCode[gcmsg.result])
end

ShzBatLeave = ShzBatLeave or {}
function ShzBatLeave.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_shzbat_pb.gcshzbatleave()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        RebotModel.DelPlayerTableId(userid)
        LogFile("error", "leave shz game sucess")
        return
    end
    LogFile("error", "leave shz game failed, result:"..ReturnCode[gcmsg.result])
end

ShzBatStart = ShzBatStart or {}
function ShzBatStart.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_shzbat_pb.gcshzbatstart()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = RebotModel.GetPlayerTableId(userid)
        if tableid ~= nil and tonumber(tableid) > 0 and gcmsg.score_info.win_score > 0 then
            --ShzBatService.ShzBatBetScore(clientId, tableid, gcmsg.score_info.win_score)
            ShzBatService.ShzBatMaliya(clientId, tableid, gcmsg.score_info.win_score)
        end
        LogFile("error", "start shz game sucess, win_score:"..gcmsg.score_info.win_score)
        return
    end
    LogFile("error", "start shz game failed, result:"..ReturnCode[gcmsg.result])
end

ShzBatBetScore = ShzBatBetScore or {}
function ShzBatBetScore.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_shzbat_pb.gcshzbatbetscore()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = RebotModel.GetPlayerTableId(userid)
        if tableid ~= nil and tableid > 0 and gcmsg.get_score > 0 then
            ShzBatService.ShzBatBetScore(clientId, tableid, gcmsg.get_score)
        end
        LogFile("error", "ShzBatBetScore game sucess, win_score:"..gcmsg.score_info.win_score)
        return
    end
    LogFile("error", "ShzBatBetScore game failed, result:"..ReturnCode[gcmsg.result])
end

ShzBatMaliya = ShzBatMaliya or {}
function ShzBatMaliya.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_shzbat_pb.gcshzbatmaliya()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local userid = RebotModel.GetRebotPlayerIdByClient(clientId)
        local tableid = RebotModel.GetPlayerTableId(userid)
        if tableid ~= nil and tonumber(tableid) > 0 and gcmsg.get_score > 0 then
            ShzBatService.ShzBatMaliya(clientId, tableid, gcmsg.get_score)
        end
        LogFile("error", "ShzBatMaliya game sucess, win_score:"..gcmsg.get_score)
        return
    end
    LogFile("error", "ShzBatMaliya game failed, result:"..ReturnCode[gcmsg.result])
end