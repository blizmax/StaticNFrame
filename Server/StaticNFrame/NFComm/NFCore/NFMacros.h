// -------------------------------------------------------------------------
//    @FileName         :    NFMacros.h
//    @Author           :    GaoYi
//    @Date             :    2017-04-01
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// ------------------------------------------------------------------------

#pragma once

#define NF_DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
  TypeName(const TypeName&);                           \
  void operator=(const TypeName&)

#define NF_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                           \
  TypeName(const TypeName&);                            \
  void operator=(const TypeName&)

#define NF_ARRAYSIZE(Array) (sizeof(Array)/sizeof(Array[0]))
