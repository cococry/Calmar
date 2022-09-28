#pragma once

#include "calmar/core/defines.hpp"

using keyCode = u32;

#define DEFINE_KEY(name, code) name = code

namespace calmar {
    namespace key {
        namespace windows {
            enum : keyCode {
                DEFINE_KEY(Backspace, 0x08),
                DEFINE_KEY(Enter, 0x0D),
                DEFINE_KEY(Tab, 0x09),
                DEFINE_KEY(Shift, 0x10),
                DEFINE_KEY(Control, 0x11),

                DEFINE_KEY(Pause, 0x13),
                DEFINE_KEY(Capital, 0x14),

                DEFINE_KEY(Escape, 0x1B),

                DEFINE_KEY(Convert, 0x1C),
                DEFINE_KEY(Nonconvert, 0x1D),
                DEFINE_KEY(Accept, 0x1E),
                DEFINE_KEY(Modechange, 0x1F),

                DEFINE_KEY(Space, 0x20),
                DEFINE_KEY(Prior, 0x21),
                DEFINE_KEY(Next, 0x22),
                DEFINE_KEY(End, 0x23),
                DEFINE_KEY(Home, 0x24),
                DEFINE_KEY(Left, 0x25),
                DEFINE_KEY(Up, 0x26),
                DEFINE_KEY(Right, 0x27),
                DEFINE_KEY(Down, 0x28),
                DEFINE_KEY(Select, 0x29),
                DEFINE_KEY(Print, 0x2A),
                DEFINE_KEY(Execute, 0x2B),
                DEFINE_KEY(Snapshot, 0x2C),
                DEFINE_KEY(Insert, 0x2D),
                DEFINE_KEY(Delete, 0x2E),
                DEFINE_KEY(Help, 0x2F),

                DEFINE_KEY(A, 0x41),
                DEFINE_KEY(B, 0x42),
                DEFINE_KEY(C, 0x43),
                DEFINE_KEY(D, 0x44),
                DEFINE_KEY(E, 0x45),
                DEFINE_KEY(F, 0x46),
                DEFINE_KEY(G, 0x47),
                DEFINE_KEY(H, 0x48),
                DEFINE_KEY(I, 0x49),
                DEFINE_KEY(J, 0x4A),
                DEFINE_KEY(K, 0x4B),
                DEFINE_KEY(L, 0x4C),
                DEFINE_KEY(M, 0x4D),
                DEFINE_KEY(N, 0x4E),
                DEFINE_KEY(O, 0x4F),
                DEFINE_KEY(P, 0x50),
                DEFINE_KEY(Q, 0x51),
                DEFINE_KEY(R, 0x52),
                DEFINE_KEY(S, 0x53),
                DEFINE_KEY(T, 0x54),
                DEFINE_KEY(U, 0x55),
                DEFINE_KEY(V, 0x56),
                DEFINE_KEY(W, 0x57),
                DEFINE_KEY(X, 0x58),
                DEFINE_KEY(Y, 0x59),
                DEFINE_KEY(Z, 0x5A),

                DEFINE_KEY(Lwin, 0x5B),
                DEFINE_KEY(Rwin, 0x5C),
                DEFINE_KEY(Apps, 0x5D),

                DEFINE_KEY(Sleep, 0x5F),

                DEFINE_KEY(Numpad0, 0x60),
                DEFINE_KEY(Numpad1, 0x61),
                DEFINE_KEY(Numpad2, 0x62),
                DEFINE_KEY(Numpad3, 0x63),
                DEFINE_KEY(Numpad4, 0x64),
                DEFINE_KEY(Numpad5, 0x65),
                DEFINE_KEY(Numpad6, 0x66),
                DEFINE_KEY(Numpad7, 0x67),
                DEFINE_KEY(Numpad8, 0x68),
                DEFINE_KEY(Numpad9, 0x69),
                DEFINE_KEY(Multiply, 0x6A),
                DEFINE_KEY(Add, 0x6B),
                DEFINE_KEY(Seperator, 0x6C),
                DEFINE_KEY(Subtract, 0x6D),
                DEFINE_KEY(Decimal, 0x6E),
                DEFINE_KEY(Divide, 0x6F),
                DEFINE_KEY(F1, 0x70),
                DEFINE_KEY(F2, 0x71),
                DEFINE_KEY(F3, 0x72),
                DEFINE_KEY(F4, 0x73),
                DEFINE_KEY(F5, 0x74),
                DEFINE_KEY(F6, 0x75),
                DEFINE_KEY(F7, 0x76),
                DEFINE_KEY(F8, 0x77),
                DEFINE_KEY(F9, 0x78),
                DEFINE_KEY(F10, 0x79),
                DEFINE_KEY(F11, 0x7A),
                DEFINE_KEY(F12, 0x7B),
                DEFINE_KEY(F13, 0x7C),
                DEFINE_KEY(F14, 0x7D),
                DEFINE_KEY(F15, 0x7E),
                DEFINE_KEY(F16, 0x7F),
                DEFINE_KEY(F17, 0x80),
                DEFINE_KEY(F18, 0x81),
                DEFINE_KEY(F19, 0x82),
                DEFINE_KEY(F20, 0x83),
                DEFINE_KEY(F21, 0x84),
                DEFINE_KEY(F22, 0x85),
                DEFINE_KEY(F23, 0x86),
                DEFINE_KEY(F24, 0x87),

                DEFINE_KEY(Numlock, 0x90),
                DEFINE_KEY(Scroll, 0x91),

                DEFINE_KEY(NumpadEqaul, 0x92),

                DEFINE_KEY(Lshift, 0xA0),
                DEFINE_KEY(Rshift, 0xA1),
                DEFINE_KEY(Lcontrol, 0xA2),
                DEFINE_KEY(Rcontrol, 0xA3),
                DEFINE_KEY(LeftAlt, 0x12),

                DEFINE_KEY(Semicolon, 0xBA),
                DEFINE_KEY(Plus, 0xBB),
                DEFINE_KEY(Comma, 0xBC),
                DEFINE_KEY(Minus, 0xBD),
                DEFINE_KEY(Period, 0xBE),
                DEFINE_KEY(Slash, 0xBF),
                DEFINE_KEY(Grave, 0xC0),

                KEYS_MAX_KEYS
            };
        }  // namespace windows

        namespace glfw {
            enum : keyCode {
                Space = 32,
                Apostrophe = 39, /* ' */
                Comma = 44,      /* , */
                Minus = 45,      /* - */
                Period = 46,     /* . */
                Slash = 47,      /* / */

                D0 = 48, /* 0 */
                D1 = 49, /* 1 */
                D2 = 50, /* 2 */
                D3 = 51, /* 3 */
                D4 = 52, /* 4 */
                D5 = 53, /* 5 */
                D6 = 54, /* 6 */
                D7 = 55, /* 7 */
                D8 = 56, /* 8 */
                D9 = 57, /* 9 */

                Semicolon = 59, /* ; */
                Equal = 61,     /* = */

                A = 65,
                B = 66,
                C = 67,
                D = 68,
                E = 69,
                F = 70,
                G = 71,
                H = 72,
                I = 73,
                J = 74,
                K = 75,
                L = 76,
                M = 77,
                N = 78,
                O = 79,
                P = 80,
                Q = 81,
                R = 82,
                S = 83,
                T = 84,
                U = 85,
                V = 86,
                W = 87,
                X = 88,
                Y = 89,
                Z = 90,

                LeftBracket = 91,  /* [ */
                Backslash = 92,    /* \ */
                RightBracket = 93, /* ] */
                GraveAccent = 96,  /* ` */

                World1 = 161, /* non-US #1 */
                World2 = 162, /* non-US #2 */

                /* Function keys */
                Escape = 256,
                Enter = 257,
                Tab = 258,
                Backspace = 259,
                Insert = 260,
                Delete = 261,
                Right = 262,
                Left = 263,
                Down = 264,
                Up = 265,
                PageUp = 266,
                PageDown = 267,
                Home = 268,
                End = 269,
                CapsLock = 280,
                ScrollLock = 281,
                NumLock = 282,
                PrintScreen = 283,
                Pause = 284,
                F1 = 290,
                F2 = 291,
                F3 = 292,
                F4 = 293,
                F5 = 294,
                F6 = 295,
                F7 = 296,
                F8 = 297,
                F9 = 298,
                F10 = 299,
                F11 = 300,
                F12 = 301,
                F13 = 302,
                F14 = 303,
                F15 = 304,
                F16 = 305,
                F17 = 306,
                F18 = 307,
                F19 = 308,
                F20 = 309,
                F21 = 310,
                F22 = 311,
                F23 = 312,
                F24 = 313,
                F25 = 314,

                /* Keypad */
                KP0 = 320,
                KP1 = 321,
                KP2 = 322,
                KP3 = 323,
                KP4 = 324,
                KP5 = 325,
                KP6 = 326,
                KP7 = 327,
                KP8 = 328,
                KP9 = 329,
                KPDecimal = 330,
                KPDivide = 331,
                KPMultiply = 332,
                KPSubtract = 333,
                KPAdd = 334,
                KPEnter = 335,
                KPEqual = 336,

                LeftShift = 340,
                LeftControl = 341,
                LeftAlt = 342,
                LeftSuper = 343,
                RightShift = 344,
                RightControl = 345,
                RightAlt = 346,
                RightSuper = 347,
                Menu = 348
            };
        }  // namespace glfw
    }      // namespace key
}  // namespace calmar