AccountLogin = AccountLogin or {}

function AccountLogin.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_human_pb.gcaccountlogin()
    gcmsg:ParseFromString(strMsg)
    
    if gcmsg.result == 0 then
        local playerId = gcmsg.pinfo.userid
        local account = gcmsg.pinfo.account

        RebotModel.SetRebotPlayerIdByClientId(clientId, playerId)
        LogFile("error", "rebot:"..account.." playerId:"..playerId.." login success!")

        local cgmsg = msg_human_pb.cggetplayerinfo()
        cgmsg.userid = playerId

        LuaNFrame.SendMsgToRebot(clientId, 0, 1003, cgmsg:ByteSize(), cgmsg:SerializeToString())

        local cgmsg = msg_human_pb.cggetinitinfo()
        cgmsg.userid = playerId
        LuaNFrame.SendMsgToRebot(clientId, 0, 1011, cgmsg:ByteSize(), cgmsg:SerializeToString())
    else
        LogFile("error", "rebot login failed!")
    end
end

UpdatePlayerInfo = UpdatePlayerInfo or {}
function UpdatePlayerInfo.execute(nMsgId, clientId, strMsg)
    --local gcmsg = msg_human_pb.gcupdateplayerinfo()
   -- gcmsg:ParseFromString(strMsg)
    --local userid = RebotModel.GetRebotPlayerIdByClient(clientId)

    --for k, v in ipairs(gcmsg.typelist) do
    --    if v == "jetton" then
    --        local value = gcmsg.valuelist[k]
    --        RebotModel.SetPlayerJetton(userid, value)
    --        break
    --    end
    --end
end

GetInitInfo = GetInitInfo or {}
function GetInitInfo.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_human_pb.gcgetinitinfo()
    gcmsg:ParseFromString(strMsg)
end

EnterTable = EnterTable or {}
function EnterTable.execute(nMsgId, clientId, strMsg)

end

GetPlayerInfo = GetPlayerInfo or {}
function GetPlayerInfo.execute(nMsgId, clientId, strMsg)
    local gcmsg = msg_human_pb.gcgetplayerinfo()
    gcmsg:ParseFromString(strMsg)
    
    RebotModel.SetPlayerJetton(gcmsg.pinfo.userid, gcmsg.pinfo.jetton)
end

function GetPlayerInfo.work(buffer)
    local allclient = RebotModel.GetAllRebotPlayerIdByClient()
    for clientId, playerId in pairs(allclient) do
        local cgmsg = msg_human_pb.cggetplayerinfo()
        cgmsg.userid = tonumber(playerId)

        LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1003, cgmsg:ByteSize(), cgmsg:SerializeToString())
    end
end

BroadCast = BroadCast or {}
function BroadCast.execute(nMsgId, clientId, strMsg)

end

NoticeInfo = NoticeInfo or {}
function NoticeInfo.execute(nMsgId, clientId, strMsg)

end

Heartbeat = Heartbeat or {}
function Heartbeat.execute(nMsgId, clientId, strMsg)

end

AccountService = AccountService or {}
function AccountService.Init()
    redisItem:set(RebotModel.rebot_max_id, g_redisInfo.redis_one*1000, RebotModel.redis_index)
end

function AccountService.UtilsLoop()
	--这一个线程是放在Utils实例中执行，所以是每秒都会执行。
	
	local tm = TimeUtils.GetTableTime()
	
    processWork("SendHeartBeat","")
    processWork("GetGameStateInfo","")
	if tm.sec == 0 then
		processWork("GetPlayerInfo","min")
	end
	
	if math.mod(tm.sec, 10) == 0 then
		--processWork("SqlUpdateTenSec","tenSec")
	end
end

SendHeartBeat = SendHeartBeat or {}

function SendHeartBeat.work(buffer)
    local allclient = RebotModel.GetAllRebotPlayerIdByClient()
    for clientId, playerId in pairs(allclient) do
        local cgmsg = msg_human_pb.cgheartbeat()
        LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1009, cgmsg:ByteSize(), cgmsg:SerializeToString())
    end
end

GetGameStateInfo = GetGameStateInfo or {}

function GetGameStateInfo.work(buffer)
    local allconnect = RebotModel.GetAccountConnnect()
    for k, clientId in ipairs(allconnect) do
        RebotModel.DelAccountConnnect(clientId)
        RebotAccount.LoginServer(clientId)
    end

    local allclient = RebotModel.GetAllRebotPlayerIdByClient()
    local count = 0
    for clientId, playerId in pairs(allclient) do
        count = count + 1
        local tableid = RebotModel.GetPlayerTableId(playerId)
        if tableid == nil or tonumber(tableid) == 0 then
            BrnnService.EnterGame(clientId, tableid)
            --[[local randgame = math.myrandom(1, 100)
            if randgame <= 20 then
                BarccatatService.EnterGame(clientId, tableid)
            elseif randgame > 20 and randgame <= 40 then
                HongheiService.EnterGame(clientId, tableid)
            elseif randgame > 40 and randgame <= 60 then
                LhdService.EnterGame(clientId, tableid)
            elseif randgame > 60 and randgame <= 80 then
                BcbmService.EnterGame(clientId, tableid)
            elseif randgame > 80 and randgame <= 100 then
                BrnnService.EnterGame(clientId, tableid)
            end   ]]
        end
    end
    LogFile("error", "rebot online num:"..count)
    count = 0
    local sendPour = 0
    local allPlayer = RebotModel.GetAllPlayerTableId()
    for playerId, tableId in pairs(allPlayer) do
        count = count + 1
        if RebotModel.GetPlayerSendPour(playerId) ~= nil then
            sendPour = sendPour +1
        end
    end

    LogFile("error", "rebot play game:"..count)
    LogFile("error", "rebot send pour:"..sendPour)
end
