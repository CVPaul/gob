#ifndef __GOB_EVENT_MULTI_THREAD_H__
#define __GOB_EVENT_MULTI_THREAD_H__
/*
* EventThread已经实现了event_loop的功能，但是是单线程的，不能很好的满足要求，因此EventMultiThread应运而生
*    - 注意即便是单线程的，也有它适用的地方，跟多线程设置线程数为1还是不一样
* 目前（2020-05）已经有一个开源的通用的基于boost实现的版本：http://threadpool.sourceforge.net/
* 这里想实现的是精简的，基于boost::asio的版本，对比一下性能，
* 更确切的说的是boost::asio的io_service，因为本身boost::asio已经有一个threadpool了：
*   - https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/thread_pool.html
*/
#include <vector>
#include <functional>
#include "common/constant.h"
#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>

namespace gob {
	typedef std::function<void(void)> FunctionPointer;
	class EventMultiThread {
		// 一个线程处理所有的事件
	public:
		EventMultiThread(
            // 参考链接：https://zhuanlan.zhihu.com/p/87390640
            // 这里实现的是：全局只分配一个io_service，并且让这个io_service在多个线程之间共享，每个线程都调用全局的io_service的run()方法。
            // 所以这里设置为核数的2倍
            bool bStrand, // 是否使用strand来解决竞态条件(race condition)
            const std::string& thread_name="",
            std::size_t pool_size = MT_CONCURRENCY_CNT);
		~EventMultiThread();
		bool start();
		int32_t post(FunctionPointer func, bool dispatch = false);
		void exit();
	public:
        std::size_t m_nPoolSize;
		std::string m_strThreadName;
	private:
		// 禁止拷贝
		EventMultiThread(const EventMultiThread&) = delete;
		EventMultiThread& operator=(const EventMultiThread&) = delete;
		void stop();
		void run(); // 外部独立调用会出现死锁的问题

	private:
        bool                                            m_bStrand;
        boost::thread_group*                            m_pPool;
        boost::asio::io_context*                        m_pService;
        // 为何要使用strand(增强版的queue): https://www.jianshu.com/p/70286c2ab544
        boost::asio::io_context::strand*                m_pStrand;
        boost::asio::executor_work_guard<
            boost::asio::io_context::executor_type>*    m_pWorkGuard;
	};
}

#endif // __GOB_EVENT_MULTI_THREAD_H__