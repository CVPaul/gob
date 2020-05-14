#include "gob/log/logging.h"
#include "gob/event/multi_thread_loop.h"

gob::MultiThreadLoop::MultiThreadLoop(
    const std::string& thread_name, std::size_t pool_size) 
    : m_nPoolSize(pool_size)
    , m_strThreadName(thread_name)
    , m_bStopped(false){
    m_pool = std::vector<std::thread*>(m_nPoolSize);
}

gob::MultiThreadLoop::~MultiThreadLoop() {
	exit();
}

bool gob::MultiThreadLoop::start() {
    for(int i = 0; i < m_nPoolSize; i++){
	    if(!m_pool[i])
            m_pool[i] = new std::thread(&MultiThreadLoop::run, this);
    }
	return true;
}

void gob::MultiThreadLoop::run() {
	while (!m_bStopped) {
		FunctionPointer func;
		{// Wait for a message to be added to the queue
			std::unique_lock<std::mutex> al(m_lockEvent);
			while (m_qEvent.empty())
				m_cv.wait(al);
			if (m_qEvent.empty())
				continue;
			func = m_qEvent.front();
			m_qEvent.pop();
		}
		func(); // 处理事件
	}
	logger_error << "MultiThreadLoop(tid:" << std::this_thread::get_id()
        << "): " << m_strThreadName << " stopped!";
}

int32_t gob::MultiThreadLoop::post(FunctionPointer func) {
	if (m_pool.size() == 0)
		return 1;
	if (m_bStopped) // 停止时不允许插入事件
		return 2;
	std::lock_guard<std::mutex> al(m_lockEvent);
	m_qEvent.push(func);
	m_cv.notify_one();
	return 0;
}

void gob::MultiThreadLoop::stop() {
	m_bStopped = true; 
}

void gob::MultiThreadLoop::exit() {
	if (m_pool.size() == 0)
		return;
	// 调用私有的stop函数去停止循环
	post(std::bind(&MultiThreadLoop::stop, this));
    for(int i = 0; i < m_pool.size(); i++){
        m_pool[i]->join();
        delete m_pool[i];
        m_pool[i] = nullptr;
    }
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
namespace dt = boost::posix_time;
    dt::ptime t = dt::microsec_clock::local_time();	
    logger_info << m_strThreadName << " stopped at " << dt::to_iso_extended_string(t);
}