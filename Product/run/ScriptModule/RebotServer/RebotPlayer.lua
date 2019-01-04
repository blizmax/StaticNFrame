
CreateClass("RebotPlayer")

function RebotPlayer:Init(index)
    self.name = "gaoyi"..index  --玩家名字 nickname
    self.loginFinish = false
    self.gameid = RebotServerModule.gameid
    self.zoneid = 1
    self.unigame_plat_key = RebotServerModule.openKey
    self.unigame_plat_login = ""
    self.sid = ""
    self.uid = 0
    self.unigame_plat_timestamp = os.time()
    self.accountid = 0
    self.gatewayurlws = ""
    self.logintempid = 0
    self.tokenid = 0
    self.reconnect_logintempid = 0
    self.reconnect_url = ""
    self.userInfo = nil
    self.user_props = nil
    self.daily_task = nil
    self.achieve_task = nil
    self.main_task = nil
    self.friends = {}
end

function RebotPlayer:RequestZoneList()
    local data = {}
    data["do"] = "request-zone-list"
    data["data"] = {}
    data["data"]["gameid"] = RebotServerModule.gameid

    local param = { name = self.name }

    unilight.HttpClientRequestPost(RebotServerModule.loginUrl, "httpLoginReturn", data, nil, param)
end

function RebotPlayer:HttpLoginReturn(respData)
    if respData.data.zonelist == nil then
        unilight.error("has no zone list!")
        return 
    end

    for k, zoneInfo in pairs(respData.data.zonelist) do
        if zoneInfo.state > 0 then
            self.zoneid = zoneInfo.zoneid
            break
        end
    end


    local data = {}
    data["do"] = "plat-token-login"
    data["gameid"] = self.gameid
    data["zoneid"] = self.zoneid
    data["unigame_plat_timestamp"] = os.time()
    data["data"] = { gameid = self.gameid,}
    data["data"]["platinfo"] = {
        aaaa = 123,
        sign = RebotServerModule.openKey,
        account = self.name,
        platid = RebotServerModule.platid,
        gameid = self.gameid,
    }

    local param = { name = self.name }

    unilight.HttpClientRequestPost(RebotServerModule.loginUrl, "plat_token_login_return", data, nil, param)
end

function RebotPlayer:PlatTokenLoginReturn(respData)
    self.unigame_plat_key = respData.data.unigame_plat_key
    self.unigame_plat_login = respData.data.unigame_plat_login
    self.sid = respData.data.sid
    self.uid = tonumber(respData.data.uid)
    self.unigame_plat_timestamp = os.time()

    RebotServerModule.rebotUidMap[self.uid] = self

    local data = {}
    data["do"] = "request-select-zone"
    data["data"] = {}
    data["gameid"] = self.gameid
    data["zoneid"] = self.zoneid
    data["uid"] = tostring(self.uid)
    data["unigame_plat_login"] = self.unigame_plat_login
    data["unigame_plat_timestamp"] = self.unigame_plat_timestamp

    local strJson = table2json(data)

    local sel_url = RebotServerModule.loginUrl .. "?unigame_plat_sign=" .. unilight.GetMD5(strJson..self.unigame_plat_timestamp..self.unigame_plat_key)

    local param = { name = self.name }

    unilight.HttpClientRequestPost(sel_url, "select_zone_return", data, nil, param)
end

function RebotPlayer:SelectZoneReturn(respData)
    self.accountid = respData.data.accountid
    self.gatewayurlws = respData.data.gatewayurlws .. "/json"
    self.gatewayurltcp = respData.data.gatewayurltcp
    self.logintempid = respData.data.logintempid;
    self.logintempid = 0
    self.tokenid = respData.data.tokenid;
    self.reconnect_logintempid = self.logintempid

    self.reconnect_url = self.gatewayurlws

    self.loginFinish = true

    self.unlinkid = TcpClient:addWebServer(NF_SERVER_TYPES.NF_ST_PROXY, self.gatewayurlws)
    if self.unlinkid ~= 0 then
        RebotServerModule.rebotUnlinkIdMap[self.unlinkid] = self
    end
end

function RebotPlayer:Connect()
    self:LoginTokenLogin()
end

function RebotPlayer:LoginTokenLogin()
    local timestamp = os.time()
    local tmp = tostring(self.accountid) .. tostring(self.reconnect_logintempid) .. tostring(timestamp) .. self.unigame_plat_key

    local data = {
        version = 20160805,
        gameid = self.gameid,
        zoneid = self.zoneid,
        accountid = self.accountid,
        logintempid = self.logintempid,
        timestamp = timestamp,
        compress = "",
        tokenmd5 = unilight.GetMD5(tmp),
        cmd_name = "Pmd.UserLoginTokenLoginUserPmd_C",
    }

    TcpClient.sendMsgByServerId(data, self.unlinkid)
end

function RebotPlayer:SendMessage(data)
    if type(data) == "table" then
        data = table2json(data)
    end
    local strMsg = {
        cmd_name = "Pmd.UserJsMessageForwardUserPmd_CS",
        msg = data,
    }

    TcpClient.sendMsgByServerId(strMsg, self.unlinkid)
end

function RebotPlayer:DoRequestPlayerBaseInfo()
    local data = { }
    data["do"] = "Cmd.UserInfoSynLobbyCmd_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:DoGetDailyLoginInfo()
    local data = { }
    data["do"] = "Cmd.GetDailyLoginInfoCmd_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:RequestGuideInfo()
    local data = { }
    data["do"] = "Cmd.GetGuideInfoCmd_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:DisConnect()

end

function RebotPlayer:ClickTravel()
    local data = { }
    data["do"] = "Cmd.StateClickCmd_C"
    data["data"] = {
        stateId = 101,
        times = 4,
        critical = 4,
    }

    self:SendMessage(data)
end

function RebotPlayer:GetRankList()
    local data = { }
    data["do"] = "Cmd.ReqGetWorldRankListInfo_C"
    data["data"] = {
        rank_type = 1,
        start_index = 1,
        endIndex = 20,
    }

    self:SendMessage(data)
end

function RebotPlayer:SendTick()
    local data = { }
    data["do"] = "Cmd.Ping_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:DoGetDailyWelfareInfo()
    local data = { }
    data["do"] = "Cmd.GetDailyWelfareInfoCmd_C"
    data["data"] = {}

    self:SendMessage(data)
end


function RebotPlayer:GetUserTaskInfo()
    local data = { }
    data["do"] = "Cmd.GetUserTaskInfo_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetDailyLoginRewardCmd(id,doubleReward)
    local data = { }
    data["do"] = "Cmd.GetDailyLoginRewardCmd_C"
    data["data"] = {
        id = id,
        doubleReward = doubleReward,
    }

    self:SendMessage(data)
end

function RebotPlayer:GetGetDailyLotteryDraw()
    local data = { }
    data["do"] = "Cmd.GetDailyLotteryDrawCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetLotteryDrawRewardId()
    local data = { }
    data["do"] = "Cmd.GetLotteryDrawRewardIdCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetLotteryDrawReward()
    local data = { }
    data["do"] = "Cmd.GetLotteryDrawRewardCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:ReqGetRewardDailyTask(taskid)
    local data = { }
    data["do"] = "Cmd.ReqGetRewardDailyTask_C"
    data["data"] = {
        task_id = taskid,
    }

    self:SendMessage(data)
end

function RebotPlayer:GetDailyDiamondReward()
    local data = { }
    data["do"] = "Cmd.GetDailyDiamondRewardCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetCollectReward()
    local data = { }
    data["do"] = "Cmd.GetCollectRewardCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:ReqBuyBuild(stateId, buildingId)
    local data = { }
    data["do"] = "Cmd.BuildingBuyCmd_C"
    data["data"] = {
        stateId = stateId,
        buildingId = buildingId,
    }

    self:SendMessage(data)
end

function RebotPlayer:ReqLevelupBuild(stateId, buildingId)
    local data = { }
    data["do"] = "Cmd.BuildingLevelupCmd_C"
    data["data"] = {
        stateId = stateId,
        buildingId = buildingId,
    }

    self:SendMessage(data)
end

function RebotPlayer:ReqRebuildBuild(stateId, buildingId)
    local data = { }
    data["do"] = "Cmd.BuildingRebuildCmd_C"
    data["data"] = {
        stateId = stateId,
        buildingId = buildingId,
    }

    self:SendMessage(data)
end

function RebotPlayer:ReqBuyAllBuild()
    for k, map in pairs(self.userInfo.world.states) do
        for k, build in pairs(TableBuilding) do
            if build.mapid == map.id then
                self:ReqBuyBuild(build.mapid, build.Id)
            end
        end
    end
end

function RebotPlayer:ReqLevelupAllBuild()
    for k, map in pairs(self.userInfo.world.states) do
        for k, build in pairs(TableBuilding) do
            if build.mapid == map.id then
                self:ReqLevelupBuild(build.mapid, build.Id)
            end
        end
    end
end

function RebotPlayer:ReqRebuildAllBuild()
    for k, map in pairs(self.userInfo.world.states) do
        for k, build in pairs(TableBuilding) do
            if build.mapid == map.id then
                self:ReqRebuildBuild(build.mapid, build.Id)
            end
        end
    end
end


function RebotPlayer:SendUserQQFriendData()
    local data = { }
    data["do"] = "Cmd.SendUserQQFriendDataCmd_C"
    data["data"] = {
        self_data = {
            head = "",
            name = self.name,
            sex = 1,
        }
    }

    self:SendMessage(data)
end


function RebotPlayer:GetRecommendFriendInfo()
    local data = { }
    data["do"] = "Cmd.SendReqRecommendFriendCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetFriendInfo()
    local data = { }
    data["do"] = "Cmd.GetUserFriendDataCmd_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:GetUserAskedAddFriends()
    local data = { }
    data["do"] = "Cmd.GetUserAskedAddFriends_C"
    data["data"] = {
    }

    self:SendMessage(data)
end

function RebotPlayer:SendReqAddFriend(friend_uid)
    local data = { }
    data["do"] = "Cmd.SendReqAddFriendCmd_C"
    data["data"] = {
        friend_uid = friend_uid,
    }

    self.friends[friend_uid] = true
    self:SendMessage(data)
end

function RebotPlayer:SendReqAgreeAddFriend(friend_uid)
    local data = { }
    data["do"] = "Cmd.SendReqAgreeAddFriendCmd_C"
    data["data"] = {
        uid = friend_uid,
        agree = true,
    }
    self.friends[friend_uid] = true
    self:SendMessage(data)
end

function RebotPlayer:RequestTravelData()
    local data = { }
    data["do"] = "Cmd.GetUserTravelInfo_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:GetTravelEmployFriend()
    local data = { }
    data["do"] = "Cmd.GetTravelEmployFriend_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:GetTravelEmployRecommend()
    local data = { }
    data["do"] = "Cmd.GetTravelEmployRecommend_C"
    data["data"] = {}

    self:SendMessage(data)
end

function RebotPlayer:EmployFriendToTravel(f_uid)
    local data = { }
    data["do"] = "Cmd.EmployFriendToTravel_C"
    data["data"] = {
        uid = f_uid,
    }

    self:SendMessage(data)
end

