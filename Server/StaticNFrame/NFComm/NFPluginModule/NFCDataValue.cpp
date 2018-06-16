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
#include "NFDefine.h"

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

void NFCDataValue::SetMapStringData(const NFCData::MapStringData& value)
{
}

void NFCDataValue::SetMapStringData(NFCData::MapStringData&& value)
{
}

void NFCDataValue::SetMapIntData(const NFCData::MapIntData& value)
{
}

void NFCDataValue::SetMapIntData(NFCData::MapIntData&& value)
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
	return NFCDataStatics::empty_array;
}

const NFCData::List& NFCDataValue::GetList() const
{
	return NFCDataStatics::empty_list;
}

const NFCData::MapStringData& NFCDataValue::GetMapStringData() const
{
	return NFCDataStatics::empty_map_string;
}

const NFCData::MapIntData& NFCDataValue::GetMapIntData() const
{
	return NFCDataStatics::empty_map_int;
}

NFCData::Array* NFCDataValue::MutableArray()
{
	return nullptr;
}

NFCData::List* NFCDataValue::MutableList()
{
	return nullptr;
}

NFCData::MapStringData* NFCDataValue::MutableMapStringData()
{
	return nullptr;
}

NFCData::MapIntData* NFCDataValue::MutableMapIntData()
{
	return nullptr;
}

void NFCDataValue::AddArrayItem(const NFCData& data)
{
}

void NFCDataValue::AddListItem(const NFCData& data)
{
}

void NFCDataValue::AddMapStringItem(const std::string& key, const NFCData& value)
{
}

void NFCDataValue::AddMapIntItem(uint64_t key, const NFCData& value)
{
}

