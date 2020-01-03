#include "proc_terrain_test.h"


void ProcTerrainTestWindowCallback::init() {
	this->hubRenderPass.setCallback([&]() {

		glm::mat4 proj =
			glm::ortho(0.0f, (float)conf_getWidth(), (float)conf_getHeight(), 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(this->procTerrain.size, this->procTerrain.size, 0.0f));

		RenderSystem::disable(GL_DEPTH_TEST);
		RenderSystem::clearColor(1.0f, 0.0f, 0.0f, 1.0f);
		RenderSystem::clear(GL_COLOR_BUFFER_BIT);

		//HUBRenderPass* pass = context->getPass<HUBRenderPass>();

		//pass->hubShader.bind();
		ShaderManager::hubShader.bind();
		
		//pass->hubShader.setProjection(proj);
		ShaderManager::hubShader.setProjection(proj);
		//pass->hubShader.setView(view);
		ShaderManager::hubShader.setView(view);
		//pass->hubShader.setModel(model);
		ShaderManager::hubShader.setModel(model);

		this->current->bind();
		quad.render(&ShaderManager::hubShader);
		//procTerrain.maskTex.unbind();
		this->current->unbind();

		//pass->hubShader.unbind();
		ShaderManager::hubShader.unbind();

		RenderSystem::enable(GL_DEPTH_TEST);
	});

	this->renderPassManager.addRenderPass(&this->hubRenderPass);

	this->quad.init();

	this->procTerrain.init("data/terrain/regular.json");

	//this->current = &procTerrain.elevationTex;
	this->current = TextureManager::getTex(TextureManager::TERRAIN_ELEVATION);

}

void ProcTerrainTestWindowCallback::update(float delta) {
	if (input_isKeyDown(Keyboard::KB_1)) {
		//this->current = &procTerrain.elevationTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_ELEVATION);
	} 

	if (input_isKeyDown(Keyboard::KB_2)) {
		//this->current = &procTerrain.maskTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_MASK);
	}

	if (input_isKeyDown(Keyboard::KB_3)) {
		//this->current = &procTerrain.maskedElevationTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_MASKED_ELEVATION);
	}

	if (input_isKeyDown(Keyboard::KB_4)) {
		//this->current = &procTerrain.moisterTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_MOISTER);
	}

	if (input_isKeyDown(Keyboard::KB_5)) {
		//this->current = &procTerrain.blendMapTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_BLEND_MAP);
	}

	if (input_isKeyDown(Keyboard::KB_6)) {
		//this->current = &procTerrain.biomesMapTex;
		this->current = TextureManager::getTex(TextureManager::TERRAIN_BIOMES);
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
}
