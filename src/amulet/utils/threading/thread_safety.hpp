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

// ************************************************************
// Requirements
// ************************************************************

#define _ASTD_REQUIRES_CAPABILITY_READ_WRITE(...) \
    __ASTD_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define _ASTD_REQUIRES_CAPABILITY_READ(...) \
    __ASTD_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

// ************************************************************
// Component requirement macros.
// These should only be used when absolutely necessary.
// ************************************************************

// Require that the mutex is held in unique mode.
// This does not check read/write permissions of this thread.
#define ASTD_REQUIRES_COMPONENT_UNIQUE(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.unique_capability)

// Require that the mutex is held in shared read only mode.
// This does not check read/write permissions of this thread.
#define ASTD_REQUIRES_COMPONENT_SHARED_READ_ONLY(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.shared_read_only_capability)

// Require that the mutex is held in shared read write mode.
// This does not check read/write permissions of this thread.
#define ASTD_REQUIRES_COMPONENT_SHARED_READ_WRITE(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.shared_read_write_capability)

// Require that the mutex is held in one of the shared modes.
// This does not check read/write permissions of this thread.
#define ASTD_REQUIRES_COMPONENT_SHARED(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.shared_capability)

// Require that the mutex is held in a mode that does not allow parallel writes.
// This does not check read/write permissions of this thread.
#define ASTD_REQUIRES_COMPONENT_NO_PARALLEL_WRITES(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.read_only_capability)

// Requires the read component.
#define ASTD_REQUIRES_COMPONENT_READ(mtx) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx)   \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.read_capability)

// Requires the read and write components.
#define ASTD_REQUIRES_COMPONENT_READ_WRITE(mtx)         \
    _ASTD_REQUIRES_CAPABILITY_READ_WRITE(mtx)           \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.read_capability) \
    _ASTD_REQUIRES_CAPABILITY_READ(mtx.write_capability)

// ************************************************************
// Internal requirement macros.
// ************************************************************

// Require read permission. No limits to other threads.
#define _ASTD_REQUIRES_Read_SharedReadWrite(mtx) \
    ASTD_REQUIRES_COMPONENT_READ(mtx)

// Require read and write permission. No limits to other threads.
#define _ASTD_REQUIRES_ReadWrite_SharedReadWrite(mtx) \
    ASTD_REQUIRES_COMPONENT_READ_WRITE(mtx)

// Require read permission. Other threads may only read.
#define _ASTD_REQUIRES_Read_SharedReadOnly(mtx) \
    ASTD_REQUIRES_COMPONENT_READ(mtx)           \
    ASTD_REQUIRES_COMPONENT_NO_PARALLEL_WRITES(mtx)

// Require read and write permission. Other threads may only read.
#define _ASTD_REQUIRES_ReadWrite_SharedReadOnly(mtx) \
    ASTD_REQUIRES_COMPONENT_READ_WRITE(mtx)          \
    ASTD_REQUIRES_COMPONENT_NO_PARALLEL_WRITES(mtx)

// Require exclusive read permission.
#define _ASTD_REQUIRES_Read_Unique(mtx)             \
    ASTD_REQUIRES_COMPONENT_READ(mtx)               \
    ASTD_REQUIRES_COMPONENT_NO_PARALLEL_WRITES(mtx) \
    ASTD_REQUIRES_COMPONENT_UNIQUE(mtx)

// Require exclusive read and write permission.
#define _ASTD_REQUIRES_ReadWrite_Unique(mtx)        \
    ASTD_REQUIRES_COMPONENT_READ_WRITE(mtx)         \
    ASTD_REQUIRES_COMPONENT_NO_PARALLEL_WRITES(mtx) \
    ASTD_REQUIRES_COMPONENT_UNIQUE(mtx)

// ************************************************************
// Public requirement macros.
// ************************************************************

// Require the mutex is held in a compatible state.
// Argument 2 is the minimum requirement for this thread. Read < ReadWrite
// Argument 3 is the upper limit of what other threads can do. SharedReadWrite < SharedReadOnly < Unique
// ASTD_REQUIRES(Read, Unique, mtx)
// ASTD_REQUIRES(ReadWrite, Unique, mtx)
// ASTD_REQUIRES(Read, SharedReadOnly, mtx)
// ASTD_REQUIRES(ReadWrite, SharedReadOnly, mtx)
// ASTD_REQUIRES(Read, SharedReadWrite, mtx)
// ASTD_REQUIRES(ReadWrite, SharedReadWrite, mtx)
#define ASTD_REQUIRES(access, share, mtx) _ASTD_REQUIRES_##access##_##share(mtx)

// This thread may read and write. Other threads are blocked.
#define ASTD_REQUIRES_UNIQUE(mtx) ASTD_REQUIRES(ReadWrite, Unique, mtx)

// This thread (and other threads) may read. No writing is allowed by any thread.
#define ASTD_REQUIRES_READ_ONLY(mtx) ASTD_REQUIRES(Read, SharedReadOnly, mtx)

// ************************************************************
// Acquire permissions
// ************************************************************

#define _ASTD_ACQUIRE_CAPABILITY_READ(...) \
    __ASTD_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define _ASTD_ACQUIRE_CAPABILITY_READ_WRITE(...) \
    __ASTD_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

// ************************************************************
// Internal acquire component macros.
// ************************************************************

#define _ASTD_ACQUIRE_COMPONENT_READ(mtx) \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx)    \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.read_capability)

#define _ASTD_ACQUIRE_COMPONENT_READ_WRITE(mtx)        \
    _ASTD_ACQUIRE_CAPABILITY_READ_WRITE(mtx)           \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.read_capability) \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.write_capability)

#define _ASTD_ACQUIRE_COMPONENT_UNIQUE(mtx)              \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.unique_capability) \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.read_only_capability)

#define _ASTD_ACQUIRE_COMPONENT_SHARED_READ_ONLY(mtx)              \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.shared_read_only_capability) \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.shared_capability)           \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.read_only_capability)

#define _ASTD_ACQUIRE_COMPONENT_SHARED_READ_WRITE(mtx)              \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.shared_read_write_capability) \
    _ASTD_ACQUIRE_CAPABILITY_READ(mtx.shared_capability)

// ************************************************************
// Internal acquire macros.
// ************************************************************

#define _ASTD_ACQUIRE_Read_Unique(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ(mtx)  \
    _ASTD_ACQUIRE_COMPONENT_UNIQUE(mtx)

#define _ASTD_ACQUIRE_Read_SharedReadOnly(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ(mtx)          \
    _ASTD_ACQUIRE_COMPONENT_SHARED_READ_ONLY(mtx)

#define _ASTD_ACQUIRE_Read_SharedReadWrite(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ(mtx)           \
    _ASTD_ACQUIRE_COMPONENT_SHARED_READ_WRITE(mtx)

#define _ASTD_ACQUIRE_ReadWrite_Unique(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ_WRITE(mtx) \
    _ASTD_ACQUIRE_COMPONENT_UNIQUE(mtx)

#define _ASTD_ACQUIRE_ReadWrite_SharedReadOnly(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ_WRITE(mtx)         \
    _ASTD_ACQUIRE_COMPONENT_SHARED_READ_ONLY(mtx)

#define _ASTD_ACQUIRE_ReadWrite_SharedReadWrite(mtx) \
    _ASTD_ACQUIRE_COMPONENT_READ_WRITE(mtx)          \
    _ASTD_ACQUIRE_COMPONENT_SHARED_READ_WRITE(mtx)

// ************************************************************
// Public acquire macros.
// ************************************************************

// Acquire the mutex in the specified state.
// Argument 2 specifies what this thread can do.
// Argument 3 specifies what other threads can do.
// ASTD_ACQUIRE(Read, Unique, mtx)
// ASTD_ACQUIRE(ReadWrite, Unique, mtx)
// ASTD_ACQUIRE(Read, SharedReadOnly, mtx)
// ASTD_ACQUIRE(ReadWrite, SharedReadOnly, mtx)
// ASTD_ACQUIRE(Read, SharedReadWrite, mtx)
// ASTD_ACQUIRE(ReadWrite, SharedReadWrite, mtx)
#define ASTD_ACQUIRE(access, share, mtx) \
    _ASTD_ACQUIRE_##access##_##share(mtx)

// Acquire the mutex with read and write permissions. Other threads are blocked.
#define ASTD_ACQUIRE_UNIQUE(mtx) ASTD_ACQUIRE(ReadWrite, Unique, mtx)

// Acquire the mutex with read permission. Other threads may also read (but not write).
#define ASTD_ACQUIRE_SHARED(mtx) ASTD_ACQUIRE(Read, SharedReadOnly, mtx)

// Acquire all mutexes acquired by this scope object in unique mode.
#define ASTD_ACQUIRE_UNIQUE_SCOPED() _ASTD_ACQUIRE_CAPABILITY_READ_WRITE()

// Acquire all mutexes acquired by this scope object in shared mode.
#define ASTD_ACQUIRE_SHARED_SCOPED() _ASTD_ACQUIRE_CAPABILITY_READ()

// ************************************************************
// Try acquire permissions
// ************************************************************

#define _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, ...) \
    __ASTD_ATTRIBUTE__(try_acquire_shared_capability(value __VA_OPT__(, ) __VA_ARGS__))

#define _ASTD_TRY_ACQUIRE_CAPABILITY_READ_WRITE(value, ...) \
    __ASTD_ATTRIBUTE__(try_acquire_capability(value __VA_OPT__(, ) __VA_ARGS__))

// ************************************************************
// Internal try acquire component macros.
// ************************************************************

#define _ASTD_TRY_ACQUIRE_COMPONENT_READ(value, mtx) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx)    \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.read_capability)

#define _ASTD_TRY_ACQUIRE_COMPONENT_READ_WRITE(value, mtx)        \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ_WRITE(value, mtx)           \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.read_capability) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.write_capability)

#define _ASTD_TRY_ACQUIRE_COMPONENT_UNIQUE(value, mtx)              \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.unique_capability) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.read_only_capability)

#define _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_ONLY(value, mtx)              \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.shared_read_only_capability) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.shared_capability)           \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.read_only_capability)

#define _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_WRITE(value, mtx)              \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.shared_read_write_capability) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value, mtx.shared_capability)

// ************************************************************
// Internal try acquire macros.
// ************************************************************

#define _ASTD_TRY_ACQUIRE_Read_Unique(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ(value, mtx)  \
    _ASTD_TRY_ACQUIRE_COMPONENT_UNIQUE(value, mtx)

#define _ASTD_TRY_ACQUIRE_Read_SharedReadOnly(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ(value, mtx)          \
    _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_ONLY(value, mtx)

#define _ASTD_TRY_ACQUIRE_Read_SharedReadWrite(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ(value, mtx)           \
    _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_WRITE(value, mtx)

#define _ASTD_TRY_ACQUIRE_ReadWrite_Unique(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ_WRITE(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_UNIQUE(value, mtx)

#define _ASTD_TRY_ACQUIRE_ReadWrite_SharedReadOnly(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ_WRITE(value, mtx)         \
    _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_ONLY(value, mtx)

#define _ASTD_TRY_ACQUIRE_ReadWrite_SharedReadWrite(value, mtx) \
    _ASTD_TRY_ACQUIRE_COMPONENT_READ_WRITE(value, mtx)          \
    _ASTD_TRY_ACQUIRE_COMPONENT_SHARED_READ_WRITE(value, mtx)

// ************************************************************
// Public try acquire macros.
// ************************************************************

// Try to acquire the mutex in the specified state.
// Argument 2 specifies what this thread can do.
// Argument 3 specifies what other threads can do.
// ASTD_TRY_ACQUIRE(Read, Unique, value, mtx)
// ASTD_TRY_ACQUIRE(ReadWrite, Unique, value, mtx)
// ASTD_TRY_ACQUIRE(Read, SharedReadOnly, value, mtx)
// ASTD_TRY_ACQUIRE(ReadWrite, SharedReadOnly, value, mtx)
// ASTD_TRY_ACQUIRE(Read, SharedReadWrite, value, mtx)
// ASTD_TRY_ACQUIRE(ReadWrite, SharedReadWrite, value, mtx)
#define ASTD_TRY_ACQUIRE(access, share, value, mtx) \
    _ASTD_TRY_ACQUIRE_##access##_##share(value, mtx)

#define ASTD_TRY_ACQUIRE_UNIQUE(value, mtx) \
    ASTD_TRY_ACQUIRE(ReadWrite, Unique, value, mtx)

#define ASTD_TRY_ACQUIRE_SHARED(value, mtx) \
    ASTD_TRY_ACQUIRE(Read, SharedReadOnly, value, mtx)

#define ASTD_TRY_ACQUIRE_UNIQUE_SCOPED(value) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ_WRITE(value)

#define ASTD_TRY_ACQUIRE_SHARED_SCOPED(value) \
    _ASTD_TRY_ACQUIRE_CAPABILITY_READ(value)

// ************************************************************
// Release macros.
// ************************************************************

#define _ASTD_RELEASE_CAPABILITY_READ(...) \
    __ASTD_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define _ASTD_RELEASE_CAPABILITY_READ_WRITE(...) \
    __ASTD_ATTRIBUTE__(release_capability(__VA_ARGS__))

// ************************************************************
// Internal release component macros.
// ************************************************************

#define _ASTD_RELEASE_COMPONENT_READ(mtx) \
    _ASTD_RELEASE_CAPABILITY_READ(mtx)    \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.read_capability)

#define _ASTD_RELEASE_COMPONENT_READ_WRITE(mtx)        \
    _ASTD_RELEASE_CAPABILITY_READ_WRITE(mtx)           \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.read_capability) \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.write_capability)

#define _ASTD_RELEASE_COMPONENT_UNIQUE(mtx)              \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.unique_capability) \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.read_only_capability)

#define _ASTD_RELEASE_COMPONENT_SHARED_READ_ONLY(mtx)              \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.shared_read_only_capability) \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.shared_capability)           \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.read_only_capability)

#define _ASTD_RELEASE_COMPONENT_SHARED_READ_WRITE(mtx)              \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.shared_read_write_capability) \
    _ASTD_RELEASE_CAPABILITY_READ(mtx.shared_capability)

// ************************************************************
// Internal release macros.
// ************************************************************

#define _ASTD_RELEASE_Read_Unique(mtx) \
    _ASTD_RELEASE_COMPONENT_READ(mtx)  \
    _ASTD_RELEASE_COMPONENT_UNIQUE(mtx)

#define _ASTD_RELEASE_Read_SharedReadOnly(mtx) \
    _ASTD_RELEASE_COMPONENT_READ(mtx)          \
    _ASTD_RELEASE_COMPONENT_SHARED_READ_ONLY(mtx)

#define _ASTD_RELEASE_Read_SharedReadWrite(mtx) \
    _ASTD_RELEASE_COMPONENT_READ(mtx)           \
    _ASTD_RELEASE_COMPONENT_SHARED_READ_WRITE(mtx)

#define _ASTD_RELEASE_ReadWrite_Unique(mtx) \
    _ASTD_RELEASE_COMPONENT_READ_WRITE(mtx) \
    _ASTD_RELEASE_COMPONENT_UNIQUE(mtx)

#define _ASTD_RELEASE_ReadWrite_SharedReadOnly(mtx) \
    _ASTD_RELEASE_COMPONENT_READ_WRITE(mtx)         \
    _ASTD_RELEASE_COMPONENT_SHARED_READ_ONLY(mtx)

#define _ASTD_RELEASE_ReadWrite_SharedReadWrite(mtx) \
    _ASTD_RELEASE_COMPONENT_READ_WRITE(mtx)          \
    _ASTD_RELEASE_COMPONENT_SHARED_READ_WRITE(mtx)

// ************************************************************
// Public release macros.
// ************************************************************

// Release the mutex in the specified state.
// Argument 2 specifies what this thread can do.
// Argument 3 specifies what other threads can do.
// These arguments must match the arguments the mutex was acquired with.
// ASTD_ACQUIRE(Read, Unique, mtx)
// ASTD_ACQUIRE(ReadWrite, Unique, mtx)
// ASTD_ACQUIRE(Read, SharedReadOnly, mtx)
// ASTD_ACQUIRE(ReadWrite, SharedReadOnly, mtx)
// ASTD_ACQUIRE(Read, SharedReadWrite, mtx)
// ASTD_ACQUIRE(ReadWrite, SharedReadWrite, mtx)
#define ASTD_RELEASE(access, share, mtx) \
    _ASTD_RELEASE_##access##_##share(mtx)

// Release the mutex with unique read and write permissions.
#define ASTD_RELEASE_UNIQUE(mtx) ASTD_RELEASE(ReadWrite, Unique, mtx)

// Release the mutex with shared read-only permission.
#define ASTD_RELEASE_SHARED(mtx) ASTD_RELEASE(Read, SharedReadOnly, mtx)

// Release all mutexes acquired by this scope.
#define ASTD_RELEASE_UNIQUE_SCOPED() _ASTD_RELEASE_CAPABILITY_READ_WRITE()

// Release all mutexes acquired by this scope.
#define ASTD_RELEASE_SHARED_SCOPED() _ASTD_RELEASE_CAPABILITY_READ()

#define ASTD_RELEASE_GENERIC(...) \
    __ASTD_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))

// ************************************************************

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

// ************************************************************
// Maybe acquire macros.
// Annotations for functions that may or may not acquire a mutex.
// Eg the try_to_lock lock constructors.
// ************************************************************

// Required by lock try_to_acquire constructors
// Marks the capability as both locked and unlocked.
// Its state must be inspected and left in the unlocked or locked state.
#define ASTD_MAYBE_ACQUIRE(access, share, mtx) ASTD_EXCLUDES(mtx)
#define ASTD_MAYBE_ACQUIRE_UNIQUE(mtx) ASTD_MAYBE_ACQUIRE(ReadWrite, Unique, mtx)
#define ASTD_MAYBE_ACQUIRE_SHARED(mtx) ASTD_MAYBE_ACQUIRE(Read, SharedReadOnly, mtx)

// ************************************************************
// Check acquired macros.
// Macros to check if a lock was previously acquired. Eg owns_lock
// Currently they are aliased to try_acquire so can only be used in the unlocked state.
// ************************************************************

// Annotation for functions that check if the mutex was held in the specified state.
// The function must return "value" if the mutex is held.
// Argument 2 specifies what this thread can do.
// Argument 3 specifies what other threads can do.
#define ASTD_CHECK_ACQUIRED(access, share, value, mtx) \
    ASTD_TRY_ACQUIRE(access, share, value, mtx)

#define ASTD_CHECK_ACQUIRED_UNIQUE(value, mtx) ASTD_CHECK_ACQUIRED(ReadWrite, Unique, value, mtx)
#define ASTD_CHECK_ACQUIRED_SHARED(value, mtx) ASTD_CHECK_ACQUIRED(Read, SharedReadOnly, value, mtx)

// Check acquired annotation for all mutexes in this scope.
#define ASTD_CHECK_ACQUIRED_UNIQUE_SCOPED(value) ASTD_TRY_ACQUIRE_UNIQUE_SCOPED(value)
#define ASTD_CHECK_ACQUIRED_SHARED_SCOPED(value) ASTD_TRY_ACQUIRE_SHARED_SCOPED(value)

// ************************************************************

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
