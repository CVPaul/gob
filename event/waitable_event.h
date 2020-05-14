#ifndef __GOB_WAITABLEEVENT_H__
#define __GOB_WAITABLEEVENT_H__

#include <mutex>
#include <atomic>
#include <condition_variable>

namespace gob {
    class WaitableEvent{
    public:
        // if manual_reset=true 需要主动调用reset(), else if manual_reset=false 会在wait结束后自动reset
        WaitableEvent(bool manual_reset);
        ~WaitableEvent();
        void signal();
        void wait();
        void reset();
        bool timed_wait(const std::chrono::milliseconds& timedelta);
        bool is_signaled();
    private:
        bool                        m_bManualReset;
        std::atomic_bool            m_bSignaled;
        std::mutex                  m_lock;
        std::condition_variable     m_cv;
    };
}

#endif // __GOB_WAITABLEEVENT_H__
