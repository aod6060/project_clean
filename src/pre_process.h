#pragma once


/*

	IStandard:	(a) -> output
	ICombine:	(a, b) -> output
	ISpliter:	(a) -> (r, g, b)
	IGenerator:	()  -> output
*/


// Blur Preprocess

/*
	The blur preprocess will take int a texture then output a blurred version
	of it so then...

	(a) -> b

*/

struct IPreProcess {
	virtual void init() = 0;
	virtual void build() = 0;
	virtual void process() = 0;
	virtual void release() = 0;
};

struct IStandardPreProcess : public IPreProcess {
	virtual void setInputTexture(Texture2D* in) = 0;
	virtual void setOutputTexture(Texture2D* out) = 0;
};


struct BlurStandardPreProcess : public IStandardPreProcess {

	Texture2D* in;
	Texture2D* out;

	RenderBuffer depth;
	FrameBuffer frameBuffer;

	QuadBlurPreProcessGeometry geom;

	uint32_t width;
	uint32_t height;

	float size = 1024.0f;

	void setSize(uint32_t width, uint32_t height);

	virtual void init();

	virtual void build();

	virtual void process();

	virtual void release();

	virtual void setInputTexture(Texture2D* in);

	virtual void setOutputTexture(Texture2D* out);

};