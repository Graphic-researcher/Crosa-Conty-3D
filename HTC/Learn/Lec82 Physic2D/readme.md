# Lec 82 Physic2D

tutorial : [here](https://www.youtube.com/watch?v=B1nO5wbPbwg&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=106)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/commit/38fc71fa73cd8319f2f7d3175d2973418d94da89)

## Component Scope

```c++
// Physics

struct Rigidbody2DComponent
{
    enum class BodyType { Static = 0, Dynamic, Kinematic };
    BodyType Type = BodyType::Static;
    bool FixedRotation = false;

    // Storage for runtime
    void* RuntimeBody = nullptr;

    Rigidbody2DComponent() = default;
    Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
};

struct BoxCollider2DComponent
{
    glm::vec2 Offset = { 0.0f, 0.0f };
    glm::vec2 Size = { 0.5f, 0.5f };

    // TODO(Yan): move into physics material in the future maybe
    float Density = 1.0f;
    float Friction = 0.5f;
    float Restitution = 0.0f;
    float RestitutionThreshold = 0.5f;

    // Storage for runtime
    void* RuntimeFixture = nullptr;

    BoxCollider2DComponent() = default;
    BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
};
```

## Scene Scope

```c++
class b2World;
b2World* m_PhysicsWorld = nullptr;
void Scene::OnRuntimeStart()
{
    m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

    auto view = m_Registry.view<Rigidbody2DComponent>();
    for (auto e : view)
    {
        Entity entity = { e, this };
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

        b2BodyDef bodyDef;
        bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
        bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
        bodyDef.angle = transform.Rotation.z;

        b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
        body->SetFixedRotation(rb2d.FixedRotation);
        rb2d.RuntimeBody = body;

        if (entity.HasComponent<BoxCollider2DComponent>())
        {
            auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

            b2PolygonShape boxShape;
            boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = bc2d.Density;
            fixtureDef.friction = bc2d.Friction;
            fixtureDef.restitution = bc2d.Restitution;
            fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
            body->CreateFixture(&fixtureDef);
        }
    }
}

void Scene::OnRuntimeStop()
{
    delete m_PhysicsWorld;
    m_PhysicsWorld = nullptr;
}
void Scene::OnUpdateRuntime(Timestep ts)
{
    // Update scripts
    // Physics
    {
        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

        // Retrieve transform from Box2D
        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view)
        {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

            b2Body* body = (b2Body*)rb2d.RuntimeBody;
            const auto& position = body->GetPosition();
            transform.Translation.x = position.x;
            transform.Translation.y = position.y;
            transform.Rotation.z = body->GetAngle();
        }
    }
    // Render 2D
}

template<>
void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
{
}

template<>
void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
{
}
```

## Scene Serializer Scope

```c++
namespace YAML {
    template<>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
    //...
}
YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
{
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}
static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
{
    switch (bodyType)
    {
        case Rigidbody2DComponent::BodyType::Static:    return "Static";
        case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
        case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
    }

    CC3D_CORE_ASSERT(false, "Unknown body type");
    return {};
}

static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
{
    if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
    if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
    if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

    CC3D_CORE_ASSERT(false, "Unknown body type");
    return Rigidbody2DComponent::BodyType::Static;
}
static void SerializeEntity(YAML::Emitter& out, Entity entity)
{
    //...
    if (entity.HasComponent<Rigidbody2DComponent>())
    {
        out << YAML::Key << "Rigidbody2DComponent";
        out << YAML::BeginMap; // Rigidbody2DComponent

        auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
        out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
        out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;

        out << YAML::EndMap; // Rigidbody2DComponent
    }

    if (entity.HasComponent<BoxCollider2DComponent>())
    {
        out << YAML::Key << "BoxCollider2DComponent";
        out << YAML::BeginMap; // BoxCollider2DComponent

        auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
        out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
        out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
        out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
        out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
        out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
        out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;

        out << YAML::EndMap; // BoxCollider2DComponent
    }
    //...
}
bool SceneSerializer::Deserialize(const std::string& filepath)
{
    //...
    auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
    if (rigidbody2DComponent)
    {
        auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
        rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
        rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
    }

    auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
    if (boxCollider2DComponent)
    {
        auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
        bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
        bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
        bc2d.Density = boxCollider2DComponent["Density"].as<float>();
        bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
        bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
        bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
    }
    //...
}
```

## CC3D Input Scope

### Scene Hierarchy Panel 

```c++
void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    //...
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

        ImGui::EndPopup();
    }
    //...
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
        ImGui::DragFloat2("Size", glm::value_ptr(component.Offset));
        ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
	});
}
```

### Editor Layer

```c++
void EditorLayer::OnScenePlay()
{
    m_SceneState = SceneState::Play;
    m_ActiveScene->OnRuntimeStart();
}

void EditorLayer::OnSceneStop()
{
    m_SceneState = SceneState::Edit;
    m_ActiveScene->OnRuntimeStop();
}
```

## Build and Result

![result](./result.gif)