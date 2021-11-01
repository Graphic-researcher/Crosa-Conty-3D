#pragma once
#ifdef CC3D_PLATFORM_WINDOWS

namespace CC3D
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define CC3D_MOUSE_BUTTON_0      ::CC3D::Mouse::Button0
#define CC3D_MOUSE_BUTTON_1      ::CC3D::Mouse::Button1
#define CC3D_MOUSE_BUTTON_2      ::CC3D::Mouse::Button2
#define CC3D_MOUSE_BUTTON_3      ::CC3D::Mouse::Button3
#define CC3D_MOUSE_BUTTON_4      ::CC3D::Mouse::Button4
#define CC3D_MOUSE_BUTTON_5      ::CC3D::Mouse::Button5
#define CC3D_MOUSE_BUTTON_6      ::CC3D::Mouse::Button6
#define CC3D_MOUSE_BUTTON_7      ::CC3D::Mouse::Button7
#define CC3D_MOUSE_BUTTON_LAST   ::CC3D::Mouse::ButtonLast
#define CC3D_MOUSE_BUTTON_LEFT   ::CC3D::Mouse::ButtonLeft
#define CC3D_MOUSE_BUTTON_RIGHT  ::CC3D::Mouse::ButtonRight
#define CC3D_MOUSE_BUTTON_MIDDLE ::CC3D::Mouse::ButtonMiddle


#endif