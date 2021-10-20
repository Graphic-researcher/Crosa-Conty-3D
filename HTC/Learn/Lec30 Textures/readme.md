# Lec30 Textures

tutorial : [here](https://www.youtube.com/watch?v=qEfohFgQ1-I&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=42)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/24b94f5cc43afcc49b870c4b7f053ccd02075c7c/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Vendor

Add stb_image library to load textures from your directory path.

## Texture Scope

![image-20211020194801731](https://i.loli.net/2021/10/20/GAi7HFhtzIoXgDm.png)

### Texture & Texture2D Class

```c++
class Texture
{
public:
	virtual ~Texture() = default;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void Bind(uint32_t slot = 0) const = 0;
};

class Texture2D : public Texture
{
public:
	static Ref<Texture2D> Create(const std::string& path);
};
OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	: m_Path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	CC3D_CORE_ASSERT(data, "Failed to load image!");
	m_Width = width;
	m_Height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}
```

### OpenGLTexture Class Derivative



```c++
class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(const std::string& path);
	virtual ~OpenGLTexture2D();

	virtual uint32_t GetWidth() const override { return m_Width;  }
	virtual uint32_t GetHeight() const override { return m_Height; }

	virtual void Bind(uint32_t slot = 0) const override;
private:
	std::string m_Path;
	uint32_t m_Width, m_Height;
	uint32_t m_RendererID;
};
OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	: m_Path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	CC3D_CORE_ASSERT(data, "Failed to load image!");
	m_Width = width;
	m_Height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}
OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_RendererID);
}
void OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}
```

## SandBox Scope

```c++
/// <summary>
/// Draw Plate UV
/// </summary>
std::string UVShaderVertexSrc = R"(
	#version 330 core
			
	layout(location = 0) in vec3 a_Position;
	layout(location = 1) in vec2 a_TexCoord;

	uniform mat4 u_ViewProjection;
	uniform mat4 u_Transform;

	out vec2 v_TexCoord;

	void main()
	{
		v_TexCoord = a_TexCoord;
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
	}
)";

std::string UVShaderFragmentSrc = R"(
	#version 330 core
			
	layout(location = 0) out vec4 color;

	in vec2 v_TexCoord;


	void main()
	{
		color = vec4(v_TexCoord, 0.0, 1.0);
	}
)";

m_UVShader.reset(CC3D::Shader::Create(UVShaderVertexSrc, UVShaderFragmentSrc));

std::string textureShaderVertexSrc = R"(
	#version 330 core
			
	layout(location = 0) in vec3 a_Position;
	layout(location = 1) in vec2 a_TexCoord;

	uniform mat4 u_ViewProjection;
	uniform mat4 u_Transform;

	out vec2 v_TexCoord;

	void main()
	{
		v_TexCoord = a_TexCoord;
		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
	}
)";

std::string textureShaderFragmentSrc = R"(
	#version 330 core
			
	layout(location = 0) out vec4 color;

	in vec2 v_TexCoord;
			
	uniform sampler2D u_Texture;

	void main()
	{
		color = texture(u_Texture, v_TexCoord);
	}
)";

m_TextureShader.reset(CC3D::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

m_Texture = CC3D::Texture2D::Create("assets/textures/Checkerboard.png");

std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_TextureShader)->Bind();
std::dynamic_pointer_cast<CC3D::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
///draw uv
glm::vec3 pos( 1.5f,0.0f, 0.0f);
glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)* glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
CC3D::Renderer::Submit(m_UVShader, m_SquareVA,transform);
///draw texture
m_Texture->Bind();
CC3D::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
```

## Build and Result

UV and Texture :

![image-20211020195540621](https://i.loli.net/2021/10/20/kpHlLJ8TQ7qx3bO.png)