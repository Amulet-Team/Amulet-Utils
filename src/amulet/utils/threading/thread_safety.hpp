#pragma once

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(AMULET_THREAD_SAFETY_ANALYSIS) && defined(__clang__) && (!defined(SWIG))
#define ASTD_THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define ASTD_THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define ASTD_CAPABILITY(x) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define ASTD_REENTRANT_CAPABILITY \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(reentrant_capability)

#define ASTD_SCOPED_CAPABILITY \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define ASTD_GUARDED_BY(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(__VA_ARGS__))

#define ASTD_PT_GUARDED_BY(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(pt_guarded_by(__VA_ARGS__))

#define ASTD_ACQUIRED_BEFORE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(__VA_ARGS__))

#define ASTD_ACQUIRED_AFTER(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(__VA_ARGS__))

#define ASTD_REQUIRES_UNIQUE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define ASTD_REQUIRES_UNIQUE_OR_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define ASTD_ACQUIRE_UNIQUE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ASTD_ACQUIRE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define ASTD_RELEASE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define ASTD_RELEASE_UNIQUE(...) ASTD_RELEASE(__VA_ARGS__)

#define ASTD_RELEASE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define ASTD_RELEASE_GENERIC(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))

#define ASTD_TRY_ACQUIRE_UNIQUE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define ASTD_TRY_ACQUIRE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define ASTD_EXCLUDES(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASTD_ASSERT_UNIQUE_CAPABILITY(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(__VA_ARGS__))

#define ASTD_ASSERT_SHARED_CAPABILITY(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(__VA_ARGS__))

#define ASTD_RETURN_CAPABILITY(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(__VA_ARGS__))

#define ASTD_NO_THREAD_SAFETY_ANALYSIS \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

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
#define ASTD_CHECK_UNIQUE_CAPABILITY(...) ASTD_TRY_ACQUIRE_UNIQUE(__VA_ARGS__)
#define ASTD_CHECK_SHARED_CAPABILITY(...) ASTD_TRY_ACQUIRE_SHARED(__VA_ARGS__)

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
