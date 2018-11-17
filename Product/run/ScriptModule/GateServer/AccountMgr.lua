CreateClass("AccountMgr")

function AccountMgr:Init()
    self.account_linkid = {}
end

function AccountMgr:AddAccount(accountid, linkid)
    self.account_linkid[accountid] = linkid
end

function AccountMgr:GetLinkId(accountid)
    return self.account_linkid[accountid] or 0
end

function AccountMgr:RemoveAccountbyLinkId(linkId)
    for k, v in pairs(self.account_linkid) do 
        if v == linkId then
            self.account_linkid[k] = nil
        end
    end
end