#ifndef CALMAR_DEFINES_INCLUDED
#define CALMAR_DEFINES_INCLUDED

#ifdef CALMAR_EXPORT
#define __declspec(dllexport)
#elif defined(CALMAR_IMPORT)
#define __declspec(dllimport)
#else
#define CALMAR_API
#endif

#define arraySize(arr) sizeof((arr)) / sizeof((arr[0]))

#define RGBA_COLOR(r, g, b, a) r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f

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

namespace calmar {
    static const char* engineVersion = "0.1";
}  // namespace calmar

#endif