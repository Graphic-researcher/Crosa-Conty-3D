# Lec33 shader library

课程 : [here](https://www.youtube.com/watch?v=Z9LE3ksHEQQ&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=45)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/a63e10337f3f04b888446c30750c7bf50aef08b1/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

使用着色器库对象来管理我们的着色器

## Shader 作用域

### Shader and ShaderLibrary

```c++
class Shader
{
public:
	virtual ~Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual const std::string& GetName() const = 0;

	static Ref<Shader> Create(const std::string& filepath);
	static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
};
Ref<Shader> Shader::Create(const std::string& filepath)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(filepath);
	}

	CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}
Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
	case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
	}

	CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
{
	CC3D_CORE_ASSERT(!Exists(name), "Shader already exists!");
	m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
	auto& name = shader->GetName();
	Add(name, shader);
}

CC3D::Ref<CC3D::Shader> ShaderLibrary::Load(const std::string& filepath)
{
	auto shader = Shader::Create(filepath);
	Add(shader);
	return shader;
}

CC3D::Ref<CC3D::Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
{
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
}

CC3D::Ref<CC3D::Shader> ShaderLibrary::Get(const std::string& name)
{
	CC3D_CORE_ASSERT(Exists(name), "Shader not found!");
	return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string& name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}
```

### OpenGLShader 

```c++
OpenGLShader::OpenGLShader(const std::string& filepath)
{
	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);
	// Extract name from filepath
	auto lastSlash = filepath.find_last_of("/\\");
	lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
	auto lastDot = filepath.rfind('.');
	auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
	m_Name = filepath.substr(lastSlash, count);
}


OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	: m_Name(name)
{
	std::unordered_map<GLenum, std::string> sources;
	sources[GL_VERTEX_SHADER] = vertexSrc;
	sources[GL_FRAGMENT_SHADER] = fragmentSrc;
	Compile(sources);
}

OpenGLShader::~OpenGLShader()
{
	glDeleteProgram(m_RendererID);
}

std::string OpenGLShader::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
		;
	}
	else
	{
		CC3D_CORE_ERROR("Could not open file '{0}'", filepath);
	}

	return result;
}
```

## SandBox 作用域

使用着色器库对象操作着色器

```c++
m_FlatColorShader = CC3D::Shader::Create("FlatColor",flatColorShaderVertexSrc,flatColorShaderFragmentSrc);
m_ShaderLibrary.Add(m_FlatColorShader);///add shader to shaderlib
auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");///add shader to shaderlib

std::dynamic_pointer_cast<CC3D::OpenGLShader>(textureShader)->Bind();
std::dynamic_pointer_cast<CC3D::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
auto TestShader = m_ShaderLibrary.Get("UVColor");//find shader by name in shaderlibray
CC3D::Renderer::Submit(TestShader, m_SquareVA,transform);//sumbit by shaderlib
auto textureShader = m_ShaderLibrary.Get("Texture");//find shader by name in shaderlibray
CC3D::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
```

