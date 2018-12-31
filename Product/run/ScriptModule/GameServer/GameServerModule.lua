GameServerModule = {}

Tcp = Tcp or {}

StartOver = StartOver or function()

end

InitTimer = InitTimer or function()
    -- body
end

UserInfo = UserInfo or {}

function GameServerModule.Init()
    unilight.createdb("userinfo","uid")						-- 玩家个人信息
    unilight.createdb(mailsys.MAIL_DB, "uid")
    
    TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_PROXY, 0, "GameServerModule.NetServerRecvHandleJson")
    TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_WORLD, 0, "GameServerModule.WorldServerRecvHandleJson")

    unilight.AddServerEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, NF_SERVER_TYPES.NF_ST_WORLD, "GameServerModule.WorldServerNetEvent")
    unilight.AddAccountEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GameServerModule.AccountEventCallBack")
    
    -- 当tcp上线时
	Tcp.account_connect = function(laccount)
		UserInfo.Connected(laccount.Id)
	end

	-- 当tcp掉线时
	Tcp.account_disconnect = function(laccount)
		UserInfo.Disconnected(laccount.Id)
	end

	Tcp.reconnect_login_ok = function(laccount)
		unilight.debug("游戏玩家断线重连了。。。。。。。。。。。。。")
        UserInfo.ReconnectLoginOk(laccount)
    end

    if rechargemgr ~= nil then
        rechargemgr.Init()
    end

    --StartOver()
    --初始化玩家系统
    if UserInfo ~= nil then
        UserInfo.Init()
    end

	local CYCLE_MIN = 60
	local CYCLE_HOUR = 3600
	local CYCLE_DAY = CYCLE_HOUR * 24
    local CYCLE_WEEKLY = CYCLE_DAY * 7
    
    unilight.addclocker("GameServerModule.GlobalDayZeroClock", 0, CYCLE_DAY) 	-- 0点时钟
    unilight.addclocker("GameServerModule.GlobalWeekZeroClock", 0, CYCLE_WEEKLY) 	-- 0点时钟
end

--保存当前是每周几天和每月第几天
function GameServerModule.GlobalDayZeroClock()
	UserInfo.ZeroTimer()
end

--保存当前是每周几天和每月第几天
function GameServerModule.GlobalWeekZeroClock()

end

function GameServerModule.AccountEventCallBack(nEvent, unLinkId, laccount)
    if nEvent == NF_ACCOUNT_EVENT_TYPE.eAccountEventType_CONNECTED then
        go.accountInfoMap[laccount.Id] = laccount
        Tcp.account_connect(laccount)
    elseif nEvent == NF_ACCOUNT_EVENT_TYPE.eAccountEventType_DISCONNECTED then
        Tcp.account_disconnect(laccount)
        go.accountInfoMap[laccount.Id] = nil
    elseif nEvent == NF_ACCOUNT_EVENT_TYPE.eAccountEventType_DISCONNECTED then
        go.accountInfoMap[laccount.Id] = laccount
        Tcp.reconnect_login_ok(laccount)
    end
end

function GameServerModule.WorldServerNetEvent(nEvent, unLinkId, serverData)
    local cmd = {}
    if nEvent == NF_MSG_TYPE.eMsgType_CONNECTED then
        Lby.lobby_connect(cmd,serverData)
    end
    if nEvent == NF_MSG_TYPE.eMsgType_DISCONNECTED then
        Lby.lobby_disconnect(cmd, serverData)
    end
end

--特殊协议
function GameServerModule.WorldServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    --unilight.debug(tostring(valueId) .. " | recv world msg |" .. strMsg)
    local table_msg = json2table(strMsg)
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["do"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Cmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
                strcmd = "Cmd" .. strcmd
                if type(Lby[strcmd]) == "function" then
                    local lobby = unilobby.lobbytaskMap[unLinkId]
                    if lobby ~= nil then
                        unilight.BeginProfiler("Lby."..strcmd)
                        Lby[strcmd](table_msg, lobby)
                        local cost = unilight.EndProfiler()
                        if cost >= 1000 then
                            unilight.error("Lby."..strcmd.." cost "..tostring(cost).." ms, something wrong.....")
                        end
                    end
                end
            end
        end
    end
    -- body
end

--特殊协议
function GameServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg) 
    local table_msg = json2table(strMsg)
    if type(table_msg["do"]) == "string" then
        if table_msg["do"] ~= "Cmd.Ping_C" then
            --unilight.debug(tostring(valueId) .. " | recv msg |" .. strMsg)
        end
    end
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
                        unilight.BeginProfiler("Net."..strcmd)
                        Net[strcmd](table_msg, laccount)
                        local cost = unilight.EndProfiler()
                        if cost >= 1000 then
                            unilight.error("Net."..strcmd.." cost "..tostring(cost).." us, something wrong.....")
                        end
                    end
                end
            end
        end
    end
    -- body
end

function GameServerModule.AfterInit()

end


function GameServerModule.Execute()

end

function GameServerModule.BeforeShut()

end

function GameServerModule.Shut()
    UserInfo.SaveAllUserInfoToDB()
    unilight.info("Server.ServerStop:停机数据处理完成")
end