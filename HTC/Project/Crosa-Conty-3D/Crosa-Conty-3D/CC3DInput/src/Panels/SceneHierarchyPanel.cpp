#include "SceneHierarchyPanel.h"
#include "CC3D/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>


namespace CC3D {
	extern const std::filesystem::path g_AssetPath;

	static std::string BoolString(bool x) { if (x)return "true"; return "false"; }

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context)
		{
			m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_Context.get() };
				DrawEntityNode(entity);
			});

			/// <summary>
			/// If you don't select anything,select context will be none
			/// </summary>
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();

	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}

	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);

		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}


	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar(
			);
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (!m_SelectionContext.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 2D"))
				{
					m_SelectionContext.AddComponent<Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 2D"))
				{
					m_SelectionContext.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			//TODO : 3D Renderer Dev
			if (!m_SelectionContext.HasComponent<MeshComponent>())
			{
				if (ImGui::MenuItem("Mesh Component"))
				{
					m_SelectionContext.AddComponent<MeshComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<MaterialComponent>())
			{
				if (ImGui::MenuItem("Material Component"))
				{
					m_SelectionContext.AddComponent<MaterialComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!m_SelectionContext.HasComponent<LightComponent>())
			{
				if (ImGui::MenuItem("Light Component"))
				{
					m_SelectionContext.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			glm::vec3 rotation = glm::degrees(component.Rotation);
			DrawVec3Control("Rotation", rotation);
			component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.Camera;
		
			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
		});
		
		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			static float thumbnailSize = 128.0f;

			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			if (nullptr!=component.Texture)
			{
				ImGui::ImageButton((ImTextureID)component.Texture->GetRendererID(),{ thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			}
			else
			{
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.Texture = texture;
					else
						CC3D_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}



			ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);

		});


		DrawComponent<MaterialComponent>("Material Component", entity, [](auto& component)
		{

		});

		DrawComponent<LightComponent>("Light Component", entity, [](auto& component)
		{

		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<MeshComponent>("Mesh Component", entity, [](auto& component)
		{
			std::unordered_map<MeshType, std::string> KeyMap =
			{
				{MeshType::None, "Empty"},
				{MeshType::Plane, "Plane"},
				{MeshType::Cube, "Cube"},
				{MeshType::Sphere, "Sphere"},
				{MeshType::Model, "Model"},
			};

			if (ImGui::Button("Set"))
			{
				ImGui::OpenPopup("Set");
			}
			if (ImGui::BeginPopup("Set"))
			{
				for (auto item : KeyMap)
				{
					if (ImGui::BeginMenu(item.second.c_str()))
					{
						if (item.first != MeshType::None && item.first != MeshType::Model)
						{
							static int sample = 1;
							ImGui::SliderInt("sample", &sample, 1, 30);
							if (ImGui::Button("OK"))
							{
								component.Reload(item.first, sample);
								component.isShowModel = false;
							}
						}
						if (item.first == MeshType::None)
						{
							if (ImGui::MenuItem("Reset"))
							{
								component.Reset();
								component.isShowModel = false;
							}
						}
						if (item.first == MeshType::Model)
						{
							if (ImGui::MenuItem("Load"))
							{
								component.isShowModel = true;
							}
						}
						ImGui::EndMenu();//if BeginMenu(item.second.c_str())
					}
				}
				ImGui::EndPopup();//if BeginPopup("Set")
			}

			if (component.isShowModel)
			{
				ImGui::Button("Model", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path ModelPath = std::filesystem::path(g_AssetPath) / path;
						std::string str = ModelPath.string();
						std::string suffix = str.substr(str.find_last_of('.') + 1);
						if (suffix!="obj")
						{
							CC3D_ERROR("Can't load file");
						}
						else
						{
							component.Reload(str);
						}
					}
					ImGui::EndDragDropTarget();
				}
			}

			///Mesh Information:
			ImGui::Text(("Mesh Type: " + KeyMap[component.Mesh->GetMeshType()]).c_str());
			ImGui::Text(("Path: " + component.Mesh->GetPathName()).c_str());
			ImGui::Text(("Vertices Number: " + std::to_string(component.Mesh->GetVerticesNum())).c_str());
			ImGui::Text(("Triangle Number: " + std::to_string(component.Mesh->GetTriangleNum())).c_str());
			ImGui::Text(("Has TexCoord: " + BoolString(component.Mesh->HasTexCoord())).c_str());
			ImGui::Text(("Has Normal: " + BoolString(component.Mesh->HasNormal())).c_str());
		});

	}//draw component end
}
