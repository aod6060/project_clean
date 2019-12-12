#pragma once



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
	void set3f(float x, float y, float z);
	void set4f(float x, float y, float z, float w);
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
	std::vector<int> list;

	void set1f(int x);
	void set2f(int x, int y);
	void set3f(int x, int y, int z);
	void set4f(int x, int y, int z, int w);

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

struct IShader;

struct IGeometry {
	virtual void init() = 0;
	virtual void render(IShader* shader) = 0;
	virtual void release() = 0;

	virtual void getVertices(std::vector<glm::vec3>& vertices) = 0;
};

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

struct TestShader : public AbstractShader {


	virtual void init();

	// Set Attributes
	void verticesPointer();

	void texCoordPointer();

	// Set Uniforms
	void setProjection(const glm::mat4& proj);
	void setView(const glm::mat4& view);
	void setModel(const glm::mat4& model);
};