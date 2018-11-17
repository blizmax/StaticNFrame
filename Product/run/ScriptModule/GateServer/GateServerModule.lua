GateServerModule = {}

function GateServerModule.Init()
	GateServerModule.gameserver_unlinkid = 0
end

function GateServerModule.AfterInit()
	--添加网关web服务器，以及网络事件回调，协议回调
	TcpServer.addRecvCallBack(NF_SERVER_TYPES.NF_ST_PROXY, 0, "GateServerModule.NetServerRecvHandleJson")
	TcpServer.addEventCallBack(NF_SERVER_TYPES.NF_ST_PROXY, "GateServerModule.ProxyServerWebNetEvent")
	TcpServer.addServer(NF_SERVER_TYPES.NF_ST_PROXY, 1, 100, 7001, true)

	--添加网关客户端，链接GameServer服务器, 以及网络事件回调，协议回调
	TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_GAME, 0, "GateServerModule.NetClientRecvHandleJson")
	TcpClient.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GateServerModule.ProxyNetEvent")
	GateServerModule.gameserver_unlinkid = TcpClient.addServer(NF_SERVER_TYPES.NF_ST_GAME, "127.0.0.1", 6005)

	--账号系统初始化
	AccountMgr:Init()
end

--特殊协议
function GateServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " strMsg:" .. strMsg)
    local table_msg = json2table(strMsg)
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["cmd_name"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Pmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
				if type(Net[strcmd]) == "function" then
					local laccount = {}
					laccount.Id = valueId
					laccount.unLinkId = unLinkId
					laccount.SendString = TcpServer.sendJsonMsg
					Net[strcmd](table_msg, laccount)
				else
					unilight.warn(cmd.." no handler!")
                end
            end
		end
    end
    -- body
end

--特殊协议
function GateServerModule.NetClientRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " valueId:" .. valueId .. " nMsgId:" .. nMsgId .. " strMsg:" .. strMsg)
    local table_msg = json2table(strMsg)
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["cmd_name"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Pmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
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

function GateServerModule.ProxyServerWebNetEvent(event, unlinkId)
    if event == NF_MSG_TYPE.eMsgType_CONNECTED then
        unilight.debug("web client connect! unlinkId:" .. tostring(unlinkId))
	elseif event == NF_MSG_TYPE.eMsgType_DISCONNECTED then
		unilight.error("web client disconnected! unlinkId:" .. tostring(unlinkId))
		AccountMgr:RemoveAccountbyLinkId(unlinkId)
    end
end


function GateServerModule.Execute()

end

function GateServerModule.BeforeShut()

end

function GateServerModule.Shut()

end

function GateServerModule.ProxyNetEvent(event, unlinkId)
    if event == NF_MSG_TYPE.eMsgType_CONNECTED then
        unilight.debug("GateServer | game connect!")
    elseif event == NF_MSG_TYPE.eMsgType_DISCONNECTED then
		unilight.error("GateServer | game disconnected!")
    end
end