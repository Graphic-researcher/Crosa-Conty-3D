# Lec19 OpenGL Shader

教程：[here](https://www.youtube.com/watch?v=QBCCHm_9HV8&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=29)

代码版本 : [here](https://github.com/Graphic-researcher/Crosa-Conty-3D/tree/0f3fc57d95538ce03a260d53503f637926473405/HTC/Project/Crosa-Conty-3D/Crosa-Conty-3D)

## Shader 作用域

```c++
class Shader
{
public:
	Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
	~Shader();

	void Bind() const;
	void Unbind() const;
private:
	uint32_t m_RendererID;
};
Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
	// Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertexSrc.c_str();
	glShaderSource(vertexShader, 1, &source, 0);

	// Compile the vertex shader
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertexShader);

		CC3D_CORE_ERROR("{0}", infoLog.data());
		CC3D_CORE_ASSERT(false, "Vertex shader compilation failure!");
		return;
	}

	// Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Send the fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragmentSrc.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	// Compile the fragment shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		// Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);

		CC3D_CORE_ERROR("{0}", infoLog.data());
		CC3D_CORE_ASSERT(false, "Fragment shader compilation failure!");
		return;
	}

	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	m_RendererID = glCreateProgram();
	GLuint program = m_RendererID;

	// Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

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
		// Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		CC3D_CORE_ERROR("{0}", infoLog.data());
		CC3D_CORE_ASSERT(false, "Shader link failure!");
		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}
```

## Application 作用域

```c++
//Application.h:
std::unique_ptr<Shader> m_Shader;
//Application.cpp:
//Application():
/// OpenGL render command
glGenVertexArrays(1, &m_VertexArray);
glBindVertexArray(m_VertexArray);

glGenBuffers(1, &m_VertexBuffer);
glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

float vertices[3 * 3] = {
-0.5f, -0.5f, 0.0f,
0.5f, -0.5f, 0.0f,
0.0f,  0.5f, 0.0f
};

glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

glGenBuffers(1, &m_IndexBuffer);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

unsigned int indices[3] = { 0, 1, 2 };
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

std::string vertexSrc = R"(
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

void main()
{
v_Position = a_Position;
gl_Position = vec4(a_Position, 1.0);	
}
)";

std::string fragmentSrc = R"(
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;

void main()
{
color = vec4(v_Position * 0.5 + 0.5, 1.0);
}
)";

m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	

//Run():
m_Shader->Bind();
```

## 构建和结果

```c++
std::string vertexSrc = R"(
	#version 330 core
			
	layout(location = 0) in vec3 a_Position;

	out vec3 v_Position;

	void main()
	{
		v_Position = a_Position;
		gl_Position = vec4(a_Position, 1.0);	
	}
)";

std::string fragmentSrc = R"(
	#version 330 core
			
	layout(location = 0) out vec4 color;

	in vec3 v_Position;

	void main()
	{
		color = vec4(v_Position * 0.5 + 0.5, 1.0);
	}
)";
```

![image-20211015211538812](https://i.loli.net/2021/10/15/LTUm1apxs8dbYte.png)