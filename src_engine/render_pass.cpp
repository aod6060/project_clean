#include "sys.h"

// RenderPass
void RenderPass::render() {
	this->callback();
}

void RenderPass::setCallback(std::function<void()> callback) {
	this->callback = callback;
}

// RenderPassManager
void RenderPassManager::addRenderPass(RenderPass* pass) {
	this->passes.push_back(pass);
}

void RenderPassManager::render() {
	std::for_each(this->passes.begin(), this->passes.end(), [&](RenderPass* pass) {
		pass->render();
	});
}

void RenderPassManager::release() {
	passes.clear();
}