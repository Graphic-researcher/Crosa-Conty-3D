#pragma once

#include "CC3D.h"

struct ParticleProps2D
{
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem2D
{
public:
	ParticleSystem2D(uint32_t MaxParticles = 10000);

	void OnUpdate(CC3D::Timestep ts);
	void OnRender(CC3D::OrthographicCamera& camera);

	void Emit(const ParticleProps2D& particleProps);
private:
	struct Particle2D
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle2D> m_ParticlePool;
	uint32_t m_PoolIndex = 999;

	CC3D::Ref<CC3D::Shader> m_ParticleShader;
	GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
};