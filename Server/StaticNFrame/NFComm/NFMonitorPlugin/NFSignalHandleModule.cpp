// -------------------------------------------------------------------------
//    @FileName         :    NFSignalHandleModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#include "NFSignalHandleModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"

static NFCSignalHandleModule* g_signalHandleModule = nullptr;

const char * SIGNAL_NAMES[] =
{
	NULL,
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGBUS",
	"SIGFPE",
	"SIGKILL",
	"SIGUSR1",
	"SIGSEGV",
	"SIGUSR2",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGSTKFLT",
	"SIGCHLD",
	"SIGCONT",
	"SIGSTOP",
	"SIGTSTP",
	"SIGTTIN",
	"SIGTTOU",
	"SIGURG",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGIO",
	"SIGPWR",
	"SIGSYS"
};

NFCSignalHandleModule::NFCSignalHandleModule()
{
	g_signalHandleModule = this;
	InitSignal();
}

NFCSignalHandleModule::~NFCSignalHandleModule()
{
	g_signalHandleModule = nullptr;
}

bool NFCSignalHandleModule::Init()
{
	return true;
}

bool NFCSignalHandleModule::ReadyExecute()
{
	InitSignal();
	return true;
}

static void sig_usr(int signo)
{
	if (g_signalHandleModule)
	{
		g_signalHandleModule->HandleSignal(signo);
	}
}

void  NFCSignalHandleModule::HandleSignal(int signo)
{
	if (signo >= 1 && signo < NF_ARRAYSIZE(SIGNAL_NAMES))
	{
		NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, "recv signo:{} desc:{}", signo, SIGNAL_NAMES[signo]);
	}
	else
	{
		NFLogWarning(NF_LOG_MONITOR_PLUGIN, 0, "recv signo:{} desc:unknown signal", signo);
	}
#if NF_PLATFORM != NF_PLATFORM_WIN
	switch (signo)
	{
	case SIGUSR1:
	case SIGUSR2:
	case SIGKILL:
	case SIGINT:
	case SIGQUIT:
	case SIGHUP:
	case SIGTERM:
	{
		m_pPluginManager->End();
	}
	break;
	default:
		break;
	}
#endif
	exit(0);
}

void NFCSignalHandleModule::InitSignal()
{
#if NF_PLATFORM != NF_PLATFORM_WIN
	signal(SIGUSR1, sig_usr);
	signal(SIGUSR2, sig_usr);
	signal(SIGKILL, sig_usr);
	signal(SIGINT, sig_usr);
	signal(SIGQUIT, sig_usr);
	signal(SIGHUP, sig_usr);
	signal(SIGTERM, sig_usr);
	// ignore signals

	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
#endif
}