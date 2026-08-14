#pragma once

#include <condition_variable>

#if defined(AMULET_THREAD_SAFETY_ANALYSIS)

#include <chrono>

#include "mutex.hpp"

namespace astd {

class condition_variable : private std::condition_variable {
public:
    using std::condition_variable::condition_variable;
    using std::condition_variable::notify_one;
    using std::condition_variable::notify_all;

    // reinterpret_cast is not really correct here but I can't work out how to correctly achieve this.
    // This code is not actually used at runtime so this should be fine.

    void wait( astd::unique_lock<astd::mutex>& lock ) {
        std::condition_variable::wait( reinterpret_cast<std::unique_lock<std::mutex>&>(lock) );
    }

    template< class Predicate >
    void wait( astd::unique_lock<astd::mutex>& lock, Predicate pred ) {
        std::condition_variable::wait( reinterpret_cast<std::unique_lock<std::mutex>&>(lock), pred );
    }

    template< class Rep, class Period >
    std::cv_status wait_for( astd::unique_lock<astd::mutex>& lock, const std::chrono::duration<Rep, Period>& rel_time ) {
        return std::condition_variable::wait_for( reinterpret_cast<std::unique_lock<std::mutex>&>(lock), rel_time );
    }

    template< class Rep, class Period, class Predicate >
    bool wait_for( astd::unique_lock<astd::mutex>& lock, const std::chrono::duration<Rep, Period>& rel_time, Predicate pred ) {
        return std::condition_variable::wait_for( reinterpret_cast<std::unique_lock<std::mutex>&>(lock), rel_time, pred );
    }

    template< class Clock, class Duration >
    std::cv_status wait_until( astd::unique_lock<astd::mutex>& lock, const std::chrono::time_point<Clock, Duration>& abs_time ) {
        return std::condition_variable::wait_until( reinterpret_cast<std::unique_lock<std::mutex>&>(lock), abs_time );
    }

    template< class Clock, class Duration, class Predicate >
    bool wait_until( astd::unique_lock<astd::mutex>& lock, const std::chrono::time_point<Clock, Duration>& abs_time, Predicate pred ) {
        return std::condition_variable::wait_until( reinterpret_cast<std::unique_lock<std::mutex>&>(lock), abs_time, pred );
    }
};

} // namespace astd

#else

namespace astd {

using condition_variable = std::condition_variable;

} // namespace astd

#endif
