// -------------------------------------------------------------------------
//    @FileName         :    NFCPluginManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCPluginManager
//
// -------------------------------------------------------------------------

#include "NFCDynLib.h"

bool NFCDynLib::Load()
{
    std::string strLibPath = "./";
    strLibPath += mstrName;
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(strLibPath.c_str());

	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "LoadPlugin:{}", mstrName);
    return mInst != NULL;
}

bool NFCDynLib::UnLoad()
{
    DYNLIB_UNLOAD(mInst);
	NFLogInfo(NF_LOG_SYSTEMLOG, 0, "UnLoadPlugin:{}", mstrName);
    return true;
}

void* NFCDynLib::GetSymbol(const char* szProcName)
{
    return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}
