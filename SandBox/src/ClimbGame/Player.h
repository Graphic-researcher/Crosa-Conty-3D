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
		
		/*if (other.maxx>minx&&other.maxx<maxx&&
			((上面)||(中间)||(下面 )))*/
			// 左碰撞
		if ((other.maxx > minx && other.maxx < maxx) && (other.minx < minx && other.minx < maxx) &&
			((maxy > other.maxy && miny < other.miny)))
			c.normal = glm::vec2(-1.0f, 0.0f);
		//右碰撞
		else if (other.minx > minx && other.minx < maxx && (other.maxx > minx && other.maxx > maxx) &&
			((maxy > other.maxy && miny < other.miny)))
			c.normal = glm::vec2(1.0f, 0.0f);
		//下碰撞
		else if (((other.maxx < maxx && other.minx > minx) || (other.maxx > minx && other.minx < minx) || (other.maxx > maxx && other.minx < maxx)) &&
			other.maxy > miny && other.miny < miny)
			c.normal = glm::vec2(0.0f, -1.0f);
		//上碰撞
		else if (((other.maxx < maxx && other.minx > minx) || (other.maxx > minx && other.minx < minx) || (other.maxx > maxx && other.minx < maxx)) &&
			other.maxy > maxy && other.miny < maxy)
			c.normal = glm::vec2(0.0f, 1.0f);
		return c;
	}

	bool intersects(const AABB& aabb) const
	{
		// 就是各轴互相是否包含，（aabb 包含当前包围盒）||  （当前的包围盒包含 aabb）
		return ((minx >= aabb.minx && minx <= aabb.maxx) || (aabb.minx >= minx && aabb.minx <= maxx)) &&
			((miny >= aabb.miny && miny <= aabb.maxy) || (aabb.miny >= miny && aabb.miny <= maxy));
	}

};

class Player
{
public:
	Player();

	void LoadAssets();
	void Movement();

	void OnUpdate(CC3D::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	void Reset();

	void SetPosition(glm::vec2 pos) { m_Position = pos; }
	const glm::vec2& GetPosition() const { return m_Position; }

	void SetVelocity(glm::vec2 v) { m_Velocity = v; }
	const glm::vec2& GetVelocity() const { return m_Velocity; }

	const AABB& GetAABB() const { return aabb; }


	uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }
private:
	float moveSpeed = 6;
	float jumpSpeed = 10;
	glm::vec2 m_Position = glm::vec2{ -10.0f, 0.0f };
	glm::vec2 m_Velocity = glm::vec2{ 0.0f, 0.0f };
	glm::vec2 m_Scale = glm::vec2(2.5f, 2.7f);

	AABB aabb;

	float m_EnginePower = 0.5f;
	float m_Gravity = 20.0f;

	float m_Time = 0.0f;

	std::vector<CC3D::Ref<CC3D::Texture2D>> idelTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> jumpTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> walkTexture;
	std::vector<CC3D::Ref<CC3D::Texture2D>> runTexture;
};