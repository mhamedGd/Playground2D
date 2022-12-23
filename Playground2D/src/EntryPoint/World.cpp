

#include "World.h"
#include "../Input/Input.h"

namespace pg2D {

	Input* Input::s_Instance = new Input();

	void World::Run() {
		Input::s_Instance->m_GLFWWindow = m_Window->GetWindow();

		Start();

		while (m_Running) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_InitializatoinTime = (float)glfwGetTime();
			m_DeltaTime = m_InitializatoinTime - m_LastFrameTime;
			m_LastFrameTime = m_InitializatoinTime;

			glm::ivec2 currentMousePos = Input::MousePosition();
			Input::SetDeltaMousePosition(currentMousePos - m_LastMousePosition);
			m_LastMousePosition = currentMousePos - glm::ivec2(0, m_Window->GetHeight());

			Update();
			Draw();

			m_Window->Update();
		}
	}

	void World::Start() {
		for (auto& s : m_ExistingScenes) {
			s->OnStart();
		}
	}

	void World::Update() {
		for (auto& s : m_ExistingScenes) {
			s->OnUpdate();
			s->m_DeltaTime = m_DeltaTime;
		}
	}

	void World::Draw() {
		for (auto& s : m_ExistingScenes) {
			s->OnDraw();
		}
	}

	void World::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_ExistingScenes.end(); it != m_ExistingScenes.begin();) {
			(*--it)->OnEvent(e);
			if (e.IsHandled()) break;
		}
	}

	void World::AddScene(const std::string& sceneName) {
		m_ExistingScenes.push_back(m_ScenesList->GetRegisteredScene(sceneName));
		m_ExistingScenes.back()->SetSceneIndex(m_ExistingScenes.size() - 1);
		//m_ExistingScenes.back()->OnStart();
	}
	void World::RemoveScene(const std::string& sceneName) {
		int sceneIndex = -1;
		for (size_t i = 0; i < m_ExistingScenes.size(); i++) {
			if (sceneName == m_ExistingScenes[i]->GetSceneName()) {
				sceneIndex = i;
				break;
			}
		}

		// DO ERROR IF SCENE DOESN'T EXIST
		if (sceneIndex == -1) return;

		m_ExistingScenes[sceneIndex] = m_ExistingScenes.back();
		m_ExistingScenes.pop_back();

		for (size_t i = sceneIndex; i < m_ExistingScenes.size(); i++) {
			if (sceneName == m_ExistingScenes[i]->GetSceneName()) {
				m_ExistingScenes[i]->SetSceneIndex(i);
			}
		}
	}

	ScenesList* World::GetScenesList() { return m_ScenesList.get(); }

	bool World::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool World::OnWindowResize(WindowResizeEvent& e) {
		glViewport(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight());
		return true;
	}
}
