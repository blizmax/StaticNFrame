go = go or {}
go.roomusermgr = {
    accounts = {}
}

function go.roomusermgr.GetRoomUserById(uid)
    return go.roomusermgr.accounts[uid]
end

function Net.CmdAccountLogin(cmd, laccount)
    go.roomusermgr.accounts[laccount.Id] = laccount
    Tcp.account_connect(laccount)
end