LuaProto = LuaProto or {}

LuaProto.pb     = require "pb"
LuaProto.pb_io   = require "pb_io"
LuaProto.pb_buffer = require "pb_buffer"
LuaProto.pb_slice  = require "pb_slice"
LuaProto.pb_conv   = require "pb_conv"
LuaProto.protoc = require "protoc"
LuaProto.serpent = require "serpent"


function LuaProto.LoadFile(protofile)
    assert(LuaProto.pb.loadfile(protofile)) -- 载入刚才编译的pb文件
end

function LuaProto.Decode(type, data)
    return LuaProto.pb.decode(type, data)
end

function LuaProto.Encode(type, data)
    return LuaProto.pb.encode(type, data)
end

function LuaProto.Defaults(type)
    type = "progame."..type
    return LuaProto.pb.defaults(type)
end

function LuaProto.PrintProto(data)
    LogFile("info", LuaProto.serpent.block(data))
end
