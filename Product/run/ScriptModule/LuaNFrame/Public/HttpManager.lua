HttpManager = HttpManager or {}

n_httpController = {
	["hlyd"] = "HttpHlyd",
	["debug"] = "HttpDebug",
	["pay"] = "HttpPay",
	["mjqj"] = "HttpMjqj",
	["game"] = "HttpGame",
}


function HttpManager.CreateController(packetid)

	--local controller = _G[n_httpController[packetid]]
	return _G[n_httpController[packetid]]
	
end