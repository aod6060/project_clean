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

		this->isHover = r.check(point);
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
	std::string temp = this->title;
	if (temp.length() == 0) {
		temp = " ";
	}
	FontRender::print(position.x, position.y, temp.c_str());


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



// UICheckBoxComponent
void UICheckBoxComponent::init() {
	this->setSize(glm::vec2(24.0f, 24.0f));
	this->setBackgroundColor(glm::vec3(0.5f));
	this->setForegroundColor(glm::vec3(1.0f));
	this->setInactiveColor(glm::vec3(0.3f, 0.0f, 0.0f));
	this->setActiveColor(glm::vec3(1.0f, 0.0f, 0.0f));
}

void UICheckBoxComponent::handleEvents(SDL_Event& e) {

	if (e.type == SDL_MOUSEMOTION) {
		UIRect r = this->toRect();

		glm::vec2 p = glm::vec2(e.motion.x, e.motion.y);

		this->isHover = r.check(p);
	}
}

void UICheckBoxComponent::render(UIShader* shader) {
	// background 20x20, Check 10x10, 
	shader->bind();
	// Background Rendering
	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
	);

	shader->setColor(this->backgroundColor);

	UISystem::quad.render(shader);

	// Check Rendering
	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(this->position + glm::vec2(6.0f), 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(12.0f, 12.0f, 0.0f))
	);

	if (this->checked) {
		shader->setColor(this->activeColor);
	}
	else {
		shader->setColor(this->inactiveColor);
	}

	UISystem::quad.render(shader);

	shader->unbind();

	// Title Rendering
	FontRender::setColor(this->foregroundColor);
	std::string temp = this->title;
	if (temp.length() == 0) {
		temp = " ";
	}
	FontRender::print(this->position.x + this->size.x + 5.0f, this->position.y, temp.c_str());
}

void UICheckBoxComponent::update(float delta) {
	if (this->isHover) {
		if (input_isIMFromConfDown("ui-action")) {
			this->checked = !this->checked;
			callback(this);
		}
	}
}

void UICheckBoxComponent::release() {

}

glm::vec2 UICheckBoxComponent::getPosition() {
	return this->position;
}

void UICheckBoxComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

glm::vec2 UICheckBoxComponent::getSize() {
	return this->size;
}

void UICheckBoxComponent::setSize(const glm::vec2& size) {
	this->size = size;
}

//std::string title;
std::string UICheckBoxComponent::getTitle() {
	return this->title;
}

void UICheckBoxComponent::setTitle(std::string title) {
	this->title = title;
}

//glm::vec3 foregroundColor;
glm::vec3 UICheckBoxComponent::getForegroundColor() {
	return this->foregroundColor;
}

void UICheckBoxComponent::setForegroundColor(const glm::vec3& foregroundColor) {
	this->foregroundColor = foregroundColor;
}

//glm::vec3 backgroundColor;
glm::vec3 UICheckBoxComponent::getBackgroundColor() {
	return this->backgroundColor;
}

void UICheckBoxComponent::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

//glm::vec3 inactiveColor;
glm::vec3 UICheckBoxComponent::getInactiveColor() {
	return this->inactiveColor;
}

void UICheckBoxComponent::setInactiveColor(const glm::vec3& inactiveColor) {
	this->inactiveColor = inactiveColor;
}

//glm::vec3 activeColor;
glm::vec3 UICheckBoxComponent::getActiveColor() {
	return this->activeColor;
}

void UICheckBoxComponent::setActiveColor(const glm::vec3& activeColor) {
	this->activeColor = activeColor;
}

//std::function<void(UICheckBoxComponent*)> callback;
void UICheckBoxComponent::setCallback(std::function<void(UICheckBoxComponent*)> callback) {
	this->callback = callback;
}

//bool checked = false;
bool UICheckBoxComponent::isChecked() {
	return this->checked;
}

void UICheckBoxComponent::setChecked(bool checked) {
	this->checked = checked;
}

UIRect UICheckBoxComponent::toRect() {
	return UIRect(this->position, this->size);
}


// UITextBoxComponent
void UITextBoxComponent::init() {
	this->size = FontRender::getSize("DDDDDDDDDDDDDDDD");
	this->foregroundColor = glm::vec3(1.0f);
	this->backgroundColor = glm::vec3(0.3f);

	for (int i = 0; i < TEXT_BOX_SIZE; i++) {
		buf[i] = '\0';
	}
}

void UITextBoxComponent::handleEvents(SDL_Event& e) {
	if (e.type == SDL_MOUSEMOTION) {
		UIRect r = this->toRect();

		glm::vec2 pos = glm::vec2(e.motion.x, e.motion.y);

		this->isHover = r.check(pos);
	}
}

void UITextBoxComponent::render(UIShader* shader) {
	shader->bind();
	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
	);

	shader->setColor(this->backgroundColor);

	UISystem::quad.render(shader);

	shader->unbind();

	FontRender::setColor(this->foregroundColor);

	std::string str(buf);

	if (str.length() == 0) {
		str = " ";
	}

	FontRender::print(this->position.x, this->position.y, str.c_str());
}

void UITextBoxComponent::update(float delta) {
	if (this->isHover) {
		if (input_isIMFromConfDown("ui-action")) {
			this->isActive = true;
		}
	}
	else {
		if (input_isIMFromConfDown("ui-action")) {
			this->isActive = false;
		}
	}

	if (this->isActive) {
		// Here are the supported input
		// a-z, A-Z, 0-1 SPACE, BACKSPACE, LSHIFT, RSHIFT

		
		if (input_isKeyDown(Keyboard::KB_LSHIFT) || input_isKeyDown(Keyboard::KB_RSHIFT)) {
			this->isShift = true;
		}

		if (input_isKeyUp(Keyboard::KB_LSHIFT) || input_isKeyUp(Keyboard::KB_RSHIFT)) {
			this->isShift = false;
		}
		

		if (this->location < TEXT_BOX_SIZE) {
			// a-z, 0-9, SPACE
			if (input_isKeyUp(Keyboard::KB_A)) {
				this->buf[this->location] = (this->isShift) ? 'A' : 'a';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_B)) {
				this->buf[this->location] = (this->isShift) ? 'B' : 'b';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_C)) {
				this->buf[this->location] = (this->isShift) ? 'C' : 'c';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_D)) {
				this->buf[this->location] = (this->isShift) ? 'D' : 'd';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_E)) {
				this->buf[this->location] = (this->isShift) ? 'E' : 'e';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_F)) {
				this->buf[this->location] = (this->isShift) ? 'F' : 'f';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_G)) {
				this->buf[this->location] = (this->isShift) ? 'G' : 'g';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_H)) {
				this->buf[this->location] = (this->isShift) ? 'H' : 'h';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_I)) {
				this->buf[this->location] = (this->isShift) ? 'I' : 'i';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_J)) {
				this->buf[this->location] = (this->isShift) ? 'J' : 'j';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_K)) {
				this->buf[this->location] = (this->isShift) ? 'K' : 'k';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_L)) {
				this->buf[this->location] = (this->isShift) ? 'L' : 'l';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_M)) {
				this->buf[this->location] = (this->isShift) ? 'M' : 'm';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_N)) {
				this->buf[this->location] = (this->isShift) ? 'N' : 'n';
				++this->location; 
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_O)) {
				this->buf[this->location] = (this->isShift) ? 'O' : 'o';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_P)) {
				this->buf[this->location] = (this->isShift) ? 'P' : 'p';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_Q)) {
				this->buf[this->location] = (this->isShift) ? 'Q' : 'q';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_R)) {
				this->buf[this->location] = (this->isShift) ? 'R' : 'r';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_S)) {
				this->buf[this->location] = (this->isShift) ? 'S' : 's';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_T)) {
				this->buf[this->location] = (this->isShift) ? 'T' : 't';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_U)) {
				this->buf[this->location] = (this->isShift) ? 'U' : 'u';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_V)) {
				this->buf[this->location] = (this->isShift) ? 'V' : 'v';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_W)) {
				this->buf[this->location] = (this->isShift) ? 'W' : 'w';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_X)) {
				this->buf[this->location] = (this->isShift) ? 'X' : 'x';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_Y)) {
				this->buf[this->location] = (this->isShift) ? 'Y' : 'y';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_Z)) {
				this->buf[this->location] = (this->isShift) ? 'Z' : 'z';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_0)) {
				this->buf[this->location] = (this->isShift) ? ')' : '0';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_1)) {
				this->buf[this->location] = (this->isShift) ? '!' : '1';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_2)) {
				this->buf[this->location] = (this->isShift) ? '@' : '2';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_3)) {
				this->buf[this->location] = (this->isShift) ? '#' : '3';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_4)) {
				this->buf[this->location] = (this->isShift) ? '$' : '4';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_5)) {
				this->buf[this->location] = (this->isShift) ? '%' : '5';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_6)) {
				this->buf[this->location] = (this->isShift) ? '^' : '6';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_7)) {
				this->buf[this->location] = (this->isShift) ? '&' : '7';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_8)) {
				this->buf[this->location] = (this->isShift) ? '*' : '8';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_9)) {
				this->buf[this->location] = (this->isShift) ? '(' : '9';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_COMMA)) {
				this->buf[this->location] = (this->isShift) ? '<' : ',';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_PERIOD)) {
				this->buf[this->location] = (this->isShift) ? '>' : '.';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_SLASH)) {
				this->buf[this->location] = (this->isShift) ? '?' : '/';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_SEMICOLON)) {
				this->buf[this->location] = (this->isShift) ? ':' : ';';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_APOSTROPHE)) {
				this->buf[this->location] = (this->isShift) ? '"' : '\'';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_LEFTBRACKET)) {
				this->buf[this->location] = (this->isShift) ? '{' : '[';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_RIGHTBRACKET)) {
				this->buf[this->location] = (this->isShift) ? '}' : ']';
				++this->location;
				this->callback(this);
			}
			
			if (input_isKeyUp(Keyboard::KB_BACKSLASH)) {
				this->buf[this->location] = (this->isShift) ? '|' : '\\';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_MINUS)) {
				this->buf[this->location] = (this->isShift) ? '_' : '-';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_EQUALS)) {
				this->buf[this->location] = (this->isShift) ? '+' : '=';
				++this->location;
				this->callback(this);
			}

			if (input_isKeyUp(Keyboard::KB_SPACE)) {
				this->buf[this->location] = ' ';
				++this->location;
				this->callback(this);
			}
		}
		// BACKSPACE

		if (this->location > 0) {
			if (input_isKeyUp(Keyboard::KB_BACKSPACE)) {
				--this->location;
				this->buf[this->location] = '\0';
				this->callback(this);
			}
		}
	}
}

void UITextBoxComponent::release() {

}

glm::vec2 UITextBoxComponent::getPosition() {
	return this->position;
}

void UITextBoxComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

glm::vec2 UITextBoxComponent::getSize() {
	return size;
}

void UITextBoxComponent::setSize(const glm::vec2& size) {
	this->size = size;
}

glm::vec3 UITextBoxComponent::getForegroundColor() {
	return this->foregroundColor;
}

void UITextBoxComponent::setForegroundColor(const glm::vec3& foregroundColor) {
	this->foregroundColor = foregroundColor;
}

glm::vec3 UITextBoxComponent::getBackgroundColor() {
	return this->backgroundColor;
}

void UITextBoxComponent::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

void UITextBoxComponent::setCallback(std::function<void(UITextBoxComponent*)> callback) {
	this->callback = callback;
}

std::string UITextBoxComponent::getText() {
	return std::string(this->buf);
}

UIRect UITextBoxComponent::toRect() {
	return UIRect(this->position, this->size);
}


// UISelect Button Component
void UISelectButtonComponent::init() {
	std::string temp = this->getCurrentID().c_str();
	if (temp.length() == 0) {
		temp = " ";
	}
	this->size = FontRender::getSize(temp.c_str());

	this->setBackgroundColor(glm::vec3(0.0f, 0.0f, 0.7f));
	this->setHoverColor(glm::vec3(0.5f, 0.5f, 1.0f));
	this->setActiveColor(glm::vec3(0.7f, 0.0f, 0.0f));
	this->setForegroundColor(glm::vec3(1.0f));

}

void UISelectButtonComponent::handleEvents(SDL_Event& e) {
	if (e.type == SDL_MOUSEMOTION) {
		UIRect r = this->toRect();

		glm::vec2 pos = glm::vec2(e.motion.x, e.motion.y);

		this->isHover = r.check(pos);
	}
}

void UISelectButtonComponent::render(UIShader* shader) {
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

	// Render Foreground
	FontRender::setColor(this->foregroundColor);
	std::string temp = types[this->index].id;
	if (temp.length() == 0) {
		temp = " ";
	}
	FontRender::print(position.x, position.y, temp.c_str());
}

void UISelectButtonComponent::update(float delta) {
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
			if (this->index < types.size() - 1) {
				this->index += 1;
			}
			else {
				this->index = 0;
			}
			std::string temp = this->getCurrentID();
			if (temp.length() == 0) {
				temp = " ";
			}
			this->size = FontRender::getSize(temp.c_str());
			this->isActive = true;
			this->activeTimer = this->activeTimerMax;
			this->callback(this);
		}

	}
	else {
		this->isActive = false;
		this->activeTimer = 0.0f;
	}
}

void UISelectButtonComponent::release() {

}

glm::vec2 UISelectButtonComponent::getPosition() {
	return this->position;
}

void UISelectButtonComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

glm::vec2 UISelectButtonComponent::getSize() {
	return this->size;
}

void UISelectButtonComponent::setSize(const glm::vec2& size) {
	this->size = size;
}

glm::vec3 UISelectButtonComponent::getForegroundColor() {
	return this->foregroundColor;
}

void UISelectButtonComponent::setForegroundColor(const glm::vec3& foregroundColor) {
	this->foregroundColor = foregroundColor;
}

glm::vec3 UISelectButtonComponent::getBackgroundColor() {
	return this->backgroundColor;
}

void UISelectButtonComponent::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

glm::vec3 UISelectButtonComponent::getHoverColor() {
	return this->hoverColor;
}

void UISelectButtonComponent::setHoverColor(const glm::vec3& hoverColor) {
	this->hoverColor = hoverColor;
}

glm::vec3 UISelectButtonComponent::getActiveColor() {
	return this->activeColor;
}

void UISelectButtonComponent::setActiveColor(const glm::vec3& activeColor) {
	this->activeColor = activeColor;
}

void UISelectButtonComponent::addValue(const UISelectButtonType& value) {
	this->types.push_back(value);
}

std::string UISelectButtonComponent::getCurrentID() {
	return this->types[index].id;
}

uint32_t UISelectButtonComponent::getCurrentValue() {
	return this->types[index].value;
}

void UISelectButtonComponent::setCallback(std::function<void(UISelectButtonComponent*)> callback) {
	this->callback = callback;
}

UIRect UISelectButtonComponent::toRect() {
	return UIRect(this->position, this->size);
}


// UISliderComponent
void UISliderComponent::init() {
	this->size = glm::vec2(100.0f, 20.0f);
	this->min = 0.0f;
	this->max = 1.0f;
	this->value = 0.0f;

	this->p = 0.0f;

	this->backgroundColor = glm::vec3(0.3f);
	this->sliderColor = glm::vec3(0.6f);
	this->foregroundColor = glm::vec3(1.0f);

}

void UISliderComponent::handleEvents(SDL_Event& e) {
	if (e.type == SDL_MOUSEMOTION) {
		UIRect r = this->toRect();
		
		glm::vec2 p = glm::vec2(e.motion.x, e.motion.y);

		this->isHover = r.check(p);
	}
}

void UISliderComponent::render(UIShader* shader) {
	shader->bind();
	// Render Background
	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
	);

	shader->setColor(this->backgroundColor);
	
	UISystem::quad.render(shader);

	// Render Slider
	shader->setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(position.x + p, position.y, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, size.y, 0.0f))
	);

	shader->setColor(this->sliderColor);

	UISystem::quad.render(shader);

	shader->unbind();

	// Render Value
	FontRender::setColor(this->foregroundColor);

	FontRender::print(
		position.x + size.x + 8.0f,
		position.y,
		"%f",
		this->value
	);

}

void UISliderComponent::update(float delta) {

	if (this->isHover) {

		if (input_isIMFromConfPress("ui-action")) {

			UIRect r;

			int mx, my;

			input_getMousePos(mx, my);

			glm::vec2 mp = glm::vec2(mx, my);

			r = this->toRect();

			if (r.check(mp)) {
				p = mp.x - this->position.x;

				this->value = this->_doValueChange(p);

				//logger_output("%f\n", this->value);
			}
		}
	}
}

void UISliderComponent::release() {

}

glm::vec2 UISliderComponent::getPosition() {
	return this->position;
}

void UISliderComponent::setPosition(const glm::vec2& pos) {
	this->position = pos;
}

glm::vec3 UISliderComponent::getForegroundColor() {
	return this->foregroundColor;
}

void UISliderComponent::setForegroundColor(const glm::vec3& foregroundColor) {
	this->foregroundColor = foregroundColor;
}

glm::vec3 UISliderComponent::getBackgroundColor() {
	return backgroundColor;
}

void UISliderComponent::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

glm::vec3 UISliderComponent::getSliderColor() {
	return this->sliderColor;
}

void UISliderComponent::setSliderColor(const glm::vec3& sliderColor) {
	this->sliderColor = sliderColor;
}

float UISliderComponent::getMin() {
	return this->min;
}

void UISliderComponent::setMin(float min) {
	this->min = min;
}

float UISliderComponent::getMax() {
	return max;
}

void UISliderComponent::setMax(float max) {
	this->max = max;
}

float UISliderComponent::getValue() {
	return this->value;
}

void UISliderComponent::setValue(float value) {
	this->value = value;
}

UIRect UISliderComponent::toRect() {
	return UIRect(this->position, this->size);
}

float UISliderComponent::_doValueChange(float p) {
	if (p <= 0.0f) {
		p = 0.0f;
	}
	else if(p >= 100.0f) {
		p = 100.0f;
	}

	return ((max - min) / 100.0f) * (p)+min;
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

	if (this->isShow()) {
		shader->bind();
		shader->setProj(glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f));
		shader->setView(glm::mat4(1.0f));

		// Render Background
		if (this->hasBackground) {
			shader->setModel(
				glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
			);

			shader->setColor(this->backgroundColor);

			UISystem::quad.render(shader);
		}

		shader->unbind();

	
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

void UIManager::setHasBackground(bool hasBackground) {
	this->hasBackground = hasBackground;
}

bool UIManager::isHasBackground() {
	return this->hasBackground;
}

void UIManager::setPosition(const glm::vec2& position) {
	this->position = position;
}

glm::vec2 UIManager::getPosition() {
	return this->position;
}

void UIManager::setSize(const glm::vec2& size) {
	this->size = size;
}

glm::vec2 UIManager::getSize() {
	return this->size;
}

void UIManager::setBackgroundColor(const glm::vec3& backgroundColor) {
	this->backgroundColor = backgroundColor;
}

glm::vec3 UIManager::getBackgroundColor() {
	return this->backgroundColor;
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

int UISystem::sizeManager() {
	return managers.size();
}
