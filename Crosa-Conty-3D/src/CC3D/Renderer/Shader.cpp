#include "ccpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace CC3D{
	
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//--------------vertex shader-----------------
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		
		// ASSERT vertexShader
		if (isCompiled = GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			
			glDeleteShader(vertexShader);

			CC3D_CORE_ERROR("{0}", infoLog.data());
			CC3D_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
		//--------------------------------------------


		//--------------fragment shader---------------
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		// ASSERT fragmentShader
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			CC3D_CORE_ERROR("{0}", infoLog.data());
			CC3D_CORE_ASSERT(false, "FRAGMENT sahder compilation failure!");
			return;
		}
		//---------------------------------------------

		//-------------Shader Program--------------
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// delete program
			glDeleteProgram(program);

			// delete vertexShader and fragmentShader
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			CC3D_CORE_ERROR("{0}", infoLog.data());
			CC3D_CORE_ASSERT(false, "Shader link failure!");
			return;
		}
		//---------------------------------------------

		// Always detach shader after a successful link
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

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}