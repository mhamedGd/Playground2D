#pragma once

#include <Playground2D.h>
#include <glm/glm.hpp>

using namespace pg2D;

class Point;
class Rect;
class ImageRectScene;


using OnMousePointClick = std::function<void(Point*)>;
using OnMousePointRelease = std::function<void(Point*)>;
using OnMousePointDrag = std::function<void(Point*, Camera2D*)>;

class Point {
	friend class Rect;
public:
	Point() {}
	Point(glm::vec2& _position, float _radius) {
		radius = _radius;
		SetPosition(_position);
		color = normalColor;
	}

	void SetMouseClickCallback(const OnMousePointClick callback) { onMouseClick = callback; }
	void SetMouseReleaseCallback(const OnMousePointRelease callback) { onMouseRelease = callback; }
	void SetMouseDragCallback(const OnMousePointDrag callback) { onMouseDrag = callback; }

	void SetPosition(const glm::vec2& _position) {
		position = _position;

		tl.x = position.x - radius;
		tl.y = position.y + radius;

		tr.x = position.x + radius;
		tr.y = position.y + radius;

		bl.x = position.x - radius;
		bl.y = position.y - radius;

		br.x = position.x + radius;
		br.y = position.y - radius;
	}

	bool IsPointInside(glm::vec2& _point) {
		if ((_point.x > tl.x && _point.y < tl.y) &&
			(_point.x < tr.x && _point.y < tr.y) &&
			(_point.x > bl.x && _point.y > bl.y) &&
			(_point.x < br.x && _point.y > bl.y))
		{
			return true;
		}
		return false;
	}

	void Update(Camera2D& _camera, glm::vec2& _point) {
		if (IsPointInside(_point) || !alreadyReleased) {
			if (Input::IsMouseButtonPressed(PG_MOUSE_BUTTON_1)) {
				if (onMouseDrag != NULL) onMouseDrag(this, &_camera);
				glm::ivec2 mouseDelta = Input::DeltaMousePosition();
				mouseDelta = _camera.ScreenPointToWorld((glm::vec2)mouseDelta);
				/* ---- Setting Position ---- */
				SetPosition(position + (glm::vec2)mouseDelta);
				if (adjacent01 != nullptr) {
					adjacent01->SetPosition(glm::vec2(position.x, adjacent01->position.y));
					adjacent02->SetPosition(glm::vec2(adjacent02->position.x, position.y));
				}
				/* -------------------------- */
				AlterVisualState(pressColor);
				if (alreadyReleased) {
					alreadyReleased = false;
					if (onMouseClick != NULL) onMouseClick(this);
				}
				return;
			}
			else {
				if (!alreadyReleased) {
					alreadyReleased = true;
					if (onMouseRelease != NULL) onMouseRelease(this);
				}
			}
			AlterVisualState(hoverColor);
		}
		else {
			AlterVisualState(normalColor);
		}
	}

private:
	bool alreadyReleased = true;
	OnMousePointClick onMouseClick;
	OnMousePointRelease onMouseRelease;
	OnMousePointDrag onMouseDrag;

	Point* adjacent01 = nullptr;
	Point* adjacent02 = nullptr;

	glm::vec2 position = glm::vec2(0.0f);
	float radius = 6.0f;

	glm::vec2 tl = glm::vec2(0.0f);
	glm::vec2 tr = glm::vec2(0.0f);
	glm::vec2 bl = glm::vec2(0.0f);
	glm::vec2 br = glm::vec2(0.0f);

	RGBA8 color;
	RGBA8 normalColor = { 100, 100, 150, 255 };
	RGBA8 hoverColor = { 200, 255, 0, 255 };
	RGBA8 pressColor = { 255, 0, 0, 255 };

	void AlterVisualState(const RGBA8& _color) {
		color = _color;
	}
};

class Rect {
	friend class ImageRectScene;
public:
	Rect() {}
	Rect(glm::vec2 _position, glm::vec2 _dimensions, GLuint _textureID) {
		m_TextureID = _textureID;
		dimensions = _dimensions;
		m_Points.resize(4);
		for (size_t i = 0; i < 4; i++) {
			m_Points[i] = new Point(glm::vec2(0.0f), 4.0);
			m_Points[i]->SetMouseClickCallback(std::bind(&Rect::ClickCallBack, this, std::placeholders::_1));
			m_Points[i]->SetMouseReleaseCallback(std::bind(&Rect::ReleaseCallBack, this, std::placeholders::_1));
		}
		m_Points[0]->adjacent01 = m_Points[2];
		m_Points[0]->adjacent02 = m_Points[1];

		m_Points[1]->adjacent01 = m_Points[3];
		m_Points[1]->adjacent02 = m_Points[0];

		m_Points[2]->adjacent01 = m_Points[0];
		m_Points[2]->adjacent02 = m_Points[3];

		m_Points[3]->adjacent01 = m_Points[1];
		m_Points[3]->adjacent02 = m_Points[2];

		SetPosition(_position);
		centerPoint = new Point(position, 10.0f);
		centerPoint->SetMouseDragCallback(std::bind(&Rect::CenterDragCallback, this, std::placeholders::_1, std::placeholders::_2));
		centerPoint->SetMouseReleaseCallback(std::bind(&Rect::ReleaseCallBack, this, std::placeholders::_1));
		centerPoint->hoverColor = { 0, 144, 255, 255 };
	}

	~Rect() {
		for (auto& p : m_Points) {
			delete p;
		}
	}

	void Update(Camera2D c, glm::vec2& _point) {
		for (size_t i = 0; i < 4; i++) {
			if (appointedPoint == nullptr || m_Points[i] == appointedPoint) {
				m_Points[i]->Update(c, _point);
				if (!m_Points[i]->alreadyReleased) return;
			}
		}

		centerPoint->Update(c, _point);
	}

	void Draw(ShapeBatch& sp, SpriteBatch& spriteB) {
		for (size_t i = 0; i < 4; i++) {
			sp.DrawCirlce(m_Points[i]->position, m_Points[i]->radius, m_Points[i]->color);
		}

		sp.DrawCirlce(centerPoint->position, centerPoint->radius, centerPoint->color);

		glm::vec2 dimensions = m_Points[1]->position - m_Points[2]->position;
		spriteB.DrawSpriteLeftBottom(m_Points[2]->position, dimensions, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), m_TextureID, { 255, 255, 255, 255 });
	}


	void SetTextureID(GLuint _textureID) {
		m_TextureID = _textureID;
	}

	bool IsInsidePoint(glm::vec2 _point) {
		for (auto& p : m_Points) {
			if (p->IsPointInside(_point)) return true;
		}

		if (centerPoint->IsPointInside(_point)) return true;

		return false;
	}

private:
	void ClickCallBack(Point* p) { appointedPoint = p; pressed = true; }
	void ReleaseCallBack(Point* p) {
		appointedPoint = nullptr;
		position = glm::vec2((m_Points[0]->position.x + m_Points[1]->position.x) / 2, (m_Points[1]->position.y + m_Points[2]->position.y) / 2);
		centerPoint->SetPosition(position);
		pressed = false;
	}

	void CenterDragCallback(Point* cp, Camera2D* c) {
		glm::ivec2 mouseDelta = Input::DeltaMousePosition();
		mouseDelta = (glm::ivec2)c->ScreenPointToWorld((glm::vec2)mouseDelta);

		position += glm::vec2(mouseDelta);
		cp->SetPosition(position);

		for (auto& p : m_Points) {
			p->SetPosition(p->position + (glm::vec2)mouseDelta);
		}
		pressed = true;
	}

	void SetPosition(glm::vec2 _position) {
		position = _position;

		m_Points[0]->SetPosition(glm::vec2(position.x - dimensions.x / 2, position.y + dimensions.y / 2));
		m_Points[1]->SetPosition(glm::vec2(position.x + dimensions.x / 2, position.y + dimensions.y / 2));

		m_Points[2]->SetPosition(glm::vec2(position.x - dimensions.x / 2, position.y - dimensions.y / 2));
		m_Points[3]->SetPosition(glm::vec2(position.x + dimensions.x / 2, position.y - dimensions.y / 2));
	}

	glm::vec2 position;
	glm::vec2 dimensions;

	bool pressed = false;
	std::vector<Point*> m_Points;
	Point* appointedPoint = nullptr;
	Point* centerPoint = nullptr;

	GLuint m_TextureID;
};

class ImageRectScene : public Scene {
public:
	ImageRectScene(const std::string& sceneName) : Scene(sceneName) {}

	~ImageRectScene() {
		for (auto& r : m_Rects) {
			delete r;
		}
	}

	virtual void OnStart() override {
		m_HudCamera = HudCamera(ownerWorld->GetWindow());

		m_Texture = Texture2D::LoadPNG("Assets/Playground2D.png");

		for (size_t i = 0; i < 1; i++) {
			m_Rects.push_back(new Rect(glm::vec2(150.0f * (i + 1), 100.0f), glm::vec2(m_Texture.width/4, m_Texture.height/4), m_Texture.textureID));
		}
	}

	virtual void OnUpdate() override {
		m_HudCamera.Update();

		for (auto& r : m_Rects) {
			if (appointedRect == r || appointedRect == nullptr)
				r->Update(m_HudCamera, m_ScreenMousePos);
		}

		m_ScreenMousePos = m_HudCamera.ScreenPointToWorld(Input::MousePosition());
		m_DeltaScreenMouse = (glm::ivec2)m_HudCamera.ScreenPointToWorld((glm::vec2)Input::DeltaMousePosition());
	}

	virtual void OnDraw() override {
		for (auto& r : m_Rects) {
			r->Draw(m_ShapeBatch, m_SpriteBatch);
		}

		m_SpriteBatch.Render(m_HudCamera);
		m_ShapeBatch.Render(m_HudCamera, 2.0f);
	}

	virtual void OnEvent(Event& e) override {
		if (e.GetEventType() == EventType::MouseButtonPressed) {
			if (Input::IsMouseButtonPressed(PG_MOUSE_BUTTON_1) && appointedRect == nullptr) {
				for (auto& r : m_Rects) {
					if (r->IsInsidePoint(m_ScreenMousePos)) appointedRect = r;
				}
			}
		}
		if (e.GetEventType() == EventType::MouseButtonReleased) {
			appointedRect = nullptr;
		}
	}

private:
	Rect* appointedRect = nullptr;
	std::vector<Rect*> m_Rects;

	Texture m_Texture;

	ShapeBatch m_ShapeBatch;
	SpriteBatch m_SpriteBatch;

	HudCamera m_HudCamera;

	glm::vec2 m_ScreenMousePos;
	glm::ivec2 m_DeltaScreenMouse;

};
