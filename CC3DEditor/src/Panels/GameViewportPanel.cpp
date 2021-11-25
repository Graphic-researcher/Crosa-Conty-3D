#include "GameViewportPanel.h"
#include "CC3D/Core/Application.h"

#include <imgui.h>

CC3D::GameViewportPanel::GameViewportPanel(const Ref<Scene>& context)
{
	SetContext(context);

	FramebufferSpecification fbSpec;
	// TODO Editor and Game viewport
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8};
	fbSpec.Width = 1280;
	fbSpec.Height = 720;
	m_Framebuffer = Framebuffer::Create(fbSpec);
}

void CC3D::GameViewportPanel::SetContext(const Ref<Scene>& context)
{
	m_Context = context;
	m_SelectionContext = {};
}

void CC3D::GameViewportPanel::OnImGuiRender()
{
	

	ImGui::Begin("Game Viewport");
	auto gameViewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto gameViewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto gameViewportOffset = ImGui::GetWindowPos();
	m_GameViewportBounds[0] = { gameViewportMinRegion.x + gameViewportOffset.x, gameViewportMinRegion.y + gameViewportOffset.y };
	m_GameViewportBounds[1] = { gameViewportMaxRegion.x + gameViewportOffset.x, gameViewportMaxRegion.y + gameViewportOffset.y };


	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_GameViewportSize = glm::vec2{ viewportPanelSize.x, viewportPanelSize.y };

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{ m_GameViewportSize.x, m_GameViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}

void CC3D::GameViewportPanel::SetFramebuffer(Ref<Framebuffer> framebuffer)
{
	m_Framebuffer = framebuffer;
}

void CC3D::GameViewportPanel::SetSelectedEntity(Entity entity)
{
	m_SelectionContext = entity;
}

void CC3D::GameViewportPanel::DrawGizmos()
{
	// TODO Gizmos 
}
