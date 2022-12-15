#pragma once
#include "Camera2D.h"

namespace pg2D {
class HudCamera : public Camera2D {
public:
	HudCamera() {}
	HudCamera(Window* window) : Camera2D(window) {}

	void Update() override;
};
}

