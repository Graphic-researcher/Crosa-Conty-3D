#pragma once

#include <CC3D.h>

#include "Random.h"

struct Collision2D
{
	glm::vec2 normal = glm::vec2(0.0f,0.0f);
};

struct AABB
{
	float minx;
	float miny;
	float maxx;
	float maxy;

	Collision2D OnCollisionEnter(AABB other)
	{
		Collision2D c;
		// 左碰撞
		/*if (other.maxx>minx&&other.maxx<maxx&&
			((上面)||(中间)||(下面 )))*/
		if (other.maxx > minx && other.maxx < maxx &&
			((maxy > other.miny && maxy < other.maxy) || (maxy > other.maxy && miny < other.miny) || (miny > other.miny && miny < other.maxy)))
			c.normal = glm::vec2(-1.0f, 0.0f);
		//右碰撞
		else if (other.minx > minx && other.minx < maxx &&
			((maxy > other.miny && maxy < other.maxy) || (maxy > other.maxy && miny < other.miny) || (miny > other.miny && miny < other.maxy)))
			c.normal = glm::vec2(1.0f, 0.0f);
		//下碰撞
		else if (other.maxx < maxx && other.minx > minx &&
			other.maxy > miny && other.miny < miny)
			c.normal = glm::vec2(0.0f, -1.0f);
		//上碰撞
		else if (other.maxx < maxx && other.minx > minx &&
			other.maxy > maxy && other.miny < maxy)
			c.normal = glm::vec2(0.0f, 1.0f);
		return c;
	}

};

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
	float moveSpeed = 4;
	glm::vec2 m_Position = glm::vec2{ -10.0f, 0.0f };
	glm::vec2 m_Velocity = glm::vec2{ 0.0f, 0.0f };
	glm::vec2 m_Scale = glm::vec2(2.5f, 2.7f);

	AABB aabb;

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;

	std::vector<CC3D::Ref<CC3D::Texture2D>> idelTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> jumpTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> walkTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> runTexture;
};