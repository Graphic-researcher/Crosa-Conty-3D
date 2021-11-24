#pragma once

#include <filesystem>
#include <map>
#include "CC3D/Renderer/Texture.h"
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
		std::map<std::string, Ref<Texture2D>> m_Images;
		// TODO ƒ£–Õ‘§¿¿Õº
		std::map<std::string, Ref<Texture2D>> m_Models;
	};

}
