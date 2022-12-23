
#include "ScenesList.h"


namespace pg2D {
	void ScenesList::RegisterScene(Scene* sceneToRegister) {
		m_RegisteredScenes.push_back(sceneToRegister);
		m_RegisteredScenes.back()->SetOwnerWorld(ownerWorld);
	}

	Scene* ScenesList::GetRegisteredScene(int sceneIndex) {
		return m_RegisteredScenes[sceneIndex];
	}

	Scene* ScenesList::GetRegisteredScene(const std::string& sceneName) {
		for (auto& s : m_RegisteredScenes) {
			if (sceneName == s->GetSceneName()) return s;
		}
		return nullptr;
	}

	void ScenesList::DisposeOfScenes() {
		for (Scene* s : m_RegisteredScenes) delete s;
	}
}
