#pragma once

#include "CC3D/Core/Layer.h"

#include "CC3D/Events/ApplicationEvent.h"
#include "CC3D/Events/KeyEvent.h"
#include "CC3D/Events/MouseEvent.h"

namespace CC3D
{
	class CC3D_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;// = default ��ʾ����c++Ĭ�ϵ�����(����)����

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}