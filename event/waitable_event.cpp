#include "gob/event/waitable_event.h"

gob::WaitableEvent::WaitableEvent(bool manual_reset)
: m_bSignaled(false), m_bManualReset(manual_reset){}

gob::WaitableEvent::~WaitableEvent(){
}

void gob::WaitableEvent::signal(){
    if(m_bSignaled)
        return;
    m_bSignaled = true;
    m_cv.notify_all();
}

void gob::WaitableEvent::wait(){
    std::unique_lock<std::mutex> al(m_lock);
    while(!m_bSignaled){
        m_cv.wait(al);
    }
    if(!m_bManualReset) reset();
}

void gob::WaitableEvent::reset(){
    m_bSignaled = false;
}

bool gob::WaitableEvent::timed_wait(
    const std::chrono::milliseconds& timedelta){
    std::unique_lock<std::mutex> al(m_lock);
    bool signaled_snapshot = m_cv.wait_for(
        al,timedelta,[this](){return this->is_signaled();});
    if (!m_bManualReset) reset();
    return signaled_snapshot;
}

bool gob::WaitableEvent::is_signaled(){
    return m_bSignaled;
}
