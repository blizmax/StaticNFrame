// -------------------------------------------------------------------------
//    @FileName         :    NFAsynMsgHelper.h
//    @Author           :    GaoYi
//    @Date             :    2019/03/24
//    @Email			:    445267987@qq.com
//    @Module           :    NFAsynMsgHelper
//
// -------------------------------------------------------------------------

#pragma once

#include "NFMpmcBlockingQueue.h"

#include <chrono>
#include <condition_variable>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <type_traits>

//
// Async overflow policy - block by default.
//
enum class async_overflow_policy
{
	block_retry,	// Block / yield / sleep until message can be enqueued
	discard_msg,	// Discard the message it enqueue fails
};

class NFAsynMsgHelper
{
	// Async msg to move to/from the queue
	// Movable only. should never be copied
	enum class async_msg_type
	{
		log,
		flush,
		terminate,
	};

	struct async_msg
	{
		size_t thread_id;
		std::string txt;
		async_msg_type msg_type;
		size_t msg_id;

		async_msg() = default;
		~async_msg() = default;

		explicit async_msg(async_msg_type aType)
			: thread_id(0),
			msg_type(aType),
			msg_id(0)
		{

		}

		async_msg(async_msg &&other) = default;
		async_msg &operator=(async_msg &&other) = default;

		async_msg(const async_msg &) = delete;
		async_msg &operator=(const async_msg &other) = delete;
	};

public:
	NFAsynMsgHelper()
	{

	}

	virtual ~NFAsynMsgHelper()
	{

	}

	void enqueue_msg(async_msg&& new_msg, async_overflow_policy policy)
	{
		if (policy == async_overflow_policy::block_retry)
		{
			_queue.Enqueue(std::move(new_msg));
		}
		else
		{
			_queue.EnqueueNoWait(std::move(new_msg));
		}
	}
private:
	std::thread _worker_thread;
	const std::function<void()> _worker_warnup_cb;
	const std::function<void()> _worker_teamdown_cb;
	NFMpmcBlockingQueue<async_msg> _queue;
};
