#include"myTexture.h"
#include<iostream>

#define TEXTURE_DIRECTORY ".\\resource\\texture\\"

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
	std::string directory = TEXTURE_DIRECTORY;
	std::string filename = name;
	std::string path = directory + filename;
	if (handle == 0)
	{
		std::cout << "failed to create texture" << std::endl;
		return;
	}
	gli::texture2d texture(gli::load(path));

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

	uint32_t bpp;
	switch (Format.Internal)
	{
	case GL_COMPRESSED_RG11_EAC:
	case GL_COMPRESSED_SIGNED_RG11_EAC:
	case GL_COMPRESSED_RGBA8_ETC2_EAC:
	case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:
		bpp = 8;
		break;
	default:
		bpp = 4;
		break;
	}

	for (std::size_t level = 0; level < mipLevels; level++)
	{
		uint32_t mipmapWidth = texture.extent(level).x;
		uint32_t mipmapHeight = texture.extent(level).y;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, Format.Internal, mipmapWidth, mipmapHeight, 0, mipmapWidth * mipmapHeight * bpp / 8, texture.data(0, 0, level));
	}
}