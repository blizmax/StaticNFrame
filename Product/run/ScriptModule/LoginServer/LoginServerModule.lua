LoginServerModule = {}



function LoginServerModule.Init()
    UserLoginMgr:Init()

    unilight.HttpServerInitServer(7000)
    unilight.HttpServerAddRequestHandler("/httplogin", NFHttpType.NF_HTTP_REQ_POST, "HttpLoginCallBack")

    unilight.debug("md5(gaoyi)=" .. unilight.GetMD5("gaoyi"))
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
    elseif body["do"] == "plat-token-login" then
        LoginServerModule.PlatTokenLogin(req, body)
    elseif body["do"] == "request-select-zone" then
        LoginServerModule.RequestSelectZone(req, body)
    end
end

function LoginServerModule.RequestSelectZone(req,cmd)
    unilight.debug("LoginServerModule.RequestSelectZone | "..table2json(cmd))
    local uid = tonumber(cmd.uid)
    local unigame_plat_login = cmd.unigame_plat_login
    local gameid = cmd.unigame_plat_login
    local zoneid = cmd.zoneid

    local msg = {}
    msg["do"] = "request-select-zone"

    local userLogin = UserLoginMgr:GetUserLogin(uid)
    if userLogin == nil then
        local msg = {error = "1"}
        local jsonmsg = json.encode(msg)

        unilight.HttpServerResponseMsg(req, jsonmsg, NFWebStatus.WEB_OK, "OK")
    end

    msg.errno = "0"
    msg.gameid = userLogin:GetGameId()
    msg.st = unilight.GetNowTime()
    msg.uid = tostring(uid)
    msg.unigame_plat_login = userLogin:GetPlatLogin()
    msg.unigame_plat_timestamp = unilight.GetNowTime()
    msg.zoneid = 1
    msg["data"] = {
        accountid = uid,
        gameid = userLogin:GetGameId(),
        gatewayurl = "http://14.17.104.12:9001/shen/user/http",
        gatewayurltcp = "14.17.104.12:9005",
        gatewayurlws = "ws://14.17.104.12:9001/shen/user",
        logintempid = 2994518,
        separatezoneuid = false,
        tokenid = 1541985442,
        zoneid = 1,
        zoneuid = 11954057,
    }
    local jsonmsg = json.encode(msg)
    unilight.HttpServerResponseMsg(req, jsonmsg, NFWebStatus.WEB_OK, "OK")
end

function LoginServerModule.PlatTokenLogin(req, cmd)
    unilight.debug("LoginServerModule.PlatTokenLogin | "..table2json(cmd))
    local account = cmd.data.platinfo.account
    local sign = cmd.data.platinfo.sign
    local gameid = cmd.gameid
    local zoneid = cmd.zoneid
    local nowTime = unilight.GetNowTime()
    local plat_key = unilight.GetMD5(sign..account..tostring(nowTime))
    local plat_login = unilight.GetMD5(sign..account).."-"..tostring(nowTime)

    local userLogin = UserLoginMgr:GetUserLoginByAccount(account)
    userLogin:SetSign(sign)
    userLogin:SetLoginTime(nowTime)
    userLogin:SetGameId(gameid)
    userLogin:SetZoneId(zoneid)

    userLogin:SetPlatKey(plat_key)
    userLogin:SetPlatLogin(plat_login)

    local plat_login_timeout = 2592000
    local uuid = userLogin:GetUid()
    local msg = {
        gameid = cmd.gameid,
        unigame_plat_key = plat_key,
        unigame_plat_login = plat_login,
        unigame_plat_login_timeout = tostring(plat_login_timeout),
        unigame_plat_timestamp = nowTime,
        zoneid = cmd.zoneid,
    }
    msg["do"] = cmd["do"]
    msg["data"] = {
        gameid = 3010,
        sid = sign .. "::" .. account,
        timezone_name = "CST",
        timezone_offset = 28800,
        uid = tostring(uuid),
        unigame_plat_key = plat_key,
        unigame_plat_login = plat_login,     
        unigame_plat_login_life = plat_login_timeout,
        platinfo = {
            aaaa = 123,
            account = account,
            email = "",
            gameid = 3010,
            gender = "",
            nickname = "",
            platid = 0,
            sign = sign,
            timestamp = tostring(nowTime),
            uid = tostring(uuid),
        }
    }

    local jsonmsg = json.encode(msg)

    unilight.HttpServerResponseMsg(req, jsonmsg, NFWebStatus.WEB_OK, "OK")
end

function LoginServerModule.RequestZoneList(req,cmd)
    unilight.debug("LoginServerModule.RequestZoneList | "..table2json(cmd))

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

    local msg = {}
    msg["do"] = cmd["do"]
    msg["error"] = "0"
    msg["st"] = unilight.GetNowTime()
    msg["data"] = {
        bestzoneid = 2,
        gameid = cmd["data"].gameid,
        gamename = "ttr",
        zoneid = 5,
        zonelist = {
            server,
        }
    }

    local jsonmsg = json.encode(msg)

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