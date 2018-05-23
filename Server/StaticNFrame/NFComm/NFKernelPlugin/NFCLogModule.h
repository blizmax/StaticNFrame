// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_LOG_MODULE_H
#define NFC_LOG_MODULE_H

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFCore/NFSimpleBuffer.h"
#include "NFComm/NFCore/NFDateTime.hpp"

class NFCLogModule
	: public NFILogModule
{
public:
	explicit NFCLogModule(NFIPluginManager* p);
	virtual ~NFCLogModule() {}

	virtual bool Awake();
	virtual bool Init();
	virtual bool Shut();
	virtual bool Finalize();

	virtual bool BeforeShut();
	virtual bool AfterInit();

	virtual bool Execute();

	///////////////////////////////////////////////////////////////////////
	virtual void LogStack();

	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const char* func = "", int line = 0);
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func = "", int line = 0);
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func = "", int line = 0);
	virtual bool LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func = "", int line = 0);

	virtual bool ChangeLogLevel(const std::string& strLevel);
protected:
	virtual bool Log(const NF_LOG_LEVEL nll, const char* format, ...);

	static std::string GetNewLogFile(const std::string& oldFile);
	static void rolloutHandler(const char* filename, std::size_t size);
	static void LogCrashHandler(int sig);
private:
	static unsigned int idx;
	uint64_t mnLogCountTotal;
	std::string strLogConfigName;
	std::string strAppLogName;
	NFSimpleBuffer mBuffer;
};

#endif
