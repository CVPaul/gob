/*
* # gob 项目的单元测试程序
* ## 日志模块
*   - path: log/
*   - functions: 打印不同级别（debug, info, warning, error , fatal)的日志
* ## EventThread模块
*   - path: event/
*   - functions: 顺序处理事件（单线程）
*/
/* windows 下出现下面错误，原因是找不大Windows的版本，解决方案是#include <sdkddkver.h>
Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately. For example:
- add -D_WIN32_WINNT=0x0601 to the compiler command line; or
- add _WIN32_WINNT=0x0601 to your project's Preprocessor Definitions.
*/
// #include <sdkddkver.h>
#include <thread>
#include <iostream>
#include "log/logging.h"
#include "event/event_loop.h"
#include "event/waitable_event.h"

#include <boost/random.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

// #include "../thirdparty/Remotery/lib/Remotery.h"
// #include "thirdparty/Remotery/Rmotery.h"

int main(){
    if (!logger::init("logs","roll")) { // 初始化，设置日志保存路径为logs, log文件的名字为test
        std::cout << "[ERROR] logger init failed!" << std::endl;
        return 1;
    }
    else
	    std::cout << "[INFO] logger init succeed!" << std::endl;
    while(1){
        logger_info << "Test log file ROLLING...";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        logger_warning << "Test log file ROLLING...";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        logger_error << "Test log file ROLLING...";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        logger::update_rotation_time();
    }
    logger_fatal << "Test system fatal crash situation...";
}

// namespace dt = boost::posix_time;

// void test(int a, int b){
//     std::cout << "[INFO] a+b(" << a + b
//         << ") < a*b("  << a * b
//         << ") is:" << (a+b < a*b ? "True":"False") << std::endl;
// }

// class CumSum{
//     private:
//         std::atomic_llong sum;
//     public:
//         CumSum():sum(0){}
//         void add(int a, int s){
// 			std::this_thread::sleep_for(
// 				std::chrono::milliseconds(s));
// 			sum += a;
// 		}
//         long long get_sum(){return sum;}
// };

// int main(int argc, char *argv[]){
//     LOG(INFO) << "glog-TEST";
// 	srand(9527);
//     /*
//     Remotery* rmt;
//     rmtError error = rmt_CreateGlobalInstance(&rmt);
//     if( RMT_ERROR_NONE != error) {
// 		printf("Error launching Remotery %d\n", error);
//         return -1;
//     }
// 	rmt_ScopedCPUSample(LogText, 0);
//     */
//     // 初始化log ==============================================================================
//     if (!logger::init("logs","gob")) { // 初始化，设置日志保存路径为logs, log文件的名字为test
//         std::cout << "[ERROR] logger init failed!" << std::endl;
//         return 1;
//     }
//     else
// 	    std::cout << "[INFO] logger init succeed!" << std::endl;
//     // log 测试 ==============================================================================
//     logger_debug << "log test";
//     logger_info << "log test";
//     logger_warning << "log test";
//     logger_error << "log test";
// #ifndef USING_GLOG
//     logger_fatal << "log test";
// #endif // USING)GLOG
//     // waitable event test  ==============================================================================
//     gob::WaitableEvent* pEvent = new gob::WaitableEvent(false); // is_mannual= false, 即为自动重置无需手动调用reset
//     if (pEvent == nullptr){
//         std::cout << "[INFO] create Waitable Event failed!" << std::endl;
//     }
//     std::thread t1([pEvent](){
//         std::cout << "[INFO] I am actually in" << std::endl;
//         pEvent->wait();
//         std::cout << "[INFO] but I need to wait until signaled!" << std::endl;
//     });
//     std::thread t2([pEvent](){
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//         std::cout << "[INFO] print before previous thread finished." << std::endl;
//         pEvent->signal();
//     });
//     t1.join();
//     t2.join();
//     delete pEvent;
//     // thread event test ==============================================================================
//     gob::EventLoop g_event_loop("unit_test_loop");
//     bool stat = g_event_loop.start(); // 启动
//     if (stat == false){
//         std::cout << "[ERROR] statr event loop failed!" << std::endl;
//         return 2;
//     }
//     g_event_loop.post(std::bind(test,1,2));
//     g_event_loop.post(std::bind(test,2,3));
//     // multi thread event ==============================================================================
//     gob::EventSingleThread* g_st_loop = new gob::EventSingleThread("single-thread");
//     gob::MultiThreadLoop*   g_mt_loop = new gob::MultiThreadLoop("muti-thread");
//     gob::EventMultiThread*  g_mt_loop_strand = new gob::EventMultiThread(true,"with-strand");
//     gob::EventMultiThread*  g_mt_loop_normal = new gob::EventMultiThread(false,"no-strand");
//     gob::EventMultiThread*  g_mt_strand_dispatch = new gob::EventMultiThread(true,"with-strand-dispatch");
//     gob::EventMultiThread*  g_mt_normal_dispatch = new gob::EventMultiThread(false,"no-strand-dispatch");
//     CumSum* sum_mt = new CumSum;
//     CumSum* sum_st = new CumSum;
//     CumSum* sum_mt_s = new CumSum;
//     CumSum* sum_mt_n = new CumSum;
//     CumSum* sum_mt_sd = new CumSum;
//     CumSum* sum_mt_nd = new CumSum;
// 	dt::ptime t = dt::microsec_clock::local_time();
// 	logger_info << "mt_size:" << g_mt_loop->m_nPoolSize
//         << " mts_size:" << g_mt_loop_strand->m_nPoolSize
//         << " mtn_size:" << g_mt_loop_normal->m_nPoolSize
//         << " test started at " << dt::to_iso_extended_string(t);
//     g_st_loop->start();
//     g_mt_loop->start();
//     g_mt_loop_normal->start();
//     g_mt_loop_strand->start();
//     g_mt_normal_dispatch->start();
//     g_mt_strand_dispatch->start();
// 	std::vector<std::thread*> threads(10);
//     for (int i = 0 ;i < 10; i++){
//         threads[i] = new std::thread([&](){
//             for (int j = 0 ; j < 1e1; j++){
// 				int s = rand()%7+2;
//                 g_st_loop->post(boost::bind(&CumSum::add,sum_st,j,s));
//                 g_mt_loop->post(boost::bind(&CumSum::add,sum_mt,j,s));
//                 g_mt_loop_strand->post(boost::bind(&CumSum::add,sum_mt_s,j,s));
//                 g_mt_loop_normal->post(boost::bind(&CumSum::add,sum_mt_n,j,s));
//                 g_mt_strand_dispatch->post(boost::bind(&CumSum::add,sum_mt_sd,j,s),true);
//                 g_mt_normal_dispatch->post(boost::bind(&CumSum::add,sum_mt_nd,j,s),true);
//             }
//         });
//     }
// 	for (int i = 0; i < 10; i++) {
// 		threads[i]->join();
// 		delete threads[i];
// 	}
//     std::thread g1([g_st_loop](){
//         g_st_loop->exit();
//     });
//     std::thread g2([g_mt_loop_normal](){
//         g_mt_loop_normal->exit();
//     });
//     std::thread g3([g_mt_loop_strand](){
//         g_mt_loop_strand->exit();
//     });
//     std::thread g5([&](){
//         g_mt_normal_dispatch->exit();
//     });
//     std::thread g6([&](){
//         g_mt_strand_dispatch->exit();
//     });
// 	std::thread g4([g_mt_loop]() {
// 		g_mt_loop->exit();
// 	});
//     g1.join();
//     g2.join();
//     g3.join();
// 	g4.join();
// 	g5.join();
// 	g6.join();
// 	delete g_st_loop, g_mt_loop_strand, g_mt_loop_normal,
// 		g_mt_loop, g_mt_normal_dispatch, g_mt_strand_dispatch;
// 	char buffer[1024];
//     sprintf(buffer,"sum_st:%lld, sum_mt_s:%lld, sum_mt_n:%lld, sum_mt:%lld",
// 		sum_st->get_sum(),sum_mt_s->get_sum(),sum_mt_n->get_sum(), sum_mt->get_sum());
// 	logger_info << "cumsum result:" << buffer;
// 	delete sum_st, sum_mt_s, sum_mt_n, sum_mt, sum_mt_sd, sum_mt_nd;
//     // 后处理  ==============================================================================
//     g_event_loop.exit();
//     logger::stop();
//     logger_info << "server stoped!";
//     // rmt_DestroyGlobalInstance(rmt);
//     return 0;
// }