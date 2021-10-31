#include "Level.h"

void Level::Init()
{
	/*std::vector<CC3D::Ref<CC3D::Texture2D>> environmentTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> backgroundTexture;*/

	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/skyline.png"));
	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/sky-a.png"));
	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/sky-b.png"));
	// index 0 是大楼
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/near-buildings-bg.png"));
	// 1，2，3是平台，分别是short，normal，long
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-short.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-normal.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-long.png"));
	//buildsPos.push_back(glm::vec3(0.0f, -24.0f * 2, 1.0f));
	buildsPos.push_back(glm::vec3(0.0f, -12.0f * 2, 1.0f));
	buildsPos.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	buildsPos.push_back(glm::vec3(0.0f, 12.0f * 2, 1.0f));
	//buildsPos.push_back(glm::vec3(0.0f, 24.0f * 2, 1.0f));

	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f*2, 0.5f));
	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f * 6, 0.5f));
	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f * 10, 0.5f));
}

void Level::OnUpdate(CC3D::Timestep ts)
{
	m_Player.OnUpdate(ts);

	// 建筑生成
	if (std::abs(cameraPos.y - buildsPos.at(buildsPos.size()/2).y) >= 12.0f)
	{ 
		glm::vec3 tv = buildsPos.back();
		buildsPos.pop_back();
		tv.y = buildsPos.at(0).y + 24.0f;
		buildsPos.insert(buildsPos.begin(),tv);
	}

	// 背景生成
	if (cameraPos.y > 24.0f * 4)
	{
		if (std::abs(cameraPos.y - backPos.at(backPos.size() / 2).y) >= 20.6)
		{
			glm::vec3 tv = backPos.back();
			backPos.pop_back();
			tv.y = backPos.at(0).y + 10.3f * 4;
			backPos.insert(backPos.begin(), tv);
		}
	}
	// 平台生成
	
	
	if (0)
	{
		GameOver();
		return;
	}

	

}

void Level::OnRender()
{
	// Draw background
	
	CC3D::Renderer2D::DrawQuad(glm::vec3(0.0f, 20.0f, 0.5f), glm::vec2(12.8f*4, 24.0f*4) , backgroundTexture.at(0));

	for (int i = 0; i < backPos.size(); i++)
	{
		CC3D::Renderer2D::DrawQuad(backPos.at(i), glm::vec2(12.8f * 4, 10.3f * 4), backgroundTexture.at(i%2+1));
	}

	// Draw Building
	for (int i = 0; i < buildsPos.size(); i++)
	{
		CC3D::Renderer2D::DrawQuad(buildsPos.at(i), glm::vec2(7.8f * 2, 12.0f * 2), buildingTexture.at(0));
	}

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
