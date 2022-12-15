#pragma once

#ifdef PLAYGROUND_2D
#define GLEW_STATIC
#endif

#include "Common.h"

#include "EntryPoint/World.h"
#include "EntryPoint/Scene.h"
#include "EntryPoint/ScenesList.h"
#include "Input/Input.h"
#include "Input/InputKeys.h"

#include "Camera/Camera2D.h"
#include "Camera/HudCamera.h"

#include "Graphics/ShapeBatch.h"
#include "Graphics/SpriteBatch.h"
#include "Graphics/FontBatch.h"
#include "Graphics/Texture.h"
#include "Graphics/Texture2D.h"

#include "Timing.h"


#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

using namespace pg2D::Timing;

namespace pg2D {
	extern void Init();
}
