#pragma once

#include "CC3D.h"


class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;
		
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(CC3D::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CC3D::Event& event) override;
private:
	CC3D::ShaderLibrary m_ShaderLibrary;
	CC3D::Ref<CC3D::Shader> m_Shader;
	CC3D::Ref<CC3D::VertexArray> m_VertexArray;

	CC3D::Ref<CC3D::Shader> m_FlatColorShader;
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;

	CC3D::Ref<CC3D::Texture2D> m_Texture;
	CC3D::Ref<CC3D::Texture2D> m_Tex;

	CC3D::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = glm::vec3(0.2f, 0.3f, 0.8f);
};