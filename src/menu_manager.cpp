#include "game.h"
#include "menu_manager.h"


// Start Menu
void StartMenu::init() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.6f;
	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.3f;

	// Classic Mode Button
	this->classModeButton.setTitle("Classic Mode");
	this->classModeButton.init();
	this->classModeButton.setPosition(
		glm::vec2(
			x + (width * 0.5f - classModeButton.size.x * 0.5f),
			y + 32.0f
		)
	);

	this->classModeButton.setButtonCallback([&](UIButtonComponent* comp) {
		win_getCallbackTemp<GameWindowCallback>()->changeState("game-state");
		this->setShow(false);
	});

	// Exit Button
	exitButton.setTitle("Exit");
	exitButton.init();
	exitButton.setPosition(
		glm::vec2(
			x + (width * 0.5f - exitButton.size.x * 0.5f),
			y + 64.0f
		)
	);

	exitButton.setButtonCallback([&](UIButtonComponent* comp) {
		win_exit();
	});


	// UI Manager
	this->manager.setHasBackground(true);
	this->manager.setPosition(glm::vec2(x, y));
	this->manager.setSize(glm::vec2(width, height));
	this->manager.setBackgroundColor(glm::vec3(0.1f));
	this->manager.addComponent(&this->classModeButton);
	this->manager.addComponent(&this->exitButton);
	this->manager.init();

	UISystem::addManager(&this->manager);
}

void StartMenu::render() {
	this->manager.render(&ShaderManager::uiShader);
}

void StartMenu::update(float delta) {
	this->manager.update(delta);
}

void StartMenu::release() {
	UISystem::removeManager(&this->manager);
	this->manager.release();
	this->exitButton.release();
	this->classModeButton.release();
}

void StartMenu::setShow(bool show) {
	this->manager.setShow(show);
}

bool StartMenu::isShow() {
	return this->manager.isShow();
}

void GameContextMenu::init() {
	float x = conf_getWidth() * 0.4f;
	float y = conf_getHeight() * 0.4f;

	float width = conf_getWidth() * 0.2f;
	float height = conf_getHeight() * 0.4f;

	// Continue
	continueButton.setTitle("Continue");
	continueButton.init();
	continueButton.setPosition(
		glm::vec2(
			x + (width * 0.5f - continueButton.size.x * 0.5f),
			y + 32.0f
		)
	);

	continueButton.setButtonCallback([&](UIButtonComponent* comp) {});

	// Exit Game Button
	exitGameButton.setTitle("Exit Game");
	exitGameButton.init();
	exitGameButton.setPosition(
		glm::vec2(
			x + (width * 0.5f - exitGameButton.size.x * 0.5f),
			y + 64.0f
		)
	);

	exitGameButton.setButtonCallback([&](UIButtonComponent* comp) {
		win_getCallbackTemp<GameWindowCallback>()->changeState("start-menu");
		this->setShow(false);
	});


	manager.setHasBackground(true);
	manager.setPosition(glm::vec2(x, y));
	manager.setSize(glm::vec2(width, height));
	manager.setBackgroundColor(glm::vec3(0.1f));
	manager.setShow(false);

	manager.addComponent(&continueButton);
	manager.addComponent(&exitGameButton);
	manager.init();

	UISystem::addManager(&manager);
}

void GameContextMenu::render() {
	manager.render(&ShaderManager::uiShader);
}

void GameContextMenu::update(float delta) {
	manager.update(delta);
}

void GameContextMenu::release() {
	UISystem::removeManager(&manager);
	manager.release();
	exitGameButton.release();
	continueButton.release();
}

void GameContextMenu::setShow(bool show) {
	manager.setShow(show);
}

bool GameContextMenu::isShow() {
	return manager.isShow();
}

void GameContextMenu::setCallback(std::function<void(UIButtonComponent*)> CB) {
	continueButton.setButtonCallback(CB);
}

// Menu Manager
StartMenu MenuManager::startMenu;
GameContextMenu MenuManager::gameContextMenu;

void MenuManager::init() {
	startMenu.init();
	gameContextMenu.init();
}

void MenuManager::render() {
	startMenu.render();
	gameContextMenu.render();
}

void MenuManager::update(float delta) {
	startMenu.update(delta);
	gameContextMenu.update(delta);
}

bool MenuManager::isShow() {
	return 
		startMenu.isShow() || 
		gameContextMenu.isShow();
}

void MenuManager::release() {
	gameContextMenu.release();
	startMenu.release();
}