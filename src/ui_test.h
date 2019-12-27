#pragma once


#include "sys.h"

struct UITestWindowCallback : public IWindowCallback {
	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	UIManager manager;
	UILabelComponent helloWorld;
	UIButtonComponent testButton1;
	UILabelComponent testLabel1;

	int counter = 0;

	virtual void init();

	virtual void update(float delta);

	virtual void fixedUpdate();

	virtual void render();

	virtual void release();

};