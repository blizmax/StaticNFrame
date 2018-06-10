// -------------------------------------------------------------------------
//    @FileName         :    NFCDataValue.cpp
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#include "NFCDataValue.h"
#include "NFCData.h"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <limits>
#include <string.h>
#include <memory>

static const int max_depth = 200;

NFCDataValue::~NFCDataValue()
{
}

bool NFCDataValue::GetBool() const
{
	return false;
}

int NFCDataValue::GetInt() const
{
	return 0;
}

int32_t NFCDataValue::GetInt32() const
{
	return 0;
}

uint32_t NFCDataValue::GetUInt32() const
{
	return 0;
}

int64_t NFCDataValue::GetInt64() const
{
	return 0;
}

uint64_t NFCDataValue::GetUInt64() const
{
	return 0;
}

float NFCDataValue::GetFloat() const
{
	return 0;
}

double NFCDataValue::GetDouble() const
{
	return 0;
}

const std::string& NFCDataValue::GetString() const
{
	return NFCDataStatics::empty_string;
}

void NFCDataValue::SetUnknown()
{
}

void NFCDataValue::SetBool(bool value)
{
}

void NFCDataValue::SetInt(int value)
{
}

void NFCDataValue::SetInt32(int32_t value)
{
}

void NFCDataValue::SetUInt32(uint32_t value)
{
}

void NFCDataValue::SetInt64(int64_t value)
{
}

void NFCDataValue::SetUInt64(uint64_t value)
{
}

void NFCDataValue::SetFloat(float value)
{
}

void NFCDataValue::SetDouble(double value)
{
}

void NFCDataValue::SetString(const std::string& value)
{
}

void NFCDataValue::SetMapStringObject(const NFCData::MapStringObject& value)
{
}

void NFCDataValue::SetMapStringObject(NFCData::MapStringObject&& value)
{
}

void NFCDataValue::SetMapIntObject(const NFCData::MapIntObject& value)
{
}

void NFCDataValue::SetMapIntObject(NFCData::MapIntObject&& value)
{
}

void NFCDataValue::SetArray(const NFCData::Array& value)
{
}

void NFCDataValue::SetArray(NFCData::Array&& value)
{
}

void NFCDataValue::SetList(const NFCData::List& value)
{
}

void NFCDataValue::SetList(NFCData::List&& value)
{
}

const NFCData::Array& NFCDataValue::GetArray() const
{
	return NFCDataStatics::empty_vector;
}

const NFCData::List& NFCDataValue::GetList() const
{
	return NFCDataStatics::empty_list;
}

const NFCData::MapStringObject& NFCDataValue::GetMapStringObject() const
{
	return NFCDataStatics::empty_map_string;
}

const NFCData::MapIntObject& NFCDataValue::GetMapIntObject() const
{
	return NFCDataStatics::empty_map_int;
}

NFCData::Array& NFCDataValue::GetArray()
{
	return NFCDataStatics::empty_vector;
}

NFCData::List& NFCDataValue::GetList()
{
	return NFCDataStatics::empty_list;
}

NFCData::MapStringObject& NFCDataValue::GetMapStringObject()
{
	return NFCDataStatics::empty_map_string;
}

NFCData::MapIntObject& NFCDataValue::GetMapIntObject()
{
	return NFCDataStatics::empty_map_int;
}