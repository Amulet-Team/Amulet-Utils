#pragma once

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(AMULET_THREAD_SAFETY_ANALYSIS) && defined(__clang__) && (!defined(SWIG))
#define __ASTD_ATTRIBUTE__(x) __attribute__((x))
#else
#define __ASTD_ATTRIBUTE__(x) // no-op
#endif

#define ASTD_CAPABILITY(x) \
    __ASTD_ATTRIBUTE__(capability(x))

#define ASTD_REENTRANT_CAPABILITY \
    __ASTD_ATTRIBUTE__(reentrant_capability)

#define ASTD_SCOPED_CAPABILITY \
    __ASTD_ATTRIBUTE__(scoped_lockable)

#define ASTD_GUARDED_BY(...) \
    __ASTD_ATTRIBUTE__(guarded_by(__VA_ARGS__))

#define ASTD_PT_GUARDED_BY(...) \
    __ASTD_ATTRIBUTE__(pt_guarded_by(__VA_ARGS__))

#define ASTD_ACQUIRED_BEFORE(...) \
    __ASTD_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ASTD_ACQUIRED_AFTER(...) \
    __ASTD_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define ASTD_REQUIRES_UNIQUE(...) \
    __ASTD_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define ASTD_REQUIRES_UNIQUE_OR_SHARED(...) \
    __ASTD_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

// ************************************************************
// Acquire permissions
// ************************************************************

#define _ASTD_ACQUIRE_CAPABILITY_READ(...) \
    __ASTD_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define _ASTD_ACQUIRE_CAPABILITY_READ_WRITE(...) \
    __ASTD_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

// ************************************************************
// Public acquire macros.
// ************************************************************

#define ASTD_ACQUIRE_UNIQUE(...) \
    __ASTD_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ASTD_ACQUIRE_SHARED(...) \
    __ASTD_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

// Acquire all mutexes acquired by this scope object in unique mode.
#define ASTD_ACQUIRE_UNIQUE_SCOPED() _ASTD_ACQUIRE_CAPABILITY_READ_WRITE()

// Acquire all mutexes acquired by this scope object in shared mode.
#define ASTD_ACQUIRE_SHARED_SCOPED() _ASTD_ACQUIRE_CAPABILITY_READ()

// ************************************************************
// Public try acquire macros.
// ************************************************************

#define ASTD_TRY_ACQUIRE_UNIQUE(value, ...) \
    __ASTD_ATTRIBUTE__(try_acquire_capability(value __VA_OPT__(,) __VA_ARGS__))

#define ASTD_TRY_ACQUIRE_SHARED(value, ...) \
    __ASTD_ATTRIBUTE__(try_acquire_shared_capability(value __VA_OPT__(,) __VA_ARGS__))

#define ASTD_TRY_ACQUIRE_UNIQUE_SCOPED(value) \
    __ASTD_ATTRIBUTE__(try_acquire_capability(value))

#define ASTD_TRY_ACQUIRE_SHARED_SCOPED(value) \
    __ASTD_ATTRIBUTE__(try_acquire_shared_capability(value))

// ************************************************************
// Release macros.
// ************************************************************

#define _ASTD_RELEASE_CAPABILITY_READ(...) \
    __ASTD_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define _ASTD_RELEASE_CAPABILITY_READ_WRITE(...) \
    __ASTD_ATTRIBUTE__(release_capability(__VA_ARGS__))

// ************************************************************
// Public release macros.
// ************************************************************


#define ASTD_RELEASE(...) \
    __ASTD_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define ASTD_RELEASE_UNIQUE(...) ASTD_RELEASE(__VA_ARGS__)

#define ASTD_RELEASE_SHARED(...) \
    __ASTD_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

// Release all mutexes acquired by this scope.
#define ASTD_RELEASE_UNIQUE_SCOPED() _ASTD_RELEASE_CAPABILITY_READ_WRITE()

// Release all mutexes acquired by this scope.
#define ASTD_RELEASE_SHARED_SCOPED() _ASTD_RELEASE_CAPABILITY_READ()

#define ASTD_RELEASE_GENERIC(...) \
    __ASTD_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))


#define ASTD_EXCLUDES(...) \
    __ASTD_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASTD_ASSERT_UNIQUE_CAPABILITY(...) \
    __ASTD_ATTRIBUTE__(assert_capability(__VA_ARGS__))

#define ASTD_ASSERT_SHARED_CAPABILITY(...) \
    __ASTD_ATTRIBUTE__(assert_shared_capability(__VA_ARGS__))

#define ASTD_RETURN_CAPABILITY(...) \
    __ASTD_ATTRIBUTE__(lock_returned(__VA_ARGS__))

#define ASTD_NO_THREAD_SAFETY_ANALYSIS \
    __ASTD_ATTRIBUTE__(no_thread_safety_analysis)

// These annotations are required to fully support unique_lock and shared_lock.
// The following are the best approximations using the existing annotations.

// Required by lock try_to_acquire constructors
// Marks the capability as both locked and unlocked.
// Its state must be inspected and left in the unlocked or locked state.
#define ASTD_MAYBE_ACQUIRE_UNIQUE(...) ASTD_EXCLUDES(__VA_ARGS__)
#define ASTD_MAYBE_ACQUIRE_SHARED(...) ASTD_EXCLUDES(__VA_ARGS__)

// Required by owns_lock to inspect the lock state without acquiring it.
// Works like try_acquire_[shared_]capability but can be used in any context.
// Currently they are aliased to try_acquire_[shared_]capability so can only be used in the unlocked state.
#define ASTD_CHECK_UNIQUE_CAPABILITY(value, ...) ASTD_TRY_ACQUIRE_UNIQUE(value __VA_OPT__(,) __VA_ARGS__)
#define ASTD_CHECK_SHARED_CAPABILITY(value, ...) ASTD_TRY_ACQUIRE_SHARED(value __VA_OPT__(,) __VA_ARGS__)

// Check acquired annotation for all mutexes in this scope.
#define ASTD_CHECK_ACQUIRED_UNIQUE_SCOPED(value) ASTD_TRY_ACQUIRE_UNIQUE_SCOPED(value)
#define ASTD_CHECK_ACQUIRED_SHARED_SCOPED(value) ASTD_TRY_ACQUIRE_SHARED_SCOPED(value)

// Required by lock destructors when the capability is not held.
// Release the capability, regardless of initial value.
#define ASTD_RELEASE_IF_HELD(...) ASTD_ASSERT_UNIQUE_CAPABILITY(__VA_ARGS__) ASTD_RELEASE(__VA_ARGS__)

// Required to swap the capabilities of two locks.
// Swap the capabilities of `a` and `b` (a, b) or (a)
#define ASTD_SWAP_CAPABILITIES(a, ...)

// Required by the move operators.
// Move the capabilities from `a` to `b` (a, b) or (a)
// Any capabilities in `b` are released.
#define ASTD_MOVE_CAPABILITIES(from, ...) ASTD_RELEASE_IF_HELD(__VA_ARGS__)

namespace astd {

// Allowed to read
class ASTD_CAPABILITY("ReadCapability") ReadCapability { };

// Allowed to write
class ASTD_CAPABILITY("WriteCapability") WriteCapability { };

// Held in unique mode. Other threads can't run in parallel.
class ASTD_CAPABILITY("UniqueCapability") UniqueCapability { };

// Other threads can only read in parallel.
class ASTD_CAPABILITY("SharedReadOnlyCapability") SharedReadOnlyCapability { };

// Other threads can read and write in parallel.
// This can only be used for public functions where the internal state is protected by another mutex.
class ASTD_CAPABILITY("SharedReadWriteCapability") SharedReadWriteCapability { };

// Held in either shared mode.
class ASTD_CAPABILITY("SharedCapability") SharedCapability { };

// Held in either unique or shared read only mode.
// Ensures that other threads cannot mutate in parallel.
class ASTD_CAPABILITY("ReadOnlyCapability") ReadOnlyCapability { };

class base_mutex {
public:
    ReadCapability read_capability;
    WriteCapability write_capability;
    UniqueCapability unique_capability;
    ReadOnlyCapability read_only_capability;
};

class base_shared_mutex : public base_mutex {
public:
    SharedReadOnlyCapability shared_read_only_capability;
    SharedReadWriteCapability shared_read_write_capability;
    SharedCapability shared_capability;
};

} // namespace astd
