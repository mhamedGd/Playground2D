#pragma once

#include <glew/glew.h>

#include "../Common.h"
#include "../Camera/Camera2D.h"

#include "ShaderProgram.h"
#include "Vertex.h"

#include <vector>


namespace pg2D {
	struct Glyph {
		Glyph(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4& uv, GLuint _textureID, RGBA8 color, float _depth) {
			textureID = _textureID;
			depth = _depth;

			glm::vec2 halfDim(dimensions / 2.0f);

			bl = { pos - halfDim						 , color, glm::vec2(uv.x, uv.y) };
			br = { pos + glm::vec2(halfDim.x, -halfDim.y), color, glm::vec2(uv.z, uv.y) };
			tr = { pos + halfDim						 , color, glm::vec2(uv.z, uv.w) };
			tl = { pos + glm::vec2(-halfDim.x, halfDim.y), color, glm::vec2(uv.x, uv.w) };
		}		

		Glyph(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4& uv, GLuint _textureID, RGBA8 color, float _depth, float angle) {
			textureID = _textureID;
			depth = _depth;

			glm::vec2 halfDim(dimensions / 2.0f);

			bl = { pos - RotatePoint(halfDim, angle)						  , color, glm::vec2(uv.x, uv.y) };
			br = { pos + RotatePoint(glm::vec2(halfDim.x, -halfDim.y), angle) , color, glm::vec2(uv.z, uv.y) };
			tr = { pos + RotatePoint(halfDim, angle)				          , color, glm::vec2(uv.z, uv.w) };
			tl = { pos + RotatePoint(glm::vec2(-halfDim.x, halfDim.y), angle) , color, glm::vec2(uv.x, uv.w) };
		}

		Glyph(const glm::vec4& rect, const glm::vec4& uv, GLuint _textureID, RGBA8 color, float _depth) {
			textureID = _textureID;
			depth = _depth;

			bl = { glm::vec2(rect.x, rect.y)	, color, glm::vec2(uv.x, uv.y) };
			br = { glm::vec2(rect.x + rect.z, rect.y)	, color, glm::vec2(uv.z, uv.y) };
			tr = { glm::vec2(rect.x + rect.z, rect.y + rect.w)	, color, glm::vec2(uv.z, uv.w) };
			tl = { glm::vec2(rect.x, rect.y + rect.w)	, color, glm::vec2(uv.x, uv.w) };
		}

		Vertex bl;
		Vertex br;
		Vertex tr;
		Vertex tl;
		float depth;
		GLuint textureID;

		glm::vec2 RotatePoint(const glm::vec2& originalPoint, float angleInDegrees) {
			float angle = angleInDegrees * PI / 180.0f;
			glm::vec2 newPoint;
			newPoint.x = originalPoint.x * cos(angle) - originalPoint.y * sin(angle);
			newPoint.y = originalPoint.x * sin(angle) + originalPoint.y * cos(angle);
			return newPoint;
		}
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint _offset, GLuint _numVerts, GLuint _textureID) : offset(_offset), numVerts(_numVerts), textureID(_textureID) {}
		GLuint offset;
		GLuint numVerts;
		GLuint textureID;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch(const std::string& shaderPath = "Assets/default.shader") {
			if (m_VAO == 0) glGenVertexArrays(1, &m_VAO);
			glBindVertexArray(m_VAO);

			if (m_VBO == 0) glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

			m_Shader.ParseShader(shaderPath);
			m_Shader.AddAttribute("vertexPosition");
			m_Shader.AddAttribute("vertexColor");
			m_Shader.AddAttribute("vertexUV");
			m_Shader.CreateShaderProgram();
		}

		void DrawSpriteLeftBottom(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color);
		void DrawSprite(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color);
		void DrawSprite(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color, float angle);
		void Render(Camera2D& camera);

	private:
		GLuint m_VAO = 0, m_VBO = 0;
		ShaderProgram m_Shader;

		std::vector<RenderBatch> m_RenderBatches;
		std::vector<Glyph> m_Glyphs;
		std::vector<Glyph*> m_GlyphPointers;

		void Begin();
		void End();
		void CreateRenderBatches();
		static bool CompareTexture(Glyph* _glyphA, Glyph* _glyphB);
	};	
}

