#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../EntryPoint/Window.h"

namespace pg2D {
	class Camera2D
	{
	public:
		Camera2D(){}
		Camera2D(Window* window) {
			m_WindowPointer = window;
			m_ProjMatrix = glm::ortho(0.0f, (float)m_WindowPointer->GetWidth(), 0.0f, (float)m_WindowPointer->GetHeight());
		}

		virtual void Update();
		void SetPosition(glm::vec2 _position);
		void SetScale(float _scale);

		const glm::vec2 ScreenPointToWorld (glm::vec2 _screenPoint) {
			glm::vec2 screenPoint = _screenPoint;
			screenPoint.y = m_WindowPointer->GetHeight() - screenPoint.y;

			screenPoint -= glm::vec2(m_WindowPointer->GetWidth() / 2, m_WindowPointer->GetHeight() / 2);
			screenPoint /= m_Scale;
			screenPoint += m_Position;

			return screenPoint;
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		const glm::vec2& GetPosition() const { return m_Position; }
		const float GetScale() const { return m_Scale; }

	protected:
		glm::mat4 m_ViewMatrix = glm::mat4(0.0f);
		glm::mat4 m_ProjMatrix = glm::mat4(0.0f);

		glm::vec2 m_Position = glm::vec2(0.0f);
		Window* m_WindowPointer = nullptr;
		float m_Scale = 1.0f;
	};
}

