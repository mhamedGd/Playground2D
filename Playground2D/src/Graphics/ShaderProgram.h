#pragma once

#include <glew/glew.h>
#include <string>

#include <glm/glm.hpp>

namespace pg2D {
	struct ShaderSource {
		std::string vertexShader;
		std::string fragmentShader;
	};

	class ShaderProgram
	{
	public:
		ShaderProgram() {}
		~ShaderProgram() { Dispose(); }

		void Dispose() { 
			glDeleteShader(m_ShaderProgramID);
		}

		void ParseShader(const std::string& filePath);
		void ParseShader(const std::string& vertexShader, const std::string& fragmentShader);
		void CreateShaderProgram();

		void AddAttribute(const std::string& attributeName);

		void Use();
		void Unuse();

		void FloatUniform(const std::string& uniformName, float value) {
			glUniform1f(GetUniformLocation(uniformName), value);
		}
		void IntUniform(const std::string& uniformName, int value) {
			glUniform1i(GetUniformLocation(uniformName), value);
		}
		void Matrix4x4Uniform(const std::string& uniformName, const glm::mat4& matrix) {
			glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, &matrix[0][0]);
		}

		GLint GetUniformLocation(const std::string& uniformName);
	private:
		GLuint m_ShaderProgramID = 0;
		int m_AttributeNumbers = 0;
		ShaderSource m_ShaderSource;
		
		GLuint CompileShader(GLuint shaderType, const std::string& shaderSource);
	};
}

