

FriendModule = {}

function FriendModule.Init()
    
end

function FriendModule.AfterInit()

end


function FriendModule.Execute()

end

function FriendModule.BeforeShut()

end

function FriendModule.Shut()

end

function Net.CmdSendReqRecommendFriendCmd_C(cmd, laccount)
    print(table2json(cmd))
end