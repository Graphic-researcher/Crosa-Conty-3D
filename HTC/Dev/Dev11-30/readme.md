# Dev 11-30

## TODO

solve Select Entity Problem

Test Material Light Mesh Shader Component to see if it works well

If have enough time ,  draw a component ui

## Progress

### Model Loading

Model load good use this function

```c++
void EditorLayer::EntityTest2()
{
    auto square4 = m_ActiveScene->CreateEntity("Car");
    std::string str = "assets/Meshes/cessna.obj";
    Ref<TriMesh> m_Mesh = CreateRef<TriMesh>(str);
    square4.AddComponent<MeshComponent>(m_Mesh);
}
```

This method can't load model

```c++
void EditorLayer::EntityTest2()
{
    auto square4 = m_ActiveScene->CreateEntity("Car");
    std::string str = "assets/Meshes/cessna.obj";
    Ref<TriMesh> m_Mesh = CreateRef<TriMesh>();
    m_Mesh->Create(str);
    square4.AddComponent<MeshComponent>(m_Mesh);
}
```

![image-20211130092116022](C:\Users\lenovo\AppData\Roaming\Typora\typora-user-images\image-20211130092116022.png)