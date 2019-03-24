// -------------------------------------------------------------------------
//    @FileName         :    NFMpmcBlockingQueue.h
//    @Author           :    GaoYi
//    @Date             :    2019/03/24
//    @Email			:    445267987@qq.com
//    @Module           :    NFMpmcBlockingQueue
//
// -------------------------------------------------------------------------

#pragma once

// multi producer-multi consumer blocking queue
// Enqueue(..) - will block until room found to put the new message
// EnqueueNoWait(..) - will return immediatly with false if no room left in the queue
// DequeueFor(..) - will block until the queue is not empty or timeout passed

#include <condition_variable>
#include <mutex>
#include <queue>

template<typename T>
class NFMpmcBlockingQueue
{
public:
	using item_type = T;
	explicit NFMpmcBlockingQueue(size_t max_items)
		: max_items_(max_items)
	{
	}

	// try to enqueue and block if no room left
	void Enqueue(T &&item)
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			pop_cv_.wait(lock, [this] { return this->q_.size() < this->max_items_; });
			q_.push(std::move(item));
		}
		push_cv_.notify_one();
	}

	// try to enqueue and return immdeialty false if no room left
	bool EnqueueNoWait(T &&item)
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			if (q_.size() == this->max_items_)
			{
				return false;
			}
			q_.push(std::forward<T>(item));
		}
		push_cv_.notify_one();
		return true;
	}

	// try to dequeue item. if no item found. wait upto timeout and try again
	// Return true, if succeeded dequeue item, false otherwise
	bool DequeueFor(T &popped_item, std::chrono::milliseconds wait_duration)
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			if (!push_cv_.wait_for(lock, wait_duration, [this] { return this->q_.size() > 0; }))
			{
				return false;
			}

			popped_item = std::move(q_.front());
			q_.pop();
		}
		pop_cv_.notify_one();
		return true;
	}

private:
	size_t max_items_;
	std::mutex queue_mutex_;
	std::condition_variable push_cv_;
	std::condition_variable pop_cv_;

	std::queue<T> q_;
};
