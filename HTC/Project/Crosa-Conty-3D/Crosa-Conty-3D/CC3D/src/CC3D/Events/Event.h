#pragma once
#include "cc3d_pch.h"
#include "CC3D/Core/Core.h"



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
		KeyPressed, KeyReleased, KeyTyped,										     //9 10
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
		bool Handled = false;
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

	};

	class EventDispatcher
	{


	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				///call that func -> function with that event
				m_Event.Handled = func(static_cast<T&>(m_Event));
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

