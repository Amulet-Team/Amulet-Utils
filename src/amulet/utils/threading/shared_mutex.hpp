#pragma once

#include <shared_mutex>

#if defined(AMULET_THREAD_SAFETY_ANALYSIS)

#include <chrono>
#include <type_traits>

#include "thread_safety.hpp"

namespace astd {

class ASTD_CAPABILITY("mutex") shared_mutex : private std::shared_mutex, public base_mutex {
public:
    using std::shared_mutex::shared_mutex;
    void lock() ASTD_ACQUIRE_UNIQUE((*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { std::shared_mutex::lock(); }
    void lock_shared() ASTD_ACQUIRE_SHARED((*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { std::shared_mutex::lock_shared(); }
    [[nodiscard]] bool try_lock() ASTD_TRY_ACQUIRE_UNIQUE(true, (*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { return std::shared_mutex::try_lock(); }
    [[nodiscard]] bool try_lock_shared() ASTD_TRY_ACQUIRE_SHARED(true, (*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { return std::shared_mutex::try_lock_shared(); }
    void unlock() ASTD_RELEASE_UNIQUE((*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { std::shared_mutex::unlock(); }
    void unlock_shared() ASTD_RELEASE_SHARED((*this)) ASTD_NO_THREAD_SAFETY_ANALYSIS { std::shared_mutex::unlock_shared(); }
};

template<class Mutex>
    requires std::is_base_of_v<base_mutex, Mutex>
class [[nodiscard]] ASTD_SCOPED_CAPABILITY shared_lock : private std::shared_lock<Mutex> {
public:
    using mutex_type = Mutex;

    // Constructors
    shared_lock() noexcept : std::shared_lock<Mutex>() {}

    explicit shared_lock(Mutex& m) ASTD_ACQUIRE_SHARED(m)
        : std::shared_lock<Mutex>(m) {}

    shared_lock(Mutex& m, std::defer_lock_t) noexcept ASTD_EXCLUDES(Read, SharedReadOnly, m)
        : std::shared_lock<Mutex>(m, std::defer_lock) {}

    shared_lock(Mutex& m, std::adopt_lock_t) ASTD_REQUIRES(Read, SharedReadOnly) ASTD_REQUIRES_COMPONENT_SHARED(m) ASTD_REQUIRES_COMPONENT_SHARED_READ_ONLY(m)
        : std::shared_lock<Mutex>(m, std::adopt_lock) {}

    shared_lock(Mutex& m, std::try_to_lock_t) ASTD_MAYBE_ACQUIRE_SHARED(m)
        : std::shared_lock<Mutex>(m, std::try_to_lock) {}

    template< class Rep, class Period >
    shared_lock(Mutex& m, const std::chrono::duration<Rep, Period>& timeout_duration) ASTD_MAYBE_ACQUIRE_SHARED(m)
        : std::shared_lock<Mutex>(m, timeout_duration) {}

    template< class Clock, class Duration >
    shared_lock(Mutex& m, const std::chrono::time_point<Clock, Duration>& timeout_time) ASTD_MAYBE_ACQUIRE_SHARED(m)
        : std::shared_lock<Mutex>(m, timeout_time) {}

    // Copy
    shared_lock( const shared_lock& other ) = delete;
    shared_lock& operator=( const shared_lock& other ) = delete;

    // Move
    // TODO: Add annotations if Clang supports moving, swapping and disowning capabilities
    shared_lock( shared_lock&& other ) noexcept ASTD_MOVE_SCOPED_CAPABILITIES(other)
        : std::shared_lock<Mutex>(std::forward<shared_lock>(other)) {}

    shared_lock& operator=( shared_lock&& other ) noexcept ASTD_MOVE_SCOPED_CAPABILITIES(other)
    {
        std::shared_lock<Mutex>::operator=(std::forward<shared_lock>(other));
        return *this;
    }

    void swap( shared_lock& other ) noexcept ASTD_SWAP_SCOPED_CAPABILITIES(other, (*this)) {
        std::shared_lock<Mutex>::swap(other);
    }

    mutex_type* release() noexcept {
        return std::shared_lock<Mutex>::release();
    }

    // Destructor
    ~shared_lock() ASTD_ASSERT_CAPABILITY(Read, Scoped) ASTD_RELEASE(Read, Scoped) { }

    void unlock() ASTD_RELEASE(Read, Scoped)
    {
        std::shared_lock<Mutex>::unlock();
    }

    void lock() ASTD_ACQUIRE(Read, Scoped) {
        std::shared_lock<Mutex>::lock();
    }

    [[nodiscard]] bool try_lock() ASTD_TRY_ACQUIRE(Read, Scoped, true) {
        return std::shared_lock<Mutex>::try_lock();
    }

    template< class Rep, class Period >
    [[nodiscard]] bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) ASTD_TRY_ACQUIRE(Read, Scoped, true)
    {
        return std::shared_lock<Mutex>::try_lock_for(timeout_duration);
    }

    template< class Clock, class Duration >
    [[nodiscard]] bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time) ASTD_TRY_ACQUIRE(Read, Scoped, true)
    {
        return std::shared_lock<Mutex>::try_lock_until(timeout_time);
    }

    // Clang thread safety analysis does not support alias analysis. Do not use this.
    mutex_type* mutex() const noexcept {
        return std::shared_lock<Mutex>::mutex();
    }

    // TODO: The following methods require a non-acquiring version of TRY_ACQUIRE
    [[nodiscard]] bool owns_lock() const noexcept ASTD_CHECK_ACQUIRED(Read, Scoped, true) {
        return std::shared_lock<Mutex>::owns_lock();
    }
    [[nodiscard]] explicit operator bool() const noexcept ASTD_CHECK_ACQUIRED(Read, Scoped, true) {
        return std::shared_lock<Mutex>::operator bool();
    }
};

} // namespace astd

template< class Mutex >
void swap( astd::shared_lock<Mutex>& lhs,
           astd::shared_lock<Mutex>& rhs ) noexcept ASTD_SWAP_SCOPED_CAPABILITIES(lhs, rhs) {
    lhs.swap(rhs);
}

#else

namespace astd {

using shared_mutex = std::shared_mutex;

template<class Mutex>
using shared_lock = std::shared_lock<Mutex>;

} // namespace astd

#endif
