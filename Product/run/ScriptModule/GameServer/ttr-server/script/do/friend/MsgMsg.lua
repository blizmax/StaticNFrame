function Net.CmdMsgPullCmd_C(cmd, laccount)
    local uid = laccount.Id
	local res = {}

	res["do"] = "Cmd.MsgPullCmd_S"
	res["data"] = {
		resultCode = 0
    }
	local friendData = FriendManager:GetOrNewFriendInfo(uid)

	res.data["records"] = {}

	friendData.message.records:ForEach(
		function(v)
			table.insert(res.data["records"], v:GetDBTable())
		end
	)

    return res
end

function Net.CmdMsgSkipCmd_C(cmd, laccount)
    local uid = laccount.Id
	local res = {}

	res["do"] = "Cmd.MsgSkipCmd_S"
	res["data"] = {}

	if cmd.data == nil or cmd.data.id == nil then
        res.data["resultCode"] = ERROR_CODE.ARGUMENT_ERROR
        return res
	end

	res.data["id"] = cmd.data.id

	local friendData = FriendManager:GetOrNewFriendInfo(uid)

	res.data["resultCode"] = friendData.message:Remove(cmd.data.id)

    return res
end

Lby.CmdGiveMessageFromCenter_CS = function(cmd, lobbyClientTask)
    local uid = cmd.data.cmd_uid

	local friendData = FriendManager:GetFriendInfo(uid)
	if friendData ~= nil then
        local msg = friendData.message:add(cmd.data, cmd.data.msgType, cmd.data.args)

        if friendData.online == true then
            --push client
            local res = {}
            res["do"] = "Cmd.MsgNewCmd_S"
            res["data"] = {
                record = msg,
            }
            UserInfo.SendInfoByUid(friendData.uid, res)
        end
	end
end