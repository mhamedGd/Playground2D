#pragma once

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Window.h"
#include "ScenesList.h"
#include "Scene.h"
#include "../Timing.h"

#include "../Common.h"
#include "../Events/Event.h"
#include "../Events/ApplicationEvent.h"


namespace pg2D {

#define BIND_EVENT_FN(x) std::bind(&World::x, this, std::placeholders::_1)

	class World
	{
	public:
		World(WindowProperties& windowProps) {
			Logger::Init();

			m_Window = std::make_unique<Window>();
			m_Window->Create(windowProps);
			//m_Window->SetEventCallback(std::bind(&World::OnEvent, this, std::placeholders::_1));
			m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

			//Input::s_Instance->m_GLFWWindow = m_Window->GetWindow();
			
			if (glewInit() != GLEW_OK) PG_CORE_ERROR("GLEW Initialization Failed");
			glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

			m_ScenesList = std::make_unique<ScenesList>(this);

			PG_CORE_INFO("World Generated With Window Name ({0}), and dimensions ({1}, {2})", windowProps.label, windowProps.width, windowProps.height);
		}

		~World() {
			m_ScenesList->DisposeOfScenes();
			m_Window->Dispose();
		}

		// Called On User End
		void Run();
		
		void Start();
		void Update();
		void Draw();
		void OnEvent(Event& e);

		void AddScene(const std::string& sceneName);
		void RemoveScene(const std::string& sceneName);
		
		ScenesList* GetScenesList();

		Window* GetWindow() { return m_Window.get(); }

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		const float TimeSinceInitialization() const { return m_InitializatoinTime; }

	private:
		bool m_Running = true;
		std::unique_ptr<Window> m_Window;
		std::vector<Scene*> m_ExistingScenes;
		std::unique_ptr<ScenesList> m_ScenesList;

		glm::ivec2 m_LastMousePosition = glm::vec2(0.0f);
		float m_LastFrameTime = 0.0f;
		Timing::TimeStep m_DeltaTime = 0.0f;
		float m_InitializatoinTime = 0.0f;

	};
}
