#pragma once

#include <string>

#include "CC3D/Core/Core.h"

namespace CC3D {

	class Texture
	{
	public:
		virtual ~Texture() = default;
		/// <summary>
		/// Get the Width of Texture
		/// </summary>
		/// <returns></returns>
		virtual uint32_t GetWidth() const = 0;
		/// <summary>
		/// Get the Height of Texture
		/// </summary>
		/// <returns></returns>
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}