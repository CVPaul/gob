#include "gob/log/logging.h"
#include "gob/event/event_thread.h"

gob::EventThread::EventThread(const std::string& thread_name) {
	m_strThreadName = thread_name;
	m_bStopped = false;
	m_pThread = nullptr;
}

gob::EventThread::~EventThread() {
	exit();
}

bool gob::EventThread::start() {
	if (!m_pThread)
		m_pThread = new std::thread(&EventThread::run, this);
	return true;
}

void gob::EventThread::run() {
	logger_info << "EventThread: " << m_strThreadName << " started!";
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
}

int32_t gob::EventThread::post(FunctionPointer func) {
	if (!m_pThread)
		return 1;
	if (m_bStopped) // 停止时不允许插入事件
		return 2;
	std::lock_guard<std::mutex> al(m_lockEvent);
	m_qEvent.push(func);
	m_cv.notify_one();
	return 0;
}

void gob::EventThread::stop() {
	m_bStopped = true; 
}

void gob::EventThread::exit() {
	if (!m_pThread)
		return;
	// 调用私有的stop函数去停止循环
	post(std::bind(&EventThread::stop, this));
	m_pThread->join();
	delete m_pThread;
	m_pThread = nullptr;
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
namespace dt = boost::posix_time;
    dt::ptime t = dt::microsec_clock::local_time();	
    logger_info << m_strThreadName << " stopped at " << dt::to_iso_extended_string(t);
}