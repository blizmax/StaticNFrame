GameServerModule = {}

function GameServerModule.Init()
	TcpServer.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GameServerModule.ProxyNetEvent")


	TcpServer.addServer(NF_SERVER_TYPES.NF_ST_GAME, 1, 100, 6005)
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
        unilight.debug("proxy connect!")
    else
        unilight.error("proxy disconnected!")
    end
end