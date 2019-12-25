#include "sys.h"

// BlurStandardPreProcess
void BlurStandardPreProcess::setSize(uint32_t width, uint32_t height) {
	this->width = width;
	this->height = height;
}

void BlurStandardPreProcess::init() {
	// Create Renderbuffer
	this->depth.init();

	// Create Framebuffer
	this->frameBuffer.init();

	// Create Geometry
	this->geom.init();
}

void BlurStandardPreProcess::build() {
	this->depth.load(GL_DEPTH24_STENCIL8, this->width, this->height);


	this->frameBuffer.bind();
	this->frameBuffer.setTexture2D(out, GL_COLOR_ATTACHMENT0);
	this->frameBuffer.setRenderBuffer(&this->depth, GL_DEPTH_ATTACHMENT);
	this->frameBuffer.checkForErrors();
	this->frameBuffer.unbind();


}

void BlurStandardPreProcess::process() {


	frameBuffer.bind();
	RenderSystem::clearColor(1, 0, 0, 1);
	RenderSystem::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ShaderManager::blurPreProcessShader.bind();

	ShaderManager::blurPreProcessShader.setProj(
		glm::ortho(0.0f, (float)this->width, (float)this->height, 0.0f)
	);
	ShaderManager::blurPreProcessShader.setView(glm::mat4(1.0f));
	ShaderManager::blurPreProcessShader.setModel(
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)) * 
		glm::scale(glm::mat4(1.0f), glm::vec3((float)this->width, (float)this->height, 0.0f))
	);
	ShaderManager::blurPreProcessShader.setSize(this->size);

	in->bind();
	geom.render(&ShaderManager::blurPreProcessShader);
	in->unbind();

	ShaderManager::blurPreProcessShader.unbind();

	frameBuffer.unbind();
}

void BlurStandardPreProcess::release() {
	this->geom.release();
	this->frameBuffer.release();
	this->depth.release();
}

void BlurStandardPreProcess::setInputTexture(Texture2D* in) {
	this->in = in;
}

void BlurStandardPreProcess::setOutputTexture(Texture2D* out) {
	this->out = out;
}

