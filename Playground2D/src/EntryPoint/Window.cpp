
#include "Window.h"

#include <iostream>

#include "../Common.h"
#include "../Events/ApplicationEvent.h"
#include "../Events/KeyEvent.h"
#include "../Events/MouseEvent.h"

namespace pg2D {
	static void GLFWErrorCallback(int error, const char* description) {
		PG_CORE_ERROR("GLFW Error ({0}, {1})", error, description);
	}


	void Window::Create(WindowProperties& windowProps) {
		m_WindowData.width = windowProps.width;
		m_WindowData.height = windowProps.height;
		m_WindowData.label = windowProps.label;

		if (!glfwInit()) PG_CORE_ERROR("GLFW Initialization Failed");
		glfwSetErrorCallback(GLFWErrorCallback);

		glfwWindowHint(GLFW_RESIZABLE, (windowProps.WindowConfigurations & WindowConfigurations::UNRESIZABLE) == WindowConfigurations::UNRESIZABLE ? GL_FALSE : GL_TRUE);
		glfwWindowHint(GLFW_MAXIMIZED, (windowProps.WindowConfigurations & WindowConfigurations::MAXIMIZED) == WindowConfigurations::MAXIMIZED ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, (windowProps.WindowConfigurations & WindowConfigurations::INVISIBLE) == WindowConfigurations::INVISIBLE ? GL_TRUE : GL_FALSE);

		m_Window = glfwCreateWindow(m_WindowData.width, m_WindowData.height, m_WindowData.label.c_str(), 0, 0);

		if (!m_Window) {
			PG_CORE_ERROR("GLFW Window Has No Value");
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_WindowData);
		glfwSwapInterval(1);

		// ---------------- Setting GLFW Callbacks -----------------
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
		// -----------------------------------------

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	void Window::Update() {
		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}
