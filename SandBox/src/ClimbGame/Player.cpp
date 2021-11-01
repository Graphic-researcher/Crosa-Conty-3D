#include "Player.h"

#include <glm/glm.hpp>

Player::Player()
{
	aabb.minx = m_Position.x - m_Scale.x;
	aabb.maxx = m_Position.x + m_Scale.x;
	aabb.miny = m_Position.y - m_Scale.y;
	aabb.maxy = m_Position.y + m_Scale.y;
}

void Player::LoadAssets()
{
	for (int i = 0; i < 4; i++)
	{
		idelTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/Player/idle/idle-" + std::to_string(i + 1) + ".png"));
	}
	
}

void Player::Movement()
{
	// Movement
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::A))
	{
		m_Scale.x = -2.5f;
		m_Velocity.x = -moveSpeed;
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::D))
	{
		m_Scale.x = 2.5f;
		m_Velocity.x = moveSpeed;
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::Space))
	{
		m_Velocity.y = jumpSpeed;
	}
}

void Player::OnUpdate(CC3D::Timestep ts)
{
	// ¸üÐÂAABBºÐ
	aabb.minx = m_Position.x - 0.5;
	aabb.maxx = m_Position.x + 0.5;
	aabb.miny = m_Position.y - 1.4;
	aabb.maxy = m_Position.y + 0.4;
	
	m_Position.y += m_Velocity.y * ts;
	m_Position.x += m_Velocity.x * ts;
	m_Velocity.y -= m_Gravity * ts;

	m_Velocity.x = m_Velocity.x * 0.8;

	// Collision dection

}

void Player::OnRender()
{
	// Draw AABB
	CC3D::Renderer2D::DrawQuad(glm::vec3(aabb.maxx, aabb.maxy, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
	CC3D::Renderer2D::DrawQuad(glm::vec3(aabb.maxx, aabb.miny, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
	CC3D::Renderer2D::DrawQuad(glm::vec3(aabb.minx, aabb.maxy, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));
	CC3D::Renderer2D::DrawQuad(glm::vec3(aabb.minx, aabb.miny, 1.0f), glm::vec2(0.1f, 0.1f), glm::vec4(0.7f, 0.2f, 0.3f, 1.0f));

	CC3D::Renderer2D::DrawQuad(glm::vec3(m_Position.x, m_Position.y, 1.0f), m_Scale, idelTexture.at(0));
}

void Player::OnImGuiRender()
{
}
