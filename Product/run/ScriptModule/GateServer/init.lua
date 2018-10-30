GateServer = { }

function GateServer.load_script_file()
	register_module(GateServerModule, "GateServerModule")
end

--加载所有的module
unilight.InsertLoadFunc(GateServer.load_script_file)

return GateServer