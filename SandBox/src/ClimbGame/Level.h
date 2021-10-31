#pragma once

#include "Player.h"

struct Pillar
{
	glm::vec3 TopPosition = glm::vec3{ 0.0f, 10.0f, 0.0f };
	glm::vec2 TopScale = glm::vec2{ 15.0f, 20.0f };

	glm::vec3 BottomPosition = glm::vec3{ 10.0f, 10.0f, 0.0f };
	glm::vec2 BottomScale = glm::vec2{ 15.0f, 20.0f };
};

class Level
{
public:
	void Init();

	void OnUpdate(CC3D::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	bool IsGameOver() const { return m_GameOver; }
	void Reset();

	Player& GetPlayer() { return m_Player; }
private:
	void CreatePillar(int index, float offset);
	bool CollisionTest();

	void GameOver();
private:
	Player m_Player;

	bool m_GameOver = false;

	float m_PillarTarget = 30.0f;
	int m_PillarIndex = 0;
	glm::vec3 m_PillarHSV = glm::vec3{ 0.0f, 0.8f, 0.8f };

	std::vector<Pillar> m_Pillars;

	CC3D::Ref<CC3D::Texture2D> m_TriangleTexture;
};