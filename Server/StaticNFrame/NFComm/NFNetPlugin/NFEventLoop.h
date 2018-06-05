// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoop.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFLoopThreadStatus.h"
#include "NFComm/NFCore/NFPlatform.h"
#include <mutex>
#include <vector>

struct event_base;
class NFPipeEventWatcher;

// This is the IO Event driving kernel. Reactor model.
// This class is a wrapper of event_base but not only a wrapper.
// It provides a simple way to run a IO Event driving loop.
// One thread one loop.
/**
* @brief 这是IO事件驱动核心, Reactor模式实现
* 这个类封装了event_base，但不仅仅是一个封装而已，
* 它提供了一个简单的方式在循环驱动中去运行IO事件
*  一个线程一个循环
*/
class NFEventLoop : public NFLoopTheadStatus
{
public:
	typedef std::function<void()> Functor;
public:
	/**
	 * @brief 构造函数, 它将自己构造一个自己的event_base, 并一直保管它
	 */
	NFEventLoop();

	/**
	* @brief 构造函数， 它将使用一个已经存在的event_base结构体
	*/
	explicit NFEventLoop(struct event_base* base);

	/**
	 * @brief 析构函数
	 */
	~NFEventLoop();

	// @brief Run the IO Event driving loop forever
	// @note It must be called in the IO Event thread

	/**
	 * @brief 线程运行的主循环, 将一直运行IO事件驱动
	 * 这个函数必须在IO线程中运行
	 * @return void
	 */
	void Run();

	/**
	 * @brief 停止IO事件循环
	 *
	 * @return void
	 */
	void Stop();

	/**
	 * @brief 在IO线程里运行函数handler，
	 * 如果这个函数本身就在io线程里，那么立马执行
	 * 如果在别的线程里，将先把事件发送到队列中然后在循环中执行
	 *
	 * @param  handler 要执行的函数
	 * @return void
	 */
	void RunInLoop(const Functor& handler);

	/**
	 * @brief 把函数保存到队列中, 然后在IO线程里执行
	 *
	 * @param  handler 要执行的函数
	 * @return void
	 */
	void QueueInLoop(const Functor& handler);

	/**
	* @brief 在IO线程里运行函数handler，
	* 如果这个函数本身就在io线程里，那么立马执行
	* 如果在别的线程里，将先把事件发送到队列中然后在循环中执行
	*
	* @param  handler 要执行的函数
	* @return void
	*/
	void RunInLoop(Functor&& handler);

	/**
	* @brief 把函数保存到队列中, 然后在IO线程里执行
	*
	* @param  handler 要执行的函数
	* @return void
	*/
	void QueueInLoop(Functor&& handler);

	/**
	 * @brief 获得数据event_base
	 *
	 * @return struct event_base*
	 */
	struct event_base* event_base();

	/**
	 * @brief 当前是否在IO线程里运行
	 *
	 * @return bool
	 */
	bool IsInLoopThread() const
	{
		return tid_ == std::this_thread::get_id();
	}

	/**
	 * @brief 队列里的函数对象个数, 线程安全
	 *
	 * @return int
	 */
	int pending_functor_count() const
	{
		return pending_functor_count_.load();
	}

	/**
	 * @brief 线程ID
	 *
	 * @return const std::thread::id&
	 */
	const std::thread::id& tid() const
	{
		return tid_;
	}

private:
	/**
	 * @brief 初始化数据
	 *
	 * @return void
	 */
	void Init();

	/**
	 * @brief 初始化watcher， 用于多线程之间的通信
	 *
	 * @return void
	 */
	void InitNotifyPipeWatcher();

	/**
	 * @brief 在IO线程里调用IO循环退出函数
	 *
	 * @return void
	 */
	void StopInLoop();

	/**
	 * @brief 在IO线程里执行队列里的函数对象
	 *
	 * @return void
	 */
	void DoPendingFunctors();

	/**
	 * @brief 获得队列里函数对象个数, 线程非安全，只能在IO线程里运行
	 *
	 * @return size_t
	 */
	size_t GetPendingQueueSize();

	/**
	 * @brief 是否队列为空，非线程安全，只能在IO线程里运行
	 *
	 * @return bool
	 */
	bool IsPendingQueueEmpty();

	/**
	* @brief libevent主要数据结构
	*/
	struct event_base* evbase_;

	/**
	* @brief evbase是否是自己创建
	*/
	bool create_evbase_myself_;

	/**
	* @brief 线程ID，线程没有运行前是主线程ID， 线程运行后是IO线程ID
	*/
	std::thread::id tid_;

	/**
	* @brief 线程锁, 用来保护pending_functors_
	*/
	std::mutex mutex_;

	/**
	* @brief 通知线程， 放了一个任务到线程队列里
	*/
	std::shared_ptr<NFPipeEventWatcher> watcher_;

	/**
	* @brief 当我们放一个任务到队列里， 需要通知线程去运行，但不想反复通知
	*/
	std::atomic<bool> notified_;

	/**
	* @brief 任务函数数组, 多个线程使用
	*/
	std::vector<Functor>* pending_functors_; // @Guarded By mutex_

	/**
	* @brief 任务数组大小, 给多个线程用
	*/
	std::atomic<int> pending_functor_count_;
};
