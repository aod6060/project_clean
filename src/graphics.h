#pragma once


#define GRAPHICS_FPS_60 1.0f / 60.0f

/*
	Wrapper Section
*/
// Shader
struct Shader {
	uint32_t id = 0;

	void init(GLenum type, std::string path);
	void release();
};

// Program
struct Program {
	uint32_t id = 0;

	std::vector<Shader*> shaders;

	uint32_t attrID = 0;

	std::map<std::string, int> attributes;

	std::map<std::string, int> uniforms;

	void init();

	void bind();

	void unbind();

	void release();

	void addShader(Shader* shader);

	// Attributes
	void setAttr(std::string name, int id);

	void enableAttr(std::string name);

	void disableAttr(std::string name);

	void pointerAttr(
		std::string name, 
		uint32_t size, 
		GLenum type);

	void bindAttr();

	void unbindAttr();

	// Uniforms
	void createUniform(std::string name);

	// Integer Uniforms
	void set1i(std::string name, int x);
	void set2i(std::string name, const glm::ivec2& v);
	void set3i(std::string name, const glm::ivec3& v);
	void set4i(std::string name, const glm::ivec4& v);

	// Float Uniforms
	void set1f(std::string name, float x);
	void set2f(std::string name, const glm::vec2& v);
	void set3f(std::string name, const glm::vec3& v);
	void set4f(std::string name, const glm::vec4& v);

	// Matrices Uniforms
	void setMat2(std::string name, const glm::mat2& m);
	void setMat3(std::string name, const glm::mat3& m);
	void setMat4(std::string name, const glm::mat4& m);

};

// VertexBuffer
struct VertexBuffer {
	uint32_t id = 0;
	std::vector<float> list;
	bool isDynamic = false;

	void set1f(float x);
	void set2f(float x, float y);
	void set2f(const glm::vec2& v);
	void set3f(float x, float y, float z);
	void set3f(const glm::vec3& v);
	void set4f(float x, float y, float z, float w);
	void set4f(const glm::vec4& v);
	void clear();

	void init(bool isDynamic = false);
	void release();
	void bind();
	void unbind();
	void update();

	int size();
	 
};

// IndexBuffer
struct IndexBuffer {
	uint32_t id = 0;
	std::vector<uint32_t> list;

	void set1f(uint32_t x);
	void set2f(uint32_t x, uint32_t y);
	void set3f(uint32_t x, uint32_t y, uint32_t z);
	void set4f(uint32_t x, uint32_t y, uint32_t z, uint32_t w);

	void init();
	void release();
	void bind();
	void unbind();
	void update();

	int size();
};

// Texture2D
struct Texture2D {
	uint32_t id = 0;
	uint32_t width;
	uint32_t height;

	void loadTexture(std::string fn);

	void init(uint32_t width, uint32_t height, uint32_t bytePerPixel, void* pixels);

	void bind(GLenum tex = GL_TEXTURE0);

	void unbind(GLenum tex = GL_TEXTURE0);

	void release();
};




/*
	HIGHLEVEL ~ Graphics
*/

// Shader Section

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

// Geometry Section

template<typename T>
struct IGeometry {
	virtual void init() = 0;
	virtual void render(T* shader) = 0;
	virtual void release() = 0;
};

struct SceneGeometry : public IGeometry<SceneShader> {

	const std::string TYPE = "scene";
	const std::string VERSION = "1.0";

	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct Face {
		uint32_t p1;
		uint32_t p2;
		uint32_t p3;
	};

	struct Mesh {
		glm::mat4 matrix;
		std::vector<Vertex> vertices;
		std::vector<Face> faces;
	};

	struct Scene {
		std::string type;
		std::string version;
		std::vector<Mesh> meshes;
	};

	struct MeshGeometry : public IGeometry<SceneShader> {
		SceneGeometry* parent = nullptr;

		std::vector<Vertex> v;
		std::vector<Face> f;

		glm::mat4 matrix;

		VertexBuffer vertices;
		VertexBuffer texCoords;
		VertexBuffer normals;
		IndexBuffer indencies;

		void setParent(SceneGeometry* parent);

		virtual void init();
		virtual void render(SceneShader* shader);
		virtual void release();
	};


	std::string path;
	glm::mat4 model;
	Scene scene;
	std::vector<MeshGeometry> geometry;

	void setFilePath(std::string path);
	void setModel(const glm::mat4& model);

	void loadScene(std::string path, Scene& scene);

	virtual void init();
	virtual void render(SceneShader* shader);
	virtual void release();
};

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

	virtual void init();
	virtual void render();
	virtual void release();
};


// RenderPass Manager
struct RenderPassManager {
	std::vector<IRenderPass*> passes;

	void addRenderPass(IRenderPass* pass);

	void init();

	void render();

	void release();
};


struct Camera {
	glm::vec3 pos;
	glm::vec2 rot;

	float fov;
	float apsect;
	float znear;
	float zfar;

	float rotSpeed = 64.0f;
	float walkingSpeed = 32.0f;

	void init(
		glm::vec3 pos,
		glm::vec2 rot,
		float fov,
		float aspect,
		float znear,
		float zfar,
		float rotSpeed = 64.0f,
		float walkingSpeed = 32.0f);

	void update(float delta);

	glm::mat4 getProjection();

	glm::mat4 getView();

};