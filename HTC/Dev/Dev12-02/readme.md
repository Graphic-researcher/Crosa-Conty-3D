# Dev12-02

TODO : draw each component ui

## Mesh Componet

code version: [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/8dbf32d7af394b1d2c0476c457e783b5adc4b295)

```c++
DrawComponent<MeshComponent>("Mesh Component", entity, [](auto& component)
{
	std::unordered_map<MeshType, std::string> KeyMap =
	{
		{MeshType::None, "None"},
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
					}
				}
				ImGui::EndMenu();//if BeginMenu(item.second.c_str())
			}
		}
		ImGui::EndPopup();//if BeginPopup("Set")
	}
});
```

![Mesh1](Mesh1.gif)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/5e8404c6e46e5bfd9d0225e94342aaf3694786d5)



## Material Component



## Light Component