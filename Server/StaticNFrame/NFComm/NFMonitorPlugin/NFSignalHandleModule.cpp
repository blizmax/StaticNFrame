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

#if NF_PLATFORM != NF_PLATFORM_WIN
const struct {
	int number;
	const char *name;
} kFailureSignals[] = {
	{ SIGSEGV, "SIGSEGV" },
	{ SIGILL, "SIGILL" },
	{ SIGFPE, "SIGFPE" },
	{ SIGABRT, "SIGABRT" },
	{ SIGBUS, "SIGBUS" },
	{ SIGTERM, "SIGTERM" },
	{ SIGUSR1, "SIGUSR1" },
	{ SIGUSR2, "SIGUSR2" },
	{ SIGKILL, "SIGKILL" },
	{ SIGINT, "SIGINT" },
	{ SIGQUIT, "SIGQUIT"},
	{ SIGHUP, "SIGHUP"},
	{ SIGTERM, "SIGTERM"},
};

// If you change this function, also change GetStackFrames below.
int GetStackTrace(void** result, int max_depth, int skip_count) {
	static const int kStackLength = 64;
	void * stack[kStackLength];
	int size;

	size = backtrace(stack, kStackLength);
	skip_count++;  // we want to skip the current frame as well
	int result_count = size - skip_count;
	if (result_count < 0)
		result_count = 0;
	if (result_count > max_depth)
		result_count = max_depth;
	for (int i = 0; i < result_count; i++)
		result[i] = stack[i + skip_count];

	return result_count;
}

// Dumps signal and stack frame information, and invokes the default
// signal handler once our job is done.
void FailureSignalHandler(int signal_number,
	siginfo_t *signal_info,
	void *ucontext) {
	// First check if we've already entered the function.  We use an atomic
	// compare and swap operation for platforms that support it.  For other
	// platforms, we use a naive method that could lead to a subtle race.

	// We assume pthread_self() is async signal safe, though it's not
	// officially guaranteed.
	pthread_t my_thread_id = pthread_self();
	// NOTE: We could simply use pthread_t rather than pthread_t* for this,
	// if pthread_self() is guaranteed to return non-zero value for thread
	// ids, but there is no such guarantee.  We need to distinguish if the
	// old value (value returned from __sync_val_compare_and_swap) is
	// different from the original value (in this case NULL).
	pthread_t* old_thread_id_pointer =
		glog_internal_namespace_::sync_val_compare_and_swap(
			&g_entered_thread_id_pointer,
			static_cast<pthread_t*>(NULL),
			&my_thread_id);
	if (old_thread_id_pointer != NULL) {
		// We've already entered the signal handler.  What should we do?
		if (pthread_equal(my_thread_id, *g_entered_thread_id_pointer)) {
			// It looks the current thread is reentering the signal handler.
			// Something must be going wrong (maybe we are reentering by another
			// type of signal?).  Kill ourself by the default signal handler.
			InvokeDefaultSignalHandler(signal_number);
		}
		// Another thread is dumping stuff.  Let's wait until that thread
		// finishes the job and kills the process.
		while (true) {
			sleep(1);
		}
	}
	// This is the first time we enter the signal handler.  We are going to
	// do some interesting stuff from here.
	// TODO(satorux): We might want to set timeout here using alarm(), but
	// mixing alarm() and sleep() can be a bad idea.

	// First dump time info.
	DumpTimeInfo();

	// Get the program counter from ucontext.
	void *pc = GetPC(ucontext);
	DumpStackFrameInfo("PC: ", pc);

#ifdef HAVE_STACKTRACE
	// Get the stack traces.
	void *stack[32];
	// +1 to exclude this function.
	const int depth = GetStackTrace(stack, ARRAYSIZE(stack), 1);
	DumpSignalInfo(signal_number, signal_info);
	// Dump the stack traces.
	for (int i = 0; i < depth; ++i) {
		DumpStackFrameInfo("    ", stack[i]);
	}
#endif

	// *** TRANSITION ***
	//
	// BEFORE this point, all code must be async-termination-safe!
	// (See WARNING above.)
	//
	// AFTER this point, we do unsafe things, like using LOG()!
	// The process could be terminated or hung at any time.  We try to
	// do more useful things first and riskier things later.

	// Flush the logs before we do anything in case 'anything'
	// causes problems.
	FlushLogFilesUnsafe(0);

	// Kill ourself by the default signal handler.
	InvokeDefaultSignalHandler(signal_number);
}
#endif

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
	//signal(SIGUSR1, sig_usr);
	//signal(SIGUSR2, sig_usr);
	//signal(SIGKILL, sig_usr);
	//signal(SIGINT, sig_usr);
	//signal(SIGQUIT, sig_usr);
	//signal(SIGHUP, sig_usr);
	//signal(SIGTERM, sig_usr);
	// ignore signals

	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

	// Build the sigaction struct.
	struct sigaction sig_action;
	memset(&sig_action, 0, sizeof(sig_action));
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags |= SA_SIGINFO;
	sig_action.sa_sigaction = &FailureSignalHandler;

	for (size_t i = 0; i < ARRAYSIZE(kFailureSignals); ++i) {
		sigaction(kFailureSignals[i].number, &sig_action, NULL);
	}
}
#endif
}