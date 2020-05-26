#ifndef __GOB_EVENT_LOOP_H__
#define __GOB_EVENT_LOOP_H__

#include "gob/event/multi_thread_loop.h"
#include "gob/event/event_multi_thread.h"
#include "gob/event/event_single_thread.h"

namespace gob {
    #ifdef USING_CPP_MT_LOOP
        typedef EventMultiThread EventLoop;
    #elif defined USING_BOOST_ST_LOOP
        typedef EventSingleThread EventLoop;
    #elif defined USING_CPP_MT_LOOP
        typedef MultiThreadLoop EventLoop;
    #else // default
        typedef MultiThreadLoop EventLoop;
    #endif
}

#endif // __GOB_EVENT_LOOP_H__