CreateClass("AccountMgr")

function AccountMgr:Init()
    self.account_linkid = {}
    self.linkid_account = {}
end

function AccountMgr:AddAccount(accountid, linkid)
    self.account_linkid[accountid] = linkid
    self.linkid_account[linkid] = accountid
end

function AccountMgr:GetLinkId(accountid)
    return self.account_linkid[accountid] or 0
end

function AccountMgr:GetAccountId(linkid)
    return self.linkid_account[linkid] or 0
end

function AccountMgr:RemoveAccountbyLinkId(linkId)
    if linkId == nil then return end
    local accountid = AccountMgr:GetAccountId(linkId)
    if accountid ~= nil then
        self.account_linkid[accountid] = nil
    end
    self.linkid_account[linkId] = nil
end