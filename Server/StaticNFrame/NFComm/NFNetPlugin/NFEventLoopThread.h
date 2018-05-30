// -------------------------------------------------------------------------
//    @FileName         :    NFEventLoopThread.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFLoopThreadStatus.h"

#include <thread>
#include <mutex>

class NFEventLoop;

class NFEventLoopThread : public NFLoopTheadStatus
{
public:
	enum
	{
		kOK = 0
	};

	// Return 0 means OK, anything else means failed.
	typedef std::function<int()> Functor;

	/**
	 * @brief
	 *
	 * @return 
	 */
	NFEventLoopThread();

	/**
	 * @brief
	 *
	 * @return 
	 */
	~NFEventLoopThread();

	/**
	 * @brief 开启线程
	 *
	 * @param  wait_thread_started =true, 主线程将被柱塞，知道线程开始运行
	 * @param  pre		线程一开始将立马被执行
	 * @param  post     线程将停止前，会被调用
	 * @return bool
	 */
	bool Start(bool wait_thread_started = true,
	           Functor pre = Functor(),
	           Functor post = Functor());

	/**
	 * @brief 停止线程的运行, 只能主线程运行
	 *
	 * @param  wait_thread_exit = true 将等待线程退出后，才继续运行
	 * @return void
	 */
	void Stop(bool wait_thread_exit = false);

	/**
	 * @brief 等待线程退出，只能在主线程里调用
	 *
	 * @return void
	 */
	void Join();

	/**
	 * @brief 设置线程名字
	 */
	void SetName(const std::string& n);

	/**
	* @brief 线程名字
	*/
	const std::string& GetName() const;

	NFEventLoop* Loop() const;

	struct event_base* event_base();

	std::thread::id GetTid() const;

	/**
	 * @brief 线程是否在运行
	 *
	 * @return bool
	 */
	bool IsRunning() const;
private:
	/**
	 * @brief 线程要运行的函数
	 */
	void Run(const Functor& pre, const Functor& post);

	/**
	* @brief 线程循环结构体
	*/
	std::shared_ptr<NFEventLoop> event_loop_;

	/**
	* @brief 线程锁
	*/
	std::mutex mutex_;

	/**
	* @brief 线程对象
	*/
	std::shared_ptr<std::thread> thread_; // Guard by mutex_

	/**
	 * @brief 名字
	 */
	std::string name_;
};

