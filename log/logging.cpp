
#include "gob/log/logging.h"

// variables

std::string logger::m_strLogPath = "";
std::string logger::m_strLogName = "";
std::atomic_bool logger::m_bInit = false;

#ifdef USING_GLOG
std::string logger::rotation_date = dt::to_iso_extended_string(
	dt::second_clock::local_time() + dt::hours(ROTATION_OFFSET_HOURS)).substr(0, 10);
#else
logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level> logger::lg = 
	logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>(logging::keywords::channel = "net");
logging::attributes::mutable_constant<std::string> logger::rotation_date = 
	logging::attributes::mutable_constant<std::string>(dt::to_iso_extended_string(
		dt::second_clock::local_time() + dt::hours(ROTATION_OFFSET_HOURS)).substr(0, 10));
boost::shared_ptr<sink_t> logger::sink = nullptr;
#endif // USING_GLOG

bool logger::init(std::string strLogPath, std::string strLogName) {
	if (m_bInit) return true;
	m_strLogPath = strLogPath;
	m_strLogName = strLogName;
	// 初始化日志
#ifdef USING_GLOG
	FLAGS_log_prefix = false; // 原有的格式完全采用自己的格式
	FLAGS_timestamp_in_logfile_name = false; // 文件名里使用自己定义时间戳（废弃默认的）
	google::InitGoogleLogging(strLogName.c_str());
	// FLAGS_log_dir = "./logs";
	google::InstallFailureSignalHandler(); // 安装信号处理程序，当程序出现崩溃时，会输出崩溃的位置等相关信息。
	update_rotation_time();	
#else
	logging::add_common_attributes();
	boost::shared_ptr<logging::core > core = logging::core::get();
	core->add_global_attribute("RotationDate", rotation_date);
	boost::shared_ptr<logging::sinks::text_multifile_backend> backend =
		boost::make_shared<logging::sinks::text_multifile_backend>();
	// Set up the file naming pattern
	backend->set_file_name_composer(logging::sinks::file::as_file_name_composer(
		logexpr::stream << strLogPath << "/"
		<< logexpr::attr<logging::trivial::severity_level>("Severity")
		<< "/" << strLogName << "."
		// 文件名还是带上Level，否者copy到一起的时候分不清
		<< logexpr::attr<logging::trivial::severity_level>("Severity") << "."
		<< logexpr::attr<std::string>("RotationDate") << ".log"));
	
	sink = boost::shared_ptr<sink_t>(new sink_t(backend));
	// Set the formatter
	sink->set_formatter(logexpr::stream
		<< "datetime=" << logexpr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S.%f")
		<< logexpr::smessage);
	core->add_sink(sink);
#endif // USING_GLOG
	m_bInit = true;
	return m_bInit;
}

void logger::update_rotation_time() {
	
	dt::ptime t = dt::second_clock::local_time();
	t += dt::hours(ROTATION_OFFSET_HOURS); // 以20点为界切割一个交易日，选择20的原因是这样使得所有的数据可以用一天表示，原油也可以
	std::string cur_time = dt::to_iso_string(t).substr(0, 14);
#ifdef USING_GLOG
	if (cur_time == rotation_date)
		return;
	rotation_date = cur_time;
	std::string folder = m_strLogPath + "/info/";
	if(!fs::exists(folder)) 
		fs::create_directory(folder);
	std::string filename = folder + m_strLogName + ".info." + rotation_date + ".log";
	google::SetLogDestination(google::INFO, filename.c_str());
	folder = m_strLogPath + "/warning/";
	if(!fs::exists(folder)) 
		fs::create_directory(folder);
	filename = folder + m_strLogName + ".warning." + rotation_date + ".log";
	google::SetLogDestination(google::WARNING, filename.c_str());
	folder = m_strLogPath + "/error/";
	if(!fs::exists(folder)) 
		fs::create_directory(folder);
	filename = folder + m_strLogName + ".error." + rotation_date + ".log";
	google::SetLogDestination(google::ERROR, filename.c_str());
	folder = m_strLogPath + "/fatal/";
	if(!fs::exists(folder)) 
		fs::create_directory(folder);
	filename = folder + m_strLogName + ".fatal." + rotation_date + ".log";
	google::SetLogDestination(google::FATAL, filename.c_str());
#else
	if (cur_time == rotation_date.get()) 
		return;
	rotation_date.set(cur_time);
#endif // USING_GLOG
}

void logger::stop() {
#ifdef USING_GLOG
	logger_fatal << "logger stopped!";
#else
	if (sink == nullptr)
		return;
	logger_info << "logger stopped!";
	boost::shared_ptr<logging::core > core = logging::core::get();
	core->remove_sink(sink);
#ifdef USING_ASYNC_LOG_MODE
	sink->stop();
#endif // USING_ASYNC_LOG_MODE
	sink->flush();
	sink.reset();
#endif // USING_GLOG
	m_bInit = false;
}
