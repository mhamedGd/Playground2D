#pragma once

#include <glfw/glfw3.h>

#include <string>
#include "../Events/Event.h"

namespace pg2D {

	enum class WindowConfigurations {
		NONE		= 0b00000000,
		UNRESIZABLE = 0b00000001,
		MAXIMIZED   = 0b00000010,
		// IF SUPPORTED BY THE SYSTEM
		INVISIBLE   = 0b00000100
	};

	inline WindowConfigurations operator|(WindowConfigurations lhs, WindowConfigurations rhs) {
		return (WindowConfigurations)((unsigned int)(lhs) | (unsigned int)(rhs));
	}
	inline WindowConfigurations operator&(WindowConfigurations lhs, WindowConfigurations rhs) {
		return (WindowConfigurations)((unsigned int)(lhs) & (unsigned int)(rhs));
	}

	struct WindowProperties {
		int width, height;
		std::string label;
		WindowConfigurations WindowConfigurations = WindowConfigurations::NONE;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window() {}
		~Window() {}
		
		void Dispose() {
			glfwDestroyWindow(m_Window);
			glfwTerminate();
		}

		void Create(WindowProperties& windowProps);
		void Update();
		
		void SetEventCallback(const EventCallbackFn callback) { m_WindowData.EventCallback = callback; }

		int GetWidth() const { return m_WindowData.width; }
		int GetHeight() const { return m_WindowData.height; }

		GLFWwindow* GetWindow() { return m_Window; }

	private:
		GLFWwindow* m_Window = nullptr;
		int m_Width = 0, m_Height = 0;

		struct WindowData {
			int height = 1360, width = 768;
			std::string label = "";

			EventCallbackFn EventCallback;
		};

		WindowData m_WindowData;

	};
}

