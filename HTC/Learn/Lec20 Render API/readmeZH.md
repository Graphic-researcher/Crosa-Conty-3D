# Lec20 Render API

课程 : [here](https://www.youtube.com/watch?v=BwCqRqqbB1Y&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=30)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/0f3fc57d95538ce03a260d53503f637926473405/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

抽象渲染api这样以后我们可以方便添加其他图形api

## 总览

![image-20211015224758458](https://i.loli.net/2021/10/15/wgYPMTHSfGFBD9k.png)

## Renderer 作用域

```c++
enum class RendererAPI
{
	None = 0, OpenGL = 1
};

class Renderer
{
public:
	inline static RendererAPI GetAPI() { return s_RendererAPI; }
private:
	static RendererAPI s_RendererAPI;
};
RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
```

## Buffer 作用域

```c++
class VertexBuffer
{
public:
	virtual ~VertexBuffer() {}

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	static VertexBuffer* Create(float* vertices, uint32_t size);
};

class IndexBuffer
{
public:
	virtual ~IndexBuffer() {}

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual uint32_t GetCount() const = 0;

	static IndexBuffer* Create(uint32_t* indices, uint32_t size);
};
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
	}

	CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
{
	switch (Renderer::GetAPI())
	{
		case RendererAPI::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::OpenGL:  return new OpenGLIndexBuffer(indices, size);
	}

	CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
```

## OpenGLBuffer 作用域

```c++
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	virtual ~OpenGLVertexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;
private:
	uint32_t m_RendererID;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
	virtual ~OpenGLIndexBuffer();

	virtual void Bind() const;
	virtual void Unbind() const;

	virtual uint32_t GetCount() const { return m_Count; }
private:
	uint32_t m_RendererID;
	uint32_t m_Count;
};
/////////////////////////////////////////////////////////////////////////////
// VertexBuffer /////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////////////////////////////////////////////
// IndexBuffer //////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
	: m_Count(count)
{
	glCreateBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
```

## Application 作用域

```c++
//Application.h 
std::unique_ptr<VertexBuffer> m_VertexBuffer;
std::unique_ptr<IndexBuffer> m_IndexBuffer;
//Application.cpp
Application::Application()
{
	//...code...

    /// OpenGL render command
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);



    float vertices[3 * 3] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    uint32_t indices[3] = { 0, 1, 2 };
    m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	//...shader code...

    m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	
}

void Application::Run()
{
	gladLoadGL();//Load GLAD so it configures OpenGL

	while (m_Running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader->Bind();

		glBindVertexArray(m_VertexArray);
		glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

		//...code...
	}
}
```

