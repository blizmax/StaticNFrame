GameServerModule = {}

function GameServerModule.Init()
	TcpServer.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GameServerModule.ProxyNetEvent")


    TcpServer.addServer(NF_SERVER_TYPES.NF_ST_GAME, 1, 100, 6005)
    
    local tmp = {
        gaoyi = "is good boy",
    }
    unilight.HttpRequestGet("http://www.qq.com", "HttpCallBack", tmp)
end

function HttpGet.HttpCallBack(state_code, respData, strUseData)
    unilight.debug("state_code:"..state_code)
    unilight.debug(respData)
    unilight.debug("strUseData:".. table2json(strUseData))
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