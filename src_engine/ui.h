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

	virtual void onShowEvent(bool show) = 0;

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

	virtual void onShowEvent(bool show);

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

	virtual void onShowEvent(bool show);


};

struct UICheckBoxComponent : public IUIComponent {

	glm::vec2 position;
	glm::vec2 size;

	std::string title;

	glm::vec3 foregroundColor;
	glm::vec3 backgroundColor;
	glm::vec3 inactiveColor;
	glm::vec3 activeColor;

	std::function<void(UICheckBoxComponent*)> callback;

	bool checked = false;
	bool isHover = false;

	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	glm::vec2 getSize();
	void setSize(const glm::vec2& size);

	//std::string title;
	std::string getTitle();
	void setTitle(std::string title);

	//glm::vec3 foregroundColor;
	glm::vec3 getForegroundColor();
	void setForegroundColor(const glm::vec3& foregroundColor);

	//glm::vec3 backgroundColor;
	glm::vec3 getBackgroundColor();
	void setBackgroundColor(const glm::vec3& backgroundColor);

	//glm::vec3 inactiveColor;
	glm::vec3 getInactiveColor();
	void setInactiveColor(const glm::vec3& inactiveColor);

	//glm::vec3 activeColor;
	glm::vec3 getActiveColor();
	void setActiveColor(const glm::vec3& activeColor);

	//std::function<void(UICheckBoxComponent*)> callback;
	void setCallback(std::function<void(UICheckBoxComponent*)> callback);

	//bool checked = false;
	bool isChecked();
	void setChecked(bool checked);

	virtual UIRect toRect();

	virtual void onShowEvent(bool show);

};

#define TEXT_BOX_SIZE 16
struct UITextBoxComponent : public IUIComponent {
	char buf[TEXT_BOX_SIZE];

	glm::vec2 position;
	glm::vec2 size;

	glm::vec3 foregroundColor;
	glm::vec3 backgroundColor;

	std::function<void(UITextBoxComponent*)> callback;

	bool isHover = false;
	bool isActive = false;

	bool isShift = false;

	int location = 0;

	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	glm::vec2 getSize();
	void setSize(const glm::vec2& size);

	glm::vec3 getForegroundColor();
	void setForegroundColor(const glm::vec3& foregroundColor);

	glm::vec3 getBackgroundColor();
	void setBackgroundColor(const glm::vec3& backgroundColor);

	void setCallback(std::function<void(UITextBoxComponent*)> callback);

	std::string getText();

	virtual UIRect toRect();

	virtual void onShowEvent(bool show);

};

struct UISelectButtonType {
	std::string id;
	uint32_t value;

	UISelectButtonType() {}

	UISelectButtonType(std::string id, uint32_t value) {
		this->id = id;
		this->value = value;
	}
};

struct UISelectButtonComponent : public IUIComponent {

	glm::vec2 position;
	glm::vec2 size;

	uint32_t index = 0;
	std::vector<UISelectButtonType> types;

	glm::vec3 backgroundColor;
	glm::vec3 hoverColor;
	glm::vec3 activeColor;
	glm::vec3 foregroundColor;

	bool isHover = false;
	bool isActive = false;

	float activeTimer = 0.0f;
	float activeTimerMax = 0.2f;

	std::function<void(UISelectButtonComponent*)> callback;

	virtual void init();

	virtual void handleEvents(SDL_Event& e);

	virtual void render(UIShader* shader);

	virtual void update(float delta);

	virtual void release();

	virtual glm::vec2 getPosition();

	virtual void setPosition(const glm::vec2& pos);

	glm::vec2 getSize();

	void setSize(const glm::vec2& size);

	glm::vec3 getForegroundColor();

	void setForegroundColor(const glm::vec3& foregroundColor);

	glm::vec3 getBackgroundColor();

	void setBackgroundColor(const glm::vec3& backgroundColor);

	glm::vec3 getActiveColor();

	void setActiveColor(const glm::vec3& activeColor);

	glm::vec3 getHoverColor();

	void setHoverColor(const glm::vec3& hoverColor);

	void setCallback(std::function<void(UISelectButtonComponent*)> callback);

	void addValue(const UISelectButtonType& value);

	std::string getCurrentID();

	uint32_t getCurrentValue();
	
	virtual UIRect toRect();

	virtual void onShowEvent(bool show);

};

struct UISliderComponent : public IUIComponent {

	glm::vec2 position;
	glm::vec2 size;

	glm::vec3 foregroundColor;
	glm::vec3 backgroundColor;
	glm::vec3 sliderColor;

	float min, max, value, p = 0.0f;

	bool isHover = false;

	virtual void init();
	virtual void handleEvents(SDL_Event& e);
	virtual void render(UIShader* shader);
	virtual void update(float delta);
	virtual void release();

	virtual glm::vec2 getPosition();
	virtual void setPosition(const glm::vec2& pos);

	glm::vec3 getForegroundColor();
	void setForegroundColor(const glm::vec3& foregroundColor);

	glm::vec3 getBackgroundColor();
	void setBackgroundColor(const glm::vec3& backgroundColor);

	glm::vec3 getSliderColor();
	void setSliderColor(const glm::vec3& sliderColor);

	float getMin();
	void setMin(float min);
	
	float getMax();
	void setMax(float max);

	float getValue();
	void setValue(float value);

	virtual UIRect toRect();

	float _doValueChange(float p);

	virtual void onShowEvent(bool show);

};



struct UIManager {
	bool hasBackground = false;

	glm::vec2 position;
	glm::vec2 size;

	glm::vec3 backgroundColor;

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


	void setHasBackground(bool hasBackground);
	bool isHasBackground();

	void setPosition(const glm::vec2& position);
	glm::vec2 getPosition();

	void setSize(const glm::vec2& size);
	glm::vec2 getSize();

	void setBackgroundColor(const glm::vec3& backgroundColor);
	glm::vec3 getBackgroundColor();

};


struct UISystem {
	static std::vector<UIManager*> managers;
	static QuadUIGeometry quad;

	static void init();
	static void release();

	static void addManager(UIManager* manager);
	static void removeManager(UIManager* manager);

	static int sizeManager();

};