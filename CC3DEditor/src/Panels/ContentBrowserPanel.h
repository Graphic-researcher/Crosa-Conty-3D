#pragma once

#include <filesystem>
#include <map>
#include "CC3D/Renderer/Texture.h"
#include "CC3D/Renderer/Model.h"

namespace CC3D {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnAttach();
		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_PictureIcon;
		
	};

}
