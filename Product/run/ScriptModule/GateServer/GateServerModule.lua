GateServerModule = {}

function GateServerModule.Init()
	
end

function GateServerModule.AfterInit()
	TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_GAME, 1111, "unilight.NetRecvHandleJson1111")
	TcpClient.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "GateServerModule.ProxyNetEvent")
	TcpClient.addServer(NF_SERVER_TYPES.NF_ST_GAME, "127.0.0.1", 6005)
end


function GateServerModule.Execute()

end

function GateServerModule.BeforeShut()

end

function GateServerModule.Shut()

end

function GateServerModule.ProxyNetEvent(event, unlinkId)
	if event == NF_MSG_TYPE.eMsgType_CONNECTED then
		local msg = { }
		msg["do"] = "Cmd.SendReqRecommendFriendCmd_C"
		local laccount = {}
		laccount.unLinkId = unlinkId
		laccount.Id = 0
		TcpClient.sendJsonMsg(msg, laccount)
    end
end