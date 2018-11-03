#include "billboard_3d.h"

Billboard3D::Billboard3D(const char* imageFilename, const GLfloat* vertices, const GLfloat* texCoords) :
	Billboard(imageFilename, vertices, texCoords)
{}

Billboard3D::Billboard3D(const char* imageFilename, float billboardWidth) :
	Billboard(imageFilename, billboardWidth)
{}

Billboard3D::Billboard3D(const char* imageFilename, float billboardWidth, float billboardHeight) :
	Billboard(imageFilename, billboardWidth, billboardHeight)
{}

Billboard3D::Billboard3D(GLsizei textureWidth, GLsizei textureHeight, float billboardWidth, float billboardHeight,
	bool flipVertically, bool flipHorizontally) :
	Billboard(textureWidth, textureHeight, billboardWidth, billboardHeight, flipVertically,
		flipHorizontally)
{}
