
#include "HudCamera.h"

namespace pg2D {
	void HudCamera::Update() {
		float width = (float)m_WindowPointer->GetWidth();
		float height = (float)m_WindowPointer->GetHeight();
		m_ProjMatrix = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

		glm::vec3 translate(-m_Position.x + width / 2, -m_Position.y + height / 2, 0.0f);
		m_ViewMatrix = glm::translate(m_ProjMatrix, translate);

		glm::vec3 vec3_Scale = glm::vec3(m_Scale, m_Scale, 0.0f);
		m_ViewMatrix = glm::scale(glm::mat4(1.0f), vec3_Scale) * m_ViewMatrix;
		SetPosition(glm::vec2((float)m_WindowPointer->GetWidth() / (2.0f * m_Scale), (float)m_WindowPointer->GetHeight() / (2.0f * m_Scale)));
	}
}