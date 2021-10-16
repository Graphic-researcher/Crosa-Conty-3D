# Lec21 Vertex buffer layout

tutorial : [here](https://www.youtube.com/watch?v=jIJFM_pi6gQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=31)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/5707cef5b6f3b475986761db1c4c51cecf9104f3/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

OpenGL prerequisite : [here](https://www.youtube.com/watch?v=x0H--CL2tUI)

## Buffer Scope

buffer.h

```c++
class BufferLayout
{
public:
	BufferLayout() {}

	BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements(elements)
	{
		CalculateOffsetsAndStride();
	}

	inline uint32_t GetStride() const { return m_Stride; }
	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
private:
	void CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;///an atribute size
			m_Stride += element.Size;///sum up to vertex size
		}
	}
private:
	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};
class VertexBuffer
{
public:
	//...code...

	virtual const BufferLayout& GetLayout() const = 0;
	virtual void SetLayout(const BufferLayout& layout) = 0;

	//...code...
};
```

OpenGLBuffer.h

```c++
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	//...code...

	virtual const BufferLayout& GetLayout() const override { return m_Layout; }
	virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

private:
	uint32_t m_RendererID;
	BufferLayout m_Layout;
};
```

## Application Scope

```c++
Application::Application()
{
	//...code...

	/// OpenGL render command
	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);


	//   COORDINATES        /     COLORS		 /
	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	//   COORDINATES        /     COLORS		 /
	//float vertices1[3 * 7] = {
	//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	//	 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
	//};

	m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

	{
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};

		m_VertexBuffer->SetLayout(layout);
	}

	uint32_t index = 0;
	const auto& layout = m_VertexBuffer->GetLayout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.GetComponentCount(),
			ShaderDataTypeToOpenGLBaseType(element.Type),
			element.Normalized ? GL_TRUE : GL_FALSE,
			layout.GetStride(),///to next vertex
			(const void*)element.Offset);///to next attribute
		index++;
	}
	uint32_t indices[3] = { 0, 1, 2 };
	m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				//color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

	m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
}
```



