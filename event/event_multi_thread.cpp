#include "gob/log/logging.h"
#include "gob/event/event_multi_thread.h"

gob::EventMultiThread::EventMultiThread(bool bStrand,
    const std::string& thread_name,std::size_t pool_size)
    : m_bStrand(bStrand)
    , m_nPoolSize(pool_size)
    , m_strThreadName(thread_name)
    , m_pPool(nullptr)
    , m_pService(nullptr)
	, m_pStrand(nullptr)
    , m_pWorkGuard(nullptr){
}

gob::EventMultiThread::~EventMultiThread() {
	exit();
}

bool gob::EventMultiThread::start() {
    // pool
    if(m_pPool == nullptr)
        m_pPool = new boost::thread_group();
    if(m_pPool == nullptr){
        logger_error << "create pool(thread_group) failed!";
        return false;
    }
    // service
    if(m_pService == nullptr)
        m_pService = new boost::asio::io_context();
    if(m_pService == nullptr){
        logger_error << "create service(io_context) failed!";
        delete m_pPool;
        m_pPool = nullptr;
        return false;
    }
    // work_guard
    if(m_pWorkGuard == nullptr){
        m_pWorkGuard = new boost::asio::executor_work_guard<
            boost::asio::io_context::executor_type>(
            boost::asio::make_work_guard(*m_pService));
    }
    if(m_pWorkGuard == nullptr){
        logger_error << "create work(work_guard) failed!";
        delete m_pPool, m_pService;
        m_pPool = nullptr;
        m_pService = nullptr;
        return false;
    }
    // strand
    if(m_bStrand){ // 使用Strand
        if(m_pStrand == nullptr)
            m_pStrand = new boost::asio::io_context::strand(*m_pService);
        if(m_pStrand == nullptr){
            logger_error << "create strand failed!";
            delete m_pPool, m_pService, m_pWorkGuard;
            m_pPool = nullptr;
            m_pService = nullptr;
            m_pWorkGuard = nullptr;
            return false;
        }
    }
	// create threads
	for (std::size_t i = 0; i < m_nPoolSize; ++i) {
		// all the threads do is execute the io_context::run()
		m_pPool->create_thread([&](){m_pService->run();});
	}
	return true;
}

int32_t gob::EventMultiThread::post(FunctionPointer func, bool dispatch) {
	if (!m_pPool)
		return 1;
	if (!m_pService)
		return 2;
    if (!m_pWorkGuard)
        return 3;
    if (m_bStrand && !m_pStrand)
        return 4;
	if (dispatch)
		boost::asio::dispatch(*m_pService, func);
	else
		boost::asio::post(*m_pService,func);
	return 0;
}

void gob::EventMultiThread::exit() {
    if (m_pService){
        m_pService->stop();
    }
    if (m_pWorkGuard){
        m_pWorkGuard->reset();
    }
    if (m_pPool){
        m_pPool->join_all();
    }
    if (m_pService){
        delete m_pService;
        m_pService = nullptr;
    }
    if (m_pStrand){
        delete m_pStrand;
        m_pStrand = nullptr;
    }
    if (m_pWorkGuard){
        delete m_pWorkGuard;
        m_pWorkGuard = nullptr;
    }
    if (m_pPool){
        delete m_pPool;
        m_pPool = nullptr;
    }
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
namespace dt = boost::posix_time;
    dt::ptime t = dt::microsec_clock::local_time();	
    logger_info << m_strThreadName << " stopped at " << dt::to_iso_extended_string(t);
}