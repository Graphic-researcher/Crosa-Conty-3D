#include "ccpch.h"
#include "CC3D/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CC3D {

	/// <summary>
	///  MVP
	/// </summary>
	/// <param name="left"></param>
	/// <param name="right"></param>
	/// <param name="top"></param>
	/// <param name="bottom"></param>
	CC3D::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		CC3D_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		CC3D_PROFILE_FUNCTION();

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void CC3D::OrthographicCamera::RecalculateViewMatrix()
	{
		CC3D_PROFILE_FUNCTION();

		// rotate around z-axis
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));// glm::rotate need degree as parameter in version 0.9.3.4

		// View matrix will be premultip by mvpMatrix of objects
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
