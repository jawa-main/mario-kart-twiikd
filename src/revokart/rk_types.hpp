#ifndef RK_TYPES
#define RK_TYPES

#define NULL 0

typedef int unk32;
typedef short unk16;
typedef unsigned char unk8;
typedef unk32 unk;

#define override
#define noexcept
#define nullptr NULL

#define ensures(cond)
#define expects(cond)

#define MWREG register
#define CONST_MWREG register

#define WPOPT volatile
#define MW_PRAG_NOINLINE _Pragma("push") _Pragma("dont_inline on")
#define MW_PRAG_OPT_S _Pragma("push") _Pragma("optimize_for_size on")
#define MW_PRAG_NO_WARN_10216 _Pragma("push") _Pragma("warning off(10216)")
#define MW_PRAG_NO_WARN(x) _Pragma("push") _Pragma("warning off(" x ")")
#define MW_PRAG_PACKED _Pragma("push") _Pragma("options align=packed")

#define MW_PRAG_END _Pragma("pop")
#define DECOMP // TODO: Move to build

#define NEVER_INLINE __attribute__((never_inline))
#define FORCE_INLINE __forceinline

#define NULLSUB void

#define RELEASE_STRIPPED_ALL NULLSUB

#define UNUSED_PARAM(x) (void)(x); // EPPC:#pragma unused
#define UNUSED

#define LOCALREF(key, type, data, ref) extern type key;

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

#endif