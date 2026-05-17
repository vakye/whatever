
// --------------------------------------------------
// NOTE(vak): A collection of definitions, types,
// and structures that are commonly used throughout
// the project.
// --------------------------------------------------

#pragma once

// --------------------------------------------------
// NOTE(vak): Keywords
// --------------------------------------------------

#define local static
#define persist static

#define InvalidCodePath __builtin_trap()
#define InvalidDefaultCase default: { InvalidCodePath; } break

// --------------------------------------------------
// NOTE(vak): Macros
// --------------------------------------------------

#define CTAssert(Expression) _Static_assert(Expression, "Compile-time assertion failed")

#define Assert(Expression) if (!(Expression)) InvalidCodePath

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

#define OffsetOf(Structure, Member) ((usize)(&(((Structure*)0)->Member)))

#define Minimum(A, B) ((A) < (B) ? (A) : (B))
#define Maximum(A, B) ((A) < (B) ? (A) : (B))

#define Clamp(Min, Value, Max) Maximum(Min, Minimum(Max, Value))

// --------------------------------------------------
// NOTE(vak): Types
// --------------------------------------------------

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef s64 ssize;
typedef u64 usize;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef u32 b32;

// --------------------------------------------------
// NOTE(vak): Type size checks
// --------------------------------------------------

CTAssert(sizeof(s8)  == 1);
CTAssert(sizeof(s16) == 2);
CTAssert(sizeof(s32) == 4);
CTAssert(sizeof(s64) == 8);

CTAssert(sizeof(u8)  == 1);
CTAssert(sizeof(u16) == 2);
CTAssert(sizeof(u32) == 4);
CTAssert(sizeof(u64) == 8);

CTAssert(sizeof(ssize) == sizeof(void*));
CTAssert(sizeof(usize) == sizeof(void*));

CTAssert(sizeof(f32) == 4);
CTAssert(sizeof(f64) == 8);

// --------------------------------------------------
// NOTE(vak): Constants
// --------------------------------------------------

#define true (1)
#define false (0)

#define S8Min  ((s8 )(-128))
#define S16Min ((s16)(-32768))
#define S32Min ((s32)(-2147483648))
#define S64Min ((s64)(-9223372036854775808))

#define S8Max  ((s8 )(+127))
#define S16Max ((s16)(+32767))
#define S32Max ((s32)(+2147483647))
#define S64Max ((s64)(+9223372036854775807))

#define U8Max  ((u8 )(+255))
#define U16Max ((u16)(+65535))
#define U32Max ((u32)(+4294967295))
#define U64Max ((u32)(+18446744073709551615))

// --------------------------------------------------
// NOTE(vak): UTF-8 string
// --------------------------------------------------

typedef struct
{
    char* Data;
    usize Size;
} string;

#define Str(Literal)        (string){Literal, sizeof(Literal) - 1}
#define StrData(Data, Size) (string){Data, Size}
