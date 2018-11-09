LoginServer = { }

function LoginServer.load_script_file()
	register_module(LoginServerModule, "LoginServerModule")
end

--加载所有的module
unilight.InsertLoadFunc(LoginServer.load_script_file)

return LoginServer