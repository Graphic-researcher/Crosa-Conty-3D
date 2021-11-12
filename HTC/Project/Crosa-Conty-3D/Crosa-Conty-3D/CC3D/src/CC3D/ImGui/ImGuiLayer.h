#pragma once
///layer
#include "CC3D/Core/Layer.h"
/// event
#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Events/MouseEvent.h"

namespace CC3D {

	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		///virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;


		void Begin();///begin window rendering
		void End();///end window rendering
		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		float m_Time = 0.0f;
	private:
		bool m_BlockEvents = true;
	};

}