#pragma once

#include "CC3D/Core/Layer.h"

#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Events/MouseEvent.h"

namespace CC3D
{
	class ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;// = default ��ʾ����c++Ĭ�ϵ�����(����)����

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;
	};
}