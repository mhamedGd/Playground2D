
#include "FontBatch.h"

namespace pg2D {
	void FontBatch::DrawString(const std::string& text, glm::vec2 pos, float scale, const RGBA8& color) {
		glm::vec2 originalPos = pos;

		if (cameraRelatedScale == 0.0f) {
			cameraRelatedScale = scale / cameraScale;
		}

		const float LINE_HEIGHT = (fontSettings.lineHeight) * cameraRelatedScale;

		//This comes after calculating the LINE_HEIGHT so to not disturb the scale multiplier
		//scale /= cameraScale;

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			const auto& charGlyph = m_CharMap[*c];

			if (*c == '\n') {
				pos.y -= LINE_HEIGHT;
				pos.x = originalPos.x;
				continue;
			}

			float xpos = pos.x + charGlyph.bearing.x * cameraRelatedScale;
			float ypos = pos.y - (charGlyph.size.y - charGlyph.bearing.y) * cameraRelatedScale;

			float width = charGlyph.size.x * cameraRelatedScale;
			float height = charGlyph.size.y * cameraRelatedScale;

			m_SpriteBatch.DrawSpriteLeftBottom(glm::vec2(xpos, ypos), glm::vec2(width, height), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), charGlyph.textureID, color);
			pos.x += (charGlyph.advance >> 6) * cameraRelatedScale;
		}
	}

	void FontBatch::Render(Camera2D& camera) {
		m_SpriteBatch.Render(camera);
		cameraScale = camera.GetScale();		
	}
}
