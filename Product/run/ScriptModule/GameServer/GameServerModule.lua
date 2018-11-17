GameServerModule = {}

function GameServerModule.Init()
    TcpServer.addRecvCallBack(NF_SERVER_TYPES.NF_ST_GAME, 0, "GameServerModule.NetServerRecvHandleJson")
	TcpServer.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GameServerModule.ProxyNetEvent")
    TcpServer.addServer(NF_SERVER_TYPES.NF_ST_GAME, 1, 100, 6005, false)
end

--特殊协议
function GameServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " valueId:" .. valueId .. " nMsgId:" .. nMsgId .. " strMsg:" .. strMsg)
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
					local laccount = {}
					laccount.Id = valueId
					laccount.unLinkId = unLinkId
					laccount.SendString = TcpServer.sendJsonMsg
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