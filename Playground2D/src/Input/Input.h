#pragma once

#include <glfw/glfw3.h>
#include <glm/glm.hpp>

namespace pg2D {
	class Input {
		friend class World;
	public:
		Input() {
			m_DeltaMousePosition = new glm::ivec2(0);
		}

		inline static bool IsKeyPressed(int keyCode) {
			auto state = glfwGetKey(s_Instance->m_GLFWWindow, keyCode);
			return state == GLFW_PRESS || state == GLFW_REPEAT;
		}

		inline static bool IsMouseButtonPressed(int button) {
			return glfwGetMouseButton(s_Instance->m_GLFWWindow, button) == GLFW_PRESS;
		}

		inline static glm::ivec2 MousePosition() {
			double xPos, yPos;
			glfwGetCursorPos(s_Instance->m_GLFWWindow, &xPos, &yPos);
			return glm::ivec2(xPos, yPos);
		}
		inline static float MousePositionX() {
			return MousePosition().x;
		}
		inline static float MousePositionY() {
			return MousePosition().y;
		}

		inline static glm::ivec2 DeltaMousePosition() {
			return *s_Instance->m_DeltaMousePosition;
		}
		inline static int DeltaMousePositionX() {
			return s_Instance->m_DeltaMousePosition->x;
		}
		inline static int DeltaMousePositionY() {
			return s_Instance->m_DeltaMousePosition->y;
		}	

	private:
		static Input* s_Instance;
		GLFWwindow* m_GLFWWindow = nullptr;
		glm::ivec2* m_DeltaMousePosition = nullptr;

		inline static void SetDeltaMousePosition(glm::ivec2& _newPos) {
			*(s_Instance->m_DeltaMousePosition) = _newPos;
		}
	};

}