// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-01-13
//    @Module           :    NFCAsyNoSqlModule
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCAsyNoSqlModule.h"
#include "NFCNoSqlDriverManager.h"

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFCore/NFProfiler.h"

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

bool SNosqlBaseParam::PackParam(std::string& strData)
{
    try
    {
        return xMsg.SerializeToString(&strData);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


bool SNosqlBaseParam::UnPackParam(const std::string& strData)
{
    try
    {
        if (!xMsg.ParseFromString(strData))
        {
            return false;
        }
    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool NFCNoSqlComponent::Init()
{
    return true;
}

bool NFCNoSqlComponent::AfterInit()
{
    m_pNoSqlDriverManager = NF_SHARE_PTR<NFINoSqlDriverManager>(NF_NEW NFCNoSqlDriverManager());

    return true;
}

int NFCNoSqlComponent::OnASyAddNoSqlServerEvent(const NFGUID& self, const int event, std::string& arg)
{
    NFMsg::PackMysqlServerInfo xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        std::cout << "NFCNoSqlComponent::OnASyAddNoSqlServerEvent Failed" << std::endl;
        return -1;
    }

    if (!m_pNoSqlDriverManager->AddNoSqlServer((int)xMsg.nserverid(), "", xMsg.strdnsip(), (int)xMsg.nport(), xMsg.strdbpwd()))
    {
        std::cout << "NFCNoSqlComponent::OnASyAddNoSqlServerEvent Failed" << std::endl;
        return -2;
    }

    //std::cout << "NFCNoSqlComponent::OnASyAddNoSqlServerEvent Sucess" << std::endl;
    return 0;
}

int NFCNoSqlComponent::OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg)
{
    m_pNoSqlDriverManager->CheckNoSql();
    //std::cout << "NFCNoSqlComponent::OnASyKeepServerAliveEvent" << std::endl;
    return 0;
}

int NFCNoSqlComponent::OnAsyLoadObjectEvent(const NFGUID& self, const int event, std::string& arg)
{
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        xMsg.set_ret(LOAD_PARAM_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr)
    {
        xMsg.set_ret(LOAD_NO_DRIVER_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    if (!pNoSqlDriver->HGet(xMsg.cache_property_key(), xMsg.self(), *xMsg.mutable_cache_property_value()))
    {
        xMsg.set_ret(LOAD_GET_CACHE_PROPERTY_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -3;
    }

    if (!pNoSqlDriver->HGet(xMsg.cache_record_key(), xMsg.self(), *xMsg.mutable_cache_record_value()))
    {
        xMsg.set_ret(LOAD_GET_CACHE_RECORD_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -4;
    }

    if (!pNoSqlDriver->HGet(xMsg.storage_property_key(), xMsg.self(), *xMsg.mutable_storage_property_value()))
    {
        xMsg.set_ret(LOAD_GET_STORAGE_PROPERTY_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -5;
    }

    if (!pNoSqlDriver->HGet(xMsg.storage_record_key(), xMsg.self(), *xMsg.mutable_storage_record_value()))
    {
        xMsg.set_ret(LOAD_GET_STORAGE_RECORD_ERROR);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -6;
    }

    xMsg.set_ret(LOAD_SUCCESS);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsySaveObjectEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = SAVE_PARAM_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = SAVE_NO_DRIVER_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    if (!pNoSqlDriver->HSet(xMsg.cache_property_key(), xMsg.self(), xMsg.cache_property_value()))
    {
        nRet = SAVE_SET_CACHE_PROPERTY_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -3;
    }

    if (!pNoSqlDriver->HSet(xMsg.cache_record_key(), xMsg.self(), xMsg.cache_record_value()))
    {
        nRet = SAVE_SET_CACHE_RECORD_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -4;
    }

    if (!pNoSqlDriver->HSet(xMsg.storage_property_key(), xMsg.self(), xMsg.storage_property_value()))
    {
        nRet = SAVE_SET_STORAGE_PROPERTY_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -5;
    }

    if (!pNoSqlDriver->HSet(xMsg.storage_record_key(), xMsg.self(), xMsg.storage_record_value()))
    {
        nRet = SAVE_SET_STORAGE_RECORD_ERROR;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -6;
    }

    nRet = SAVE_SUCCESS;

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyDelEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->Del(xMsg.str_key()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyExistsEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->Exists(xMsg.str_key()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyExpireEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->Expire(xMsg.str_key(), (unsigned int)xMsg.time()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyExpireatEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->Expireat(xMsg.str_key(), (unsigned int)xMsg.time()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsySetEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->Set(xMsg.str_key(), xMsg.str_value()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyGetEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    std::string strValue;
    xMsg.set_opt_ret(pNoSqlDriver->Get(xMsg.str_key(), strValue));
    xMsg.set_str_value(strValue);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsySetNXEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = -1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = -2;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->SetNX(xMsg.str_key(), xMsg.str_value()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsySetEXEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->SetEX(xMsg.str_key(), xMsg.str_value(), (unsigned int)xMsg.time()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyHSetEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->HSet(xMsg.str_key(), xMsg.str_field(), xMsg.str_value()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyHGetEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    std::string str_value;
    xMsg.set_opt_ret(pNoSqlDriver->HGet(xMsg.str_key(), xMsg.str_field(), str_value));
    xMsg.set_str_value(str_value);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyHDelEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->HDel(xMsg.str_key(), xMsg.str_field()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyHExistsEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->HExists(xMsg.str_key(), xMsg.str_field()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZAddEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->ZAdd(xMsg.str_key(), xMsg.score(), xMsg.str_member()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZIncrbyEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    xMsg.set_opt_ret(pNoSqlDriver->ZIncrBy(xMsg.str_key(), xMsg.str_member(), xMsg.increment()));

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZScoreEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    double zScore;
    xMsg.set_opt_ret(pNoSqlDriver->ZScore(xMsg.str_key(), xMsg.str_member(), zScore));
    xMsg.set_score(zScore);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZCardEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    int count;
    xMsg.set_opt_ret(pNoSqlDriver->ZCard(xMsg.str_key(), count));
    xMsg.set_count(count);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZRankEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    int rank;
    xMsg.set_opt_ret(pNoSqlDriver->ZRank(xMsg.str_key(), xMsg.str_member(), rank));
    xMsg.set_rank(rank);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZCountEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    int count;
    xMsg.set_opt_ret(pNoSqlDriver->ZCount(xMsg.str_key(), xMsg.start(), xMsg.end(), count));
    xMsg.set_count(count);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZRevRangeEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    std::vector<std::pair<std::string, double>> memberScoreVec;
    xMsg.set_opt_ret(pNoSqlDriver->ZRevRange(xMsg.str_key(), xMsg.start(), xMsg.end(), memberScoreVec));
    for (int i = 0; i < memberScoreVec.size(); i++)
    {
        ::NFMsg::NosqlMemberSocre* pInfo = xMsg.add_member_score();
        pInfo->set_member(memberScoreVec[i].first);
        pInfo->set_score(memberScoreVec[i].second);
        pInfo->set_rank(xMsg.start() + i);
    }

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZRevRankEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    int rank;
    xMsg.set_opt_ret(pNoSqlDriver->ZRevRank(xMsg.str_key(), xMsg.str_member(), rank));
    xMsg.set_rank(rank);

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZRangeEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    std::vector<std::pair<std::string, double>> memberScoreVec;
    xMsg.set_opt_ret(pNoSqlDriver->ZRange(xMsg.str_key(), xMsg.start(), xMsg.end(), memberScoreVec));
    for (int i = 0; i < memberScoreVec.size(); i++)
    {
        ::NFMsg::NosqlMemberSocre* pInfo = xMsg.add_member_score();
        pInfo->set_member(memberScoreVec[i].first);
        pInfo->set_score(memberScoreVec[i].second);
        pInfo->set_rank(xMsg.start() + i);
    }

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnAsyZRangeByScoreEvent(const NFGUID& self, const int event, std::string& arg)
{
    int nRet = 0;
    NFMsg::PackNoSqlParam xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -1;
    }

    NFINoSqlDriver* pNoSqlDriver = m_pNoSqlDriverManager->GetNoSqlDriver();
    if (pNoSqlDriver == nullptr || !pNoSqlDriver->Enable())
    {
        nRet = 1;
        xMsg.set_ret(nRet);
        arg.clear();
        xMsg.SerializeToString(&arg);
        return -2;
    }

    std::vector<std::pair<std::string, double>> memberScoreVec;
    xMsg.set_opt_ret(pNoSqlDriver->ZRangeByScore(xMsg.str_key(), xMsg.start(), xMsg.end(), memberScoreVec));
    for (int i = 0; i < memberScoreVec.size(); i++)
    {
        ::NFMsg::NosqlMemberSocre* pInfo = xMsg.add_member_score();
        pInfo->set_member(memberScoreVec[i].first);
        pInfo->set_score(memberScoreVec[i].second);
    }

    xMsg.set_ret(nRet);
    arg.clear();
    xMsg.SerializeToString(&arg);
    return 0;
}

int NFCNoSqlComponent::OnASyncEvent(const NFGUID& self, const int from, const int event, std::string& arg)
{
    int nRet = 0;
    switch (event)
    {
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ADDSERVER:
        {
            return OnASyAddNoSqlServerEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_KEEPALIVESERVER:
        {
            return OnASyKeepServerAliveEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_LOAD_OBJECT:
        {
            return OnAsyLoadObjectEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SAVE_OBJECT:
        {
            return OnAsySaveObjectEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_DEL:
        {
            return OnAsyDelEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXISTS:
        {
            return OnAsyExistsEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIRE:
        {
            return OnAsyExpireEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIREAT:
        {
            return OnAsyExpireatEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SET:
        {
            return OnAsySetEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_GET:
        {
            return OnAsyGetEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETNX:
        {
            return OnAsySetNXEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETEX:
        {
            return OnAsySetEXEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HSET:
        {
            return OnAsyHSetEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HGET:
        {
            return OnAsyHGetEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HDEL:
        {
            return OnAsyHDelEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HEXISTS:
        {
            return OnAsyHExistsEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZADD:
        {
            return OnAsyZAddEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZINCRBY:
        {
            return OnAsyZIncrbyEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZSCORE:
        {
            return OnAsyZScoreEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCARD:
        {
            return OnAsyZCardEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANK:
        {
            return OnAsyZRankEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCOUNT:
        {
            return OnAsyZCountEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANGE:
        {
            return OnAsyZRevRangeEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANK:
        {
            return OnAsyZRevRankEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGE:
        {
            return OnAsyZRangeEvent(self, event, arg);
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGEBYSCORE:
        {
            return OnAsyZRangeByScoreEvent(self, event, arg);
        }
        break;
        default:
        {
        }
        break;
    }

    return nRet;
}


/////////////////////////////////////////////

NFCAsyNoSqlModule::NFCAsyNoSqlModule(NFIPluginManager* p)
{
    nCurReqID = 0;
    mnSuitIndex = 0;
    mLastKeepAliveTime = NFGetTime();
    pPluginManager = p;
}

NFCAsyNoSqlModule::~NFCAsyNoSqlModule()
{

}

bool NFCAsyNoSqlModule::Init()
{
    m_pActorModule = pPluginManager->FindModule<NFIActorModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    StartActorPool(10);
    return true;
}

bool NFCAsyNoSqlModule::BeforeShut()
{
    CloseActorPool();
    return true;
}

bool NFCAsyNoSqlModule::Shut()
{
    return true;
}

bool NFCAsyNoSqlModule::AddCallBack(const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr)
{
    for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
        m_pActorModule->AddEndFunc(nAcotrID, nSubMsgID, functorPtr);
    }
    return true;
}

int NFCAsyNoSqlModule::GetActor()
{
    if (mActorList.Count() <= 0)
    {
        return -1;
    }

    mnSuitIndex++;
    mnSuitIndex = mnSuitIndex % mActorList.Count();

    int i = 0;
    for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
    {
        if (i < mActorList.Count() && i == mnSuitIndex)
        {
            return *pData;
        }

        ++i;
    }

    return -1;
}

bool NFCAsyNoSqlModule::AfterInit()
{
    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::NoSqlServer::ThisName());
    if (xLogicClass)
    {
        const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
        for (int i = 0; i < strIdList.size(); ++i)
        {
            const std::string& strId = strIdList[i];

            //const int nServerID = m_pElementModule->GetPropertyInt(strId, NFrame::NoSqlServer::ServerID());
            const int nPort = (int)m_pElementModule->GetPropertyInt(strId, NFrame::NoSqlServer::Port());
            const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
            const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());

            if (AddNoSqlServer(i, "", strIP, nPort, strAuth))
            {
                std::ostringstream strLog;
                strLog << "NFCAsyNoSqlModule Connected NoSqlServer[" << strIP << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
                m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

            }
            else
            {
                std::ostringstream strLog;
                strLog << "NFCAsyNoSqlModule Cannot connect NoSqlServer[" << strIP << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
                m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
            }
        }
    }
    return true;
}

bool NFCAsyNoSqlModule::StartActorPool(const int nCount)
{
    for (int i = 0; i < nCount; i++)
    {
        int nActorID = m_pActorModule->RequireActor<NFCNoSqlComponent>(this, &NFCAsyNoSqlModule::RequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int>(NF_NEW int(nActorID)));
        }
    }

    return true;
}

bool NFCAsyNoSqlModule::CloseActorPool()
{
    int nActor = 0;
    for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
    {
        m_pActorModule->ReleaseActor(nActor);
    }

    mActorList.ClearAll();
    return true;
}

template<typename ClassParam>
ClassParam* NFCAsyNoSqlModule::UnpackResult(const std::string& strMsgData)
{
    SNosqlBaseParam xResultparam;
    if (!xResultparam.UnPackParam(strMsgData))
    {
        return NULL;
    }

    NF_SHARE_PTR<SNosqlBaseParam> pReqBaseData = mReqList.GetElement(xResultparam.xMsg.req());
    if (NULL == pReqBaseData)
    {
        return NULL;
    }

    ClassParam* pReqData = dynamic_cast<ClassParam*>(pReqBaseData.get());
    if (NULL == pReqData)
    {
        return NULL;
    }

    pReqData->xMsg = xResultparam.xMsg;

    return pReqData;
}

int NFCAsyNoSqlModule::OnBaseNosqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SNosqlBaseParam* pReqData = UnpackResult<SNosqlBaseParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    int nReq = pReqData->xMsg.req();
    int nRet = pReqData->xMsg.ret();

    if (pReqData->mFunReturnRsp)
    {
        pReqData->mFunReturnRsp(pReqData->self, pReqData->xMsg, pReqData->mStrUseData);
    }

    mReqList.RemoveElement(nReq);
    return nRet;
}

int NFCAsyNoSqlModule::RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    int nRet = 0;
    switch (nEventID)
    {
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ADDSERVER:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_KEEPALIVESERVER:
        {
            nRet = 0;
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_LOAD_OBJECT:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SAVE_OBJECT:
        {
            BEGIN_PROFILE("Asyn Nosql Load Or Save Object Event");
            nRet = OnBaseNosqlAsyEnd(self, nFormActor, nEventID, strData);
            END_PROFILE();
        }
        break;
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_DEL:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXISTS:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIRE:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_EXPIREAT:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SET:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_GET:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETNX:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_SETEX:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HSET:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HGET:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HDEL:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_HEXISTS:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZADD:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZINCRBY:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZSCORE:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCARD:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANK:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZCOUNT:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANGE:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZREVRANK:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGE:
        case NFCAsyNoSqlModule::ACOTER_NOSQL_EVENT_ZRANGEBYSCORE:
        {
            BEGIN_PROFILE("Asyn Nosql Event");
            nRet = OnBaseNosqlAsyEnd(self, nFormActor, nEventID, strData);
            END_PROFILE();
        }
        break;
        default:
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_WARING_NORMAL, self, "Nosql Asyn Event:" + lexical_cast<std::string>(nEventID), "Not Handled!", __FUNCTION__, __LINE__);
        }
        break;
    }

    if (nRet != 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "Nosql Asyn Event:" + lexical_cast<std::string>(nEventID), "Return Not 0:" + lexical_cast<std::string>(nRet), __FUNCTION__, __LINE__);
    }
    return nRet;
}

bool NFCAsyNoSqlModule::Execute()
{
    if (mLastKeepAliveTime + 60 * 1000 < NFGetTime())
    {
        KeepAliveMysqlServer();
        mLastKeepAliveTime = NFGetTime();
    }

    return true;
}

bool NFCAsyNoSqlModule::AddNoSqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string& strAuthKey)
{
    NFMsg::PackMysqlServerInfo xMsg;

    xMsg.set_nport(nPort);
    xMsg.set_strdnsip(strIP);
    xMsg.set_strdbpwd(strAuthKey);
    xMsg.set_nserverid(nServerID);
    std::string arg;

    if (!xMsg.SerializeToString(&arg))
    {
        return false;
    }

    const int nEvetID = ACOTER_NOSQL_EVENT_ADDSERVER;
    for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
        m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}

bool NFCAsyNoSqlModule::KeepAliveMysqlServer()
{
    std::string arg;
    const int nEvetID = ACOTER_NOSQL_EVENT_KEEPALIVESERVER;
    for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
        m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}

int NFCAsyNoSqlModule::ApplyRequest(NF_SHARE_PTR<SNosqlBaseParam> pParam, const int nEvetID/* = ACOTERMYSQLEVENT_USEDB*/)
{
    int nAcotrID = GetActor();
    if (nAcotrID <= 0)
    {
        return -2;
    }

    std::string arg;
    if (!pParam->PackParam(arg))
    {
        return -3;
    }

    if (!mReqList.AddElement(pParam->xMsg.req(), pParam))
    {
        return -4;
    }

    if (!m_pActorModule->SendMsgToActor(nAcotrID, pParam->self, nEvetID, arg))
    {
        mReqList.RemoveElement(pParam->xMsg.req());
        return -5;
    }

    return 0;
}

bool NFCAsyNoSqlModule::LoadNFObject(const NFGUID& self, const NFMsg::PackNoSqlParam xMsg, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlLoadObjectParam());
    if (NULL == pParam)
    {
        return false;
    }
    int nType = pParam->xMsg.type();

    pParam->xMsg = xMsg;
    pParam->xMsg.set_type(nType);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::SaveNFObject(const NFGUID& self, const NFMsg::PackNoSqlParam xMsg, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlSaveObjectParam());
    if (NULL == pParam)
    {
        return false;
    }
    int nType = pParam->xMsg.type();

    pParam->xMsg = xMsg;
    pParam->xMsg.set_type(nType);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Del(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlDelParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Exists(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlExistsParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Expire(const NFGUID& self, const std::string& strKey, unsigned int nSecs, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlExpireParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);
    pParam->xMsg.set_time(nSecs);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Expireat(const NFGUID& self, const std::string& strKey, unsigned int nUnixTime, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlExpireatParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);
    pParam->xMsg.set_time(nUnixTime);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Set(const NFGUID& self, const std::string& strKey, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)                           //设置strKey值为strValue
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlSetParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);
    pParam->xMsg.set_str_value(strValue);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::Get(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)                                                        //获得strKey的值
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlGetParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

//SET if Not eXists
bool NFCAsyNoSqlModule::SetNX(const NFGUID& self, const std::string& strKey, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlSetNXParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_value(strValue);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

//set key->value and set Expire time
bool NFCAsyNoSqlModule::SetEX(const NFGUID& self, const std::string& strKey, const std::string& strValue,
                              const unsigned int nSeconds, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlSetEXParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_value(strValue);
    pParam->xMsg.set_time(nSeconds);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::HSet(const NFGUID& self, const std::string& strKey, const std::string& strField, const std::string& strValue, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlHSetParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_field(strField);
    pParam->xMsg.set_str_value(strValue);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::HGet(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlHGetParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_field(strField);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::HExists(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlHExistsParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_field(strField);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::HDel(const NFGUID& self, const std::string& strKey, const std::string& strField, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlHDelParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_field(strField);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZAdd(const NFGUID& self, const std::string& strKey, const double nScore, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZAddParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_score(nScore);
    pParam->xMsg.set_str_member(strMember);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZIncrBy(const NFGUID& self, const std::string& strKey, const std::string& strMember, const double nIncrement, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZIncrbyParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_member(strMember);
    pParam->xMsg.set_increment(nIncrement);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZScore(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZScoreParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_member(strMember);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZCard(const NFGUID& self, const std::string& strKey, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZCardParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZRank(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZRankParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_member(strMember);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZCount(const NFGUID& self, const std::string& strKey, const int nMin, const int nMax, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZCountParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_start(nMin);
    pParam->xMsg.set_end(nMax);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}


bool NFCAsyNoSqlModule::ZRevRange(const NFGUID& self, const std::string& strKey, const int nStart, const int nStop, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZRevRangeParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_start(nStart);
    pParam->xMsg.set_end(nStop);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZRevRank(const NFGUID& self, const std::string& strKey, const std::string& strMember, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZRevRankParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_str_member(strMember);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZRange(const NFGUID& self, const std::string& strKey, const int nStartIndex, const int nEndIndex, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZRangeParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_start(nStartIndex);
    pParam->xMsg.set_end(nEndIndex);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyNoSqlModule::ZRangeByScore(const NFGUID& self, const std::string& strKey, const int nMin, const int nMax, const NOSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SNosqlBaseParam> pParam(NF_NEW SNosqlZRangeByScoreParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->xMsg.set_str_key(strKey);
    pParam->xMsg.set_start(nMin);
    pParam->xMsg.set_end(nMax);
    pParam->xMsg.set_req(nCurReqID++);

    pParam->mStrUseData = strUseData;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->xMsg.type()) < 0)
    {
        return false;
    }

    return true;
}