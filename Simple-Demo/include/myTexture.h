#pragma once
#include<GLES3/gl3.h>
#include <gli/gli.hpp>

class myTexture
{
public:
	myTexture();
	~myTexture();
	GLuint getTextureHandle();
	void load(const char* name);
private:
	GLuint handle;
	GLuint width;
	GLuint height;
	GLuint mipLevels;
};
