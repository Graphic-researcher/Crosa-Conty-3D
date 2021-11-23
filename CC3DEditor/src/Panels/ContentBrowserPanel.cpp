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
				// �����ϼ�Ŀ¼
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();//��ǰ�ļ�·��
			auto relativePath = std::filesystem::relative(path, s_AssetPath);//�ϳ����·��
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())//������ļ���
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();//�����ļ���
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))//��������ļ���
				{
				}
			}
		}

		ImGui::End();
	}

}
