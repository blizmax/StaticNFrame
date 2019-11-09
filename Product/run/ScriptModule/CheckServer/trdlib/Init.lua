

--require("./external/TimeUtils.lua")
require('trdlib.external.TimeUtils')
require("trdlib.external.functions")
require("trdlib.external.luaDump")

luajson = require("trdlib.external.json")
redisConnect = require("trdlib.external.redisConnect")

mysqlConnect = require("trdlib.external.mysqlConnect")


package.path = package.path..';./trdlib/libprotobuf/?.lua'   --由于这里protobuf的特殊性，必须把包含protobuf的目录加到环境变量中
package.path = package.path..';./trdlib/lua/?.lua'
require("trdlib.libprotobuf.protobuf")

