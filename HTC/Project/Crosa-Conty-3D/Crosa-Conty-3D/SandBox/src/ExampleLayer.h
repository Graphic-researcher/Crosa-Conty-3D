#pragma once
#include "CC3D.h"
class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer();


	void OnUpdate(CC3D::Timestep ts) override;

	virtual void OnImGuiRender() override;


	void OnEvent(CC3D::Event& e) override;


private:
	///Render	
	CC3D::ShaderLibrary m_ShaderLibrary;
	CC3D::Ref<CC3D::Shader> m_Shader;
	CC3D::Ref<CC3D::Shader> m_FlatColorShader, m_UVShader;
	CC3D::Ref<CC3D::VertexArray> m_VertexArray;
	CC3D::Ref<CC3D::VertexArray> m_SquareVA;
	CC3D::Ref<CC3D::Texture2D> m_Texture, m_ChernoLogoTexture;
	CC3D::OrthographicCameraController m_CameraController;

	///Render Data
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };


	///Transform Test
	glm::vec3 m_SquareTransform;
	float m_SquareMoveSpeed = 7.0f;
};


