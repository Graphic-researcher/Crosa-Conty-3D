#include "ccpch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace CC3D {

	// Once we have projects, change this
	// filesystem https://en.cppreference.com/w/cpp/filesystem
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				// 返回上级目录
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();//当前文件路径
			auto relativePath = std::filesystem::relative(path, s_AssetPath);//合成相对路径
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())//如果是文件夹
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();//进入文件夹
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))//如果不是文件夹
				{
				}
			}
		}

		ImGui::End();
	}

}
