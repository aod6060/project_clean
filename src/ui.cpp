#include "sys.h"

// UIRect
UIRect::UIRect(const glm::vec2& pos, const glm::vec2& size) {
	this->pos = pos;
	this->size = size;
}

glm::vec2 UIRect::getPos() {
	return this->pos;
}

glm::vec2 UIRect::getSize() {
	return this->size;
}

float UIRect::left() {
	return this->pos.x;
}

float UIRect::right() {
	return this->pos.x + this->size.x;
}

float UIRect::top() {
	return this->pos.y;
}

float UIRect::bottom() {
	return this->pos.y + this->size.y;
}

bool UIRect::check(const glm::vec2& point) {
	return
		this->left() <= point.x &&
		this->right() >= point.x &&
		this->top() <= point.y &&
		this->bottom() >= point.y;
}



// UILabelComponent
void UILabelComponent::init() {
	// i don't need this for the moment...
	//this->color = glm::vec3(1.0f);
	this->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

void UILabelComponent::handleEvents(SDL_Event& e) {
	// I don't need this for the moment...
}

void UILabelComponent::render(UIShader* shader) {
	FontRender::setColor(this->color);
	std::string temp = this->getTitle();

	if (temp.length() == 0) {
		temp = " ";
	}

	FontRender::print(this->position.x, this->position.y, temp.c_str());
}

void UILabelComponent::update(float delta) {
	// I don't need this for the moment...
}

void UILabelComponent::release() {
	// I don't need this for the moment...
}

glm::vec2 UILabelComponent::getPosition() {
	return this->position;
}

void UILabelComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

std::string UILabelComponent::getTitle() {
	return title;
}

void UILabelComponent::setTitle(std::string title) {
	this->title = title;
}

void UILabelComponent::setColor(const glm::vec3& color) {
	this->color = color;
}

glm::vec3 UILabelComponent::getColor() {
	return this->color;
}

UIRect UILabelComponent::toRect() {
	glm::vec2 size = FontRender::getSize(this->title.c_str());
	return UIRect(this->position, size);
}



// UIButtonComponent
void UIButtonComponent::init() {
	// Not real sure what to do here
	this->size = FontRender::getSize(this->title.c_str());
	this->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.7f));
	this->setHoverColor(glm::vec3(0.5f, 0.5f, 1.0f));
	this->setActiveColor(glm::vec3(0.7f, 0.0f, 0.0f));
	this->setForegroundColor(glm::vec3(1.0f));
}

void UIButtonComponent::handleEvents(SDL_Event& e) {
	if (e.type == SDL_MOUSEMOTION) {
		UIRect r = this->toRect();

		glm::vec2 point = glm::vec2(e.motion.x, e.motion.y);

		if (r.check(point)) {
			this->isHover = true;
		}
		else {
			this->isHover = false;
		}
	}
}

void UIButtonComponent::render(UIShader* shader) {
	// Render Background
	shader->bind();

	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
	);

	if (this->isHover) {
		if (this->isActive) {
			shader->setColor(this->activeColor);
		}
		else {
			shader->setColor(this->hoverColor);
		}
	}
	else {
		shader->setColor(this->backgroundColor);
	}

	UISystem::quad.render(shader);

	shader->unbind();

	// Render Title
	FontRender::setColor(this->foregroundColor);
	FontRender::print(position.x, position.y, this->title.c_str());


}

void UIButtonComponent::update(float delta) {
	if (this->isHover) {
		if (this->isActive) {
			if (this->activeTimer <= 0.0f) {
				this->isActive = false;
				this->activeTimer = 0.0f;
			}
			else {
				this->activeTimer -= delta;
			}
		}
		if (input_isIMFromConfDown("ui-action")) {
			this->isActive = true;
			this->activeTimer = this->activeTimerMax;
			this->buttonCallback(this);
		}
	}
	else {
		if (this->isActive) {
			this->isActive = false;
			this->activeTimer = 0.0f;
		}
	}
}

void UIButtonComponent::release() {
	// Not real sure what to do here
}

glm::vec2 UIButtonComponent::getPosition() {
	return this->position;
}

void UIButtonComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

glm::vec2 UIButtonComponent::getSize() {
	return this->size;
}

void UIButtonComponent::setSize(const glm::vec2& size) {
	this->size = size;
}

std::string UIButtonComponent::getTitle() {
	return this->title;
}

void UIButtonComponent::setTitle(std::string title) {
	this->title = title;
}

glm::vec3 UIButtonComponent::getBackgroundColor() {
	return this->backgroundColor;
}

void UIButtonComponent::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

glm::vec3 UIButtonComponent::getHoverColor() {
	return this->hoverColor;
}

void UIButtonComponent::setHoverColor(const glm::vec3& hoverColor) {
	this->hoverColor = hoverColor;
}

glm::vec3 UIButtonComponent::getActiveColor() {
	return this->activeColor;
}

void UIButtonComponent::setActiveColor(const glm::vec3& activeColor) {
	this->activeColor = activeColor;
}

glm::vec3 UIButtonComponent::getForegroundColor() {
	return this->foregroundColor;
}

void UIButtonComponent::setForegroundColor(const glm::vec3& foregroundColor) {
	this->foregroundColor = foregroundColor;
}

void UIButtonComponent::setButtonCallback(std::function<void(UIButtonComponent*)> buttonCallback) {
	this->buttonCallback = buttonCallback;
}

UIRect UIButtonComponent::toRect() {
	return UIRect(this->position, this->size);
}



// UIManager
void UIManager::addComponent(IUIComponent* comp) {
	this->components.push_back(comp);
}

void UIManager::removeComponent(IUIComponent* comp) {
	int i = 0;

	for (; i < components.size(); i++) {
		if (comp == components[i]) {
			break;
		}
	}

	if (i < components.size()) {
		components.erase(components.begin() + i);
	}
}

void UIManager::handleEvents(SDL_Event& e) {
	if (this->isShow()) {
		std::for_each(components.begin(), components.end(), [&](IUIComponent* comp) {
			comp->handleEvents(e);
		});
	}
}

void UIManager::render(UIShader* shader) {


	shader->bind();
	shader->setProj(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
	shader->setView(glm::mat4(1.0f));
	shader->unbind();

	if (this->isShow()) {
		std::for_each(components.begin(), components.end(), [&](IUIComponent* comp) {
			comp->render(shader);
		});
	}
}

void UIManager::update(float delta) {
	if (this->isShow()) {
		std::for_each(components.begin(), components.end(), [&](IUIComponent* comp) {
			comp->update(delta);
		});
	}
}

bool UIManager::isShow() {
	return this->show;
}

void UIManager::setShow(bool show) {
	this->show = show;
}

void UIManager::toggleShow() {
	this->setShow(!isShow());
}

void UIManager::init() {
	// Don't know what to do with this one...
}

void UIManager::release() {
	this->components.clear();
}



// UISystem
std::vector<UIManager*> UISystem::managers;
QuadUIGeometry UISystem::quad;

void UISystem::init() {
	quad.init();

	win_register_event([&](SDL_Event& e) {
		std::for_each(managers.begin(), managers.end(), [&](UIManager* man) {
			man->handleEvents(e);
		});
	});
}

void UISystem::release() {
	managers.clear();
	quad.release();
}

void UISystem::addManager(UIManager* manager) {
	UISystem::managers.push_back(manager);
}

void UISystem::removeManager(UIManager* manager) {
	int i = 0;

	for (; i < managers.size(); i++) {
		if (manager == UISystem::managers[i]) {
			break;
		}
	}

	if (i < managers.size()) {
		managers.erase(managers.begin() + i);
	}
}
