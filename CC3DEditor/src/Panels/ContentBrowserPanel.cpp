#include "ccpch.h"
#include "ContentBrowserPanel.h"

#include <imgui.h>
#include <assimp/Importer.hpp>

namespace CC3D {
	// Once we have projects, change this
	// filesystem https://en.cppreference.com/w/cpp/filesystem
	extern const std::filesystem::path g_AssetPath = "assets";

	namespace Utils
	{
		bool IsImageByTail(std::filesystem::path path)
		{
			std::string file_exten = path.extension().u8string();
		
			if (file_exten == ".jpg" || file_exten == ".tif"
				|| file_exten == ".png" || file_exten == ".bmp"
				|| file_exten == ".gif" || file_exten == ".ico")
				return true;
			return false;
		}
		bool isModelByExtension(std::filesystem::path path)
		{
			std::string file_exten = path.extension().u8string();

			if (file_exten == ".obj" || file_exten == ".fbx")
				return true;
			return false;
		}
	}

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/FolderIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/FileIcon.png");
		m_PictureIcon = Texture2D::Create("Resources/Icons/PictureIcon.png");
	}

	void ContentBrowserPanel::OnAttach()
	{
		
		// TODO 进度条
		for (auto const& directory : std::filesystem::recursive_directory_iterator{ m_CurrentDirectory })
		{
			// 遍历所有文件夹，读取图片索引
			if (Utils::IsImageByTail(directory.path()))
			{
				Ref<Texture2D> image = Texture2D::Create(directory.path().u8string());
				if(image->IsLoaded())
					m_Images.emplace(directory.path().u8string(),image);
				else
					m_Images.emplace(directory.path().u8string(), m_PictureIcon);
			}
			if (Utils::isModelByExtension(directory.path()))
			{
				// TODO 模型的预览图
				Model model(directory.path().u8string());
				m_Models.emplace(directory.path().u8string(), model);
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
				// 返回上级目录
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		// 根据缩放控制Content的布局
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
			if (directoryEntry.is_directory())							// 文件夹
				icon = m_DirectoryIcon;
			else if (Utils::IsImageByTail(directoryEntry.path().u8string()))		// 图片
				icon = m_Images[directoryEntry.path().u8string()];
			else                                                        // 其他
				icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;

			// 按钮背景透明
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();
			//-------------------------
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))// 双击且在图标上
			{
				if (directoryEntry.is_directory())//是文件夹
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
