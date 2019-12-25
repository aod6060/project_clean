#pragma once

struct RenderPass {
	std::function<void()> callback;

	void render();
	void setCallback(std::function<void()> callback);
};

// RenderPass Manager
struct RenderPassManager {
	std::vector<RenderPass*> passes;

	void addRenderPass(RenderPass* pass);

	void render();
};
