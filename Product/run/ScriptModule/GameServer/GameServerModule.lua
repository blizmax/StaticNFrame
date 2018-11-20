GameServerModule = {}

Tcp = Tcp or {}

GameServerModule.gameserver_linkid = 0

StartOver = StartOver or function()

end

InitTimer = InitTimer or function()
    -- body
end

function GameServerModule.Init()
    TcpServer.addRecvCallBack(NF_SERVER_TYPES.NF_ST_GAME, 0, "GameServerModule.NetServerRecvHandleJson")
	TcpServer.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GameServerModule.ProxyNetEvent")
    GameServerModule.gameserver_linkid = TcpServer.addServer(NF_SERVER_TYPES.NF_ST_GAME, 1, 100, 6005, false)

    unilight.addtimer("UserInfo.Update", 1)

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
    
    StartOver()

    InitTimer()
end

--特殊协议
function GameServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug(tostring(valueId) .. " | recv msg |" .. strMsg)
    local table_msg = json2table(strMsg)
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
                    if laccount == nil then
                        laccount = {}
                        laccount.Id = valueId
                        laccount.unLinkId = unLinkId
                        laccount.SendString = TcpServer.sendJsonMsg
                    end

                    Net[strcmd](table_msg, laccount)
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

end

function GameServerModule.ProxyNetEvent(event, unlinkId)
    if event == NF_MSG_TYPE.eMsgType_CONNECTED then
        unilight.debug("GameServer | proxy connect!")
    elseif event == NF_MSG_TYPE.eMsgType_DISCONNECTED then
        unilight.error("GameServer | proxy disconnected!")
    end
end