// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: achieve.proto

#ifndef PROTOBUF_achieve_2eproto__INCLUDED
#define PROTOBUF_achieve_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace proto {
namespace message {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_achieve_2eproto();
void protobuf_AssignDesc_achieve_2eproto();
void protobuf_ShutdownFile_achieve_2eproto();

class Achieve_CS_GetAchieveInfo;
class AchieveInfo;
class Achieve_SC_GetAchieveInfo;
class Achieve_CS_GetAchieveReward;
class Achieve_SC_GetAchieveReward;
class Achieve_SC_UpdateAchieve;
class Achieve_CS_Upgrade;
class Achieve_SC_Upgrade;

// ===================================================================

class Achieve_CS_GetAchieveInfo : public ::google::protobuf::Message {
 public:
  Achieve_CS_GetAchieveInfo();
  virtual ~Achieve_CS_GetAchieveInfo();

  Achieve_CS_GetAchieveInfo(const Achieve_CS_GetAchieveInfo& from);

  inline Achieve_CS_GetAchieveInfo& operator=(const Achieve_CS_GetAchieveInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_CS_GetAchieveInfo& default_instance();

  void Swap(Achieve_CS_GetAchieveInfo* other);

  // implements Message ----------------------------------------------

  Achieve_CS_GetAchieveInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_CS_GetAchieveInfo& from);
  void MergeFrom(const Achieve_CS_GetAchieveInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_CS_GetAchieveInfo)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;


  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[1];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_CS_GetAchieveInfo* default_instance_;
};
// -------------------------------------------------------------------

class AchieveInfo : public ::google::protobuf::Message {
 public:
  AchieveInfo();
  virtual ~AchieveInfo();

  AchieveInfo(const AchieveInfo& from);

  inline AchieveInfo& operator=(const AchieveInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const AchieveInfo& default_instance();

  void Swap(AchieveInfo* other);

  // implements Message ----------------------------------------------

  AchieveInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AchieveInfo& from);
  void MergeFrom(const AchieveInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional int32 con_num = 2;
  inline bool has_con_num() const;
  inline void clear_con_num();
  static const int kConNumFieldNumber = 2;
  inline ::google::protobuf::int32 con_num() const;
  inline void set_con_num(::google::protobuf::int32 value);

  // optional int32 status = 3;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 3;
  inline ::google::protobuf::int32 status() const;
  inline void set_status(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.AchieveInfo)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_con_num();
  inline void clear_has_con_num();
  inline void set_has_status();
  inline void clear_has_status();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 con_num_;
  ::google::protobuf::int32 status_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static AchieveInfo* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_SC_GetAchieveInfo : public ::google::protobuf::Message {
 public:
  Achieve_SC_GetAchieveInfo();
  virtual ~Achieve_SC_GetAchieveInfo();

  Achieve_SC_GetAchieveInfo(const Achieve_SC_GetAchieveInfo& from);

  inline Achieve_SC_GetAchieveInfo& operator=(const Achieve_SC_GetAchieveInfo& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_SC_GetAchieveInfo& default_instance();

  void Swap(Achieve_SC_GetAchieveInfo* other);

  // implements Message ----------------------------------------------

  Achieve_SC_GetAchieveInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_SC_GetAchieveInfo& from);
  void MergeFrom(const Achieve_SC_GetAchieveInfo& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 points = 1;
  inline bool has_points() const;
  inline void clear_points();
  static const int kPointsFieldNumber = 1;
  inline ::google::protobuf::int32 points() const;
  inline void set_points(::google::protobuf::int32 value);

  // repeated .proto.message.AchieveInfo achieve_list = 2;
  inline int achieve_list_size() const;
  inline void clear_achieve_list();
  static const int kAchieveListFieldNumber = 2;
  inline const ::proto::message::AchieveInfo& achieve_list(int index) const;
  inline ::proto::message::AchieveInfo* mutable_achieve_list(int index);
  inline ::proto::message::AchieveInfo* add_achieve_list();
  inline const ::google::protobuf::RepeatedPtrField< ::proto::message::AchieveInfo >&
      achieve_list() const;
  inline ::google::protobuf::RepeatedPtrField< ::proto::message::AchieveInfo >*
      mutable_achieve_list();

  // repeated int32 points_id_list = 3;
  inline int points_id_list_size() const;
  inline void clear_points_id_list();
  static const int kPointsIdListFieldNumber = 3;
  inline ::google::protobuf::int32 points_id_list(int index) const;
  inline void set_points_id_list(int index, ::google::protobuf::int32 value);
  inline void add_points_id_list(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      points_id_list() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_points_id_list();

  // optional int32 cup_id = 4;
  inline bool has_cup_id() const;
  inline void clear_cup_id();
  static const int kCupIdFieldNumber = 4;
  inline ::google::protobuf::int32 cup_id() const;
  inline void set_cup_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_SC_GetAchieveInfo)
 private:
  inline void set_has_points();
  inline void clear_has_points();
  inline void set_has_cup_id();
  inline void clear_has_cup_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::proto::message::AchieveInfo > achieve_list_;
  ::google::protobuf::int32 points_;
  ::google::protobuf::int32 cup_id_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > points_id_list_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_SC_GetAchieveInfo* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_CS_GetAchieveReward : public ::google::protobuf::Message {
 public:
  Achieve_CS_GetAchieveReward();
  virtual ~Achieve_CS_GetAchieveReward();

  Achieve_CS_GetAchieveReward(const Achieve_CS_GetAchieveReward& from);

  inline Achieve_CS_GetAchieveReward& operator=(const Achieve_CS_GetAchieveReward& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_CS_GetAchieveReward& default_instance();

  void Swap(Achieve_CS_GetAchieveReward* other);

  // implements Message ----------------------------------------------

  Achieve_CS_GetAchieveReward* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_CS_GetAchieveReward& from);
  void MergeFrom(const Achieve_CS_GetAchieveReward& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_CS_GetAchieveReward)
 private:
  inline void set_has_id();
  inline void clear_has_id();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_CS_GetAchieveReward* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_SC_GetAchieveReward : public ::google::protobuf::Message {
 public:
  Achieve_SC_GetAchieveReward();
  virtual ~Achieve_SC_GetAchieveReward();

  Achieve_SC_GetAchieveReward(const Achieve_SC_GetAchieveReward& from);

  inline Achieve_SC_GetAchieveReward& operator=(const Achieve_SC_GetAchieveReward& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_SC_GetAchieveReward& default_instance();

  void Swap(Achieve_SC_GetAchieveReward* other);

  // implements Message ----------------------------------------------

  Achieve_SC_GetAchieveReward* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_SC_GetAchieveReward& from);
  void MergeFrom(const Achieve_SC_GetAchieveReward& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 result = 1;
  inline bool has_result() const;
  inline void clear_result();
  static const int kResultFieldNumber = 1;
  inline ::google::protobuf::int32 result() const;
  inline void set_result(::google::protobuf::int32 value);

  // optional int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional int32 status = 3;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 3;
  inline ::google::protobuf::int32 status() const;
  inline void set_status(::google::protobuf::int32 value);

  // optional int32 points = 4;
  inline bool has_points() const;
  inline void clear_points();
  static const int kPointsFieldNumber = 4;
  inline ::google::protobuf::int32 points() const;
  inline void set_points(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_SC_GetAchieveReward)
 private:
  inline void set_has_result();
  inline void clear_has_result();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_status();
  inline void clear_has_status();
  inline void set_has_points();
  inline void clear_has_points();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 result_;
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 status_;
  ::google::protobuf::int32 points_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_SC_GetAchieveReward* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_SC_UpdateAchieve : public ::google::protobuf::Message {
 public:
  Achieve_SC_UpdateAchieve();
  virtual ~Achieve_SC_UpdateAchieve();

  Achieve_SC_UpdateAchieve(const Achieve_SC_UpdateAchieve& from);

  inline Achieve_SC_UpdateAchieve& operator=(const Achieve_SC_UpdateAchieve& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_SC_UpdateAchieve& default_instance();

  void Swap(Achieve_SC_UpdateAchieve* other);

  // implements Message ----------------------------------------------

  Achieve_SC_UpdateAchieve* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_SC_UpdateAchieve& from);
  void MergeFrom(const Achieve_SC_UpdateAchieve& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional int32 con_num = 2;
  inline bool has_con_num() const;
  inline void clear_con_num();
  static const int kConNumFieldNumber = 2;
  inline ::google::protobuf::int32 con_num() const;
  inline void set_con_num(::google::protobuf::int32 value);

  // optional int32 status = 3;
  inline bool has_status() const;
  inline void clear_status();
  static const int kStatusFieldNumber = 3;
  inline ::google::protobuf::int32 status() const;
  inline void set_status(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_SC_UpdateAchieve)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_con_num();
  inline void clear_has_con_num();
  inline void set_has_status();
  inline void clear_has_status();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 con_num_;
  ::google::protobuf::int32 status_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_SC_UpdateAchieve* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_CS_Upgrade : public ::google::protobuf::Message {
 public:
  Achieve_CS_Upgrade();
  virtual ~Achieve_CS_Upgrade();

  Achieve_CS_Upgrade(const Achieve_CS_Upgrade& from);

  inline Achieve_CS_Upgrade& operator=(const Achieve_CS_Upgrade& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_CS_Upgrade& default_instance();

  void Swap(Achieve_CS_Upgrade* other);

  // implements Message ----------------------------------------------

  Achieve_CS_Upgrade* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_CS_Upgrade& from);
  void MergeFrom(const Achieve_CS_Upgrade& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_CS_Upgrade)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;


  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[1];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_CS_Upgrade* default_instance_;
};
// -------------------------------------------------------------------

class Achieve_SC_Upgrade : public ::google::protobuf::Message {
 public:
  Achieve_SC_Upgrade();
  virtual ~Achieve_SC_Upgrade();

  Achieve_SC_Upgrade(const Achieve_SC_Upgrade& from);

  inline Achieve_SC_Upgrade& operator=(const Achieve_SC_Upgrade& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Achieve_SC_Upgrade& default_instance();

  void Swap(Achieve_SC_Upgrade* other);

  // implements Message ----------------------------------------------

  Achieve_SC_Upgrade* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Achieve_SC_Upgrade& from);
  void MergeFrom(const Achieve_SC_Upgrade& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline ::google::protobuf::int32 ret() const;
  inline void set_ret(::google::protobuf::int32 value);

  // optional int32 cup_id = 2;
  inline bool has_cup_id() const;
  inline void clear_cup_id();
  static const int kCupIdFieldNumber = 2;
  inline ::google::protobuf::int32 cup_id() const;
  inline void set_cup_id(::google::protobuf::int32 value);

  // optional int32 points = 3;
  inline bool has_points() const;
  inline void clear_points();
  static const int kPointsFieldNumber = 3;
  inline ::google::protobuf::int32 points() const;
  inline void set_points(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:proto.message.Achieve_SC_Upgrade)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_cup_id();
  inline void clear_has_cup_id();
  inline void set_has_points();
  inline void clear_has_points();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 ret_;
  ::google::protobuf::int32 cup_id_;
  ::google::protobuf::int32 points_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_achieve_2eproto();
  friend void protobuf_AssignDesc_achieve_2eproto();
  friend void protobuf_ShutdownFile_achieve_2eproto();

  void InitAsDefaultInstance();
  static Achieve_SC_Upgrade* default_instance_;
};
// ===================================================================


// ===================================================================

// Achieve_CS_GetAchieveInfo

// -------------------------------------------------------------------

// AchieveInfo

// optional int32 id = 1;
inline bool AchieveInfo::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AchieveInfo::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AchieveInfo::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AchieveInfo::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 AchieveInfo::id() const {
  return id_;
}
inline void AchieveInfo::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional int32 con_num = 2;
inline bool AchieveInfo::has_con_num() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AchieveInfo::set_has_con_num() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AchieveInfo::clear_has_con_num() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AchieveInfo::clear_con_num() {
  con_num_ = 0;
  clear_has_con_num();
}
inline ::google::protobuf::int32 AchieveInfo::con_num() const {
  return con_num_;
}
inline void AchieveInfo::set_con_num(::google::protobuf::int32 value) {
  set_has_con_num();
  con_num_ = value;
}

// optional int32 status = 3;
inline bool AchieveInfo::has_status() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void AchieveInfo::set_has_status() {
  _has_bits_[0] |= 0x00000004u;
}
inline void AchieveInfo::clear_has_status() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void AchieveInfo::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::google::protobuf::int32 AchieveInfo::status() const {
  return status_;
}
inline void AchieveInfo::set_status(::google::protobuf::int32 value) {
  set_has_status();
  status_ = value;
}

// -------------------------------------------------------------------

// Achieve_SC_GetAchieveInfo

// optional int32 points = 1;
inline bool Achieve_SC_GetAchieveInfo::has_points() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Achieve_SC_GetAchieveInfo::set_has_points() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Achieve_SC_GetAchieveInfo::clear_has_points() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Achieve_SC_GetAchieveInfo::clear_points() {
  points_ = 0;
  clear_has_points();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveInfo::points() const {
  return points_;
}
inline void Achieve_SC_GetAchieveInfo::set_points(::google::protobuf::int32 value) {
  set_has_points();
  points_ = value;
}

// repeated .proto.message.AchieveInfo achieve_list = 2;
inline int Achieve_SC_GetAchieveInfo::achieve_list_size() const {
  return achieve_list_.size();
}
inline void Achieve_SC_GetAchieveInfo::clear_achieve_list() {
  achieve_list_.Clear();
}
inline const ::proto::message::AchieveInfo& Achieve_SC_GetAchieveInfo::achieve_list(int index) const {
  return achieve_list_.Get(index);
}
inline ::proto::message::AchieveInfo* Achieve_SC_GetAchieveInfo::mutable_achieve_list(int index) {
  return achieve_list_.Mutable(index);
}
inline ::proto::message::AchieveInfo* Achieve_SC_GetAchieveInfo::add_achieve_list() {
  return achieve_list_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::proto::message::AchieveInfo >&
Achieve_SC_GetAchieveInfo::achieve_list() const {
  return achieve_list_;
}
inline ::google::protobuf::RepeatedPtrField< ::proto::message::AchieveInfo >*
Achieve_SC_GetAchieveInfo::mutable_achieve_list() {
  return &achieve_list_;
}

// repeated int32 points_id_list = 3;
inline int Achieve_SC_GetAchieveInfo::points_id_list_size() const {
  return points_id_list_.size();
}
inline void Achieve_SC_GetAchieveInfo::clear_points_id_list() {
  points_id_list_.Clear();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveInfo::points_id_list(int index) const {
  return points_id_list_.Get(index);
}
inline void Achieve_SC_GetAchieveInfo::set_points_id_list(int index, ::google::protobuf::int32 value) {
  points_id_list_.Set(index, value);
}
inline void Achieve_SC_GetAchieveInfo::add_points_id_list(::google::protobuf::int32 value) {
  points_id_list_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
Achieve_SC_GetAchieveInfo::points_id_list() const {
  return points_id_list_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
Achieve_SC_GetAchieveInfo::mutable_points_id_list() {
  return &points_id_list_;
}

// optional int32 cup_id = 4;
inline bool Achieve_SC_GetAchieveInfo::has_cup_id() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Achieve_SC_GetAchieveInfo::set_has_cup_id() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Achieve_SC_GetAchieveInfo::clear_has_cup_id() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Achieve_SC_GetAchieveInfo::clear_cup_id() {
  cup_id_ = 0;
  clear_has_cup_id();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveInfo::cup_id() const {
  return cup_id_;
}
inline void Achieve_SC_GetAchieveInfo::set_cup_id(::google::protobuf::int32 value) {
  set_has_cup_id();
  cup_id_ = value;
}

// -------------------------------------------------------------------

// Achieve_CS_GetAchieveReward

// optional int32 id = 1;
inline bool Achieve_CS_GetAchieveReward::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Achieve_CS_GetAchieveReward::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Achieve_CS_GetAchieveReward::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Achieve_CS_GetAchieveReward::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Achieve_CS_GetAchieveReward::id() const {
  return id_;
}
inline void Achieve_CS_GetAchieveReward::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// -------------------------------------------------------------------

// Achieve_SC_GetAchieveReward

// optional int32 result = 1;
inline bool Achieve_SC_GetAchieveReward::has_result() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Achieve_SC_GetAchieveReward::set_has_result() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Achieve_SC_GetAchieveReward::clear_has_result() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Achieve_SC_GetAchieveReward::clear_result() {
  result_ = 0;
  clear_has_result();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveReward::result() const {
  return result_;
}
inline void Achieve_SC_GetAchieveReward::set_result(::google::protobuf::int32 value) {
  set_has_result();
  result_ = value;
}

// optional int32 id = 2;
inline bool Achieve_SC_GetAchieveReward::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Achieve_SC_GetAchieveReward::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Achieve_SC_GetAchieveReward::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Achieve_SC_GetAchieveReward::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveReward::id() const {
  return id_;
}
inline void Achieve_SC_GetAchieveReward::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional int32 status = 3;
inline bool Achieve_SC_GetAchieveReward::has_status() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Achieve_SC_GetAchieveReward::set_has_status() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Achieve_SC_GetAchieveReward::clear_has_status() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Achieve_SC_GetAchieveReward::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveReward::status() const {
  return status_;
}
inline void Achieve_SC_GetAchieveReward::set_status(::google::protobuf::int32 value) {
  set_has_status();
  status_ = value;
}

// optional int32 points = 4;
inline bool Achieve_SC_GetAchieveReward::has_points() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Achieve_SC_GetAchieveReward::set_has_points() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Achieve_SC_GetAchieveReward::clear_has_points() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Achieve_SC_GetAchieveReward::clear_points() {
  points_ = 0;
  clear_has_points();
}
inline ::google::protobuf::int32 Achieve_SC_GetAchieveReward::points() const {
  return points_;
}
inline void Achieve_SC_GetAchieveReward::set_points(::google::protobuf::int32 value) {
  set_has_points();
  points_ = value;
}

// -------------------------------------------------------------------

// Achieve_SC_UpdateAchieve

// optional int32 id = 1;
inline bool Achieve_SC_UpdateAchieve::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Achieve_SC_UpdateAchieve::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Achieve_SC_UpdateAchieve::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Achieve_SC_UpdateAchieve::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Achieve_SC_UpdateAchieve::id() const {
  return id_;
}
inline void Achieve_SC_UpdateAchieve::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional int32 con_num = 2;
inline bool Achieve_SC_UpdateAchieve::has_con_num() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Achieve_SC_UpdateAchieve::set_has_con_num() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Achieve_SC_UpdateAchieve::clear_has_con_num() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Achieve_SC_UpdateAchieve::clear_con_num() {
  con_num_ = 0;
  clear_has_con_num();
}
inline ::google::protobuf::int32 Achieve_SC_UpdateAchieve::con_num() const {
  return con_num_;
}
inline void Achieve_SC_UpdateAchieve::set_con_num(::google::protobuf::int32 value) {
  set_has_con_num();
  con_num_ = value;
}

// optional int32 status = 3;
inline bool Achieve_SC_UpdateAchieve::has_status() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Achieve_SC_UpdateAchieve::set_has_status() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Achieve_SC_UpdateAchieve::clear_has_status() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Achieve_SC_UpdateAchieve::clear_status() {
  status_ = 0;
  clear_has_status();
}
inline ::google::protobuf::int32 Achieve_SC_UpdateAchieve::status() const {
  return status_;
}
inline void Achieve_SC_UpdateAchieve::set_status(::google::protobuf::int32 value) {
  set_has_status();
  status_ = value;
}

// -------------------------------------------------------------------

// Achieve_CS_Upgrade

// -------------------------------------------------------------------

// Achieve_SC_Upgrade

// required int32 ret = 1;
inline bool Achieve_SC_Upgrade::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Achieve_SC_Upgrade::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Achieve_SC_Upgrade::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Achieve_SC_Upgrade::clear_ret() {
  ret_ = 0;
  clear_has_ret();
}
inline ::google::protobuf::int32 Achieve_SC_Upgrade::ret() const {
  return ret_;
}
inline void Achieve_SC_Upgrade::set_ret(::google::protobuf::int32 value) {
  set_has_ret();
  ret_ = value;
}

// optional int32 cup_id = 2;
inline bool Achieve_SC_Upgrade::has_cup_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Achieve_SC_Upgrade::set_has_cup_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Achieve_SC_Upgrade::clear_has_cup_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Achieve_SC_Upgrade::clear_cup_id() {
  cup_id_ = 0;
  clear_has_cup_id();
}
inline ::google::protobuf::int32 Achieve_SC_Upgrade::cup_id() const {
  return cup_id_;
}
inline void Achieve_SC_Upgrade::set_cup_id(::google::protobuf::int32 value) {
  set_has_cup_id();
  cup_id_ = value;
}

// optional int32 points = 3;
inline bool Achieve_SC_Upgrade::has_points() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Achieve_SC_Upgrade::set_has_points() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Achieve_SC_Upgrade::clear_has_points() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Achieve_SC_Upgrade::clear_points() {
  points_ = 0;
  clear_has_points();
}
inline ::google::protobuf::int32 Achieve_SC_Upgrade::points() const {
  return points_;
}
inline void Achieve_SC_Upgrade::set_points(::google::protobuf::int32 value) {
  set_has_points();
  points_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace message
}  // namespace proto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_achieve_2eproto__INCLUDED