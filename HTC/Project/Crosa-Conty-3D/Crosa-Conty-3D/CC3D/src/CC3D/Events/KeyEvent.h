#pragma once

#include "CC3D/Events/Event.h"
#include "CC3D/Core/Input.h"

namespace CC3D { 

	class KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const { return m_KeyCode; }


		///The | (bitwise OR) in C or C++ takes two numbers as operands and does OR on every bit of two numbers. \
		///The result of OR is 1 if any of the two bits is 1. \
		///refer: https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode) {}
	
		KeyCode m_KeyCode;///keyboard value
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount)
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
		///						virtual EventType GetEventType() const override { return GetStaticType(); }
		///						virtual const char* GetName() const override { return "KeyPressed"; }
	private:
		int m_RepeatCount;///allow us to press a key and it can operate repeatly until we release it
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};


	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};

}