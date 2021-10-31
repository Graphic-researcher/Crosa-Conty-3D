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


		void Begin();///begin window rendering
		void End();///end window rendering

	private:
		float m_Time = 0.0f;
	private:

	};

}