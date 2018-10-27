GameServer = { }

function GameServer.load_script_file()
	register_module(FriendModule, "FriendModule")
	register_module(TestModule, "TestModule")
end

return GameServer