// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: st_human_packet_code.proto

#ifndef PROTOBUF_INCLUDED_st_5fhuman_5fpacket_5fcode_2eproto
#define PROTOBUF_INCLUDED_st_5fhuman_5fpacket_5fcode_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_st_5fhuman_5fpacket_5fcode_2eproto 

namespace protobuf_st_5fhuman_5fpacket_5fcode_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_st_5fhuman_5fpacket_5fcode_2eproto
namespace NFMsg {
}  // namespace NFMsg
namespace NFMsg {

enum st_human_packet_code {
  Client_Msg_AccountLogin = 1001,
  Server_Msg_AccountLogin = 1002,
  Client_Msg_GetPlayerInfo = 1003,
  Server_Msg_GetPlayerInfo = 1004,
  Client_Msg_ReConnect = 1005,
  Server_Msg_ReConnect = 1006,
  Client_Msg_KitPlayer = 1007,
  Server_Msg_KitPlayer = 1008,
  Client_Msg_HeartBeat = 1009,
  Server_Msg_HeartBeat = 1010,
  Client_Msg_GetInitInfo = 1011,
  Server_Msg_GetInitInfo = 1012,
  Client_Msg_BroadCast = 1013,
  Server_Msg_BroadCast = 1014,
  Client_Msg_GetMailList = 1015,
  Server_Msg_GetMailList = 1016,
  Client_Msg_GetReadMail = 1017,
  Server_Msg_GetReadMail = 1018,
  Client_Msg_GetMailGoods = 1019,
  Server_Msg_GetMailGoods = 1020,
  Client_Msg_DeleteMail = 1021,
  Server_Msg_DeleteMail = 1022,
  Client_Msg_PlayerStatus = 1023,
  Server_Msg_PlayerStatus = 1024,
  Client_Msg_UpdatePlayerInfo = 1025,
  Server_Msg_UpdatePlayerInfo = 1026,
  Client_Msg_UpdateGoodsList = 1027,
  Server_Msg_UpdateGoodsList = 1028,
  Client_Msg_NoticeInfo = 1029,
  Server_Msg_NoticeInfo = 1030,
  Client_Msg_EnterTable = 1033,
  Server_Msg_EnterTable = 1034
};
bool st_human_packet_code_IsValid(int value);
const st_human_packet_code st_human_packet_code_MIN = Client_Msg_AccountLogin;
const st_human_packet_code st_human_packet_code_MAX = Server_Msg_EnterTable;
const int st_human_packet_code_ARRAYSIZE = st_human_packet_code_MAX + 1;

const ::google::protobuf::EnumDescriptor* st_human_packet_code_descriptor();
inline const ::std::string& st_human_packet_code_Name(st_human_packet_code value) {
  return ::google::protobuf::internal::NameOfEnum(
    st_human_packet_code_descriptor(), value);
}
inline bool st_human_packet_code_Parse(
    const ::std::string& name, st_human_packet_code* value) {
  return ::google::protobuf::internal::ParseNamedEnum<st_human_packet_code>(
    st_human_packet_code_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace NFMsg

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::NFMsg::st_human_packet_code> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NFMsg::st_human_packet_code>() {
  return ::NFMsg::st_human_packet_code_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_st_5fhuman_5fpacket_5fcode_2eproto
