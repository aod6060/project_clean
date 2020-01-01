#pragma once


#include <sys.h>

struct UITestWindowCallback : public IWindowCallback {
	RenderPassManager renderPassManager;
	RenderPass mainRenderPass;

	UIManager manager;
	UILabelComponent helloWorld;
	// Button Test
	UIButtonComponent testButton1;
	UILabelComponent testLabel1;
	// Check Box
	UICheckBoxComponent testCheckBox1;
	UILabelComponent testLabel2;

	// TextBox
	UITextBoxComponent testTexBox;

	// SelectButtonComponent
	UISelectButtonComponent testSelectButton;

	// SliderComponent
	UISliderComponent testSlider;

	int counter = 0;

	virtual void init();

	virtual void update(float delta);

	virtual void fixedUpdate();

	virtual void render();

	virtual void release();

};