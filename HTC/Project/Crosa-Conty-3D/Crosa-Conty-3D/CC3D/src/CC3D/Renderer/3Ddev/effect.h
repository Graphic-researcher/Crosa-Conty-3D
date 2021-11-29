#pragma once

#include "CC3D/Core/Core.h"
#include "CC3D/Renderer/Shader.h"
#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/EditorCamera.h"

namespace CC3D
{
	class Cubemap
	{
	public:
		static Ref<Cubemap>Create(const std::string& path);
		static Ref<Cubemap>Create();
			
		virtual ~Cubemap() = default;
		virtual uint32_t GetCubemapID() const = 0;
		virtual uint32_t GetTextureID() const = 0;
		virtual std::string GetPathName() const = 0;
		virtual Ref<Texture2D>& GetTexture() = 0;
		
		virtual void BindCubeMap(EditorCamera& camera, uint32_t slot = 0) const = 0;
		virtual void BindIrradianceMap(uint32_t slot = 0) const = 0;
		virtual void BindPrefilterMap(uint32_t slot = 0) const = 0;
		virtual void BindBRDFLUTTMap(uint32_t slot = 0) const = 0;
	};
}