#pragma once

#ifndef NULL
#define NULL 0
#endif

// Unknown types

//! Describes an unknown 32 bit value
typedef int unk32;
//! Describes an unknown 16 bit value
typedef short unk16;
//! Describes an unknown 8 bit value
typedef unsigned char unk8;
//! Unknown value of unknown size
typedef unk32 unk;

// Necesary for CW
#if __cplusplus < 201103L && !defined(_WIN32)
#define override
#define noexcept
#define nullptr NULL
#endif

#define ensures(cond)
#define expects(cond)

#ifdef __CWCC__
#define MWREG register
#define CONST_MWREG register

#define WPOPT volatile
#define MW_PRAG_NOINLINE _Pragma("push") _Pragma("dont_inline on")
#define MW_PRAG_OPT_S _Pragma("push") _Pragma("optimize_for_size on")
#define MW_PRAG_NO_WARN_10216 _Pragma("push") _Pragma("warning off(10216)")
#define MW_PRAG_PACKED _Pragma("push") _Pragma("options align=packed")

#define MW_PRAG_END _Pragma("pop")
#define DECOMP // TODO: Move to build

#define FORCE_INLINE __inline
#define NEVER_INLINE __attribute__((never_inline))

#else
#define MWREG
#define CONST_MWREG const
#define MW_PRAG_NOINLINE
#define MW_PRAG_END
#define MW_PRAG_OPT_S
#define MW_PRAG_PACKED
#define MW_PRAG_NO_WARN_10216

#define FORCE_INLINE __forceinline
#define NEVER_INLINE
#endif

// A function that does nothing
#define NULLSUB void

// We know it's release stripped usually from context. i.e. allocated on debug
// heap
#define RELEASE_STRIPPED_ALL NULLSUB

#define UNUSED_PARAM(x) (void)(x); // EPPC:#pragma unused
#define UNUSED

#ifdef DECOMP

#define LOCALREF(key, type, data, ref) extern type key;
#else
#define LINKED_ELSEWHERE

#define LOCALREF(key, type, data, ref) type key = data;
#endif

// Standard types
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned long size_t;

typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float f32;
typedef double f64;
typedef volatile f32 vf32;
typedef volatile f64 vf64;

#define TRUE 1
#define FALSE 0

#define static_assert(cond) __static_assert(cond, #cond)

#define ROUND_UP(x, n) (((u32)(x) + n - 1) & ~(n - 1))
#define ROUND_DOWN(x, n) (((u32)(x)) & ~(n - 1))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

inline u32 min32(u32 a, u32 b)
{
    return a <= b ? a : b;
}

#ifdef __cplusplus
#define restrict

class NonCopyable
{
public:
    inline NonCopyable() {}

private:
    inline NonCopyable(const NonCopyable &) {}
};

// #define static_assert(cond) __static_assert(cond, #cond)
#endif // __cplusplus

#define INLINE_ELSEWHERE(x)
