// -------------------------------------------------------------------------
//    @FileName         :    NFDataNode.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------
#pragma once

#include "NFCData.h"
#include <string>
#include "NFBitValue.hpp"


class NFDataNode
{
public:
	enum DATA_NODE_FEATURE
	{
		PF_PUBLIC = 0, //send to others
		PF_PRIVATE = 1, //send to self
		PF_REAL_TIME = 2, //send real-time when changed
		PF_SAVE = 3, //if need save to database
	};

	bool IsPublic() const
	{
		return NFBitValue<int8_t>::HaveBitValue(mFeature, PF_PUBLIC);
	}

	bool IsPrivate() const
	{
		return NFBitValue<int8_t>::HaveBitValue(mFeature, PF_PRIVATE);
	}

	bool IsRealTime() const
	{
		return NFBitValue<int8_t>::HaveBitValue(mFeature, PF_REAL_TIME);
	}

	bool IsSave() const
	{
		return NFBitValue<int8_t>::HaveBitValue(mFeature, PF_SAVE);
	}

	bool Changed() const
	{
		return !(mValue.IsNullValue());
	}

	const std::string& GetName() const
	{
		return mName;
	}

	const NFCData& GetValue() const
	{
		return mValue;
	}

	int GetType() const
	{
		return mValue.GetType();
	}

	int8_t GetFeature() const
	{
		return mFeature;
	}

	std::string mName; //DataNode name, max = 16 bytes
	NFCData mValue; //DataNode value
	int8_t mFeature = 0; //DataNode feature
};

