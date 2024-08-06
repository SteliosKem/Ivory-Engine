#include "pch.h"
#include "Rendering/Renderer.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Ivory {
	OpenGLShader::OpenGLShader(const std::string& vertex_src, const std::string& fragment_src) {
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const char* source = vertex_src.c_str();
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

			// Use the infoLog as you see fit.

			IV_CORE_ERROR("Vertext shader compilation failed");
			IV_CORE_ERROR((const char*)infoLog.data());
			IV_CORE_ASSERT(false, "Vertext shader compilation failed");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragment_src.c_str();
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

			// Use the infoLog as you see fit.

			IV_CORE_ERROR("Shader linkage failed");
			IV_CORE_ERROR((const char*)infoLog.data());
			IV_CORE_ASSERT(false, "Shader linkage failed");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_rendererID = glCreateProgram();
		GLuint program = m_rendererID;

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

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}

	void OpenGLShader::bind() const {
		glUseProgram(m_rendererID);
	}

	void OpenGLShader::unbind() const {
		glDeleteProgram(m_rendererID);
	}

	void OpenGLShader::upload_uniform_mat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::upload_uniform_mat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::upload_uniform_vec3(const std::string& name, const glm::vec3& vector) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform3f(location, vector.x, vector.y, vector.z);
	}
	void OpenGLShader::upload_uniform_vec4(const std::string& name, const glm::vec4& vector) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}
	void OpenGLShader::upload_uniform_vec2(const std::string& name, const glm::vec2& vector) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform2f(location, vector.x, vector.y);
	}
	void OpenGLShader::upload_uniform_float(const std::string& name, float num) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1f(location, num);
	}
	void OpenGLShader::upload_uniform_int(const std::string& name, int num) {
		GLint location = glGetUniformLocation(m_rendererID, name.c_str());
		glUniform1i(location, num);
	}

}