#include "ccpch.h"
#include "CC3D/Renderer/Light.h"
#include "CC3D/Renderer/Material.h"

namespace CC3D {



#pragma region PointLight
	void PointLight::Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot)
	{
		material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
		material->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", position);
		material->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
		material->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
		material->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
		material->SetFloat("u_PointLight[" + std::to_string(slot) + "].intensity", Intensity);
	}
#pragma endregion
	
}