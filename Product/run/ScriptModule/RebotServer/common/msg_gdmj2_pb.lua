-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
local st_gdmj_pb = require("st_gdmj_pb")
local st_human_pb = require("st_human_pb")
----------nimol modify---------
local ST_GDMJ_PB_GDMJINFO = st_gdmj_pb.ST_GDMJ_PB_GDMJINFO
local ST_GDMJ_PB_GDMJNEXTACTION = st_gdmj_pb.ST_GDMJ_PB_GDMJNEXTACTION
local ST_GDMJ_PB_GDMJPLAYDETAIL = st_gdmj_pb.ST_GDMJ_PB_GDMJPLAYDETAIL
local ST_GDMJ_PB_GDMJTDH = st_gdmj_pb.ST_GDMJ_PB_GDMJTDH
local ST_GDMJ_PB_GDMJUSER = st_gdmj_pb.ST_GDMJ_PB_GDMJUSER
module('msg_gdmj2_pb')


local TITEM = protobuf.Descriptor();
local TITEM_M_MAXUSER_FIELD = protobuf.FieldDescriptor();
local TITEM_M_TINFO_FIELD = protobuf.FieldDescriptor();
local TITEM_M_USERLIST_FIELD = protobuf.FieldDescriptor();
local TITEM_M_USERMODIFY_FIELD = protobuf.FieldDescriptor();
local TITEM_M_DETAILLIST_FIELD = protobuf.FieldDescriptor();
local TITEM_M_NEXTINFO_FIELD = protobuf.FieldDescriptor();
local TITEM_M_VIPROOMINFO_FIELD = protobuf.FieldDescriptor();
local TITEM_M_ISMODIFY_FIELD = protobuf.FieldDescriptor();

TITEM_M_MAXUSER_FIELD.name = "m_maxUser"
TITEM_M_MAXUSER_FIELD.full_name = ".progame.titem.m_maxUser"
TITEM_M_MAXUSER_FIELD.number = 1
TITEM_M_MAXUSER_FIELD.index = 0
TITEM_M_MAXUSER_FIELD.label = 1
TITEM_M_MAXUSER_FIELD.has_default_value = false
TITEM_M_MAXUSER_FIELD.default_value = 0
TITEM_M_MAXUSER_FIELD.type = 5
TITEM_M_MAXUSER_FIELD.cpp_type = 1

TITEM_M_TINFO_FIELD.name = "m_tInfo"
TITEM_M_TINFO_FIELD.full_name = ".progame.titem.m_tInfo"
TITEM_M_TINFO_FIELD.number = 2
TITEM_M_TINFO_FIELD.index = 1
TITEM_M_TINFO_FIELD.label = 1
TITEM_M_TINFO_FIELD.has_default_value = false
TITEM_M_TINFO_FIELD.default_value = nil
TITEM_M_TINFO_FIELD.message_type = ST_GDMJ_PB_GDMJINFO
TITEM_M_TINFO_FIELD.type = 11
TITEM_M_TINFO_FIELD.cpp_type = 10

TITEM_M_USERLIST_FIELD.name = "m_userList"
TITEM_M_USERLIST_FIELD.full_name = ".progame.titem.m_userList"
TITEM_M_USERLIST_FIELD.number = 3
TITEM_M_USERLIST_FIELD.index = 2
TITEM_M_USERLIST_FIELD.label = 3
TITEM_M_USERLIST_FIELD.has_default_value = false
TITEM_M_USERLIST_FIELD.default_value = {}
TITEM_M_USERLIST_FIELD.message_type = ST_GDMJ_PB_GDMJUSER
TITEM_M_USERLIST_FIELD.type = 11
TITEM_M_USERLIST_FIELD.cpp_type = 10

TITEM_M_USERMODIFY_FIELD.name = "m_userModify"
TITEM_M_USERMODIFY_FIELD.full_name = ".progame.titem.m_userModify"
TITEM_M_USERMODIFY_FIELD.number = 4
TITEM_M_USERMODIFY_FIELD.index = 3
TITEM_M_USERMODIFY_FIELD.label = 3
TITEM_M_USERMODIFY_FIELD.has_default_value = false
TITEM_M_USERMODIFY_FIELD.default_value = {}
TITEM_M_USERMODIFY_FIELD.type = 5
TITEM_M_USERMODIFY_FIELD.cpp_type = 1

TITEM_M_DETAILLIST_FIELD.name = "m_detailList"
TITEM_M_DETAILLIST_FIELD.full_name = ".progame.titem.m_detailList"
TITEM_M_DETAILLIST_FIELD.number = 5
TITEM_M_DETAILLIST_FIELD.index = 4
TITEM_M_DETAILLIST_FIELD.label = 3
TITEM_M_DETAILLIST_FIELD.has_default_value = false
TITEM_M_DETAILLIST_FIELD.default_value = {}
TITEM_M_DETAILLIST_FIELD.message_type = ST_GDMJ_PB_GDMJPLAYDETAIL
TITEM_M_DETAILLIST_FIELD.type = 11
TITEM_M_DETAILLIST_FIELD.cpp_type = 10

TITEM_M_NEXTINFO_FIELD.name = "m_nextInfo"
TITEM_M_NEXTINFO_FIELD.full_name = ".progame.titem.m_nextInfo"
TITEM_M_NEXTINFO_FIELD.number = 6
TITEM_M_NEXTINFO_FIELD.index = 5
TITEM_M_NEXTINFO_FIELD.label = 1
TITEM_M_NEXTINFO_FIELD.has_default_value = false
TITEM_M_NEXTINFO_FIELD.default_value = nil
TITEM_M_NEXTINFO_FIELD.message_type = ST_GDMJ_PB_GDMJNEXTACTION
TITEM_M_NEXTINFO_FIELD.type = 11
TITEM_M_NEXTINFO_FIELD.cpp_type = 10

TITEM_M_VIPROOMINFO_FIELD.name = "m_viproomInfo"
TITEM_M_VIPROOMINFO_FIELD.full_name = ".progame.titem.m_viproomInfo"
TITEM_M_VIPROOMINFO_FIELD.number = 7
TITEM_M_VIPROOMINFO_FIELD.index = 6
TITEM_M_VIPROOMINFO_FIELD.label = 1
TITEM_M_VIPROOMINFO_FIELD.has_default_value = false
TITEM_M_VIPROOMINFO_FIELD.default_value = nil
TITEM_M_VIPROOMINFO_FIELD.message_type = ST_GDMJ_PB_GDMJTDH
TITEM_M_VIPROOMINFO_FIELD.type = 11
TITEM_M_VIPROOMINFO_FIELD.cpp_type = 10

TITEM_M_ISMODIFY_FIELD.name = "m_isModify"
TITEM_M_ISMODIFY_FIELD.full_name = ".progame.titem.m_isModify"
TITEM_M_ISMODIFY_FIELD.number = 8
TITEM_M_ISMODIFY_FIELD.index = 7
TITEM_M_ISMODIFY_FIELD.label = 1
TITEM_M_ISMODIFY_FIELD.has_default_value = false
TITEM_M_ISMODIFY_FIELD.default_value = 0
TITEM_M_ISMODIFY_FIELD.type = 5
TITEM_M_ISMODIFY_FIELD.cpp_type = 1

TITEM.name = "titem"
TITEM.full_name = ".progame.titem"
TITEM.nested_types = {}
TITEM.enum_types = {}
TITEM.fields = {TITEM_M_MAXUSER_FIELD, TITEM_M_TINFO_FIELD, TITEM_M_USERLIST_FIELD, TITEM_M_USERMODIFY_FIELD, TITEM_M_DETAILLIST_FIELD, TITEM_M_NEXTINFO_FIELD, TITEM_M_VIPROOMINFO_FIELD, TITEM_M_ISMODIFY_FIELD}
TITEM.is_extendable = false
TITEM.extensions = {}

titem = protobuf.Message(TITEM)

----------nimol modify---------
MSG_GDMJ2_PB_TITEM = TITEM