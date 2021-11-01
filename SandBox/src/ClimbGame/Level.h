#pragma once

#include "Player.h"

struct Pillar
{
	glm::vec3 TopPosition = glm::vec3{ 0.0f, 10.0f, 0.0f };
	glm::vec2 TopScale = glm::vec2{ 15.0f, 20.0f };

	glm::vec3 BottomPosition = glm::vec3{ 10.0f, 10.0f, 0.0f };
	glm::vec2 BottomScale = glm::vec2{ 15.0f, 20.0f };
};

enum TilesetType
{
	shortTileset = 1, normalTileset, longTileset
};
struct Tileset
{
	TilesetType type;
	glm::vec3 tilesetPos;
	glm::vec2 Scale;
	AABB aabb;
	Tileset(TilesetType t, glm::vec3 pos)
	{
		float factor = 0.8f;
		tilesetPos = pos;
		tilesetPos.y -= 0.7;
		type = t;
		switch (type)
		{
		case TilesetType::shortTileset:
			Scale = glm::vec2(5.0f, 1.6f) * factor;
			aabb.minx = pos.x - 0.5 * Scale.x;
			aabb.maxx = pos.x + 0.5 * Scale.x;
			aabb.miny = pos.y - 0.5 * Scale.y;
			aabb.maxy = pos.y + 0.5 * Scale.y;
			break;
		case TilesetType::normalTileset:
			Scale = glm::vec2(7.0f, 1.6f) * factor;
			aabb.minx = pos.x - 0.5 * Scale.x;
			aabb.maxx = pos.x + 0.5 * Scale.x;
			aabb.miny = pos.y - 0.5 * Scale.y;
			aabb.maxy = pos.y + 0.5 * Scale.y;
			break;
		case TilesetType::longTileset:
			Scale = glm::vec2(9.0f, 1.6f) * factor;
			aabb.minx = pos.x - 0.5 * Scale.x;
			aabb.maxx = pos.x + 0.5 * Scale.x;
			aabb.miny = pos.y - 0.5 * Scale.y;
			aabb.maxy = pos.y + 0.5 * Scale.y;
			break;
		default:
			break;
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