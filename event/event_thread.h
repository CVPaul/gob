#ifndef __GOB_EVENTTHREAD_H__
#define __GOB_EVENTTHREAD_H__
/*
想实现异步信号量的功能
boost的signal/signal2都是同步的信号槽，无法实现快速返回的功能（速度跟直接调用没有本质区别）
chromium::base的EventThread强依赖于chromium::base，跟剥离chromium::base的初衷相悖
EventThread reference to: 
https://www.smork.info/blog/2010/11/24/entry101124-164443.html
https://www.codeproject.com/Articles/1169105/Cplusplus-std-thread-Event-Loop-with-Message-Queue
https://www.cnblogs.com/fnlingnzb-learner/p/10402276.html // boost io service
*/
#include <queue>
#include <mutex>
#include <string>
#include <atomic>
#include <thread>
#include <functional>
#include <condition_variable> 

namespace gob {
	typedef std::function<void(void)> FunctionPointer;
	class EventThread {
		// 一个线程处理所有的事件
	public:
		EventThread(const std::string& thread_name);
		~EventThread();
		bool start();
		int32_t post(FunctionPointer func);
		void exit();
	public:
		std::string m_strThreadName;
	private:
		// 禁止拷贝
		EventThread(const EventThread&) = delete;
		EventThread& operator=(const EventThread&) = delete;
		void stop();
		void run(); // 外部独立调用会出现死锁的问题

	private:
		std::thread* m_pThread;
		std::queue<FunctionPointer> m_qEvent;
		std::condition_variable m_cv;
		std::mutex m_lockEvent;
		std::atomic_bool m_bStopped;
	};
}

#endif // __GOB_EVENTTHREAD_H__
