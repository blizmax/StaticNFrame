// -------------------------------------------------------------------------
//    @FileName         :    NFIObject.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/09
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"

class NFIObject
{
public:
    explicit NFIObject(uint64_t guid) {}

    virtual ~NFIObject() = default;

    virtual void Update() = 0;
    virtual uint64_t Self() = 0;

    virtual bool CheckHeartBeatExist(const std::string& name) = 0;
    virtual bool RemoveHeartBeat(const std::string& name) = 0;

    virtual bool CheckNodeExist(const std::string& name) = 0;

    virtual bool SetNodeBool(const std::string& name, const bool value) = 0;
    virtual bool SetNodeInt(const std::string& name, const int32_t value) = 0;
    virtual bool SetNodeInt64(const std::string& name, const int64_t value) = 0;
    virtual bool SetNodeFloat(const std::string& name, const float value) = 0;
    virtual bool SetNodeDouble(const std::string& name, const double value) = 0;
    virtual bool SetNodeString(const std::string& name, const std::string& value) = 0;

    virtual bool GetNodeBool(const std::string& name) = 0;
    virtual int32_t GetNodeInt(const std::string& name) = 0;
    virtual int64_t GetNodeInt64(const std::string& name) = 0;
    virtual float GetNodeFloat(const std::string& name) = 0;
    virtual double GetNodeDouble(const std::string& name) = 0;
    virtual const char* GetNodeString(const std::string& name) = 0;

    virtual bool CheckTableExist(const std::string& name) = 0;

    virtual bool SetTableBool(const std::string& name, const int row, const int col, const bool value) = 0;
    virtual bool SetTableInt(const std::string& name, const int row, const int col, const int32_t value) = 0;
    virtual bool SetTableInt64(const std::string& name, const int row, const int col, const int64_t value) = 0;
    virtual bool SetTableFloat(const std::string& name, const int row, const int col, const float value) = 0;
    virtual bool SetTableDouble(const std::string& name, const int row, const int col, const double value) = 0;
    virtual bool SetTableString(const std::string& name, const int row, const int col, const std::string& value) = 0;

    virtual bool GetTableBool(const std::string& name, const int row, const int col) = 0;
    virtual int32_t GetTableInt(const std::string& name, const int row, const int col) = 0;
    virtual int64_t GetTableInt64(const std::string& name, const int row, const int col) = 0;
    virtual float GetTableFloat(const std::string& name, const int row, const int col) = 0;
    virtual double GetTableDouble(const std::string& name, const int row, const int col) = 0;
    virtual const char* GetTableString(const std::string& name, const int row, const int col) = 0;
};