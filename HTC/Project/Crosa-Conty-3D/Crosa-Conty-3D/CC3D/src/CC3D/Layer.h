#pragma once

#include "CC3D/Core.h"
#include "CC3D/Events/Event.h"

namespace CC3D {

	class CC3D_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}///push to layer stack
		virtual void OnDetach() {}///remove
		virtual void OnUpdate() {}///call when layer update
		virtual void OnEvent(Event& event) {}///get event send to layer

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}