#include "proc_terrain_test.h"


void ProcTerrainTestWindowCallback::init() {
	this->hubRenderPass.setCallback([&](RenderContext* context) {

		glm::mat4 proj =
			glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(this->procTerrain.size, this->procTerrain.size, 0.0f));

		context->disable(GL_DEPTH_TEST);
		context->clearColor(1.0f, 0.0f, 0.0f, 1.0f);
		context->clear(GL_COLOR_BUFFER_BIT);

		HUBRenderPass* pass = context->getPass<HUBRenderPass>();

		pass->hubShader.bind();

		pass->hubShader.setProjection(proj);
		pass->hubShader.setView(view);
		pass->hubShader.setModel(model);

		this->current->bind();
		quad.render(&pass->hubShader);
		//procTerrain.maskTex.unbind();
		this->current->unbind();

		pass->hubShader.unbind();
	});

	this->renderPassManager.addRenderPass(&this->hubRenderPass);
	this->renderPassManager.init();

	this->quad.init();

	this->procTerrain.init("data/terrain/regular.json");

	this->current = &procTerrain.elevationTex;

}

void ProcTerrainTestWindowCallback::update(float delta) {
	if (input_isKeyDown(Keyboard::KB_1)) {
		this->current = &procTerrain.elevationTex;
	} 

	if (input_isKeyDown(Keyboard::KB_2)) {
		this->current = &procTerrain.maskTex;
	}

	if (input_isKeyDown(Keyboard::KB_3)) {
		this->current = &procTerrain.maskedElevationTex;
	}

	if (input_isKeyDown(Keyboard::KB_4)) {
		this->current = &procTerrain.moisterTex;
	}

	if (input_isKeyDown(Keyboard::KB_5)) {
		this->current = &procTerrain.blendMapTex;
	}

	if (input_isKeyDown(Keyboard::KB_6)) {
		this->current = &procTerrain.biomesMapTex;
	}
}

void ProcTerrainTestWindowCallback::fixedUpdate() {

}

void ProcTerrainTestWindowCallback::render() {
	this->renderPassManager.render();
}

void ProcTerrainTestWindowCallback::release() {
	procTerrain.release();
	this->quad.release();
	this->renderPassManager.release();
}
