#pragma once

#include "CC3D.h"
#include "ParticleSystem.h"

class ParticleSystemBox2D : public CC3D::Layer
{
public:
	ParticleSystemBox2D();
	virtual ~ParticleSystemBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(CC3D::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CC3D::Event& e) override;

private:
	CC3D::OrthographicCameraController m_CameraController;
	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
};