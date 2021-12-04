#include "ccpch.h"
#include "CC3D/Renderer/Light.h"
#include "CC3D/Renderer/Material.h"

namespace CC3D {



#pragma region PointLight
	void PointLight::Bind(const Ref<Material>& material, const glm::vec3& position, const uint32_t& slot)
	{
		int a = 0;
		//shader->Bind();
		//shader->SetFloat3("u_Light[" + std::to_string(slot) + "].color", Color);
		//shader->SetFloat3("u_Light[" + std::to_string(slot) + "].position", position);
		//shader->SetFloat("u_Light[" + std::to_string(slot) + "].intensity", Intensity);
	}
#pragma endregion
	
}