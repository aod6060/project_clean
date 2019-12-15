#pragma once


#include "sys.h"


class GameWindowCallback : public IWindowCallback {
private:
	RenderPassManager renderPassManager;
	MainRenderPass mainRenderPass;

	enum MeshType {
		MT_CUBE = 0,
		MT_SPHERE,
		MT_CYLINDER,
		MT_TORUS,
		MT_MONKEY,
		MT_MULTI_MESH_TEST
	};

	bool isWire = false;

	MeshType meshType = MeshType::MT_CUBE;

	SceneGeometry cube;
	SceneGeometry sphere;
	SceneGeometry cylinder;
	SceneGeometry torus;
	SceneGeometry monkey;
	SceneGeometry multiMeshTest;

	Texture2D angry;

	float yrot = 0.0f;

public:
	virtual void init();
	virtual void update(float delta);
	virtual void fixedUpdate();
	virtual void render();
	virtual void release();
};