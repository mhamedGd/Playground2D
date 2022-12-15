
#include "SpriteBatch.h"


#include <algorithm>

namespace pg2D {
	void SpriteBatch::Begin() {
		m_RenderBatches.clear();
		m_Glyphs.clear();
	}

	void SpriteBatch::DrawSprite(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color) {
		m_Glyphs.emplace_back(pos, dimensions, uvRect, textureID, color, 0.0f);
	}

	void SpriteBatch::DrawSpriteLeftBottom(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color) {
		m_Glyphs.emplace_back(glm::vec4(pos.x, pos.y, dimensions.x, dimensions.y), uvRect, textureID, color, 0.0f);		
	}

	void SpriteBatch::DrawSprite(const glm::vec2& pos, const glm::vec2& dimensions, const glm::vec4 uvRect, GLuint textureID, RGBA8 color, float angle) {
		m_Glyphs.emplace_back(pos, dimensions, uvRect, textureID, color, 0.0f, angle);
	}

	void SpriteBatch::End() {
		m_GlyphPointers.resize(m_Glyphs.size());
		for (size_t i = 0; i < m_Glyphs.size(); i++) {
			m_GlyphPointers[i] = &m_Glyphs[i];
		}

		std::stable_sort(m_GlyphPointers.begin(), m_GlyphPointers.end(), CompareTexture);
		CreateRenderBatches();
	}

	void SpriteBatch::Render(Camera2D& camera) {
		End();

		m_Shader.Use();
		glUniformMatrix4fv(m_Shader.GetUniformLocation("MVP"), 1, GL_FALSE, &camera.GetViewMatrix()[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(m_Shader.GetUniformLocation("genericSampler"), 0);

		glBindVertexArray(m_VAO);
		for (size_t i = 0; i < m_RenderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, m_RenderBatches[i].textureID);
			glDrawArrays(GL_TRIANGLES, m_RenderBatches[i].offset, m_RenderBatches[i].numVerts);
		}
		glBindVertexArray(0);
		m_Shader.Unuse();
		
		Begin();
	}

	void SpriteBatch::CreateRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(m_GlyphPointers.size() * 6);

		if (m_GlyphPointers.empty()) return;

		int offset = 0;
		int vertex_Nums = 0;

		m_RenderBatches.emplace_back(offset, 6, m_GlyphPointers[0]->textureID);
		vertices[vertex_Nums++] = m_GlyphPointers[0]->bl;
		vertices[vertex_Nums++] = m_GlyphPointers[0]->tr;
		vertices[vertex_Nums++] = m_GlyphPointers[0]->tl;
		vertices[vertex_Nums++] = m_GlyphPointers[0]->bl;
		vertices[vertex_Nums++] = m_GlyphPointers[0]->br;
		vertices[vertex_Nums++] = m_GlyphPointers[0]->tr;
		offset += 6;

		for (size_t i = 1; i < m_GlyphPointers.size(); i++) {
			if (m_GlyphPointers[i - 1]->textureID != m_GlyphPointers[i]->textureID) m_RenderBatches.emplace_back(offset, 6, m_GlyphPointers[i]->textureID);
			else m_RenderBatches.back().numVerts += 6;

			vertices[vertex_Nums++] = m_GlyphPointers[i]->bl;
			vertices[vertex_Nums++] = m_GlyphPointers[i]->tr;
			vertices[vertex_Nums++] = m_GlyphPointers[i]->tl;
			vertices[vertex_Nums++] = m_GlyphPointers[i]->bl;
			vertices[vertex_Nums++] = m_GlyphPointers[i]->br;
			vertices[vertex_Nums++] = m_GlyphPointers[i]->tr;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bool SpriteBatch::CompareTexture(Glyph* _glyphA, Glyph* _glyphB) {
		return _glyphA->textureID < _glyphB->textureID;
	}

}
