RebotServer = { }

function RebotServer.load_script_file()
	register_module(RebotServerModule, "RebotServerModule")
end

--加载所有的module
unilight.InsertLoadFunc(RebotServer.load_script_file)

return RebotServer