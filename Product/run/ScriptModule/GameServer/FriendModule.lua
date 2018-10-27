FriendModule = {}

function FriendModule.Init()
    unilight.addEventCallBack(NF_SERVER_TYPES.NF_ST_GAME, "FriendModule.ProxyNetEvent")
end

function FriendModule.NetRecvTest1111(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " valueId:" .. valueId .. " nMsgId:" .. nMsgId .. " strMsg:" .. strMsg)
    local table_msg = json2table(strMsg)
    -- body
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

function Net.CmdSendReqRecommendFriendCmd_C(cmd, unLinkId, valueId)
    print(table2json(cmd))
end