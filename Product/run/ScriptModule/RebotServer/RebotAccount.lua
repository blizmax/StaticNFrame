RebotAccount = RebotAccount or {}

--account login
function RebotAccount.LoginServer(clientId)
    local loginmsg =  LuaProto.Defaults("cgaccountlogin") --msg_human_pb.cgaccountlogin()
    local rebotId = RebotModel.GetRebotMaxId()
    local rebotname = "rebot_test"..rebotId
    loginmsg.cid = rebotname
    loginmsg.nickname = rebotname
    loginmsg.devname = rebotname
    loginmsg.account = rebotname
    loginmsg.password = rebotname
    loginmsg.bindtype = BindType.bindType_TianYu
    local data = LuaProto.Encode("cgaccountlogin", loginmsg)

    LuaNFrame.SendMsgToRebot(tonumber(clientId), 0, 1001, #data, data)
end

function RebotAccount.DisConnect(clientId)
    RebotModel.DelRebotPlayerIdByClient(clientId)
end