#pragma once


struct IShader {

	virtual void init() = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void release() = 0;

	virtual void bindAttr() = 0;
	virtual void unbindAttr() = 0;
	virtual void pointerAttr(
		std::string name,
		uint32_t size,
		GLenum type) = 0;
};

struct AbstractShader : public IShader {
	Shader vertex;
	Shader fragment;

	Program program;

	virtual void init() = 0;
	virtual void bind();
	virtual void unbind();
	virtual void release();

	virtual void bindAttr();
	virtual void unbindAttr();
	virtual void pointerAttr(
		std::string name,
		uint32_t size,
		GLenum type);
};

struct Camera;

struct SceneShader : public AbstractShader {

	virtual void init();

	// Attributes
	void verticesPointer();
	void texCoordPointer();
	void normalPointer();

	// Uniforms
	void setProjective(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);

	void setCamera(Camera* camera);
};

struct TerrainShader : public AbstractShader {

	virtual void init();

	// Attributes
	void verticesPointer();
	void texCoordPointer();
	void normalPointer();

	// Uniforms
	void setProjective(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);

	void setCamera(Camera* camera);

	void setTexScale(float scale);

};

struct HUBShader : public AbstractShader {
	virtual void init();

	// Attributes
	void verticesPointer();
	void texCoordPointer();

	// Uniforms
	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);
};

struct WaterShader : public AbstractShader {
	virtual void init();

	// Attributes
	void verticePointer();
	void texCoordPointer();
	void normalPointer();

	// Uniforms
	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);

	void setCamera(Camera* camera);

	void setTexScale(float scale);

	void setTimeDelta(float timeDelta);
};


// PreProcessor Shaders
struct BlurPreProcessShader : public AbstractShader {
	virtual void init();

	// Attributes
	void verticesPointer();
	void texCoordPointer();

	// Uniforms
	void setProj(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);
	void setSize(float size);

};

// FontRender Shader
struct FontRenderShader : public AbstractShader {
	virtual void init();

	// Attributes
	void verticesPointer();
	void texCoordsPointer();

	// Uniforms
	void setProj(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);
	void setColor(const glm::vec3& color);

};

// UI Shaders

struct ShaderManager {
	// Regular Shaders
	static SceneShader sceneShader;
	static TerrainShader terrainShader;
	static HUBShader hubShader;
	static WaterShader waterShader;
	
	// Pre Process Shaders
	static BlurPreProcessShader blurPreProcessShader;

	// FontRender
	static FontRenderShader fontRenderShader;

	static void init();

	static void release();
};