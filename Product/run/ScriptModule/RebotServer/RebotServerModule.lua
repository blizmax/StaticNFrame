RebotServerModule = {}
RebotServerModule.RebotIndex = os.time()

--RebotServerModule.loginUrl = "http://loginxcxtest.zqgame.com/httplogin"
--RebotServerModule.loginUrl = "http://ttrserver.staticnframe.com:7000/httplogin"
RebotServerModule.loginUrl = "http://149.28.158.210:7000/httplogin"
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

    unilight.addtimer("RebotServerModule.addRebot", 1)
end

function RebotServerModule.addRebot(timer)
    for i = 1, 5 do
        local rebot = RebotPlayer:New()
        rebot:Init(RebotServerModule.GetRebotIndex())
        RebotServerModule.rebotNameMap[rebot.name] = rebot

        rebot:RequestZoneList()
    end

    local num = 0
    for k, v in pairs(RebotServerModule.rebotNameMap) do
        num = num + 1
    end

    if num >= 2000 then
        unilight.stoptimer(timer)
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
    if string.find(strMsg, "GetUserTaskInfo_S") ~= nil then
        --unilight.debug("| recv msg |" .. strMsg)
    end
    
    local table_msg = json2table(strMsg)
    if #table_msg > 0 then
        for i, table_info in ipairs(table_msg) do
            RebotServerModule.HandleRecvMsg(table_info, unLinkId)
        end
    else
        RebotServerModule.HandleRecvMsg(table_msg, unLinkId)
    end

    -- body
end

function RebotServerModule.HandleRecvMsg(table_msg, unLinkId)
    if table_msg["cmd_name"] == "Pmd.UserLoginReturnOkLoginUserPmd_S" then
        RebotServerModule.UserLoginReturnOkLoginUserPmd_S(unLinkId)
        return
    end
    if table_msg["cmd_name"] ~= "Pmd.UserJsMessageForwardUserPmd_CS" then
        return
    end
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
                    local rebot = RebotServerModule.rebotUnlinkIdMap[unLinkId]
                    if rebot ~= nil then
                        Net[strcmd](table_msg, rebot)
                        --unilight.debug("| handle msg |" .. table2json(table_msg))
                    end
                end
            end
        end
    end
end


function HttpClient.httpLoginReturn(state_code, respData, param)
    --unilight.debug("HttpClient.httpPostReturn");

    local rebot_name = param["name"]

    local rebot = RebotServerModule.rebotNameMap[rebot_name]

    if rebot ~= nil then
        rebot:HttpLoginReturn(respData)
    end
end

function HttpClient.plat_token_login_return(state_code, respData, param)
    --unilight.debug("HttpClient.plat_token_login_return")

    local rebot_name = param["name"]

    local rebot = RebotServerModule.rebotNameMap[rebot_name]

    if rebot ~= nil then
        rebot:PlatTokenLoginReturn(respData)
    end
end

function HttpClient.select_zone_return(state_code, respData, param)
    --unilight.debug("HttpClient.select_zone_return")

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

    local num = math.random(1,6)
    if num == 1 then
        rebot:GetRecommendFriendInfo()
    elseif num == 2 then
        rebot:GetFriendInfo()
    elseif num == 3 then
        rebot:GetRankList()
    elseif num == 4 then
        local add = false
        for f_uid, v in pairs(RebotServerModule.rebotUidMap) do
            if add == true then
                rebot:SendReqAddFriend(f_uid)
                break
            end
            if f_uid == uid then
                add = true
            end
        end
    elseif num == 5 then
        rebot:GetTravelEmployFriend()
        rebot:GetTravelEmployRecommend()
    else
        rebot:ReqBuyAllBuild()
        rebot:ReqLevelupAllBuild()
        rebot:ReqRebuildAllBuild()
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


function RebotServerModule.UserLoginReturnOkLoginUserPmd_S(unLinkId)
    local rebot = RebotServerModule.rebotUnlinkIdMap[unLinkId]
    if rebot ~= nil then
        rebot:DoRequestPlayerBaseInfo()

        unilight.addtimer("RebotServerModule.UpdateTick", 5, rebot.uid)
    end
end

function Net.CmdUserInfoSynLobbyCmd_S(cmd, rebot)
    rebot.userInfo = cmd.data.userInfo
    rebot.user_props = cmd.data.user_props

    unilight.debug("account"..rebot.name.." login success.............")

    rebot:DoGetDailyLoginInfo()
    rebot:RequestTravelData()
    rebot:RequestGuideInfo()
    rebot:GetRecommendFriendInfo()
    rebot:GetFriendInfo()
    rebot:SendUserQQFriendData()
    rebot:GetUserAskedAddFriends()
    rebot:GetUserTaskInfo()
    rebot:DoGetDailyWelfareInfo()
    rebot:GetGetDailyLotteryDraw()
    rebot:GetLotteryDrawRewardId()
    rebot:GetLotteryDrawReward()
    rebot:GetDailyDiamondReward()
    rebot:GetCollectReward()

    unilight.addtimer("RebotServerModule.UpdateSec", 1, rebot.uid)
    unilight.addtimer("RebotServerModule.UpdateFriend", 5, rebot.uid)

    rebot:ReqBuyAllBuild()
end

function Net.CmdBuildingBuyCmd_S(cmd, rebot)
	local stateId = cmd.data.stateId
	local buildingId = cmd.data.buildingId
    local resultcode = cmd.data.resultCode

    if resultcode == 0 then
        for k, map in pairs(rebot.userInfo.world.states) do
            if stateId == map.id then
                local buildid = {
                    id = buildingId,
                    lv = 1,
                    buildlv = 1,
                }
                table.insert(map.buildings, buildid)
            end
        end
    end
end

function Net.CmdGetUserTaskInfo_S(cmd, rebot)

    rebot.daily_task = cmd.data.daily_task
    rebot.achieve_task = cmd.data.achieve_task
    rebot.main_task = cmd.data.main_task
end

function Net.CmdNotifyUserTaskFinish_S(cmd, rebot)
    rebot:ReqGetRewardDailyTask(cmd.data.task_id)
end

function Net.CmdGetDailyLoginInfoCmd_S(cmd, rebot)
    for loginid, v in pairs(cmd.data.data) do
        rebot:GetDailyLoginRewardCmd(loginid,1)
    end
end

function Net.CmdSendReqBeAddFriendCmd_S(cmd, rebot)
    local f_uid = cmd.data.uid
    rebot:SendReqAgreeAddFriend(f_uid)
end

function Net.CmdGetTravelEmployFriend_S(cmd, rebot)
    local member = cmd.data.member
    for k, v in pairs(member) do
        local uid = v.uid
        rebot:EmployFriendToTravel(uid)
    end
end

function Net.CmdGetTravelEmployRecommend_S(cmd, rebot)
    local member = cmd.data.member
    for k, v in pairs(member) do
        local uid = v.uid
        rebot:EmployFriendToTravel(uid)
    end
end