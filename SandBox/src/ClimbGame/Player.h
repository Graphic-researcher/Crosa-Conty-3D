#pragma once

#include <CC3D.h>

#include "Random.h"

class Player
{
public:
	Player();

	void LoadAssets();

	void OnUpdate(CC3D::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	void Reset();

	const glm::vec2& GetPosition() const { return m_Position; }

	uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }
private:
	glm::vec2 m_Position = glm::vec2{ -10.0f, 0.0f };
	glm::vec2 m_Velocity = glm::vec2{ 5.0f, 0.0f };

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;

	std::vector<CC3D::Ref<CC3D::Texture2D>> idelTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> jumpTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> walkTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> runTexture;
};