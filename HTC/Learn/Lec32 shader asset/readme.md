# Lec32 shader asset

tutorial : [here](https://www.youtube.com/watch?v=8wFEzIYRZXg&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=45)

code version : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/6282245e17574521172576b215d874641c43783f/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

Add some method to load shader from eternal file.

## Shader Scope

### Shader

```c++
Shader* Shader::Create(const std::string& filepath)
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::API::None:    CC3D_CORE_ASSERT(false, "RendererAPI::None is currently not 		supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return new OpenGLShader(filepath);
    }

    CC3D_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
```

### OpenGLShader

```c++
static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	CC3D_CORE_ASSERT(false, "Unknown shader type!");
	return 0;
}

OpenGLShader::OpenGLShader(const std::string& filepath)
{
	std::string source = ReadFile(filepath);
	auto shaderSources = PreProcess(source);
	Compile(shaderSources);
}


OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
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
	std::ifstream in(filepath, std::ios::in, std::ios::binary);
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

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		CC3D_CORE_ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		CC3D_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
{
	GLuint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs(shaderSources.size());
	for (auto& kv : shaderSources)
	{
		GLenum type = kv.first;
		const std::string& source = kv.second;

		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			CC3D_CORE_ERROR("{0}", infoLog.data());
			CC3D_CORE_ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs.push_back(shader);
	}

	m_RendererID = program;

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);

		for (auto id : glShaderIDs)
			glDeleteShader(id);

		CC3D_CORE_ERROR("{0}", infoLog.data());
		CC3D_CORE_ASSERT(false, "Shader link failure!");
		return;
	}

	for (auto id : glShaderIDs)
		glDetachShader(program, id);
}
```

## Load from file

```c++
m_TextureShader.reset(CC3D::Shader::Create("assets/shaders/Texture.glsl"));
```





