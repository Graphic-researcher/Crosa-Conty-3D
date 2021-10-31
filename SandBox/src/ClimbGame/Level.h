#pragma once

#include "Player.h"

struct Pillar
{
	glm::vec3 TopPosition = glm::vec3{ 0.0f, 10.0f, 0.0f };
	glm::vec2 TopScale = glm::vec2{ 15.0f, 20.0f };

	glm::vec3 BottomPosition = glm::vec3{ 10.0f, 10.0f, 0.0f };
	glm::vec2 BottomScale = glm::vec2{ 15.0f, 20.0f };
};

enum tilesetType
{
	shortTileset = 1, normalTileset, longTileset
};
struct Tileset
{
	tilesetType type;
	glm::vec3 tilesetPos;
	glm::vec2 Scale;
	Tileset(tilesetType t, glm::vec3 pos)
	{
		float factor = 0.8f;
		tilesetPos = pos;
		tilesetPos.y -= 0.7;
		type = t;
		switch (type)
		{
		case tilesetType::shortTileset:
			Scale = glm::vec2(5.0f, 1.6f) * factor;
		case tilesetType::normalTileset:
			Scale = glm::vec2(7.0f, 1.6f) * factor;
		case tilesetType::longTileset:
			Scale = glm::vec2(9.0f, 1.6f) * factor;
		}
	}
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

	void ChangeCameraPosition(glm::vec3 p) { cameraPos = p; }
private:
	void CreatePillar(int index, float offset);
	bool CollisionTest();

	void GameOver();
private:
	Player m_Player;
	glm::vec3 cameraPos;

	std::vector<glm::vec3> buildsPos;
	std::vector<glm::vec3> backPos;

	std::vector<Tileset> tilesets;
	glm::vec3 tilesetsOffset = glm::vec3(0.0f, 0.0f, 0.0f);


	glm::vec3 currentBuildPos = glm::vec3(0.0f, 0.0f, 1.0f);
	

	bool m_GameOver = false;

	float m_PillarTarget = 30.0f;
	int m_PillarIndex = 0;
	glm::vec3 m_PillarHSV = glm::vec3{ 0.0f, 0.8f, 0.8f };

	std::vector<Pillar> m_Pillars;

	std::vector<CC3D::Ref<CC3D::Texture2D>> buildingTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> backgroundTexture;
};