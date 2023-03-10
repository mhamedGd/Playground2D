#define PLAYGROUND_2D

#include <Playground2D.h>

#include "Examples/ImageRect.h"

using namespace pg2D;


int main() {
	WindowProperties windowProps = { 1366, 768, "ImageRect Test", WindowConfigurations::UNRESIZABLE };
	World world(windowProps);

	world.GetScenesList()->RegisterScene(new ImageRectScene("ImageRect"));
	world.AddScene("ImageRect");

	world.Run();
	return 0;
}