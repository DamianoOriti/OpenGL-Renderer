#pragma once

#include <vector>
#include <string>
#include "../common/vector2.h"
#include "../common/vector3.h"
#include "texture.h"
#include "buffer.h"
#include "../common/utility.h"
#include "color.h"

class ShadelessModel3D
{
public:
	struct VertexAttribs;

	Vector3f position_;
	Vector3f rotationAxis_;
	float angle_;
	Color colorScale_;

	ShadelessModel3D(const char* smdlFilename);
	~ShadelessModel3D();

	const Texture* getTexture() const;
	const Buffer& getVertexBuffer() const;
	const Buffer& getIndexBuffer() const;

private:
	Texture* pTexture_;
	Buffer vertexBuffer_;
	Buffer indexBuffer_;
};

struct ShadelessModel3D::VertexAttribs
{
	Vector3f position;
	Vector2f texCoords;
};
