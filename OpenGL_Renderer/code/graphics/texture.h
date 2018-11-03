#pragma once

#include <map>
#include <GL/glew.h>
#include "image.h"
#include "color.h"

class Texture
{
	friend class Renderer;

public:
	Texture();
	Texture(const char* imageFilename, bool useMipmaps);
	Texture(GLsizei width, GLsizei height, GLenum format, const void* pixels, bool useMipmaps);
	~Texture();

	void write(const char* imageFilename, bool useMipmaps);
	void write(const void* pixels);
	void clear(const Color& color);

	GLuint getId() const;
	GLsizei getWidth() const;
	GLsizei getHeight() const;

private:
	GLuint id_;
	GLsizei width_;
	GLsizei height_;
	GLenum format_;
};
