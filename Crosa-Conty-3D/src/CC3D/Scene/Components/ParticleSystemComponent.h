#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Random.h"

#include "CC3D.h"


namespace CC3D {
	struct ParticleProps2D
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};
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

	// The Particle System 
	struct ParticleSystemComponent
	{
		ParticleProps2D particleProps;
		std::vector<Particle2D> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		CC3D::Ref<CC3D::Shader> m_ParticleShader;
		GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;

		ParticleSystemComponent(uint32_t MaxParticles = 10000)
		{
			m_ParticlePool.resize(MaxParticles);

			particleProps.ColorBegin = glm::vec4{ 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
			particleProps.ColorEnd = glm::vec4{ 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
			//m_Particle.ColorBegin = glm::vec4{ 1.0f, 0.4f, 1.0f, 1.0f };
			//m_Particle.ColorEnd = glm::vec4{ 0.0f, 0.4f, 0.0f, 1.0f };
			particleProps.SizeBegin = 0.5f, particleProps.SizeVariation = 0.3f, particleProps.SizeEnd = 0.0f;
			particleProps.LifeTime = 10.0f;
			particleProps.Velocity = glm::vec2{ 0.0f, 0.0f };
			particleProps.VelocityVariation = glm::vec2{ 3.0f, 1.0f };
			particleProps.Position = glm::vec2{ 0.0f, 0.0f };
		}

		void OnUpdate(CC3D::Timestep ts)
		{

			for (auto& particle : m_ParticlePool)
			{
				if (!particle.Active)
					continue;

				if (particle.LifeRemaining <= 0.0f)
				{
					particle.Active = false;
					continue;
				}

				particle.LifeRemaining -= ts;
				particle.Position += particle.Velocity * (float)ts;
				particle.Rotation += 0.01f * ts;
			}
		}
		void OnRender()
		{
			for (auto& particle : m_ParticlePool)
			{
				if (!particle.Active)
					continue;

				// Fade away particles
				float life = particle.LifeRemaining / particle.LifeTime;
				glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
				//color.a = color.a * life;

				float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

				CC3D::Renderer2D::DrawRotatedQuad(particle.Position, glm::vec2(size, size), particle.Rotation, color);
			}
		}

		void Emit()
		{
			Particle2D& particle = m_ParticlePool[m_PoolIndex];
			particle.Active = true;
			particle.Position = particleProps.Position;
			particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

			// Velocity
			particle.Velocity = particleProps.Velocity;
			particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
			particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

			// Color
			particle.ColorBegin = particleProps.ColorBegin;
			particle.ColorEnd = particleProps.ColorEnd;

			particle.LifeTime = particleProps.LifeTime;
			particle.LifeRemaining = particleProps.LifeTime;
			particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
			particle.SizeEnd = particleProps.SizeEnd;

			m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
		}

		ParticleSystemComponent(const ParticleSystemComponent&) = default;
	};


	

	
}
