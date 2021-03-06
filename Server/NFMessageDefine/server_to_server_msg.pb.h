// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: server_to_server_msg.proto

#ifndef PROTOBUF_INCLUDED_server_5fto_5fserver_5fmsg_2eproto
#define PROTOBUF_INCLUDED_server_5fto_5fserver_5fmsg_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_server_5fto_5fserver_5fmsg_2eproto 

namespace protobuf_server_5fto_5fserver_5fmsg_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[3];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_server_5fto_5fserver_5fmsg_2eproto
namespace NFMsg {
class NotifyPlayerDisconnect;
class NotifyPlayerDisconnectDefaultTypeInternal;
extern NotifyPlayerDisconnectDefaultTypeInternal _NotifyPlayerDisconnect_default_instance_;
class NotifyPlayerReport;
class NotifyPlayerReportDefaultTypeInternal;
extern NotifyPlayerReportDefaultTypeInternal _NotifyPlayerReport_default_instance_;
class NotifyProxyPacketMsg;
class NotifyProxyPacketMsgDefaultTypeInternal;
extern NotifyProxyPacketMsgDefaultTypeInternal _NotifyProxyPacketMsg_default_instance_;
}  // namespace NFMsg
namespace google {
namespace protobuf {
template<> ::NFMsg::NotifyPlayerDisconnect* Arena::CreateMaybeMessage<::NFMsg::NotifyPlayerDisconnect>(Arena*);
template<> ::NFMsg::NotifyPlayerReport* Arena::CreateMaybeMessage<::NFMsg::NotifyPlayerReport>(Arena*);
template<> ::NFMsg::NotifyProxyPacketMsg* Arena::CreateMaybeMessage<::NFMsg::NotifyProxyPacketMsg>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace NFMsg {

// ===================================================================

class NotifyPlayerDisconnect : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:NFMsg.NotifyPlayerDisconnect) */ {
 public:
  NotifyPlayerDisconnect();
  virtual ~NotifyPlayerDisconnect();

  NotifyPlayerDisconnect(const NotifyPlayerDisconnect& from);

  inline NotifyPlayerDisconnect& operator=(const NotifyPlayerDisconnect& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  NotifyPlayerDisconnect(NotifyPlayerDisconnect&& from) noexcept
    : NotifyPlayerDisconnect() {
    *this = ::std::move(from);
  }

  inline NotifyPlayerDisconnect& operator=(NotifyPlayerDisconnect&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const NotifyPlayerDisconnect& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NotifyPlayerDisconnect* internal_default_instance() {
    return reinterpret_cast<const NotifyPlayerDisconnect*>(
               &_NotifyPlayerDisconnect_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(NotifyPlayerDisconnect* other);
  friend void swap(NotifyPlayerDisconnect& a, NotifyPlayerDisconnect& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline NotifyPlayerDisconnect* New() const final {
    return CreateMaybeMessage<NotifyPlayerDisconnect>(NULL);
  }

  NotifyPlayerDisconnect* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<NotifyPlayerDisconnect>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const NotifyPlayerDisconnect& from);
  void MergeFrom(const NotifyPlayerDisconnect& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NotifyPlayerDisconnect* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string account = 2;
  bool has_account() const;
  void clear_account();
  static const int kAccountFieldNumber = 2;
  const ::std::string& account() const;
  void set_account(const ::std::string& value);
  #if LANG_CXX11
  void set_account(::std::string&& value);
  #endif
  void set_account(const char* value);
  void set_account(const char* value, size_t size);
  ::std::string* mutable_account();
  ::std::string* release_account();
  void set_allocated_account(::std::string* account);

  // optional uint64 user_id = 1;
  bool has_user_id() const;
  void clear_user_id();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::uint64 user_id() const;
  void set_user_id(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:NFMsg.NotifyPlayerDisconnect)
 private:
  void set_has_user_id();
  void clear_has_user_id();
  void set_has_account();
  void clear_has_account();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr account_;
  ::google::protobuf::uint64 user_id_;
  friend struct ::protobuf_server_5fto_5fserver_5fmsg_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class NotifyPlayerReport : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:NFMsg.NotifyPlayerReport) */ {
 public:
  NotifyPlayerReport();
  virtual ~NotifyPlayerReport();

  NotifyPlayerReport(const NotifyPlayerReport& from);

  inline NotifyPlayerReport& operator=(const NotifyPlayerReport& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  NotifyPlayerReport(NotifyPlayerReport&& from) noexcept
    : NotifyPlayerReport() {
    *this = ::std::move(from);
  }

  inline NotifyPlayerReport& operator=(NotifyPlayerReport&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const NotifyPlayerReport& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NotifyPlayerReport* internal_default_instance() {
    return reinterpret_cast<const NotifyPlayerReport*>(
               &_NotifyPlayerReport_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(NotifyPlayerReport* other);
  friend void swap(NotifyPlayerReport& a, NotifyPlayerReport& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline NotifyPlayerReport* New() const final {
    return CreateMaybeMessage<NotifyPlayerReport>(NULL);
  }

  NotifyPlayerReport* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<NotifyPlayerReport>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const NotifyPlayerReport& from);
  void MergeFrom(const NotifyPlayerReport& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NotifyPlayerReport* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string ip = 2;
  bool has_ip() const;
  void clear_ip();
  static const int kIpFieldNumber = 2;
  const ::std::string& ip() const;
  void set_ip(const ::std::string& value);
  #if LANG_CXX11
  void set_ip(::std::string&& value);
  #endif
  void set_ip(const char* value);
  void set_ip(const char* value, size_t size);
  ::std::string* mutable_ip();
  ::std::string* release_ip();
  void set_allocated_ip(::std::string* ip);

  // optional uint64 user_id = 1;
  bool has_user_id() const;
  void clear_user_id();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::uint64 user_id() const;
  void set_user_id(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:NFMsg.NotifyPlayerReport)
 private:
  void set_has_user_id();
  void clear_has_user_id();
  void set_has_ip();
  void clear_has_ip();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr ip_;
  ::google::protobuf::uint64 user_id_;
  friend struct ::protobuf_server_5fto_5fserver_5fmsg_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class NotifyProxyPacketMsg : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:NFMsg.NotifyProxyPacketMsg) */ {
 public:
  NotifyProxyPacketMsg();
  virtual ~NotifyProxyPacketMsg();

  NotifyProxyPacketMsg(const NotifyProxyPacketMsg& from);

  inline NotifyProxyPacketMsg& operator=(const NotifyProxyPacketMsg& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  NotifyProxyPacketMsg(NotifyProxyPacketMsg&& from) noexcept
    : NotifyProxyPacketMsg() {
    *this = ::std::move(from);
  }

  inline NotifyProxyPacketMsg& operator=(NotifyProxyPacketMsg&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const NotifyProxyPacketMsg& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const NotifyProxyPacketMsg* internal_default_instance() {
    return reinterpret_cast<const NotifyProxyPacketMsg*>(
               &_NotifyProxyPacketMsg_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  void Swap(NotifyProxyPacketMsg* other);
  friend void swap(NotifyProxyPacketMsg& a, NotifyProxyPacketMsg& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline NotifyProxyPacketMsg* New() const final {
    return CreateMaybeMessage<NotifyProxyPacketMsg>(NULL);
  }

  NotifyProxyPacketMsg* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<NotifyProxyPacketMsg>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const NotifyProxyPacketMsg& from);
  void MergeFrom(const NotifyProxyPacketMsg& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(NotifyProxyPacketMsg* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated uint64 user_id = 1;
  int user_id_size() const;
  void clear_user_id();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::uint64 user_id(int index) const;
  void set_user_id(int index, ::google::protobuf::uint64 value);
  void add_user_id(::google::protobuf::uint64 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
      user_id() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
      mutable_user_id();

  // optional bytes msg = 3;
  bool has_msg() const;
  void clear_msg();
  static const int kMsgFieldNumber = 3;
  const ::std::string& msg() const;
  void set_msg(const ::std::string& value);
  #if LANG_CXX11
  void set_msg(::std::string&& value);
  #endif
  void set_msg(const char* value);
  void set_msg(const void* value, size_t size);
  ::std::string* mutable_msg();
  ::std::string* release_msg();
  void set_allocated_msg(::std::string* msg);

  // optional uint32 msg_id = 2;
  bool has_msg_id() const;
  void clear_msg_id();
  static const int kMsgIdFieldNumber = 2;
  ::google::protobuf::uint32 msg_id() const;
  void set_msg_id(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:NFMsg.NotifyProxyPacketMsg)
 private:
  void set_has_msg_id();
  void clear_has_msg_id();
  void set_has_msg();
  void clear_has_msg();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::uint64 > user_id_;
  ::google::protobuf::internal::ArenaStringPtr msg_;
  ::google::protobuf::uint32 msg_id_;
  friend struct ::protobuf_server_5fto_5fserver_5fmsg_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// NotifyPlayerDisconnect

// optional uint64 user_id = 1;
inline bool NotifyPlayerDisconnect::has_user_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void NotifyPlayerDisconnect::set_has_user_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void NotifyPlayerDisconnect::clear_has_user_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void NotifyPlayerDisconnect::clear_user_id() {
  user_id_ = GOOGLE_ULONGLONG(0);
  clear_has_user_id();
}
inline ::google::protobuf::uint64 NotifyPlayerDisconnect::user_id() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyPlayerDisconnect.user_id)
  return user_id_;
}
inline void NotifyPlayerDisconnect::set_user_id(::google::protobuf::uint64 value) {
  set_has_user_id();
  user_id_ = value;
  // @@protoc_insertion_point(field_set:NFMsg.NotifyPlayerDisconnect.user_id)
}

// optional string account = 2;
inline bool NotifyPlayerDisconnect::has_account() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NotifyPlayerDisconnect::set_has_account() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NotifyPlayerDisconnect::clear_has_account() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NotifyPlayerDisconnect::clear_account() {
  account_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_account();
}
inline const ::std::string& NotifyPlayerDisconnect::account() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyPlayerDisconnect.account)
  return account_.GetNoArena();
}
inline void NotifyPlayerDisconnect::set_account(const ::std::string& value) {
  set_has_account();
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:NFMsg.NotifyPlayerDisconnect.account)
}
#if LANG_CXX11
inline void NotifyPlayerDisconnect::set_account(::std::string&& value) {
  set_has_account();
  account_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:NFMsg.NotifyPlayerDisconnect.account)
}
#endif
inline void NotifyPlayerDisconnect::set_account(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_account();
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:NFMsg.NotifyPlayerDisconnect.account)
}
inline void NotifyPlayerDisconnect::set_account(const char* value, size_t size) {
  set_has_account();
  account_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:NFMsg.NotifyPlayerDisconnect.account)
}
inline ::std::string* NotifyPlayerDisconnect::mutable_account() {
  set_has_account();
  // @@protoc_insertion_point(field_mutable:NFMsg.NotifyPlayerDisconnect.account)
  return account_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NotifyPlayerDisconnect::release_account() {
  // @@protoc_insertion_point(field_release:NFMsg.NotifyPlayerDisconnect.account)
  if (!has_account()) {
    return NULL;
  }
  clear_has_account();
  return account_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NotifyPlayerDisconnect::set_allocated_account(::std::string* account) {
  if (account != NULL) {
    set_has_account();
  } else {
    clear_has_account();
  }
  account_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), account);
  // @@protoc_insertion_point(field_set_allocated:NFMsg.NotifyPlayerDisconnect.account)
}

// -------------------------------------------------------------------

// NotifyPlayerReport

// optional uint64 user_id = 1;
inline bool NotifyPlayerReport::has_user_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void NotifyPlayerReport::set_has_user_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void NotifyPlayerReport::clear_has_user_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void NotifyPlayerReport::clear_user_id() {
  user_id_ = GOOGLE_ULONGLONG(0);
  clear_has_user_id();
}
inline ::google::protobuf::uint64 NotifyPlayerReport::user_id() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyPlayerReport.user_id)
  return user_id_;
}
inline void NotifyPlayerReport::set_user_id(::google::protobuf::uint64 value) {
  set_has_user_id();
  user_id_ = value;
  // @@protoc_insertion_point(field_set:NFMsg.NotifyPlayerReport.user_id)
}

// optional string ip = 2;
inline bool NotifyPlayerReport::has_ip() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NotifyPlayerReport::set_has_ip() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NotifyPlayerReport::clear_has_ip() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NotifyPlayerReport::clear_ip() {
  ip_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_ip();
}
inline const ::std::string& NotifyPlayerReport::ip() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyPlayerReport.ip)
  return ip_.GetNoArena();
}
inline void NotifyPlayerReport::set_ip(const ::std::string& value) {
  set_has_ip();
  ip_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:NFMsg.NotifyPlayerReport.ip)
}
#if LANG_CXX11
inline void NotifyPlayerReport::set_ip(::std::string&& value) {
  set_has_ip();
  ip_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:NFMsg.NotifyPlayerReport.ip)
}
#endif
inline void NotifyPlayerReport::set_ip(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_ip();
  ip_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:NFMsg.NotifyPlayerReport.ip)
}
inline void NotifyPlayerReport::set_ip(const char* value, size_t size) {
  set_has_ip();
  ip_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:NFMsg.NotifyPlayerReport.ip)
}
inline ::std::string* NotifyPlayerReport::mutable_ip() {
  set_has_ip();
  // @@protoc_insertion_point(field_mutable:NFMsg.NotifyPlayerReport.ip)
  return ip_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NotifyPlayerReport::release_ip() {
  // @@protoc_insertion_point(field_release:NFMsg.NotifyPlayerReport.ip)
  if (!has_ip()) {
    return NULL;
  }
  clear_has_ip();
  return ip_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NotifyPlayerReport::set_allocated_ip(::std::string* ip) {
  if (ip != NULL) {
    set_has_ip();
  } else {
    clear_has_ip();
  }
  ip_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ip);
  // @@protoc_insertion_point(field_set_allocated:NFMsg.NotifyPlayerReport.ip)
}

// -------------------------------------------------------------------

// NotifyProxyPacketMsg

// repeated uint64 user_id = 1;
inline int NotifyProxyPacketMsg::user_id_size() const {
  return user_id_.size();
}
inline void NotifyProxyPacketMsg::clear_user_id() {
  user_id_.Clear();
}
inline ::google::protobuf::uint64 NotifyProxyPacketMsg::user_id(int index) const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyProxyPacketMsg.user_id)
  return user_id_.Get(index);
}
inline void NotifyProxyPacketMsg::set_user_id(int index, ::google::protobuf::uint64 value) {
  user_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:NFMsg.NotifyProxyPacketMsg.user_id)
}
inline void NotifyProxyPacketMsg::add_user_id(::google::protobuf::uint64 value) {
  user_id_.Add(value);
  // @@protoc_insertion_point(field_add:NFMsg.NotifyProxyPacketMsg.user_id)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >&
NotifyProxyPacketMsg::user_id() const {
  // @@protoc_insertion_point(field_list:NFMsg.NotifyProxyPacketMsg.user_id)
  return user_id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::uint64 >*
NotifyProxyPacketMsg::mutable_user_id() {
  // @@protoc_insertion_point(field_mutable_list:NFMsg.NotifyProxyPacketMsg.user_id)
  return &user_id_;
}

// optional uint32 msg_id = 2;
inline bool NotifyProxyPacketMsg::has_msg_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void NotifyProxyPacketMsg::set_has_msg_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void NotifyProxyPacketMsg::clear_has_msg_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void NotifyProxyPacketMsg::clear_msg_id() {
  msg_id_ = 0u;
  clear_has_msg_id();
}
inline ::google::protobuf::uint32 NotifyProxyPacketMsg::msg_id() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyProxyPacketMsg.msg_id)
  return msg_id_;
}
inline void NotifyProxyPacketMsg::set_msg_id(::google::protobuf::uint32 value) {
  set_has_msg_id();
  msg_id_ = value;
  // @@protoc_insertion_point(field_set:NFMsg.NotifyProxyPacketMsg.msg_id)
}

// optional bytes msg = 3;
inline bool NotifyProxyPacketMsg::has_msg() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void NotifyProxyPacketMsg::set_has_msg() {
  _has_bits_[0] |= 0x00000001u;
}
inline void NotifyProxyPacketMsg::clear_has_msg() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void NotifyProxyPacketMsg::clear_msg() {
  msg_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_msg();
}
inline const ::std::string& NotifyProxyPacketMsg::msg() const {
  // @@protoc_insertion_point(field_get:NFMsg.NotifyProxyPacketMsg.msg)
  return msg_.GetNoArena();
}
inline void NotifyProxyPacketMsg::set_msg(const ::std::string& value) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:NFMsg.NotifyProxyPacketMsg.msg)
}
#if LANG_CXX11
inline void NotifyProxyPacketMsg::set_msg(::std::string&& value) {
  set_has_msg();
  msg_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:NFMsg.NotifyProxyPacketMsg.msg)
}
#endif
inline void NotifyProxyPacketMsg::set_msg(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:NFMsg.NotifyProxyPacketMsg.msg)
}
inline void NotifyProxyPacketMsg::set_msg(const void* value, size_t size) {
  set_has_msg();
  msg_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:NFMsg.NotifyProxyPacketMsg.msg)
}
inline ::std::string* NotifyProxyPacketMsg::mutable_msg() {
  set_has_msg();
  // @@protoc_insertion_point(field_mutable:NFMsg.NotifyProxyPacketMsg.msg)
  return msg_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* NotifyProxyPacketMsg::release_msg() {
  // @@protoc_insertion_point(field_release:NFMsg.NotifyProxyPacketMsg.msg)
  if (!has_msg()) {
    return NULL;
  }
  clear_has_msg();
  return msg_.ReleaseNonDefaultNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void NotifyProxyPacketMsg::set_allocated_msg(::std::string* msg) {
  if (msg != NULL) {
    set_has_msg();
  } else {
    clear_has_msg();
  }
  msg_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), msg);
  // @@protoc_insertion_point(field_set_allocated:NFMsg.NotifyProxyPacketMsg.msg)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace NFMsg

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_server_5fto_5fserver_5fmsg_2eproto
