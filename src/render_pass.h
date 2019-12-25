#pragma once


// IRenderPass Section
struct IRenderPass;

struct RenderContext {
	IRenderPass* pass = nullptr;

	RenderContext(IRenderPass* pass);
	void enable(GLenum e);
	void disable(GLenum e);
	void clearColor(float r, float g, float b, float a);
	void clear(int clear);
	// I'll add in more functions later for the render context

	template<typename T>
	T* getPass() {
		return (T*)pass;
	}
};

struct IRenderPass {
	virtual void init() = 0;
	virtual void render() = 0;
	virtual void release() = 0;

	virtual void setCallback(std::function<void(RenderContext* renderContext)> callback) = 0;
};

struct AbstractRenderPass : public IRenderPass {

	std::function<void(RenderContext*)> callback;

	virtual void init() = 0;
	virtual void render() = 0;
	virtual void release() = 0;

	virtual void setCallback(std::function<void(RenderContext*)> callback);
};


struct MainRenderPass : public AbstractRenderPass {
	SceneShader sceneShader;
	TerrainShader terrainShader;
	WaterShader waterShader;

	virtual void init();
	virtual void render();
	virtual void release();
};

struct HUBRenderPass : public AbstractRenderPass {
	HUBShader hubShader;

	virtual void init();
	virtual void release();
	virtual void render();
};

// RenderPass Manager
struct RenderPassManager {
	std::vector<IRenderPass*> passes;

	void addRenderPass(IRenderPass* pass);

	void init();

	void render();

	void release();
};
