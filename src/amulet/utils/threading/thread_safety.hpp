#pragma once

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(__clang__) && (!defined(SWIG))
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

#define ASTD_REQUIRES(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(requires_capability(__VA_ARGS__))

#define ASTD_REQUIRES_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(requires_shared_capability(__VA_ARGS__))

#define ASTD_ACQUIRE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquire_capability(__VA_ARGS__))

#define ASTD_ACQUIRE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(acquire_shared_capability(__VA_ARGS__))

#define ASTD_RELEASE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_capability(__VA_ARGS__))

#define ASTD_RELEASE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_shared_capability(__VA_ARGS__))

#define ASTD_RELEASE_GENERIC(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(release_generic_capability(__VA_ARGS__))

#define ASTD_TRY_ACQUIRE(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_capability(__VA_ARGS__))

#define ASTD_TRY_ACQUIRE_SHARED(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(try_acquire_shared_capability(__VA_ARGS__))

#define ASTD_EXCLUDES(...) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(__VA_ARGS__))

#define ASTD_ASSERT_CAPABILITY(x) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(assert_capability(x))

#define ASTD_ASSERT_SHARED_CAPABILITY(x) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(assert_shared_capability(x))

#define ASTD_RETURN_CAPABILITY(x) \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

#define ASTD_NO_THREAD_SAFETY_ANALYSIS \
  ASTD_THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)
