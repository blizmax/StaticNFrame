// -------------------------------------------------------------------------
//    @FileName         :    NFCDataValue.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFPluginModule
//
// -------------------------------------------------------------------------

#pragma once

#include "NFCData.h"

class NFCDataValue
{
public:
	virtual ~NFCDataValue();

	virtual NF_DATA_TYPE Type() const = 0;
    //Get data
    virtual bool GetBool() const;
	virtual int  GetInt() const;
    virtual int32_t GetInt32() const;
    virtual uint32_t GetUInt32() const;
    virtual int64_t GetInt64() const;
    virtual uint64_t GetUInt64() const;
    virtual float GetFloat() const;
    virtual double GetDouble() const;
    virtual const std::string& GetString() const;

    //Set data
    virtual void SetUnknown();
    virtual void SetBool(bool value);
    virtual void SetInt(int value);
    virtual void SetInt32(int32_t value);
    virtual void SetUInt32(uint32_t value);
    virtual void SetInt64(int64_t value);
    virtual void SetUInt64(uint64_t value);
    virtual void SetFloat(float value);
    virtual void SetDouble(double value);
	virtual void SetString(const std::string& value);

    virtual const NFCData::Array& GetArray() const;
    virtual const NFCData::List& GetList() const;
    virtual const NFCData::MapStringObject& GetMapStringObject() const;
    virtual const NFCData::MapIntObject& GetMapIntObject() const;

    virtual NFCData::Array& GetArray();
    virtual NFCData::List& GetList();
    virtual NFCData::MapStringObject& GetMapStringObject();
    virtual NFCData::MapIntObject& GetMapIntObject();

	virtual void SetMapStringObject(const NFCData::MapStringObject& value);
	virtual void SetMapStringObject(NFCData::MapStringObject&& value);

	virtual void SetMapIntObject(const NFCData::MapIntObject& value);
	virtual void SetMapIntObject(NFCData::MapIntObject&& value);


	virtual void SetArray(const NFCData::Array& value);
	virtual void SetArray(NFCData::Array&& value);

	virtual void SetList(const NFCData::List& value);
	virtual void SetList(NFCData::List&& value);
};
