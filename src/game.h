#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:
	Shader testVertex;
	Shader testFragment;

	Program testProgram;

	VertexBuffer quad;
	VertexBuffer quadTexCoords;
	IndexBuffer quadIndencies;

	Texture2D angry;

	float yrot = 0.0f;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};