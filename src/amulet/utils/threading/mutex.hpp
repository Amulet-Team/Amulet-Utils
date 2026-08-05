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
class [[nodiscard]] ASTD_SCOPED_CAPABILITY lock_guard {
private:
    Mutex& _m;

public:
    using mutex_type = Mutex;

    explicit lock_guard( Mutex& m ) ASTD_ACQUIRE(m) : _m(m) { m.lock(); }
    lock_guard( Mutex& m, std::adopt_lock_t ) ASTD_REQUIRES(m) : _m(m) {}
    lock_guard( const lock_guard& ) = delete;
    lock_guard& operator=( const lock_guard& ) = delete;
    ~lock_guard() ASTD_RELEASE() { _m.unlock(); }
};

template<class Mutex>
class [[nodiscard]] ASTD_SCOPED_CAPABILITY unique_lock {
private:
    Mutex* _m;
    bool owns;

public:
    using mutex_type = Mutex;

    // Constructors
    unique_lock() noexcept : _m(nullptr), owns(false) {}

    explicit unique_lock( Mutex& m ) ASTD_ACQUIRE(m)
        : _m(&m), owns(true) { m.lock(); }

    unique_lock( Mutex& m, std::defer_lock_t ) noexcept ASTD_EXCLUDES(m)
        : _m(&m), owns(false) {}

    unique_lock( Mutex& m, std::adopt_lock_t ) ASTD_REQUIRES(m)
        : _m(&m), owns(true) {}

    unique_lock( Mutex& m, std::try_to_lock_t ) ASTD_EXCLUDES(m)
        : _m(&m), owns(m.try_lock()) {}

    template< class Rep, class Period >
    unique_lock( Mutex& m, const std::chrono::duration<Rep, Period>& timeout_duration ) ASTD_EXCLUDES(m)
        : _m(&m), owns(m.try_lock_for(timeout_duration)) {}

    template< class Clock, class Duration >
    unique_lock( Mutex& m, const std::chrono::time_point<Clock, Duration>& timeout_time ) ASTD_EXCLUDES(m)
        : _m(&m), owns(m.try_lock_until(timeout_time)) {}

    // Copy
    unique_lock( unique_lock& other ) = delete;
    unique_lock& operator=( unique_lock& other ) = delete;

    // Move
    // TODO: Add annotations if Clang supports moving, swapping and disowning capabilities
    unique_lock( unique_lock&& other ) noexcept
        : _m(std::exchange(other._m, nullptr))
        , owns(std::exchange(other.owns, false)) {}

    unique_lock& operator=( unique_lock&& other ) noexcept
    {
        if (this != &other) {
            if (_m && owns) _m->unlock();
            _m = std::exchange(other._m, nullptr);
            owns = std::exchange(other.owns, false);
        }
        return *this;
    }

    void swap( unique_lock& other ) noexcept {
        std::swap(_m, other._m);
        std::swap(owns, other.owns);
    }

    mutex_type* release() noexcept ASTD_RETURN_CAPABILITY(m)  {
        owns = false;
        return std::exchange(_m, nullptr);
    }

    // Destructor
    ~unique_lock() ASTD_RELEASE() { if (_m && owns) _m->unlock(); }

    void lock() ASTD_ACQUIRE() {
        _m->lock();
        owns = true;
    }

    [[nodiscard]] bool try_lock() ASTD_TRY_ACQUIRE(true) {
        return owns = _m->try_lock();
    }

    template< class Rep, class Period >
    [[nodiscard]] bool try_lock_for( const std::chrono::duration<Rep, Period>& timeout_duration ) ASTD_TRY_ACQUIRE(true) {
        return owns = _m->try_lock_for(timeout_duration);
    }

    template< class Clock, class Duration >
    [[nodiscard]] bool try_lock_until( const std::chrono::time_point<Clock, Duration>& timeout_time ) ASTD_TRY_ACQUIRE(true) {
        return owns = _m->try_lock_until(timeout_time);
    }

    void unlock() ASTD_RELEASE() {
        _m->unlock();
        owns = false;
    }

    // Clang thread safety analysis does not support alias analysis. Do not use this.
    mutex_type* mutex() const noexcept { return _m; }

    // TODO: The following methods require a non-acquiring version of TRY_ACQUIRE
    bool owns_lock() const noexcept { return owns; }
    explicit operator bool() const noexcept { return owns; }
};

} // namespace astd

template< class Mutex >
void swap( unique_lock<Mutex>& lhs,
           unique_lock<Mutex>& rhs ) noexcept {
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
