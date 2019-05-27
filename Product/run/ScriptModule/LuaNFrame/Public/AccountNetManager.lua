AccountNetManager = AccountNetManager or {}

function AccountNetManager.execute(luaFunc, nEvent, unLinkId, pServerData)
    _G[luaFunc].execute(nEvent, unLinkId, pServerData)
end