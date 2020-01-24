#pragma once


#include <sys.h>

struct IMenu {
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void update(float delta) = 0;
	virtual void release() = 0;

	virtual void setShow(bool show) = 0;
	virtual bool isShow() = 0;
};

struct StartMenu : public IMenu {
	UIManager manager;
	UIButtonComponent classModeButton;
	UIButtonComponent exitButton;

	virtual void init();
	virtual void render();
	virtual void update(float delta);
	virtual void release();

	virtual void setShow(bool show);
	virtual bool isShow();
};

struct GameContextMenu : public IMenu {
	UIManager manager;
	UIButtonComponent continueButton;
	UIButtonComponent exitGameButton;

	virtual void init();
	virtual void render();
	virtual void update(float delta);
	virtual void release();

	virtual void setShow(bool show);
	virtual bool isShow();

	void setCallback(std::function<void(UIButtonComponent*)> CB);
};

struct MenuManager {
	static StartMenu startMenu;
	static GameContextMenu gameContextMenu;

	static void init();

	static void render();

	static void update(float delta);

	static bool isShow();

	static void release();
};