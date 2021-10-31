#include "Level.h"

void Level::Init()
{
	/*std::vector<CC3D::Ref<CC3D::Texture2D>> environmentTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> backgroundTexture;*/

	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/skyline.png"));
	// index 0 是大楼
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/near-buildings-bg.png"));
	// 1，2，3是平台，分别是short，normal，long
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-short.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-normal.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-long.png"));
}

void Level::OnUpdate(CC3D::Timestep ts)
{
	m_Player.OnUpdate(ts);


	
	if (0)
	{
		GameOver();
		return;
	}

	

}

void Level::OnRender()
{
	CC3D::Renderer2D::DrawQuad(glm::vec3(0.0f, 20.0f, 0.5f), glm::vec2(12.8f*4, 24.0f*4) , backgroundTexture.at(0));
	CC3D::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(7.8f*2, 12.0f*2), buildingTexture.at(0));

	m_Player.OnRender();
}

void Level::OnImGuiRender()
{
}

void Level::Reset()
{
}

void Level::CreatePillar(int index, float offset)
{
}

bool Level::CollisionTest()
{
	return false;
}

void Level::GameOver()
{
}
