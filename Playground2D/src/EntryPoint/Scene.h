#pragma once

#include <string>

#include "../Timing.h"
#include "../Events/Event.h"

namespace pg2D {
	class World;
	class Scene
	{
	friend class World;
	public:
		//explicit So That Subclasses On User End Can Inherit it
		explicit Scene(const std::string& _sceneName) {
			sceneName = _sceneName;			
		}
		void SetSceneIndex(int _sceneIndex) { sceneIndex = _sceneIndex; }

		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnDraw() {}
		virtual void OnEvent(Event& e) {}

		const std::string& GetSceneName() const { return sceneName; }
		const int GetSceneIndex() const { return sceneIndex; }

		void SetOwnerWorld(World* _ownerWolrd) { ownerWorld = _ownerWolrd; }

	protected:
		World* ownerWorld;
		std::string sceneName = "";
		int sceneIndex = -1;

		Timing::TimeStep m_DeltaTime;
	};
}

