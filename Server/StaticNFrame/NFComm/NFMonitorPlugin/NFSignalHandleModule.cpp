// -------------------------------------------------------------------------
//    @FileName         :    NFSignalHandleModule.cpp
//    @Author           :    Yi.Gao
//    @Date             :    2019-3-26
//    @Module           :    NFMonitorPlugin
//
// -------------------------------------------------------------------------

#include "NFSignalHandleModule.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#include <signal.h>
#include <time.h>
#include <algorithm>
#include <string.h>

static NFCSignalHandleModule* g_signalHandleModule = nullptr;

#if NF_PLATFORM != NF_PLATFORM_WIN

#include <ucontext.h>
#include <sys/ucontext.h>
#include <execinfo.h>

bool Symbolize(void *pc, char *out, int out_size) {
	return false;
}

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

// Returns the program counter from signal context, NULL if unknown.
void* GetPC(void* ucontext_in_void) {
  if (ucontext_in_void != NULL) {
    ucontext_t *context = reinterpret_cast<ucontext_t *>(ucontext_in_void);
    return (void*)context->uc_link;
  }
  return NULL;
}

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

// The class is used for formatting error messages.  We don't use printf()
// as it's not async signal safe.
class MinimalFormatter {
 public:
  MinimalFormatter(char *buffer, int size)
      : buffer_(buffer),
        cursor_(buffer),
        end_(buffer + size) {
  }

  // Returns the number of bytes written in the buffer.
  int num_bytes_written() const { return cursor_ - buffer_; }

  // Appends string from "str" and updates the internal cursor.
  void AppendString(const char* str) {
    int i = 0;
    while (str[i] != '\0' && cursor_ + i < end_) {
      cursor_[i] = str[i];
      ++i;
    }
    cursor_ += i;
  }

  // Formats "number" in "radix" and updates the internal cursor.
  // Lowercase letters are used for 'a' - 'z'.
  void AppendUint64(uint64_t number, int radix) {
    int i = 0;
    while (cursor_ + i < end_) {
      const int tmp = number % radix;
      number /= radix;
      cursor_[i] = (tmp < 10 ? '0' + tmp : 'a' + tmp - 10);
      ++i;
      if (number == 0) {
        break;
      }
    }
    // Reverse the bytes written.
    std::reverse(cursor_, cursor_ + i);
    cursor_ += i;
  }

  // Formats "number" as hexadecimal number, and updates the internal
  // cursor.  Padding will be added in front if needed.
  void AppendHexWithPadding(uint64_t number, int width) {
    char* start = cursor_;
    AppendString("0x");
    AppendUint64(number, 16);
    // Move to right and add padding in front if needed.
    if (cursor_ < start + width) {
      const int64_t delta = start + width - cursor_;
      std::copy(start, cursor_, start + delta);
      std::fill(start, start + delta, ' ');
      cursor_ = start + width;
    }
  }

 private:
  char *buffer_;
  char *cursor_;
  const char * const end_;
};

// Dumps time information.  We don't dump human-readable time information
// as localtime() is not guaranteed to be async signal safe.
void DumpTimeInfo() {
  time_t time_in_sec = time(NULL);
  char buf[256];  // Big enough for time info.
  MinimalFormatter formatter(buf, sizeof(buf));
  formatter.AppendString("*** Aborted at ");
  formatter.AppendUint64(time_in_sec, 10);
  formatter.AppendString(" (unix time)");
  formatter.AppendString(" try \"date -d @");
  formatter.AppendUint64(time_in_sec, 10);
  formatter.AppendString("\" if you are using GNU date ***\n");

  NFLogError(NF_LOG_SYSTEMLOG, 0, "{}", std::string(buf, formatter.num_bytes_written()));
}

// Dumps information about the signal to STDERR.
void DumpSignalInfo(int signal_number, siginfo_t *siginfo) {
  // Get the signal name.
  const char* signal_name = NULL;
  for (size_t i = 0; i < NF_ARRAYSIZE(kFailureSignals); ++i) {
    if (signal_number == kFailureSignals[i].number) {
      signal_name = kFailureSignals[i].name;
    }
  }

  char buf[256];  // Big enough for signal info.
  MinimalFormatter formatter(buf, sizeof(buf));

  formatter.AppendString("*** ");
  if (signal_name) {
    formatter.AppendString(signal_name);
  } else {
    // Use the signal number if the name is unknown.  The signal name
    // should be known, but just in case.
    formatter.AppendString("Signal ");
    formatter.AppendUint64(signal_number, 10);
  }
  formatter.AppendString(" (@0x");
  formatter.AppendUint64(reinterpret_cast<uintptr_t>(siginfo->si_addr), 16);
  formatter.AppendString(")");
  formatter.AppendString(" received by PID ");
  formatter.AppendUint64(getpid(), 10);
  formatter.AppendString(" (TID 0x");
  // We assume pthread_t is an integral number or a pointer, rather
  // than a complex struct.  In some environments, pthread_self()
  // returns an uint64 but in some other environments pthread_self()
  // returns a pointer.  Hence we use C-style cast here, rather than
  // reinterpret/static_cast, to support both types of environments.
  formatter.AppendUint64((uintptr_t)pthread_self(), 16);
  formatter.AppendString(") ");
  // Only linux has the PID of the signal sender in si_pid.
#ifdef OS_LINUX
  formatter.AppendString("from PID ");
  formatter.AppendUint64(siginfo->si_pid, 10);
  formatter.AppendString("; ");
#endif
  formatter.AppendString("stack trace: ***\n");
  NFLogError(NF_LOG_SYSTEMLOG, 0, "{}", std::string(buf, formatter.num_bytes_written()));
}

// Dumps information about the stack frame to STDERR.
void DumpStackFrameInfo(const char* prefix, void* pc) {
  // Get the symbol name.
  const char *symbol = "(unknown)";
  char symbolized[1024];  // Big enough for a sane symbol.
  // Symbolizes the previous address of pc because pc may be in the
  // next function.
  if (Symbolize(reinterpret_cast<char *>(pc) - 1,
                symbolized, sizeof(symbolized))) {
    symbol = symbolized;
  }

  char buf[1024];  // Big enough for stack frame info.
  MinimalFormatter formatter(buf, sizeof(buf));

  formatter.AppendString(prefix);
  formatter.AppendString("@ ");
  const int width = 2 * sizeof(void*) + 2;  // + 2  for "0x".
  formatter.AppendHexWithPadding(reinterpret_cast<uintptr_t>(pc), width);
  formatter.AppendString(" ");
  formatter.AppendString(symbol);
  formatter.AppendString("\n");
  NFLogError(NF_LOG_SYSTEMLOG, 0, "{}", std::string(buf, formatter.num_bytes_written()));
}

// Invoke the default signal handler.
void InvokeDefaultSignalHandler(int signal_number) {
  struct sigaction sig_action;
  memset(&sig_action, 0, sizeof(sig_action));
  sigemptyset(&sig_action.sa_mask);
  sig_action.sa_handler = SIG_DFL;
  sigaction(signal_number, &sig_action, NULL);
  kill(getpid(), signal_number);
}

// This variable is used for protecting FailureSignalHandler() from
// dumping stuff while another thread is doing it.  Our policy is to let
// the first thread dump stuff and let other threads wait.
// See also comments in FailureSignalHandler().
static pthread_t* g_entered_thread_id_pointer = NULL;

// Wrapper of __sync_val_compare_and_swap. If the GCC extension isn't
// defined, we try the CPU specific logics (we only support x86 and
// x86_64 for now) first, then use a naive implementation, which has a
// race condition.
template<typename T>
inline T sync_val_compare_and_swap(T* ptr, T oldval, T newval) {
#if defined(HAVE___SYNC_VAL_COMPARE_AND_SWAP)
  return __sync_val_compare_and_swap(ptr, oldval, newval);
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
  T ret;
  __asm__ __volatile__("lock; cmpxchg %1, (%2);"
                       :"=a"(ret)
                        // GCC may produces %sil or %dil for
                        // constraint "r", but some of apple's gas
                        // dosn't know the 8 bit registers.
                        // We use "q" to avoid these registers.
                       :"q"(newval), "q"(ptr), "a"(oldval)
                       :"memory", "cc");
  return ret;
#else
  T ret = *ptr;
  if (ret == oldval) {
    *ptr = newval;
  }
  return ret;
#endif
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
		sync_val_compare_and_swap(
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

	// Get the stack traces.
	void *stack[32];
	// +1 to exclude this function.
	const int depth = GetStackTrace(stack, NF_ARRAYSIZE(stack), 1);
	DumpSignalInfo(signal_number, signal_info);
	// Dump the stack traces.
	for (int i = 0; i < depth; ++i) {
		DumpStackFrameInfo("    ", stack[i]);
	}

	if (g_signalHandleModule)
	{
		g_signalHandleModule->HandleSignal(signal_number);
	}

	// Kill ourself by the default signal handler.
	InvokeDefaultSignalHandler(signal_number);
}
#endif

NFCSignalHandleModule::NFCSignalHandleModule(NFIPluginManager* p)
{
	m_pPluginManager = p;
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
	return true;
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
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

	// Build the sigaction struct.
	struct sigaction sig_action;
	memset(&sig_action, 0, sizeof(sig_action));
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_flags |= SA_SIGINFO;
	sig_action.sa_sigaction = &FailureSignalHandler;

	for (size_t i = 0; i < NF_ARRAYSIZE(kFailureSignals); ++i) {
		sigaction(kFailureSignals[i].number, &sig_action, NULL);
	}
#endif
}

