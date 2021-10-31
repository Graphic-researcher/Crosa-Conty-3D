#pragma once

namespace CC3D
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

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
		Equal = 61, /* = */

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
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
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
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define CC3D_KEY_SPACE           ::CC3D::Key::Space
#define CC3D_KEY_APOSTROPHE      ::CC3D::Key::Apostrophe    /* ' */
#define CC3D_KEY_COMMA           ::CC3D::Key::Comma         /* , */
#define CC3D_KEY_MINUS           ::CC3D::Key::Minus         /* - */
#define CC3D_KEY_PERIOD          ::CC3D::Key::Period        /* . */
#define CC3D_KEY_SLASH           ::CC3D::Key::Slash         /* / */
#define CC3D_KEY_0               ::CC3D::Key::D0
#define CC3D_KEY_1               ::CC3D::Key::D1
#define CC3D_KEY_2               ::CC3D::Key::D2
#define CC3D_KEY_3               ::CC3D::Key::D3
#define CC3D_KEY_4               ::CC3D::Key::D4
#define CC3D_KEY_5               ::CC3D::Key::D5
#define CC3D_KEY_6               ::CC3D::Key::D6
#define CC3D_KEY_7               ::CC3D::Key::D7
#define CC3D_KEY_8               ::CC3D::Key::D8
#define CC3D_KEY_9               ::CC3D::Key::D9
#define CC3D_KEY_SEMICOLON       ::CC3D::Key::Semicolon     /* ; */
#define CC3D_KEY_EQUAL           ::CC3D::Key::Equal         /* = */
#define CC3D_KEY_A               ::CC3D::Key::A
#define CC3D_KEY_B               ::CC3D::Key::B
#define CC3D_KEY_C               ::CC3D::Key::C
#define CC3D_KEY_D               ::CC3D::Key::D
#define CC3D_KEY_E               ::CC3D::Key::E
#define CC3D_KEY_F               ::CC3D::Key::F
#define CC3D_KEY_G               ::CC3D::Key::G
#define CC3D_KEY_H               ::CC3D::Key::H
#define CC3D_KEY_I               ::CC3D::Key::I
#define CC3D_KEY_J               ::CC3D::Key::J
#define CC3D_KEY_K               ::CC3D::Key::K
#define CC3D_KEY_L               ::CC3D::Key::L
#define CC3D_KEY_M               ::CC3D::Key::M
#define CC3D_KEY_N               ::CC3D::Key::N
#define CC3D_KEY_O               ::CC3D::Key::O
#define CC3D_KEY_P               ::CC3D::Key::P
#define CC3D_KEY_Q               ::CC3D::Key::Q
#define CC3D_KEY_R               ::CC3D::Key::R
#define CC3D_KEY_S               ::CC3D::Key::S
#define CC3D_KEY_T               ::CC3D::Key::T
#define CC3D_KEY_U               ::CC3D::Key::U
#define CC3D_KEY_V               ::CC3D::Key::V
#define CC3D_KEY_W               ::CC3D::Key::W
#define CC3D_KEY_X               ::CC3D::Key::X
#define CC3D_KEY_Y               ::CC3D::Key::Y
#define CC3D_KEY_Z               ::CC3D::Key::Z
#define CC3D_KEY_LEFT_BRACKET    ::CC3D::Key::LeftBracket   /* [ */
#define CC3D_KEY_BACKSLASH       ::CC3D::Key::Backslash     /* \ */
#define CC3D_KEY_RIGHT_BRACKET   ::CC3D::Key::RightBracket  /* ] */
#define CC3D_KEY_GRAVE_ACCENT    ::CC3D::Key::GraveAccent   /* ` */
#define CC3D_KEY_WORLD_1         ::CC3D::Key::World1        /* non-US #1 */
#define CC3D_KEY_WORLD_2         ::CC3D::Key::World2        /* non-US #2 */

/* Function keys */
#define CC3D_KEY_ESCAPE          ::CC3D::Key::Escape
#define CC3D_KEY_ENTER           ::CC3D::Key::Enter
#define CC3D_KEY_TAB             ::CC3D::Key::Tab
#define CC3D_KEY_BACKSPACE       ::CC3D::Key::Backspace
#define CC3D_KEY_INSERT          ::CC3D::Key::Insert
#define CC3D_KEY_DELETE          ::CC3D::Key::Delete
#define CC3D_KEY_RIGHT           ::CC3D::Key::Right
#define CC3D_KEY_LEFT            ::CC3D::Key::Left
#define CC3D_KEY_DOWN            ::CC3D::Key::Down
#define CC3D_KEY_UP              ::CC3D::Key::Up
#define CC3D_KEY_PAGE_UP         ::CC3D::Key::PageUp
#define CC3D_KEY_PAGE_DOWN       ::CC3D::Key::PageDown
#define CC3D_KEY_HOME            ::CC3D::Key::Home
#define CC3D_KEY_END             ::CC3D::Key::End
#define CC3D_KEY_CAPS_LOCK       ::CC3D::Key::CapsLock
#define CC3D_KEY_SCROLL_LOCK     ::CC3D::Key::ScrollLock
#define CC3D_KEY_NUM_LOCK        ::CC3D::Key::NumLock
#define CC3D_KEY_PRINT_SCREEN    ::CC3D::Key::PrintScreen
#define CC3D_KEY_PAUSE           ::CC3D::Key::Pause
#define CC3D_KEY_F1              ::CC3D::Key::F1
#define CC3D_KEY_F2              ::CC3D::Key::F2
#define CC3D_KEY_F3              ::CC3D::Key::F3
#define CC3D_KEY_F4              ::CC3D::Key::F4
#define CC3D_KEY_F5              ::CC3D::Key::F5
#define CC3D_KEY_F6              ::CC3D::Key::F6
#define CC3D_KEY_F7              ::CC3D::Key::F7
#define CC3D_KEY_F8              ::CC3D::Key::F8
#define CC3D_KEY_F9              ::CC3D::Key::F9
#define CC3D_KEY_F10             ::CC3D::Key::F10
#define CC3D_KEY_F11             ::CC3D::Key::F11
#define CC3D_KEY_F12             ::CC3D::Key::F12
#define CC3D_KEY_F13             ::CC3D::Key::F13
#define CC3D_KEY_F14             ::CC3D::Key::F14
#define CC3D_KEY_F15             ::CC3D::Key::F15
#define CC3D_KEY_F16             ::CC3D::Key::F16
#define CC3D_KEY_F17             ::CC3D::Key::F17
#define CC3D_KEY_F18             ::CC3D::Key::F18
#define CC3D_KEY_F19             ::CC3D::Key::F19
#define CC3D_KEY_F20             ::CC3D::Key::F20
#define CC3D_KEY_F21             ::CC3D::Key::F21
#define CC3D_KEY_F22             ::CC3D::Key::F22
#define CC3D_KEY_F23             ::CC3D::Key::F23
#define CC3D_KEY_F24             ::CC3D::Key::F24
#define CC3D_KEY_F25             ::CC3D::Key::F25

/* Keypad */
#define CC3D_KEY_KP_0            ::CC3D::Key::KP0
#define CC3D_KEY_KP_1            ::CC3D::Key::KP1
#define CC3D_KEY_KP_2            ::CC3D::Key::KP2
#define CC3D_KEY_KP_3            ::CC3D::Key::KP3
#define CC3D_KEY_KP_4            ::CC3D::Key::KP4
#define CC3D_KEY_KP_5            ::CC3D::Key::KP5
#define CC3D_KEY_KP_6            ::CC3D::Key::KP6
#define CC3D_KEY_KP_7            ::CC3D::Key::KP7
#define CC3D_KEY_KP_8            ::CC3D::Key::KP8
#define CC3D_KEY_KP_9            ::CC3D::Key::KP9
#define CC3D_KEY_KP_DECIMAL      ::CC3D::Key::KPDecimal
#define CC3D_KEY_KP_DIVIDE       ::CC3D::Key::KPDivide
#define CC3D_KEY_KP_MULTIPLY     ::CC3D::Key::KPMultiply
#define CC3D_KEY_KP_SUBTRACT     ::CC3D::Key::KPSubtract
#define CC3D_KEY_KP_ADD          ::CC3D::Key::KPAdd
#define CC3D_KEY_KP_ENTER        ::CC3D::Key::KPEnter
#define CC3D_KEY_KP_EQUAL        ::CC3D::Key::KPEqual

#define CC3D_KEY_LEFT_SHIFT      ::CC3D::Key::LeftShift
#define CC3D_KEY_LEFT_CONTROL    ::CC3D::Key::LeftControl
#define CC3D_KEY_LEFT_ALT        ::CC3D::Key::LeftAlt
#define CC3D_KEY_LEFT_SUPER      ::CC3D::Key::LeftSuper
#define CC3D_KEY_RIGHT_SHIFT     ::CC3D::Key::RightShift
#define CC3D_KEY_RIGHT_CONTROL   ::CC3D::Key::RightControl
#define CC3D_KEY_RIGHT_ALT       ::CC3D::Key::RightAlt
#define CC3D_KEY_RIGHT_SUPER     ::CC3D::Key::RightSuper
#define CC3D_KEY_MENU            ::CC3D::Key::Menu
