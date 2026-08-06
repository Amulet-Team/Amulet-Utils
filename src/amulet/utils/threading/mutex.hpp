#pragma once

#include <mutex>
#include <chrono>

#include "thread_safety.hpp"

namespace astd {

#if defined(AMULET_STD)

class ASTD_CAPABILITY("mutex") mutex : private std::mutex {
public:
    using std::mutex::mutex;
    using std::mutex::native_handle;
    void lock() ASTD_ACQUIRE() { std::mutex::lock(); }
    [[nodiscard]] bool try_lock() ASTD_TRY_ACQUIRE(true) { return std::mutex::try_lock(); }
    void unlock() ASTD_RELEASE() { std::mutex::unlock(); }
};

template<class Mutex>
class [[nodiscard]] ASTD_SCOPED_CAPABILITY lock_guard : private std::lock_guard<Mutex> {
public:
    using std::lock_guard<Mutex>::lock_guard;

    explicit lock_guard( Mutex& m ) ASTD_ACQUIRE(m) : std::lock_guard<Mutex>(m) {}
    lock_guard( Mutex& m, std::adopt_lock_t ) ASTD_REQUIRES(m) : std::lock_guard<Mutex>(m, std::adopt_lock) {}
    lock_guard( const lock_guard& ) = delete;
    lock_guard& operator=( const lock_guard& ) = delete;
    ~lock_guard() ASTD_RELEASE() {}
};

template<class Mutex>
class [[nodiscard]] ASTD_SCOPED_CAPABILITY unique_lock : private std::unique_lock<Mutex> {
public:
    using mutex_type = Mutex;

    // Constructors
    unique_lock() noexcept : std::unique_lock<Mutex>() {}

    explicit unique_lock( Mutex& m ) ASTD_ACQUIRE(m)
        : std::unique_lock<Mutex>(m) {}

    unique_lock( Mutex& m, std::defer_lock_t ) noexcept ASTD_EXCLUDES(m)
        : std::unique_lock<Mutex>(m, std::defer_lock) {}

    unique_lock( Mutex& m, std::adopt_lock_t ) ASTD_REQUIRES(m)
        : std::unique_lock<Mutex>(m, std::adopt_lock) {}

    unique_lock( Mutex& m, std::try_to_lock_t ) ASTD_MAYBE_ACQUIRE(m)
        : std::unique_lock<Mutex>(m, std::try_to_lock) {}

    template< class Rep, class Period >
    unique_lock( Mutex& m, const std::chrono::duration<Rep, Period>& timeout_duration ) ASTD_MAYBE_ACQUIRE(m)
        : std::unique_lock<Mutex>(m, timeout_duration) {}

    template< class Clock, class Duration >
    unique_lock( Mutex& m, const std::chrono::time_point<Clock, Duration>& timeout_time ) ASTD_MAYBE_ACQUIRE(m)
        : std::unique_lock<Mutex>(m, timeout_time) {}

    // Copy
    unique_lock( unique_lock& other ) = delete;
    unique_lock& operator=( unique_lock& other ) = delete;

    // Move
    // TODO: Add annotations if Clang supports moving, swapping and disowning capabilities
    unique_lock( unique_lock&& other ) noexcept ASTD_MOVE_CAPABILITIES(other)
        : std::unique_lock<Mutex>(std::forward<unique_lock>(other)) {}

    unique_lock& operator=( unique_lock&& other ) noexcept ASTD_MOVE_CAPABILITIES(other)
    {
        return std::unique_lock<Mutex>::operator=(std::forward<unique_lock>(other));
    }

    void swap( unique_lock& other ) noexcept ASTD_SWAP_CAPABILITIES(other) {
        std::unique_lock<Mutex>::swap(other);
    }

    mutex_type* release() noexcept {
        return std::unique_lock<Mutex>::release();
    }

    // Destructor
    ~unique_lock() ASTD_RELEASE_IF_HELD() {}

    void unlock() ASTD_RELEASE() {
        std::unique_lock<Mutex>::unlock();
    }

    void lock() ASTD_ACQUIRE() {
        std::unique_lock<Mutex>::lock();
    }

    [[nodiscard]] bool try_lock() ASTD_TRY_ACQUIRE(true) {
        return std::unique_lock<Mutex>::try_lock();
    }

    template< class Rep, class Period >
    [[nodiscard]] bool try_lock_for( const std::chrono::duration<Rep, Period>& timeout_duration ) ASTD_TRY_ACQUIRE(true) {
        return std::unique_lock<Mutex>::try_lock_for(timeout_duration);
    }

    template< class Clock, class Duration >
    [[nodiscard]] bool try_lock_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) ASTD_TRY_ACQUIRE(true) {
        return std::unique_lock<Mutex>::try_lock_until(timeout_time);
    }

    // Clang thread safety analysis does not support alias analysis. Do not use this.
    mutex_type* mutex() const noexcept {
        return std::unique_lock<Mutex>::mutex();
    }

    // TODO: The following methods require a non-acquiring version of TRY_ACQUIRE
    [[nodiscard]] bool owns_lock() const noexcept ASTD_CHECK_CAPABILITY(true) {
        return std::unique_lock<Mutex>::owns_lock();
    }
    [[nodiscard]] explicit operator bool() const noexcept ASTD_CHECK_CAPABILITY(true) {
        return std::unique_lock<Mutex>::operator bool();
    }
};

} // namespace astd

template< class Mutex >
void swap( astd::unique_lock<Mutex>& lhs,
           astd::unique_lock<Mutex>& rhs ) noexcept ASTD_SWAP_CAPABILITIES(lhs, rhs) {
    lhs.swap(rhs);
}

#else

namespace astd {

using mutex = std::mutex;

template<class Mutex>
using lock_guard = std::lock_guard<Mutex>;

template<class Mutex>
using unique_lock = std::unique_lock<Mutex>;

} // namespace astd

#endif
