#ifndef CALMAR_DEFINES_INCLUDED
#define CALMAR_DEFINES_INCLUDED

#ifdef CALMAR_EXPORT
#define CALMAR_API __declspec(dllexport)
#elif defined(CALMAR_IMPORT)
#define CALMAR_API __declspec(dllimport)
#else
#define CALMAR_API
#endif

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using render_id = unsigned int;

#endif