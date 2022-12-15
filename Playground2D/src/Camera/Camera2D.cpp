
#include "Camera2D.h"

namespace pg2D {
	void Camera2D::Update() {
		float width = (float)m_WindowPointer->GetWidth();
		float height = (float)m_WindowPointer->GetHeight();
		m_ProjMatrix = glm::ortho(0.0f, width, 0.0f, height);

		glm::vec3 translate(-m_Position.x + width / 2, -m_Position.y + height / 2, 0.0f);
		m_ViewMatrix = glm::translate(m_ProjMatrix, translate);

		glm::vec3 vec3_Scale = glm::vec3(m_Scale, m_Scale, 0.0f);
		m_ViewMatrix = glm::scale(glm::mat4(1.0f), vec3_Scale) * m_ViewMatrix;
	}

	void Camera2D::SetPosition(glm::vec2 _position) {
		m_Position = _position;
	}
	
	void Camera2D::SetScale(float _scale) {
		m_Scale = _scale;
	}
}
