
#include "Texture2D.h"

#include <stb/stb_image.h>
#include <iostream>

#include "../Common.h"

namespace pg2D {
	Texture Texture2D::LoadPNG(const std::string& filePath) {
		Texture image{ 0, 0, 0, 0};

		stbi_set_flip_vertically_on_load(1);
		unsigned char* imageBuffer = stbi_load(filePath.c_str(), &image.width, &image.height, &image.bpp, 4);

		if (stbi_failure_reason())
			PG_CORE_ERROR("STB_IMAGE Failed to load Image at ({0}), failure reason: {1}", filePath, stbi_failure_reason());

		glGenTextures(1, &image.textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, image.textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		if (imageBuffer) stbi_image_free(imageBuffer);

		return image;
	}
}
