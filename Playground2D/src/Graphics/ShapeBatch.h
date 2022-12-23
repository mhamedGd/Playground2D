#pragma once

#include <vector>
#include <string>
#include <math.h>
#include <filesystem>
#include <iostream>

#include <glew/glew.h>
#include <glm/glm.hpp>

#include "../Common.h"
#include "../Camera/Camera2D.h"

#include "ShaderProgram.h"
#include "Vertex.h"


namespace pg2D {
	class ShapeBatch
	{

	public:
		ShapeBatch() {			
			/*
			m_Shader.ParseShader("debug.shader");
			m_Shader.AddAttribute("vertexPosition");
			m_Shader.CreateShaderProgram();
			*/
			glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

			glGenBuffers(1, &m_IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);


			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			m_Shader.ParseShader("Assets/debug.shader");
			m_Shader.AddAttribute("vertexPosition");
			m_Shader.AddAttribute("vertecColor");
			m_Shader.CreateShaderProgram();			
		}
		~ShapeBatch() { }

		void DrawLine(const glm::vec2& startPos, const glm::vec2& targetPos, RGBA8 color);
		void DrawBox(const glm::vec2& origin, const glm::vec2& dimensions, RGBA8 color);
		void DrawBox(const glm::vec2& origin, const glm::vec2& dimensions, RGBA8 color, float angleInDegrees);
		void DrawCirlce(const glm::vec2& center, float radius, RGBA8 color);

		void Render(Camera2D& m_Camera, float lineWidth);

	private:
		GLuint m_VAO = 0, m_VBO = 0, m_IBO = 0;
		std::vector<Vertex> m_Verts;
		std::vector<GLuint> m_Indices;
		int m_NumElements = 0;

		ShaderProgram m_Shader;

		glm::vec2 RotatePoint(const glm::vec2& originalPoint, float angleInDegrees) {
			float angle = angleInDegrees * PI / 180.0f;
			glm::vec2 newPoint;
			newPoint.x = originalPoint.x * cos(angle) - originalPoint.y * sin(angle);
			newPoint.y = originalPoint.x * sin(angle) + originalPoint.y * cos(angle);
			return newPoint;
		}
		void Finalize();
	};
}

