#pragma once

#include "Scene.h"

#include <vector>
#include <string>

namespace pg2D {
	class World;

	class ScenesList
	{
	public:
		ScenesList(World* _ownerWolrd) {
			ownerWorld = _ownerWolrd;
		}
		~ScenesList() {}
		// Scenes Can't Be Accessed In World.cpp Unless They're Registered
		void RegisterScene(Scene* sceneToRegister);

		Scene* GetRegisteredScene(int sceneIndex);
		Scene* GetRegisteredScene(const std::string& sceneName);

		void DisposeOfScenes();

	private:
		std::vector<Scene*> m_RegisteredScenes;
		World* ownerWorld = nullptr;
	};
}

