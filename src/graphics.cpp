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

void VertexBuffer::set3f(float x, float y, float z) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
}

void VertexBuffer::set4f(float x, float y, float z, float w) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
	this->list.push_back(w);
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
void IndexBuffer::set1f(int x) {
	this->list.push_back(x);
}

void IndexBuffer::set2f(int x, int y) {
	this->list.push_back(x);
	this->list.push_back(y);
}

void IndexBuffer::set3f(int x, int y, int z) {
	this->list.push_back(x);
	this->list.push_back(y);
	this->list.push_back(z);
}

void IndexBuffer::set4f(int x, int y, int z, int w) {
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size() * sizeof(int), this->list.data(), GL_DYNAMIC_DRAW);
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
