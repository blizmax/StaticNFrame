RebotServerModule = {}
RebotServerModule.RebotIndex = os.time()

RebotServerModule.loginUrl = "http://loginxcxtest.zqgame.com/httplogin"
--RebotServerModule.loginUrl = "http://ttrserver.staticnframe.com:7000/httplogin"
--RebotServerModule.loginUrl = "http://127.0.0.1:7000/httplogin"
RebotServerModule.gameid = 3010
RebotServerModule.zoneid = 1
RebotServerModule.platid = 0
RebotServerModule.openKey = "0"

RebotServerModule.rebotNameMap = {}
RebotServerModule.rebotUidMap = {}
RebotServerModule.rebotUnlinkIdMap = {}

function RebotServerModule.GetRebotIndex()
    RebotServerModule.RebotIndex = RebotServerModule.RebotIndex + 1
    return RebotServerModule.RebotIndex
end

function RebotServerModule.Init()
    RebotServerModule.RebotIndex = unilight.GetInitTime()
    TcpClient.addEventCallBack(NF_SERVER_TYPES.NF_ST_PROXY, "RebotServerModule.NetEventCallBack")
    TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_PROXY, 0, "RebotServerModule.NetServerRecvHandleJson")

    for i = 1, 200 do
        local rebot = RebotPlayer:New()
        rebot:Init(RebotServerModule.GetRebotIndex())
        RebotServerModule.rebotNameMap[rebot.name] = rebot

        rebot:RequestZoneList()
    end
end

function RebotServerModule.NetEventCallBack(nEvent, unLinkId)
    if nEvent == NF_MSG_TYPE.eMsgType_CONNECTED then
        local rebot = RebotServerModule.rebotUnlinkIdMap[unLinkId]
        if rebot ~= nil then
            rebot:Connect()
        end
    elseif nEvent == NF_MSG_TYPE.eMsgType_DISCONNECTED then
        local rebot = RebotServerModule.rebotUnlinkIdMap[unLinkId]
        if rebot ~= nil then
            rebot:DisConnect()
        end
    end
end

function RebotServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug(tostring(valueId) .. " | recv msg |" .. strMsg)
    local table_msg = json2table(strMsg)
    table_msg = table_msg["msg"]
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["do"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Cmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
                strcmd = "Cmd" .. strcmd
                if type(Net[strcmd]) == "function" then
                    local laccount = go.roomusermgr.GetRoomUserById(valueId)
                    if laccount ~= nil then
                        Net[strcmd](table_msg, laccount)
                    end
                end
            end
        end
    end
    -- body
end

function HttpClient.httpLoginReturn(state_code, respData, param)
    unilight.debug("HttpClient.httpPostReturn");

    local rebot_name = param["name"]

    local rebot = RebotServerModule.rebotNameMap[rebot_name]

    if rebot ~= nil then
        rebot:HttpLoginReturn(respData)
    end
end

function HttpClient.plat_token_login_return(state_code, respData, param)
    unilight.debug("HttpClient.plat_token_login_return")

    local rebot_name = param["name"]

    local rebot = RebotServerModule.rebotNameMap[rebot_name]

    if rebot ~= nil then
        rebot:PlatTokenLoginReturn(respData)
    end
end

function HttpClient.select_zone_return(state_code, respData, param)
    unilight.debug("HttpClient.select_zone_return")

    if respData.errno ~= "0" then
        unilight.error("select_zone_return error")
    end
 
    local rebot_name = param["name"]

    local rebot = RebotServerModule.rebotNameMap[rebot_name]

    if rebot ~= nil then
        rebot:SelectZoneReturn(respData)
    end

end

function RebotServerModule.UpdateSec(uid, timer)
    local rebot = RebotServerModule.rebotUidMap[uid]
    if rebot == nil then return end

    rebot:ClickTravel()
end

function RebotServerModule.UpdateFriend(uid, timer)
    local rebot = RebotServerModule.rebotUidMap[uid]
    if rebot == nil then return end

    local num = math.random(1,3)
    if num == 1 then
        rebot:GetRecommendFriendInfo()
    elseif num == 2 then
        rebot:GetFriendInfo()
    elseif num == 3 then
        rebot:GetRankList()
    end
    
end


function RebotServerModule.UpdateTick(uid, timer)
    local rebot = RebotServerModule.rebotUidMap[uid]
    if rebot == nil then return end

    rebot:SendTick()
end

function RebotServerModule.AfterInit()

end


function RebotServerModule.Execute()

end

function RebotServerModule.BeforeShut()

end

function RebotServerModule.Shut()

end
