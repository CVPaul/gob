#ifndef __GOB_MULTI_THREAD_LOOP_H__
#define __GOB_MULTI_THREAD_LOOP_H__

#include <queue>
#include <mutex>
#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable> 

#include "common/constant.h"

namespace gob {
	typedef std::function<void(void)> FunctionPointer;
	class MultiThreadLoop {
		// 一个线程处理所有的事件
	public:
		MultiThreadLoop(const std::string& thread_name,
            std::size_t pool_size = MT_CONCURRENCY_CNT);
		~MultiThreadLoop();
		bool start();
		int32_t post(FunctionPointer func);
		void exit();
	public:
        int             m_nPoolSize;
		std::string     m_strThreadName;
	private:
		// 禁止拷贝
		MultiThreadLoop(const MultiThreadLoop&) = delete;
		MultiThreadLoop& operator=(const MultiThreadLoop&) = delete;
		void stop();
		void run(); // 外部独立调用会出现死锁的问题

	private:
		std::vector<std::thread*>       m_pool;
		std::queue<FunctionPointer>     m_qEvent;
		std::condition_variable         m_cv;
		std::mutex                      m_lockEvent;
		std::atomic_bool                m_bStopped;
	};
}

#endif // __GOB_MULTI_THREAD_LOOP_H__