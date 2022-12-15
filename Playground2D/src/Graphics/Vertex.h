#pragma once

#include <glm/glm.hpp>

struct RGBA8 {
	unsigned char r, g, b, a;
};

struct Vertex {
	glm::vec2 position;
	RGBA8 color;
	glm::vec2 uv;
};
