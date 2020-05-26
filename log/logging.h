#ifndef __GOB_LOGGING_H__
#define __GOB_LOGGING_H__

// #define USING_ASYNC_LOG_MODE // using sync log sink for debug reason

#include <string>
#include <atomic>

#define ROTATION_OFFSET_HOURS 4
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace fs = boost::filesystem;
namespace dt = boost::posix_time;

#ifdef USING_GLOG
#include "gob/include/glog/logging.h"
#else
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>


namespace logging = boost::log;
namespace logexpr = boost::log::expressions;

// #define USING_ASYNC_LOG_MODE

#ifdef USING_ASYNC_LOG_MODE
	typedef logging::sinks::asynchronous_sink<logging::sinks::text_multifile_backend,
		logging::sinks::unbounded_fifo_queue> sink_t;
#else
	typedef logging::sinks::synchronous_sink<logging::sinks::text_multifile_backend> sink_t;
#endif // USING_ASYNC_LOG_MODE
#endif // USING_GLOG

class logger {
private:
	logger() {};
public:
#ifdef USING_GLOG
	static std::string rotation_date;
#else
	static logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level> lg;
	static boost::shared_ptr<sink_t> sink;
	static logging::attributes::mutable_constant<std::string> rotation_date;
#endif // USING_GLOG
public:
	// functions
	static bool init(std::string strLogPath, std::string strLogName);
	static void stop();
	static void update_rotation_time();
private:
	static std::atomic_bool m_bInit;
	static std::string m_strLogPath;
	static std::string m_strLogName;
};

#define LOG_EXTRA_INFO "||scope=" << __FUNCTION__ << "@" << __FILE__ << ":"  << __LINE__ << "||"

#ifdef USING_GLOG
#define datetimeinfo	dt::to_iso_extended_string(dt::microsec_clock::local_time())
#define logger_trace	LOG(INFO) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
#define logger_debug	DLOG(INFO) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
#define logger_info		LOG(INFO) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
#define logger_warning	LOG(WARNING) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
#define logger_error	LOG(ERROR) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
// 注意在调用完Fatal error之后程序会在打印完成后退出
#define logger_fatal	LOG(FATAL) << "datetime=" << datetimeinfo << LOG_EXTRA_INFO
#else
#define logger_trace	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::trace) << LOG_EXTRA_INFO
#define logger_debug	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::debug) << LOG_EXTRA_INFO
#define logger_info		BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::info) << LOG_EXTRA_INFO
#define logger_warning	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::warning) << LOG_EXTRA_INFO
#define logger_error	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::error) << LOG_EXTRA_INFO
#define logger_fatal	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::fatal) << LOG_EXTRA_INFO
#endif // USING_GLOG
#endif // __GOB_LOGGING_H__