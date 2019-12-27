#pragma once


struct UIRect {
	glm::vec2 pos;
	glm::vec2 size;

	UIRect() {}

	UIRect(const glm::vec2& pos, const glm::vec2& size);

	glm::vec2 getPos();

	glm::vec2 getSize();

	float left();

	float right();

	float top();

	float bottom();

	bool check(const glm::vec2& point);
};

struct IUIComponent {
	virtual void init() = 0;
	virtual void handleEvents(SDL_Event& e) = 0;
	virtual void render(UIShader* shader) = 0;
	virtual void update(float delta) = 0;
	virtual void release() = 0;

	virtual glm::vec2 getPosition() = 0;
	virtual void setPosition(const glm::vec2& pos) = 0;

	virtual UIRect toRect() = 0;
};

struct UILabelComponent : public IUIComponent {
	glm::vec2 position;
	std::string title;
	glm::vec3 color;

	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	std::string getTitle();
	void setTitle(std::string title);

	void setColor(const glm::vec3& color);
	glm::vec3 getColor();

	virtual UIRect toRect();
};

struct UIButtonComponent : public IUIComponent {
	glm::vec2 position;
	glm::vec2 size;

	std::string title;
	glm::vec3 backgroundColor;
	glm::vec3 hoverColor;
	glm::vec3 activeColor;
	glm::vec3 foregroundColor;


	std::function<void(UIButtonComponent*)> buttonCallback;

	bool isHover = false;
	bool isActive = false;

	float activeTimer = 0.0f;
	float activeTimerMax = 0.2f;

	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	glm::vec2 getSize();
	void setSize(const glm::vec2& size);

	std::string getTitle();
	void setTitle(std::string title);

	glm::vec3 getBackgroundColor();
	void setBackgroundColor(const glm::vec3& backgroundColor);

	glm::vec3 getHoverColor();
	void setHoverColor(const glm::vec3& hoverColor);

	glm::vec3 getActiveColor();
	void setActiveColor(const glm::vec3& activeColor);

	glm::vec3 getForegroundColor();
	void setForegroundColor(const glm::vec3& foregroundColor);

	void setButtonCallback(std::function<void(UIButtonComponent*)> buttonCallback);

	virtual UIRect toRect();

};

struct UICheckBoxComponent : public IUIComponent {
	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	virtual UIRect toRect();
};

struct UIManager {

	std::vector<IUIComponent*> components;

	bool show = false;

	void addComponent(IUIComponent* comp);
	void removeComponent(IUIComponent* comp);

	void handleEvents(SDL_Event& e);

	void render(UIShader* shader);

	void update(float delta);

	void init();
	void release();

	bool isShow();
	void setShow(bool show);
	void toggleShow();
};


struct UISystem {
	static std::vector<UIManager*> managers;
	static QuadUIGeometry quad;

	static void init();
	static void release();

	static void addManager(UIManager* manager);
	static void removeManager(UIManager* manager);
};