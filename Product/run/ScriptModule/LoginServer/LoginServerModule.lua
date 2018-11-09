LoginServerModule = {}

function LoginServerModule.Init()
    unilight.HttpServerInitServer(5009)
    unilight.HttpServerAddRequestHandler("/httplogin", NFHttpType.NF_HTTP_REQ_POST, "HttpLoginCallBack")
end

--C++将调用这个函数作为httpserver回调
--req是一个userdata数据结构，C++中是NFHttpRequest
--包含一下属性:
--url
--path
--remoteHost
--type
--body
--params
--headers
function HttpServer.HttpLoginCallBack(req)
    unilight.debug("HttpServer.HttpLoginCallBack")
    local tmp = {
        usrl = req.url,
        path = req.path,
        remoteHost = req.remoteHost,
        type = req.type,
        body = req.body,
        params = req.params,
        headers = req.headers,
    }
    unilight.debug(table2json(tmp))

    local body = json2table(req.body)

    if body["do"] == "request-zone-list" then
        LoginServerModule.RequestZoneList(req, body)
    end
end

function LoginServerModule.RequestZoneList(req,cmd)
    unilight.debug("LoginServerModule.RequestZoneList | "..table2json(cmd))

    local msg = {}
    msg["do"] = cmd["do"]
    msg["error"] = "0"
    msg["st"] = 111111111
    msg["data"] = {
        bestzoneid = 2,
        gameid = cmd["data"].gameid,
        gamename = "ttr",
        zoneid = 5,
        zonelist = {
        }
    }

    local server = {
        gameid = cmd["data"].gameid,
        gamename = "ttr",
        newzoneid = 0,
        onlinenum = 100,
        opentime = "2018-11-05 20:56:23",
        state = 1,
        zoneid = 1,
        zonename = "高义",
    }

    table.insert(msg["data"].zonelist, server)
    local jsonmsg = table2json(msg)

    unilight.HttpServerResponseMsg(req, jsonmsg, NFWebStatus.WEB_OK, "OK")
end

function LoginServerModule.AfterInit()

end


function LoginServerModule.Execute()

end

function LoginServerModule.BeforeShut()

end

function LoginServerModule.Shut()

end