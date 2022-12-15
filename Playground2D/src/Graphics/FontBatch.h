#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include <glew/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Common.h"
#include "SpriteBatch.h"
#include "../Camera/Camera2D.h"

namespace pg2D {

	struct CharGlyph {
		GLuint textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned int advance;
	};

	struct FontBatchSettings {
		float fontSize;
		float lineHeight;
	};

	class FontBatch
	{
	public:
		FontBatch(const std::string& fontPath, const FontBatchSettings& fontBatchSettings = { 32.0f, 32.0f }) {
			fontSettings = fontBatchSettings;

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			FT_Library ft;
			if (FT_Init_FreeType(&ft)) PG_CORE_ERROR("FREETYPE INITIALIZATION FAILED");
			
			FT_Face face;
			if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) PG_CORE_ERROR("FREETYPE FAILED TO LOAD FONT");

			FT_Set_Pixel_Sizes(face, 0, fontSettings.fontSize);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (unsigned char c = 0; c < 128; c++) {
				//7th Parameter -> ASCII start offset, 8th Parameter -> offset + 8th parameter
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					PG_CORE_WARN("FREE TYPE FAILED TO LOAD GLYPH: {0}", c);
					continue;
				}

				GLuint textureID;
				glGenTextures(1, &textureID);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);

				CharGlyph thisChar = {
					textureID,
					glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					static_cast<unsigned int>(face->glyph->advance.x)
				};

				//m_CharMap.insert(std::make_pair((char)i, thisChar));
				m_CharMap[c] = thisChar;
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}

		~FontBatch() {}

		void DrawString(const std::string& text, glm::vec2 pos, float scale, const RGBA8& color);
		void Render(Camera2D& camera);

	private:
		SpriteBatch m_SpriteBatch = SpriteBatch("Assets/font.shader");

		FontBatchSettings fontSettings;
		std::unordered_map<char, CharGlyph> m_CharMap;

		float cameraScale = 32.0f;
		float cameraRelatedScale = 0.0f;
	};
}
