
#include "ShapeBatch.h"

#include <iostream>

namespace pg2D {
	void ShapeBatch::DrawLine(const glm::vec2& startPos, const glm::vec2& targetPos, RGBA8 color) {
		int vertsSize = m_Verts.size();
		m_Verts.resize(vertsSize + 2);
		m_Verts[vertsSize] = { startPos, color };
		m_Verts[vertsSize + 1] = { targetPos, color };
				
		
		m_Indices.push_back(vertsSize);
		m_Indices.push_back(vertsSize + 1);		
	}

	void ShapeBatch::DrawBox(const glm::vec2& origin, const glm::vec2& dimensions, RGBA8 color) {
		int vertsSize = m_Verts.size();
		m_Verts.resize(vertsSize + 4);
		glm::vec2 halfDim(dimensions / 2.0f);

		glm::vec2 bl(-halfDim);
		glm::vec2 br(halfDim.x, -halfDim.y);
		glm::vec2 tr(halfDim);
		glm::vec2 tl(-halfDim.x, halfDim.y);

		m_Verts[vertsSize]	   = { origin + bl, color };
		m_Verts[vertsSize + 1] = { origin + br, color };
		m_Verts[vertsSize + 2] = { origin + tr, color };
		m_Verts[vertsSize + 3] = { origin + tl, color };

		m_Indices.push_back(vertsSize);
		m_Indices.push_back(vertsSize + 1);

		m_Indices.push_back(vertsSize + 1);
		m_Indices.push_back(vertsSize + 2);

		m_Indices.push_back(vertsSize + 2);
		m_Indices.push_back(vertsSize + 3);

		m_Indices.push_back(vertsSize + 3);
		m_Indices.push_back(vertsSize);
	}

	void ShapeBatch::DrawBox(const glm::vec2& origin, const glm::vec2& dimensions, RGBA8 color, float angleInDegrees) {

		int vertsSize = m_Verts.size();
		m_Verts.resize(vertsSize + 4);

		glm::vec2 halfDim(dimensions / 2.0f);

		glm::vec2 bl(-halfDim);
		glm::vec2 br(halfDim.x, -halfDim.y);
		glm::vec2 tr(halfDim);
		glm::vec2 tl(-halfDim.x, halfDim.y);
		
		m_Verts[vertsSize]	   = { origin + RotatePoint(bl, angleInDegrees), color };
		m_Verts[vertsSize + 1] = { origin + RotatePoint(br, angleInDegrees), color };
		m_Verts[vertsSize + 2] = { origin + RotatePoint(tr, angleInDegrees), color };
		m_Verts[vertsSize + 3] = { origin + RotatePoint(tl, angleInDegrees), color };

		m_Indices.push_back(vertsSize);
		m_Indices.push_back(vertsSize + 1);

		m_Indices.push_back(vertsSize + 1);
		m_Indices.push_back(vertsSize + 2);

		m_Indices.push_back(vertsSize + 2);
		m_Indices.push_back(vertsSize + 3);

		m_Indices.push_back(vertsSize + 3);
		m_Indices.push_back(vertsSize);
	}

	void ShapeBatch::DrawCirlce(const glm::vec2& center, float radius, RGBA8 color) {
		const int VERTS_NUMBER = 16;

		int vertsSize = m_Verts.size();
		m_Verts.resize(vertsSize + VERTS_NUMBER);

		glm::vec2 right(1.0f, 0.0f);
		for (int i = 0; i < VERTS_NUMBER; i++) {
			float angle = ((float)i / (float)VERTS_NUMBER) * 2.0f * PI;
			m_Verts[vertsSize + i] = { glm::vec2(center.x + cos(angle) * radius, center.y + sin(angle) * radius), color };
		}

		m_Indices.reserve(VERTS_NUMBER * 2);
		for (int i = 0; i < VERTS_NUMBER - 1; i++) {
			m_Indices.push_back(vertsSize + i);
			m_Indices.push_back(vertsSize + i + 1);
		}
		m_Indices.push_back(vertsSize + VERTS_NUMBER - 1);
		m_Indices.push_back(vertsSize);
	}

	void ShapeBatch::Finalize() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

		glBufferData(GL_ARRAY_BUFFER, m_Verts.size() * sizeof(Vertex), m_Verts.data(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_NumElements = m_Indices.size();

		m_Indices.clear();
		m_Verts.clear();
	}

	void ShapeBatch::Render(Camera2D& m_Camera, float lineWidth) {
		Finalize();
		glBindVertexArray(m_VAO);

		m_Shader.Use();

		glUniformMatrix4fv(m_Shader.GetUniformLocation("MVP"), 1, GL_FALSE, &m_Camera.GetViewMatrix()[0][0]);
		
		glLineWidth(lineWidth);

		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_LINES, m_NumElements, GL_UNSIGNED_INT, nullptr);

		m_Shader.Unuse();

		glBindVertexArray(0);

	}
}
