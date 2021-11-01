#include "Level.h"
#include "Random.h"
#include "iostream"

void Level::Init()
{
	m_Player.LoadAssets();
	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/skyline.png"));
	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/sky-a.png"));
	backgroundTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/sky-b.png"));
	// index 0 是大楼
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/near-buildings-bg.png"));
	// 1，2，3是平台，分别是short，normal，long
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-short.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-normal.png"));
	buildingTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/BackGround/tileset-long.png"));
	//buildsPos.push_back(glm::vec3(0.0f, -24.0f * 2, 0.5f));
	buildsPos.push_back(glm::vec3(0.0f, 12.0f * 2, 0.5f));
	buildsPos.push_back(glm::vec3(0.0f, 0.0f, 0.5f));
	buildsPos.push_back(glm::vec3(0.0f, -12.0f * 2, 0.5f));
	//buildsPos.push_back(glm::vec3(0.0f, 24.0f * 2, 0.5f));

	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f * 10, 0.1f));
	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f * 6, 0.1f));
	backPos.push_back(glm::vec3(0.0f, 24.0f * 2 + 10.3f * 2, 0.1f));

	Random::Init();
	for (int i = 0; i < 12; i++)
	{
		tilesets.push_back(Tileset(static_cast<TilesetType>(rand()%3+1), glm::vec3(float(rand() % 10)-5, 24.0f, 0.7f) - tilesetsOffset));
		tilesetsOffset.y += 6.0;
	}
	// 初始化角色位置
	m_Player.SetPosition(glm::vec2(tilesets.at(tilesets.size() / 2 - 2).tilesetPos.x, tilesets.at(tilesets.size() / 2 - 2).tilesetPos.y + 2));
}

void Level::OnUpdate(CC3D::Timestep ts)
{
	m_Player.Movement();
	// 碰撞检测
	for (Tileset &t : tilesets)
	{
		Collision2D c = t.aabb.OnCollisionEnter(m_Player.GetAABB());
		// 左碰撞
		if (c.normal.x < -0.01)
		{
			auto v = m_Player.GetVelocity();
			v.x = 0;
			m_Player.SetVelocity(v);
		}
		// 右碰撞
		else if (c.normal.x > 0.01)
		{
			auto v = m_Player.GetVelocity();
			v.x = 0;
			m_Player.SetVelocity(v);
		}
		// 上碰撞
		else if(c.normal.y > 0.01)
		{
			// TODO 角色会抖动
			auto ab = m_Player.GetAABB();
			auto pos = m_Player.GetPosition();
			if (ab.miny < t.aabb.maxy)
			{
				pos.y += t.aabb.maxy - ab.miny;
			}
			auto v = m_Player.GetVelocity();
			v.y = 0 > v.y ? 0 : v.y;
			m_Player.SetPosition(pos);
			m_Player.SetVelocity(v);
		}
		// 下碰撞
		else if (c.normal.y < -0.01)
		{
			auto v = m_Player.GetVelocity();
			v.y = - 5;
			m_Player.SetVelocity(v);
		}
	}

	m_Player.OnUpdate(ts);

	// 建筑生成
	if (cameraPos.y - buildsPos.at(buildsPos.size()/2).y >= 12.0f)
	{ 
		glm::vec3 tv = buildsPos.back();
		buildsPos.pop_back();
		tv.y = buildsPos.at(0).y + 24.0f;
		buildsPos.insert(buildsPos.begin(),tv);
	}

	// 平台生成
	if (cameraPos.y - tilesets.at(tilesets.size() / 2).tilesetPos.y >= 1.6f)
	{
		Tileset t(static_cast<TilesetType>(rand() % 3 + 1), glm::vec3(float((rand() % 10) - 5), tilesets.at(0).tilesetPos.y + 6.0f, 0.7f));
		tilesets.pop_back();
		tilesets.insert(tilesets.begin(), t);
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
	
	if (0)
	{
		GameOver();
		return;
	}

	

}

void Level::OnRender()
{
	// Draw background
	CC3D::Renderer2D::DrawQuad(glm::vec3(0.0f, 20.0f, 0.1f), glm::vec2(12.8f * 4, 24.0f * 4), backgroundTexture.at(0));
	for (int i = 0; i < backPos.size(); i++)
	{
		CC3D::Renderer2D::DrawQuad(backPos.at(i), glm::vec2(12.8f * 4, 10.3f * 4), backgroundTexture.at(i % 2 + 1));
	}

	// Draw Building
	for (int i = 0; i < buildsPos.size(); i++)
	{
		CC3D::Renderer2D::DrawQuad(buildsPos.at(i), glm::vec2(7.8f * 2, 12.0f * 2), buildingTexture.at(0));
	}

	// Draw Tileset
	for (int i = 0; i < tilesets.size(); i++)
	{
		CC3D::Renderer2D::DrawQuad(glm::vec3(tilesets.at(i).aabb.maxx, tilesets.at(i).aabb.maxy, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
		CC3D::Renderer2D::DrawQuad(glm::vec3(tilesets.at(i).aabb.maxx, tilesets.at(i).aabb.miny, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
		CC3D::Renderer2D::DrawQuad(glm::vec3(tilesets.at(i).aabb.minx, tilesets.at(i).aabb.maxy, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
		CC3D::Renderer2D::DrawQuad(glm::vec3(tilesets.at(i).aabb.minx, tilesets.at(i).aabb.miny, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));

		CC3D::Renderer2D::DrawQuad(tilesets.at(i).tilesetPos, tilesets.at(i).Scale, buildingTexture.at(tilesets.at(i).type));
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
