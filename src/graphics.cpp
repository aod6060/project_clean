#include "sys.h"

// Shaders
void Shader::init(GLenum type, std::string path) {
	id = glCreateShader(type);

	std::ifstream in(path);
	std::stringstream ss;
	std::string temp;

	while (std::getline(in, temp)) {
		ss << temp << std::endl;
	}

	in.close();

	std::string src = ss.str();

	logger_output("%s\n", src.c_str());

	const char* c_src = src.c_str();

	glShaderSource(id, 1, &c_src, 0);
	glCompileShader(id);

	char log[1024];
	int len;

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
	
	if (len > 0) {
		glGetShaderInfoLog(id, len, 0, log);
		logger_output("%s\n", log);
	}
}

void Shader::release() {
	glDeleteShader(this->id);
	this->id = 0;
}

// Program functions
void Program::addShader(Shader* shader) {
	this->shaders.push_back(shader);
}

void Program::init() {
	this->id = glCreateProgram();

	std::for_each(this->shaders.begin(), this->shaders.end(), [&](Shader* shader) {
		glAttachShader(this->id, shader->id);
	});

	glLinkProgram(this->id);

	char log[1024];
	int len;

	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);

	if (len > 0) {
		glGetProgramInfoLog(id, len, 0, log);
		logger_output("%s\n", log);
	}

	glGenVertexArrays(1, &this->attrID);
}

void Program::bind() {
	glUseProgram(this->id);
}

void Program::unbind() {
	glUseProgram(0);
}

void Program::release() {
	glDeleteVertexArrays(1, &this->attrID);

	std::for_each(shaders.begin(), shaders.end(), [&](Shader* shader) {
		glDetachShader(this->id, shader->id);
	});

	shaders.clear();

	glDeleteProgram(this->id);
	this->id = 0;
}

// Attributes
void Program::setAttr(std::string name, int id) {
	this->attributes[name] = id;
}

void Program::enableAttr(std::string name) {
	glEnableVertexAttribArray(this->attributes[name]);
}

void Program::disableAttr(std::string name) {
	glDisableVertexAttribArray(this->attributes[name]);
}

void Program::pointerAttr(
	std::string name,
	uint32_t size,
	GLenum type) {
	glVertexAttribPointer(
		this->attributes[name],
		size,
		type,
		GL_FALSE,
		0,
		0
	);
}

void Program::bindAttr() {
	glBindVertexArray(this->attrID);
}

void Program::unbindAttr() {
	glBindVertexArray(0);
}

// Uniforms
void Program::createUniform(std::string name) {
	this->uniforms[name] = glGetUniformLocation(this->id, name.c_str());
}

// Integer Uniforms
void Program::set1i(std::string name, int x) {
	glUniform1i(this->uniforms[name], x);
}

void Program::set2i(std::string name, const glm::ivec2& v) {
	glUniform2i(this->uniforms[name], v.x, v.y);
}

void Program::set3i(std::string name, const glm::ivec3& v) {
	glUniform3i(this->uniforms[name], v.x, v.y, v.z);
}

void Program::set4i(std::string name, const glm::ivec4& v) {
	glUniform4i(this->uniforms[name], v.x, v.y, v.z, v.w);
}


// Float Uniforms
void Program::set1f(std::string name, float x) {
	glUniform1f(this->uniforms[name], x);
}

void Program::set2f(std::string name, const glm::vec2& v) {
	glUniform2f(this->uniforms[name], v.x, v.y);
}

void Program::set3f(std::string name, const glm::vec3& v) {
	glUniform3f(this->uniforms[name], v.x, v.y, v.z);
}

void Program::set4f(std::string name, const glm::vec4& v) {
	glUniform4f(this->uniforms[name], v.x, v.y, v.z, v.w);
}


// Matrices Uniforms
void Program::setMat2(std::string name, const glm::mat2& m) {
	glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Program::setMat3(std::string name, const glm::mat3& m) {
	glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}

void Program::setMat4(std::string name, const glm::mat4& m) {
	glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
}



// VertexBuffer
void VertexBuffer::set1f(float x) {
	this->list.push_back(x);
}

void VertexBuffer::set2f(float x, float y) {
	this->list.push_back(x);
	this->list.push_back(y);
}

void VertexBuffer::set2f(const glm::vec2& v) {
	this->set2f(v.x, v.y);
}

void VertexBuffer::set3f(float x, float y, float z) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
}

void VertexBuffer::set3f(const glm::vec3& v) {
	this->set3f(v.x, v.y, v.z);
}

void VertexBuffer::set4f(float x, float y, float z, float w) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
	this->list.push_back(w);
}

void VertexBuffer::set4f(const glm::vec4& v) {
	this->set4f(v.x, v.y, v.z, v.w);
}

void VertexBuffer::clear() {
	this->list.clear();
}

void VertexBuffer::init(bool isDynamic) {
	this->isDynamic = isDynamic;
	glGenBuffers(1, &this->id);
}

void VertexBuffer::release() {
	glDeleteBuffers(1, &this->id);
	this->clear();
}

void VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::update() {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, this->size() * sizeof(float), list.data(), (this->isDynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	this->unbind();
}

int VertexBuffer::size() {
	return this->list.size();
}



// Index Buffer
void IndexBuffer::set1f(uint32_t x) {
	this->list.push_back(x);
}

void IndexBuffer::set2f(uint32_t x, uint32_t y) {
	this->list.push_back(x);
	this->list.push_back(y);
}

void IndexBuffer::set3f(uint32_t x, uint32_t y, uint32_t z) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
}

void IndexBuffer::set4f(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
	this->list.push_back(w);
}

void IndexBuffer::init() {
	glGenBuffers(1, &this->id);
}

void IndexBuffer::release() {
	glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::update() {
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size() * sizeof(uint32_t), this->list.data(), GL_STATIC_DRAW);
	unbind();
}

int IndexBuffer::size() {
	return this->list.size();
}



// Texture2D

void Texture2D::loadTexture(std::string fn) {
	SDL_Surface* surf = IMG_Load(fn.c_str());

	if (surf == nullptr) {
		return;
	}

	this->init(
		surf->w,
		surf->h,
		surf->format->BytesPerPixel,
		surf->pixels);

	SDL_FreeSurface(surf);
}

void Texture2D::init(
	uint32_t width,
	uint32_t height,
	uint32_t bytePerPixel,
	void* pixels) {

	this->width = width;
	this->height = height;

	GLenum format = GL_RGB;

	if (bytePerPixel == 4) {
		format = GL_RGBA;
	}

	if (this->id == 0) {
		glGenTextures(1, &this->id);
	}

	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		this->width,
		this->height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	this->unbind();
}

void Texture2D::initEmpty(uint32_t width, uint32_t height) {
	this->width = width;
	this->height = height;

	if (this->id == 0) {
		glGenTextures(1, &this->id);
	}

	this->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		this->width,
		this->height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		nullptr);

	//glGenerateMipmap(GL_TEXTURE_2D);

	this->unbind();

}

void Texture2D::bind(GLenum tex) {
	glActiveTexture(tex);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::unbind(GLenum tex) {
	glActiveTexture(tex);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::release() {
	glDeleteTextures(1, &this->id);
}


// RenderBuffer
void RenderBuffer::init() {
	glGenRenderbuffers(1, &this->id);
}

void RenderBuffer::load(GLenum storage, uint32_t width, uint32_t height) {
	this->width = width;
	this->height = height;

	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, storage, this->width, this->height);
	unbind();
}

void RenderBuffer::bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RenderBuffer::unbind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBuffer::release() {
	glDeleteRenderbuffers(1, &this->id);
}

// Framebuffer
void FrameBuffer::init() {
	glGenFramebuffers(1, &this->id);
}

void FrameBuffer::release() {
	glDeleteFramebuffers(1, &this->id);
}

void FrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setTexture2D(Texture2D* tex, GLenum attachment) {
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		attachment,
		GL_TEXTURE_2D,
		tex->id,
		0);

	logger_output("tex: %d\n", tex->id);
}

void FrameBuffer::setRenderBuffer(RenderBuffer* buffer, GLenum attachment) {
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		attachment,
		GL_RENDERBUFFER,
		buffer->id
	);

	logger_output("renderbuf: %d\n", buffer->id);
}

void FrameBuffer::checkForErrors() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		logger_output("Error: Framebuffer wasn't created!\n");
	}
	else {
		logger_output("Success: Framebuffer created\n");
	}
}


// RenderSystem
void RenderSystem::enable(GLenum e) {
	glEnable(e);
}

void RenderSystem::disable(GLenum e) {
	glDisable(e);
}

void RenderSystem::clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void RenderSystem::clearColor(const glm::vec4& v) {
	clearColor(v.r, v.g, v.b, v.a);
}

void RenderSystem::clear(int clear) {
	glClear(clear);
}

void RenderSystem::drawArrays(GLenum type, GLint first, GLint count) {
	glDrawArrays(type, first, count);
}

void RenderSystem::drawElements(GLenum mode, GLsizei size, GLenum type) {
	glDrawElements(mode, size, type, 0);
}

void RenderSystem::viewport(int x, int y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}