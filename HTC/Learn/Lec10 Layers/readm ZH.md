# Lec10 Layers

tutorial : [here](https://www.youtube.com/watch?v=_Kj6BSfM6P4&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=13)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/5a1bc8cbae8e5f6d95184be29e3006b0ff464853/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## 什么是层?

层决定了渲染的顺序

就像PS里的图层那样，另外，最开始的层要最后渲染，所以层适合被栈存储

## Layer作用域

### Layer Class

```c++
class CC3D_API Layer
{
public:
	Layer(const std::string& name = "Layer");
	virtual ~Layer();

	virtual void OnAttach() {}///push to layer stack
	virtual void OnDetach() {}///remove
	virtual void OnUpdate() {}///call when layer update
	virtual void OnEvent(Event& event) {}///get event send to layer

	inline const std::string& GetName() const { return m_DebugName; }
protected:
	std::string m_DebugName;
};

Layer::Layer(const std::string& debugName)
: m_DebugName(debugName)
{
}

Layer::~Layer()
{

}
```

### LayerStack Class

```c++
class CC3D_API LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	void PopLayer(Layer* layer);
	void PopOverlay(Layer* overlay);

	/// every frame this would be iterate over	
	///Also need to be reversed,because we need to pop the event
	std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_Layers.end(); }
private:		
	std::vector<Layer*> m_Layers;
	std::vector<Layer*>::iterator m_LayerInsert;
};
///Layer:Operate from List begin
///OverLayer:Operate from List end
LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

/// When app shut down,destory layer stack

LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

///Push the layer to right place (before overlayer)
void LayerStack::PushLayer(Layer* layer)
{
	m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
}

///Push to back of list
///overlayer always be after layer
void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

/// when pop layer,we won't lose it,
///because we can push back the layer and the layer is just pop temporally
void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

 
void LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}
```

## Application作用域

in Application.cpp :

```c++
void Application::Run()
{
	while (m_Running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for (Layer* layer : m_LayerStack)
			layer->OnUpdate();
		m_Window->OnUpdate();
	}
}

void Application::PushLayer(Layer* layer)
{
	m_LayerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* layer)
{
	m_LayerStack.PushOverlay(layer);
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
	CC3D_CORE_INFO("{0}", e);
	for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
	{
		(*--it)->OnEvent(e);
		if (e.Handled)
			break;
	}
}
```

## SandBox作用域

in SandBoxApp.cpp :

```c++
class ExampleLayer : public CC3D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		CC3D_INFO("ExampleLayer::Update");
	}

	void OnEvent(CC3D::Event& event) override
	{
		CC3D_TRACE("{0}", event);
	}

};

class SandBox : public CC3D::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox() {}

};
```

## 构建和结果

![image-20211013164559960](https://i.loli.net/2021/10/13/MsjV3hKq7gUtGD2.png)