#pragma once
#include <sstream>

#include "CC3D/Core/Base.h"
#include "CC3D/Events/Event.h"

namespace CC3D {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Crosa Conty 3D",
			uint32_t width = 1280,
			uint32_t height = 720)
			:Title(title), Width(width), Height(height)
		{

		}
	};

	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;// 事件发生时调用的函数

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}