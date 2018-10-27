FriendModule = {}

function FriendModule.Init()
    unilight.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "FriendModule.ProxyNetEvent")
end

function FriendModule.AfterInit()

end


function FriendModule.Execute()

end

function FriendModule.BeforeShut()

end

function FriendModule.Shut()

end

function FriendModule.ProxyNetEvent(event, unlinkId)
    if event == NF_MSG_TYPE.eMsgType_CONNECTED then
        unilight.debug("proxy connect!")
    else
        unilight.error("proxy disconnected!")
    end
end