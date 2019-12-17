#include "sys.h"

// RenderContext
RenderContext::RenderContext(IRenderPass* pass) {
	this->pass = pass;
}

void RenderContext::enable(GLenum e) {
	glEnable(e);
}

void RenderContext::disable(GLenum e) {
	glDisable(e);
}

void RenderContext::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void RenderContext::clear(int clear) {
	glClear(clear);
}

// I'll add in more functions later for the render context

// AbstractRenderPass
void AbstractRenderPass::setCallback(std::function<void(RenderContext*)> callback) {
	this->callback = callback;
}

// MainRenderPass
void MainRenderPass::init() {
	sceneShader.init();
	terrainShader.init();
}

void MainRenderPass::render() {
	RenderContext context(this);

	this->callback(&context);
}

void MainRenderPass::release() {
	terrainShader.release();
	sceneShader.release();
}

void RenderPassManager::addRenderPass(IRenderPass* pass) {
	this->passes.push_back(pass);
}

void RenderPassManager::init() {
	std::for_each(passes.begin(), passes.end(), [&](IRenderPass* pass) {
		pass->init();
	});
}

void RenderPassManager::render() {
	std::for_each(this->passes.begin(), this->passes.end(), [&](IRenderPass* pass) {
		pass->render();
	});
}

void RenderPassManager::release() {
	std::for_each(passes.begin(), passes.end(), [&](IRenderPass* pass) {
		pass->release();
	});
	this->passes.clear();
}