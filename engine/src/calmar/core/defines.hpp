#ifndef CALMAR_DEFINES_INCLUDED
#define CALMAR_DEFINES_INCLUDED

#ifdef CALMAR_EXPORT
#define CALMAR_API __declspec(dllexport)
#elif defined(CALMAR_IMPORT)
#define CALMAR_API __declspec(dllimport)
#else
#define CALMAR_API
#endif

// Simple alias for an unsinged char
using u8 = unsigned char;
// Simple alias for an unsinged short
using u16 = unsigned short;
// Simple alias for an unsinged int
using u32 = unsigned int;
// Simple alias for an unsinged long long
using u64 = unsigned long long;

// Simple alias for an singed char
using i8 = signed char;
// Simple alias for an singed short
using i16 = signed short;
// Simple alias for an singed int
using i32 = signed int;
// Simple alias for an  singed long long
using i64 = signed long long;

// Simple alias for an unsinged int
using render_id = unsigned int;

#endif