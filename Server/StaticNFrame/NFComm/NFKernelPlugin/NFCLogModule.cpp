// -------------------------------------------------------------------------
//    @FileName         :    NFCLogModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLogModule
//    @Desc             :
// -------------------------------------------------------------------------

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <stdarg.h>
#include "NFCLogModule.h"

//#define ELPP_THREAD_SAFE                              //����Ҫ����
#include "common/easylogging++.h"
#include "termcolor.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFProfiler.h"


#define LOG_BUFFER_LENGTH (1024 * 10)

INITIALIZE_EASYLOGGINGPP;

unsigned int NFCLogModule::idx = 0;

bool NFCLogModule::CheckLogFileExist(const char* filename)
{
	if (filename == nullptr) return false;

    std::stringstream stream;
    stream << filename << "." << ++idx;
    std::fstream file;
    file.open(stream.str(), std::ios::in);
    if (file)
    {
        return CheckLogFileExist(filename);
    }

    return false;
}

void NFCLogModule::rolloutHandler(const char* filename, std::size_t size)
{
	if (filename == nullptr) return;

    std::stringstream stream;
    if (!CheckLogFileExist(filename))
    {
        stream << filename << "." << idx;
        int ret = rename(filename, stream.str().c_str());
		if (ret < 0)
		{
			std::cout << "rename file:" << filename << " failed!" << std::endl;
		}
    }
}

NFCLogModule::NFCLogModule(NFIPluginManager* p)
{
	mBuffer.Init(LOG_BUFFER_LENGTH);
    pPluginManager = p;
	mnLogCountTotal = 0;
    strLogConfigName = pPluginManager->GetLogConfigName();
    if (strLogConfigName.empty())
    {
        strLogConfigName = pPluginManager->GetAppName();
    }

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DEBUG_MODE
    strAppLogName = pPluginManager->GetConfigPath() + "Config/logconfig/" + strLogConfigName + "_win.conf";
#else
    strAppLogName = pPluginManager->GetConfigPath() + "Config/logconfig/" + strLogConfigName + "_win.conf";
#endif

#else
#ifdef NF_DEBUG_MODE
    strAppLogName = pPluginManager->GetConfigPath() + "Config/logconfig/" + strLogConfigName + ".conf";
#else
    strAppLogName = pPluginManager->GetConfigPath() + "Config/logconfig/" + strLogConfigName + ".conf";
#endif
#endif

    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);

    el::Configurations conf(strAppLogName);

    std::cout << "LogConfig: " << strAppLogName << std::endl;

    el::Loggers::reconfigureAllLoggers(conf);
    el::Helpers::installPreRollOutCallback(rolloutHandler);
}

bool NFCLogModule::Awake()
{
    return true;
}

bool NFCLogModule::Init()
{
    return true;
}

bool NFCLogModule::Finalize()
{
    el::Helpers::uninstallPreRollOutCallback();
    return true;
}

bool NFCLogModule::Shut()
{
    return true;
}

bool NFCLogModule::BeforeShut()
{
    return true;
}

bool NFCLogModule::AfterInit()
{
    return true;
}

bool NFCLogModule::Execute()
{
    return true;
}

bool NFCLogModule::Log(const NF_LOG_LEVEL nll, const char* format, ...)
{
    BEGIN_PROFILE("WRITE_LOG");

	int ret = 0;
    va_list args;
    va_start(args, format);
	ret = vsnprintf(mBuffer.data(), mBuffer.size(), format, args);
    va_end(args);

	if (ret < 0) return false;

    mnLogCountTotal++;
    switch (nll)
    {
        case NLL_DEBUG_NORMAL:
        {
            std::cout << termcolor::green;
            LOG(DEBUG) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
        }
        break;
        case NLL_INFO_NORMAL:
        {
            std::cout << termcolor::green;
            LOG(INFO) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
        }
        break;
        case NLL_WARING_NORMAL:
        {
            std::cout << termcolor::yellow;
            LOG(WARNING) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
        }
        break;
        case NLL_ERROR_NORMAL:
        {
            std::cout << termcolor::red;
            LOG(ERROR) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
            //LogStack();
        }
        break;
        case NLL_FATAL_NORMAL:
        {
            std::cout << termcolor::red;
            LOG(FATAL) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
        }
        break;
        default:
        {
            std::cout << termcolor::green;
            LOG(INFO) << mnLogCountTotal << " | " << pPluginManager->GetAppID() << " | " << mBuffer.data();
        }
        break;
    }

    std::cout << termcolor::reset;
    //memset(szBuffer, 0, writeSize);
    END_PROFILE();
    return true;
}

void NFCLogModule::LogStack()
{

    //To Add
    /*
    #ifdef NF_DEBUG_MODE
    time_t t = time(0);
    char szDmupName[MAX_PATH];
    tm* ptm = localtime(&t);

    sprintf(szDmupName, "%d_%d_%d_%d_%d_%d.dmp",  ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

    HANDLE hDumpFile = CreateFile(szDmupName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    //dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;


    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

    CloseHandle(hDumpFile);

    #endif
    */
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const std::string& strDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%lld] | %s %s | %s %d", ident, strInfo.c_str(), strDesc.c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%lld] | %s %s", ident, strInfo.c_str(), strDesc.c_str());
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const int64_t nDesc, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%lld] | %s %lld | %s %d", ident, strInfo.c_str(), nDesc, func, line);
    }
    else
    {
        Log(nll, "Indent[%lld] | %s %lld", ident, strInfo.c_str(), nDesc);
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::ostringstream& stream, const char* func, int line)
{
    if (line > 0)
    {
        Log(nll, "Indent[%lld] | %s | %s %d", ident, stream.str().c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%lld] | %s", ident, stream.str().c_str());
    }

    return true;
}

bool NFCLogModule::LogNormal(const NF_LOG_LEVEL nll, const uint64_t ident, const std::string& strInfo, const char* func /*= ""*/, int line /*= 0*/)
{
    if (line > 0)
    {
        Log(nll, "Indent[%d] %s %s %d", ident, strInfo.c_str(), func, line);
    }
    else
    {
        Log(nll, "Indent[%d] %s", ident, strInfo.c_str());
    }

    return true;
}

bool NFCLogModule::ChangeLogLevel(const std::string& strLevel)
{
    el::Level logLevel = el::LevelHelper::convertFromString(strLevel.c_str());
    el::Logger* pLogger = el::Loggers::getLogger("default");
    if (NULL == pLogger)
    {
        return false;
    }

    el::Configurations* pConfigurations = pLogger->configurations();

    switch (logLevel)
    {
        case el::Level::Fatal:
        {
            el::Configuration errorConfiguration(el::Level::Error, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&errorConfiguration);
        }
		break;
        case el::Level::Error:
        {
            el::Configuration warnConfiguration(el::Level::Warning, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&warnConfiguration);
        }
		break;
        case el::Level::Warning:
        {
            el::Configuration infoConfiguration(el::Level::Info, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&infoConfiguration);
        }
		break;
        case el::Level::Info:
        {
            el::Configuration debugConfiguration(el::Level::Debug, el::ConfigurationType::Enabled, "false");
            pConfigurations->set(&debugConfiguration);

        }
		break;
        case el::Level::Debug:
            break;
        default:
            break;
    }

    el::Loggers::reconfigureAllLoggers(*pConfigurations);
    LogNormal(NLL_INFO_NORMAL, 0, "[Log] Change log level", strLevel, __FUNCTION__, __LINE__);
    return true;
}
