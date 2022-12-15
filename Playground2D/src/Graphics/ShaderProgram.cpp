
#include "ShaderProgram.h"

#include <fstream>
#include <iostream>

#include "../Common.h"

namespace pg2D {
	void ShaderProgram::ParseShader(const std::string& filePath) {
		std::ifstream shaderFile(filePath);

		std::string ss[2];
		enum class ShaderType {NONE = -1, VERTEX = 0, FRAGMENT};
		ShaderType _shaderType = ShaderType::NONE;
		std::string line;
		while (getline(shaderFile, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) _shaderType = ShaderType::VERTEX;
				if (line.find("fragment") != std::string::npos) _shaderType = ShaderType::FRAGMENT;
			}
			else {
				ss[(int)_shaderType] += line + '\n';
			}
		}

		m_ShaderSource = { ss[0], ss[1] };
	}

	void ShaderProgram::ParseShader(const std::string& vertexShader, const std::string& fragmentShader) {
		m_ShaderSource = { vertexShader, fragmentShader };
	}

	GLuint ShaderProgram::CompileShader(GLuint shaderType, const std::string& shaderSource) {
		GLuint shader = glCreateShader(shaderType);
		const char* shaderSrc = shaderSource.c_str();

		glShaderSource(shader, 1, &shaderSrc, nullptr);
		glCompileShader(shader);

		GLint compileStatus;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus == GL_FALSE) {
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(shader, length, &length, message);
			PG_CORE_ERROR("FAILED TO LOAD SHADER ({0})", shader);
			PG_CORE_ERROR("[Shader Error] {0}: {1}", (shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment"), message);
		}

		return shader;
	}

	void ShaderProgram::CreateShaderProgram() {
		GLuint program = glCreateProgram();
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, m_ShaderSource.vertexShader);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, m_ShaderSource.fragmentShader);

		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if(m_ShaderProgramID == 0) m_ShaderProgramID = program;

	}

	void ShaderProgram::Use() {
		glUseProgram(m_ShaderProgramID);
		for (int i = 0; i < m_AttributeNumbers; i++) {
			glEnableVertexAttribArray(m_AttributeNumbers);
		}
	}

	void ShaderProgram::Unuse() {
		glUseProgram(0);
		for (int i = 0; i < m_AttributeNumbers; i++) {
			glDisableVertexAttribArray(m_AttributeNumbers);
		}
	}

	void ShaderProgram::AddAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_ShaderProgramID, m_AttributeNumbers++, attributeName.c_str());
	}

	GLint ShaderProgram::GetUniformLocation(const std::string& uniformName) {
		return glGetUniformLocation(m_ShaderProgramID, uniformName.c_str());
	}
}
