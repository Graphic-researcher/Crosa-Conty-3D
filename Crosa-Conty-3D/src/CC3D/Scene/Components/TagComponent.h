#pragma once
#include <string>

namespace CC3D {

	struct TagComponent
	{
		std::string Tag;
		// TODO change to enum
		bool IsStatic = true;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
		TagComponent(const std::string& tag, const bool isStatic)
			: Tag(tag), IsStatic(isStatic) {}
	};
}