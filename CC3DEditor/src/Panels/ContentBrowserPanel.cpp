#include "ccpch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>

namespace CC3D {
	// Once we have projects, change this
	// filesystem https://en.cppreference.com/w/cpp/filesystem
	extern const std::filesystem::path g_AssetPath = "assets";

	namespace Utils
	{
		bool IsImageByTail(const std::wstring& path)
		{
			std::wstring file_exten;
			size_t pos = path.rfind(L'.');
			if (pos == std::wstring::npos)
				return false;
			file_exten = path.substr(pos, std::wstring::npos);
			//��file_extenתСд
			for (size_t u = 0; u < file_exten.length(); u++)
			{
				if (file_exten[u] >= L'A' && file_exten[u] <= 'Z')
				{
					file_exten[u] += L'a' - L'A';
				}
			}
			if (file_exten == L".jpg" || file_exten == L".tif"
				|| file_exten == L".png" || file_exten == L".bmp"
				|| file_exten == L".gif" || file_exten == L".ico")
				return true;
			return false;
		}
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
	}

	void ContentBrowserPanel::OnAttach()
	{
		// ���������ļ��У���ȡͼƬ����
		// TODO ������
		for (auto const& directory : std::filesystem::recursive_directory_iterator{ m_CurrentDirectory })
		{
			if (Utils::IsImageByTail(directory.path()))
			{
				Ref<Texture2D> image = Texture2D::Create(directory.path().u8string());
				m_Images.emplace(directory.path().u8string(),image);
			}
		}

	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				// �����ϼ�Ŀ¼
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		// �������ſ���Content�Ĳ���
		static float padding = 16.0f;
		static float thumbnailSize = 58.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);
			std::string filenameString = relativePath.filename().string();


			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon;
			if (directoryEntry.is_directory())							// �ļ���
				icon = m_DirectoryIcon;
			else if (Utils::IsImageByTail(directoryEntry.path()))		// ͼƬ
				icon = m_Images[directoryEntry.path().u8string()];
			else                                                        // ����
				icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			// ��ť����͸��
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			CC3D_WARN(icon->GetRendererID());
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();
			//-------------------------
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))// ˫������ͼ����
			{
				if (directoryEntry.is_directory())//���ļ���
					m_CurrentDirectory /= path.filename();

			}
			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
	}

}
