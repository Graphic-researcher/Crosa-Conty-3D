#pragma once

#include "CC3D/Core/Base.h"
#include "CC3D/Events/Event.h"
#include "CC3D/Core/Timestep.h"

namespace CC3D {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event){}

		virtual void OnImGuiRender(){}

		const std::string& GetName()const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}

