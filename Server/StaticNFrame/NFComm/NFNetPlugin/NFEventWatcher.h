// -------------------------------------------------------------------------
//    @FileName         :    NFEventWatcher.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once

#include "NFComm/NFCore/NFPlatform.h"
#include "NFDuration.h"
#include <functional>
#include <event2/util.h>

struct event;
struct event_base;

class NFEventLoop;

/**
* @brief 多线程事件通知对象基类
*/
class NFEventWatcher
{
public:
	typedef std::function<void()> Handler;

	/**
	 * @brief 析构函数
	 */
	virtual ~NFEventWatcher();

	/**
	 * @brief 初始化，主要的初始化在继承类DoInit中运行，
	 * 这个函数最终把事件set到event_base里, 让IO循环检查他的事件
	 *
	 * @return bool
	 */
	bool Init();

	/**
	 * @brief 释放event， 如果有cancel回调函数，调用回调, 必须在event线程里运行
	 *
	 * @return void
	 */
	void Cancel();

	/**
	 * @brief 设置event cancel回调函数
	 *
	 * @param  cb
	 * @return void
	 */
	void SetCancelCallback(const Handler& cb);

	/**
	 * @brief 清理掉事件回调函数对象
	 *
	 * @return void
	 */
	void ClearHandler()
	{
		handler_ = Handler();
	}

protected:
	/**
	 * @brief 设置最大超时等待， 0 永久等待， 直到有事件发送， 必须在event线程里调用
	 *
	 * @param  timeout
	 * @return bool
	 */
	bool Watch(NFDuration timeout);

	/**
	 * @brief 构造函数, 存入event_base, 事件发生时要回调的函数
	 *
	 * @param  evbase
	 * @param  handler
	 * @return
	 */
	NFEventWatcher(struct event_base* evbase, const Handler& handler);
	NFEventWatcher(struct event_base* evbase, Handler&& handler);

	/**
	 * @brief 主要调用DoClose, 由继承类实现
	 *
	 * @return void
	 */
	void Close();

	/**
	 * @brief 释放事件
	 *
	 * @return void
	 */
	void FreeEvent();

	/**
	 * @brief 主要的初始化，在继承类里实现
	 *
	 * @return bool
	 */
	virtual bool DoInit() = 0;

	/**
	* @brief 主要的关闭函数，在继承类里实现
	*
	* @return bool
	*/
	virtual void DoClose()
	{
	}

	/**
	* @brief 事件结构体，自己控制
	*/
	struct event* event_;

	/**
	* @brief 外来的event_base结构
	*/
	struct event_base* evbase_;

	/**
	* @brief 事件结构体有没有add
	*/
	bool attached_;

	/**
	* @brief 主事件回调
	*/
	Handler handler_;

	/**
	* @brief cancel回调
	*/
	Handler cancel_callback_;
};

/**
* @brief 管道通信类
*/
class NFPipeEventWatcher : public NFEventWatcher
{
public:
	/**
	* @brief 构造函数, 存入event_base, 事件发生时要回调的函数
	*
	* @param  evbase
	* @param  handler
	* @return
	*/
	NFPipeEventWatcher(NFEventLoop* loop, const Handler& handler);
	NFPipeEventWatcher(NFEventLoop* loop, Handler&& handler);

	/**
	 * @brief 析构函数
	 *
	 * @return
	 */
	~NFPipeEventWatcher();

	/**
	 * @brief 主要作用是把事件加到事件列表中，函数名写个异步等待是几个意思
	 *
	 * @return bool
	 */
	bool AsyncWait();

	/**
	 * @brief 通过往写管道写数据，来发通知
	 *
	 * @return void
	 */
	void Notify();

	/**
	 * @brief 返回写通道
	 *
	 * @return evutil_socket_t
	 */
	evutil_socket_t wfd() const
	{
		return pipe_[0];
	}

private:
	/**
	 * @brief 初始化管道, 意思大概是
	 * 当notify往写通道写数据时，读通道触发事件，调用HandlerFn,
	 *  HandlerFn从读通道读取数据后，调用事件回调
	 *
	 * @return bool
	 */
	virtual bool DoInit();

	/**
	 * @brief 关闭管道
	 *
	 * @return void
	 */
	virtual void DoClose();

	/**
	 * @brief 读通道触发事件回调
	 *
	 * @param  fd
	 * @param  which
	 * @param  v
	 * @return void
	 */
	static void HandlerFn(evutil_socket_t fd, short which, void* v);

	/**
	* @brief 管道， 0是写通道，1是读通道
	*/
	evutil_socket_t pipe_[2]; // Write to pipe_[0] , Read from pipe_[1]
};

/**
* @brief 定时器超时
*/
class NFTimerEventWatcher : public NFEventWatcher
{
public:
	/**
	 * @brief 构造函数, 保存event_base, 处理函数，以及时间
	 *
	 * @param  loop
	 * @param  handler
	 * @param  timeout
	 * @return
	 */
	NFTimerEventWatcher(NFEventLoop* loop, const Handler& handler, NFDuration timeout);
	NFTimerEventWatcher(NFEventLoop* loop, Handler&& handler, NFDuration timeout);
	NFTimerEventWatcher(struct event_base* loop, const Handler& handler, NFDuration timeout);
	NFTimerEventWatcher(struct event_base* loop, Handler&& handler, NFDuration timeout);

	/**
	 * @brief 超时等待，超过规定时间就调用HandlerFn函数
	 *
	 * @return bool
	 */
	bool AsyncWait();

private:
	/**
	 * @brief 初始化，发生时间就调用HandlerFn
	 *
	 * @return bool
	 */
	virtual bool DoInit();

	/**
	 * @brief 回调事件函数
	 *
	 * @param  fd
	 * @param  which
	 * @param  v
	 * @return void
	 */
	static void HandlerFn(evutil_socket_t fd, short which, void* v);
private:
	NFDuration timeout_;
};

/**
* @brief 信号事件处理, 收到信号就处理事件回调handler
*/
class NFSignalEventWatcher : public NFEventWatcher
{
public:
	NFSignalEventWatcher(evutil_socket_t signo, NFEventLoop* loop, const Handler& handler);
	NFSignalEventWatcher(evutil_socket_t signo, NFEventLoop* loop, Handler&& handler);

	/**
	 * @brief 一直等待，直到事件被触发
	 *
	 * @return bool
	 */
	bool AsyncWait();
private:
	/**
	 * @brief 初始化，收到信号就调用HandlerFn
	 *
	 * @return bool
	 */
	virtual bool DoInit();

	/**
	 * @brief 调用handler
	 *
	 * @param  sn
	 * @param  which
	 * @param  v
	 * @return void
	 */
	static void HandlerFn(evutil_socket_t sn, short which, void* v);

	/**
	* @brief 信号
	*/
	int signo_;
};
