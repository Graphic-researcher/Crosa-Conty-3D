#pragma once

#include "ccpch.h"

#include "CC3D/Core.h"
#include "CC3D/Events/Event.h"

namespace CC3D {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Crosa Conty 3D",
			unsigned int width = 1280,
			unsigned int height = 720)
			:Title(title), Width(width), Height(height)
		{

		}
	};

	class CC3D_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;// �¼�����ʱ���õĺ���

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}