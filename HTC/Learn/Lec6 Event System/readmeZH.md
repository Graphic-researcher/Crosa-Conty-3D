# Lec6 Event System

课程地址: [here](https://www.youtube.com/watch?v=xnopUoZbMEk&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=10)

代码版本: [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/befd36ece846fdad9b3617bb43476e0296edfc28/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## 类视图

![Class.cd](https://i.loli.net/2021/10/10/H5lIWOqnyVzrwNo.png)

## 事件系统

Event.h in CC3D

```c++
#pragma once

#include "CC3D/Core.h"

#include <string>
#include <functional>

namespace CC3D {

	// Events in CC3D are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,//1 2 3 4 5
		AppTick, AppUpdate, AppRender,									     //6 7 8
		KeyPressed, KeyReleased,										     //9 10
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled	 //11 12 13 14
	};

	///Marcos in Core.h : #define BIT(x) (1 << x)
	///for event mutiple category
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication    = BIT(0),//all the Application event	1
		EventCategoryInput          = BIT(1),//all the Input	   event	2
		EventCategoryKeyboard       = BIT(2),//all the Keyboard	   event	4
		EventCategoryMouse          = BIT(3),//all the Mouse	   event	8
		EventCategoryMouseButton    = BIT(4) //all the MouseButton event	16
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }
	///Base Event Class
	class CC3D_API Event
	{
		friend class EventDispatcher;
	public:
		///to be inherited
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		

		inline bool IsInCategory(EventCategory category)
		{
			///ruturn 0 : not in any category at all , Otherwise it is or not in that category
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

		///see more details about std::function:
		//https://en.cppreference.com/w/cpp/utility/functional/function

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		///T:any event (such as window resizing)
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				///call that func -> function with that event
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
```

ApplicationEvent.h in CC3D

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace CC3D {

	class CC3D_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class CC3D_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class CC3D_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class CC3D_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class CC3D_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
```

KeyEvent.h in CC3D

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace CC3D { 

	class CC3D_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }


		///The | (bitwise OR) in C or C++ takes two numbers as operands and does OR on every bit of two numbers. \
		The result of OR is 1 if any of the two bits is 1. \
		refer: https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		int m_KeyCode;///keyboard value
	};

	class CC3D_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
		///same as follow:
		///static EventType GetStaticType() { return EventType::KeyPressed; }
		///virtual EventType GetEventType() const override { return GetStaticType(); }
		///virtual const char* GetName() const override { return "KeyPressed"; }
	private:
		int m_RepeatCount;///allow us to press a key and it can operate repeatly until we release it
	};

	class CC3D_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}
```

MouseEvent.h in CC3D

```c++
#pragma once

#include "Event.h"

#include <sstream>

namespace CC3D {

	class CC3D_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class CC3D_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_XOffset, m_YOffset;
	};

	class CC3D_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class CC3D_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class CC3D_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

}
```

## 改变其他文件

Core.h in CC3D  : 添加宏指令

```c++
//for Event.h
#define BIT(x) (1 << x)
```

Log.h in CC3D : 添加头文件

```
#include "spdlog/fmt/ostr.h"
```

Application.cpp in CC3D : 增加代码测试事件系统

```c++
void Application::Run()
{
    WindowResizeEvent e(1280, 720);
    WindowCloseEvent e1;
    KeyPressedEvent e2(1, 3);
    printf("Window Width: %d\n", e.GetWidth());
    if (e.IsInCategory(EventCategoryApplication))
    {
        /// #include "spdlog/fmt/ostr.h"
        /// 这个头文件可以自动提取类对象里的ToString方法
        /// 如果不包含 "spdlog/fmt/ostr.h"也行
        /// 那你就自己写上ToString方法，例如：
        /// CC3D_TRACE(e.ToString())
        
        //CC3D_TRACE(e.ToString());
        CC3D_TRACE(e);
        CC3D_TRACE(e1);
        CC3D_TRACE(e2);
    }
    if (e.IsInCategory(EventCategoryInput))
    {
        CC3D_TRACE(e);
        CC3D_TRACE(e1);
        CC3D_TRACE(e2);
    }
    if (e2.IsInCategory(EventCategoryKeyboard))
    {
        CC3D_TRACE(e);
        CC3D_TRACE(e1);
        CC3D_TRACE(e2);
    }

    while (true)
    {

    }
}
```

![image-20211010191058710](https://i.loli.net/2021/10/10/bGiY5mW1F97fnw4.png)