#include "Player.h"

#include <glm/glm.hpp>

Player::Player()
{
}

void Player::LoadAssets()
{
	for (int i = 0; i < 4; i++)
	{
		idelTexture.push_back(CC3D::Texture2D::Create("src/ClimbGame/ClimbGame/Textures/Player/idle/idle-" + std::to_string(i + 1) + ".png"));
	}
	
}

void Player::OnUpdate(CC3D::Timestep ts)
{
	// Movement
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::A))
	{
		m_Scale.x = -2.5f;
		m_Velocity.x = -moveSpeed;
		m_Position.x += m_Velocity.x * ts;
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::D))
	{
		m_Scale.x = 2.5f;
		m_Velocity.x = moveSpeed;
		m_Position.x += m_Velocity.x * ts;
	}
	if (CC3D::Input::IsKeyPressed(CC3D::KeyCode::Space))
	{
		m_Position.y += moveSpeed * ts;
	}
	m_Velocity.x = m_Velocity.x * 0.99;

	// Collision dection

}

void Player::OnRender()
{
	CC3D::Renderer2D::DrawQuad(glm::vec3(m_Position.x, m_Position.y, 1.0f), m_Scale, idelTexture.at(0));
}

void Player::OnImGuiRender()
{
}
