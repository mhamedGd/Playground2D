#pragma once
#include <string>
#include <glew/glew.h>

#include "Texture.h"

namespace pg2D {
	class Texture2D
	{
	public:
		static Texture LoadPNG(const std::string& filePath);
	};	
}
