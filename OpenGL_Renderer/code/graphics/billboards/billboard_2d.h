#pragma once

#include "../billboard.h"
#include "../../common/vector2.h"

class Billboard2D :public Billboard
{
public:
	Vector2f position_;

	Billboard2D(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords);
	Billboard2D(const char* imageFilename, float billboardWidth);
	Billboard2D(const char* imageFilename, float billboardWidth, float billboardHeight);
	Billboard2D(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth, float billboardHeight,
		bool flipVertically, bool flipHorizontally);
};
