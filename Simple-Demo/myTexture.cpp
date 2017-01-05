#include"myTexture.h"
#include<iostream>

myTexture::myTexture()
{
	handle = 0;
	width = 0;
	height = 0;
	mipLevels = 0;
	glGenTextures(1, &handle);
	if (handle == 0)
	{
		std::cout << "failed to create texture" << std::endl;
		return;
	}
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

myTexture::~myTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &handle);
}

GLuint myTexture::getTextureHandle()
{
	return handle;
}

void myTexture::load(const char* name)
{
	if (handle == 0)
	{
		std::cout << "failed to create texture" << std::endl;
		return;
	}
	gli::texture2d texture(gli::load(name));

	if (texture.empty())
	{
		std::cout << "failed to open the texture file" << std::endl;
		return;
	}
	
	width = static_cast<uint32_t>(texture.extent().x);
	height = static_cast<uint32_t>(texture.extent().y);
	mipLevels = static_cast<uint32_t>(texture.levels());

	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipLevels - 1);

	gli::gl GL(gli::gl::PROFILE_GL33);
	gli::gl::format const Format = GL.translate(texture.format(), texture.swizzles());

	glCompressedTexImage2D(GL_TEXTURE_2D, 0, Format.Internal, width, height, 0, 0, texture.data(0, 0, 0));
	
}