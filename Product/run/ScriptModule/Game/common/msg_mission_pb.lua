-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
module('msg_mission_pb')


local CGMISSIONSHARE = protobuf.Descriptor();
local CGMISSIONSHARE_USERID_FIELD = protobuf.FieldDescriptor();
local CGMISSIONSHARE_SHARECHANNEL_FIELD = protobuf.FieldDescriptor();
local CGMISSIONSHARE_SHARETYPE_FIELD = protobuf.FieldDescriptor();
local CGMISSIONSHARE_RESULT_FIELD = protobuf.FieldDescriptor();
local GCMISSIONSHARE = protobuf.Descriptor();
local GCMISSIONSHARE_RESULT_FIELD = protobuf.FieldDescriptor();

CGMISSIONSHARE_USERID_FIELD.name = "userid"
CGMISSIONSHARE_USERID_FIELD.full_name = ".progame.cgmissionshare.userid"
CGMISSIONSHARE_USERID_FIELD.number = 1
CGMISSIONSHARE_USERID_FIELD.index = 0
CGMISSIONSHARE_USERID_FIELD.label = 1
CGMISSIONSHARE_USERID_FIELD.has_default_value = false
CGMISSIONSHARE_USERID_FIELD.default_value = 0
CGMISSIONSHARE_USERID_FIELD.type = 5
CGMISSIONSHARE_USERID_FIELD.cpp_type = 1

CGMISSIONSHARE_SHARECHANNEL_FIELD.name = "sharechannel"
CGMISSIONSHARE_SHARECHANNEL_FIELD.full_name = ".progame.cgmissionshare.sharechannel"
CGMISSIONSHARE_SHARECHANNEL_FIELD.number = 2
CGMISSIONSHARE_SHARECHANNEL_FIELD.index = 1
CGMISSIONSHARE_SHARECHANNEL_FIELD.label = 1
CGMISSIONSHARE_SHARECHANNEL_FIELD.has_default_value = false
CGMISSIONSHARE_SHARECHANNEL_FIELD.default_value = ""
CGMISSIONSHARE_SHARECHANNEL_FIELD.type = 9
CGMISSIONSHARE_SHARECHANNEL_FIELD.cpp_type = 9

CGMISSIONSHARE_SHARETYPE_FIELD.name = "sharetype"
CGMISSIONSHARE_SHARETYPE_FIELD.full_name = ".progame.cgmissionshare.sharetype"
CGMISSIONSHARE_SHARETYPE_FIELD.number = 3
CGMISSIONSHARE_SHARETYPE_FIELD.index = 2
CGMISSIONSHARE_SHARETYPE_FIELD.label = 1
CGMISSIONSHARE_SHARETYPE_FIELD.has_default_value = false
CGMISSIONSHARE_SHARETYPE_FIELD.default_value = 0
CGMISSIONSHARE_SHARETYPE_FIELD.type = 5
CGMISSIONSHARE_SHARETYPE_FIELD.cpp_type = 1

CGMISSIONSHARE_RESULT_FIELD.name = "result"
CGMISSIONSHARE_RESULT_FIELD.full_name = ".progame.cgmissionshare.result"
CGMISSIONSHARE_RESULT_FIELD.number = 4
CGMISSIONSHARE_RESULT_FIELD.index = 3
CGMISSIONSHARE_RESULT_FIELD.label = 1
CGMISSIONSHARE_RESULT_FIELD.has_default_value = false
CGMISSIONSHARE_RESULT_FIELD.default_value = 0
CGMISSIONSHARE_RESULT_FIELD.type = 5
CGMISSIONSHARE_RESULT_FIELD.cpp_type = 1

CGMISSIONSHARE.name = "cgmissionshare"
CGMISSIONSHARE.full_name = ".progame.cgmissionshare"
CGMISSIONSHARE.nested_types = {}
CGMISSIONSHARE.enum_types = {}
CGMISSIONSHARE.fields = {CGMISSIONSHARE_USERID_FIELD, CGMISSIONSHARE_SHARECHANNEL_FIELD, CGMISSIONSHARE_SHARETYPE_FIELD, CGMISSIONSHARE_RESULT_FIELD}
CGMISSIONSHARE.is_extendable = false
CGMISSIONSHARE.extensions = {}
GCMISSIONSHARE_RESULT_FIELD.name = "result"
GCMISSIONSHARE_RESULT_FIELD.full_name = ".progame.gcmissionshare.result"
GCMISSIONSHARE_RESULT_FIELD.number = 1
GCMISSIONSHARE_RESULT_FIELD.index = 0
GCMISSIONSHARE_RESULT_FIELD.label = 1
GCMISSIONSHARE_RESULT_FIELD.has_default_value = false
GCMISSIONSHARE_RESULT_FIELD.default_value = 0
GCMISSIONSHARE_RESULT_FIELD.type = 5
GCMISSIONSHARE_RESULT_FIELD.cpp_type = 1

GCMISSIONSHARE.name = "gcmissionshare"
GCMISSIONSHARE.full_name = ".progame.gcmissionshare"
GCMISSIONSHARE.nested_types = {}
GCMISSIONSHARE.enum_types = {}
GCMISSIONSHARE.fields = {GCMISSIONSHARE_RESULT_FIELD}
GCMISSIONSHARE.is_extendable = false
GCMISSIONSHARE.extensions = {}

cgmissionshare = protobuf.Message(CGMISSIONSHARE)
gcmissionshare = protobuf.Message(GCMISSIONSHARE)

----------nimol modify---------
MSG_MISSION_PB_CGMISSIONSHARE = CGMISSIONSHARE
MSG_MISSION_PB_GCMISSIONSHARE = GCMISSIONSHARE