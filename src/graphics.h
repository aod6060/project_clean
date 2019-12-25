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

struct RenderBuffer {
	uint32_t id = 0;
	uint32_t width = 0;
	uint32_t height = 0;

	void init();
	void release();
};

struct FrameBuffer {
	uint32_t id = 0;

	void init();
	void release();

	void bind();
	void unbind();

	void setTexture2D(Texture2D& tex, GLenum attachment);

	void setRenderBuffer(RenderBuffer& buffer, GLenum attachment);
};
