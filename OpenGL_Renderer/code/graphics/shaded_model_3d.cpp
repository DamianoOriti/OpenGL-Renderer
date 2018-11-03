#include "shaded_model_3d.h"

ShadedModel3D::ShadedModel3D(const char* filename) :
	position_(0.0f, 0.0f, 0.0f),
	rotationAxis_(0.0f, 1.0f, 0.0f),
	angle_(0.0f),
	scale_(1.0f),
	pData_(nullptr)
{
	try
	{
		pData_ = new ShadedModel3D::Data(filename);
	}
	catch (...)
	{
		if (pData_ != nullptr)
		{
			delete pData_;
			pData_ = nullptr;
		}

		throw;
	}
}

ShadedModel3D::~ShadedModel3D()
{
	if (pData_ != nullptr)
	{
		delete pData_;
		pData_ = nullptr;
	}
}

const ShadedModel3D::Data* ShadedModel3D::getData() const
{
	return pData_;
}

ShadedModel3D::Data::Data(const char* filename) :
	vertexBuffer(GL_ARRAY_BUFFER),
	indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
	if (strstr(filename, ".fbx") != nullptr)
	{
		readFbxFile(filename);
	}
	else
	{
		throw std::exception("3d model file format not supported!");
	}
}

void ShadedModel3D::Data::readFbxFile(const char* filename)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		filename,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
	);

	std::string directoryPath(filename);
	extractDirectoryPath(directoryPath);
	size_t offset = directoryPath.find("/models/");
	directoryPath.replace(offset, 10, "/textures/");

	size_t numberOfMeshes = pScene->mNumMeshes;

	boundingSphereRadius = 200.0f;

	triangleGroups.resize(numberOfMeshes);
	triangleGroups.shrink_to_fit();

	GLsizei totVertices = 0;
	GLsizei totIndices = 0;
	auto meshes = pScene->mMeshes;
	for (size_t i = 0; i < numberOfMeshes; i++)
	{
		auto mesh = meshes[i];
		totVertices += mesh->mNumVertices;
		totIndices += mesh->mNumFaces * 3;
	}
	std::vector<VertexAttribs> vertices(totVertices);
	std::vector<GLuint> indices(totIndices);

	totIndices = 0;
	totVertices = 0;
	size_t indexOffset;
	size_t numberOfVertices;
	size_t numberOfFaces;
	aiString diffuseImagePath;
	aiString normalImagePath;
	aiString specularImagePath;
	auto materials = pScene->mMaterials;
	for (size_t i = 0; i < numberOfMeshes; i++)
	{
		auto mesh = meshes[i];

		indexOffset = totVertices;

		// Get textures
		{
			int materialIndex = mesh->mMaterialIndex;
			auto material = materials[materialIndex];

			if (material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &diffuseImagePath) == aiReturn_FAILURE)
			{
				diffuseImagePath.Set("default_diffuse.png");
			}

			if (material->GetTexture(aiTextureType::aiTextureType_NORMALS, 0, &normalImagePath) == aiReturn_FAILURE)
			{
				normalImagePath.Set("default_normal.png");
			}

			if (material->GetTexture(aiTextureType::aiTextureType_SPECULAR, 0, &specularImagePath) == aiReturn_FAILURE)
			{
				specularImagePath.Set("default_specular.png");
			}

			diffuseImagePath.Set(directoryPath + diffuseImagePath.C_Str());
			normalImagePath.Set(directoryPath + normalImagePath.C_Str());
			specularImagePath.Set(directoryPath + specularImagePath.C_Str());
		}

		// Get material reflectivity
		float materialReflectivity = 0.0f;

		numberOfFaces = mesh->mNumFaces;
		TriangleGroup triangleGroup(normalImagePath.C_Str(), diffuseImagePath.C_Str(),
			specularImagePath.C_Str(), materialReflectivity, totIndices, numberOfFaces);

		triangleGroups[i] = std::move(triangleGroup);

		// Get vertices
		{
			if (!mesh->HasTangentsAndBitangents())
			{
				throw std::exception("mesh has no tangent base!");
			}

			numberOfVertices = mesh->mNumVertices;
			for (size_t j = 0; j < numberOfVertices; j++)
			{
				vertices[totVertices].position = mesh->mVertices[j];
				vertices[totVertices].texCoords = mesh->mTextureCoords[0][j];
				vertices[totVertices].normal = mesh->mNormals[j];
				vertices[totVertices].tangent = mesh->mTangents[j];
				vertices[totVertices].bitangent = mesh->mBitangents[j];

				totVertices++;
			}
		}

		// Get indices
		{
			for (size_t j = 0; j < numberOfFaces; j++)
			{
				indices[totIndices++] = mesh->mFaces[j].mIndices[0] + indexOffset;
				indices[totIndices++] = mesh->mFaces[j].mIndices[1] + indexOffset;
				indices[totIndices++] = mesh->mFaces[j].mIndices[2] + indexOffset;
			}
		}
	}

	vertexBuffer.write(GL_ARRAY_BUFFER, vertices.data(), totVertices * sizeof(VertexAttribs),
		GL_STATIC_DRAW);

	indexBuffer.write(GL_ELEMENT_ARRAY_BUFFER, indices.data(), totIndices * sizeof(GLuint),
		GL_STATIC_DRAW);
}

ShadedModel3D::Material::Material() :
	pNormalMap(nullptr),
	pDiffuseColorMap(nullptr),
	pSpecularMap(nullptr)
{
}

ShadedModel3D::Material::Material(Material&& other) :
	pNormalMap(other.pNormalMap),
	pDiffuseColorMap(other.pDiffuseColorMap),
	pSpecularMap(other.pSpecularMap),
	reflectivity(other.reflectivity)
{
	other.pNormalMap = nullptr;
	other.pDiffuseColorMap = nullptr;
	other.pSpecularMap = nullptr;
}

ShadedModel3D::Material::Material(const char* normalMapFilename, const char* diffuseColorMapFilename, const char* specularMapFilename,
	GLfloat reflectivity) :
	pNormalMap(nullptr),
	pDiffuseColorMap(nullptr),
	pSpecularMap(nullptr),
	reflectivity(reflectivity)
{
	try
	{
		this->pNormalMap = new Texture(normalMapFilename, true);

		this->pDiffuseColorMap = new Texture(diffuseColorMapFilename, true);

		this->pSpecularMap = new Texture(specularMapFilename, true);
	}
	catch (...)
	{
		this->~Material();

		throw;
	}
}

ShadedModel3D::Material::~Material()
{
	if (this->pSpecularMap != nullptr)
	{
		delete this->pSpecularMap;
		this->pSpecularMap = nullptr;
	}

	if (this->pDiffuseColorMap != nullptr)
	{
		delete this->pDiffuseColorMap;
		this->pDiffuseColorMap = nullptr;
	}

	if (this->pNormalMap != nullptr)
	{
		delete this->pNormalMap;
		this->pNormalMap = nullptr;
	}
}

ShadedModel3D::Material& ShadedModel3D::Material::operator=(Material&& other)
{
	this->pNormalMap = other.pNormalMap;
	this->pDiffuseColorMap = other.pDiffuseColorMap;
	this->pSpecularMap = other.pSpecularMap;
	this->reflectivity = other.reflectivity;

	other.pNormalMap = nullptr;
	other.pDiffuseColorMap = nullptr;
	other.pSpecularMap = nullptr;

	return *this;
}

ShadedModel3D::TriangleGroup::TriangleGroup() :
	material()
{
}

ShadedModel3D::TriangleGroup::TriangleGroup(TriangleGroup&& other) :
	material(std::move(other.material)),
	indexBufferOffset(other.indexBufferOffset),
	numberOfTriangles(other.numberOfTriangles)
{
}

ShadedModel3D::TriangleGroup::TriangleGroup(const char* normalMapFilename,
	const char* diffuseColorMapFilename, const char* specularMapFilename, GLfloat materialReflectivity,
	GLsizei indexBufferOffset, GLsizei numberOfTriangles) :
	material(normalMapFilename, diffuseColorMapFilename, specularMapFilename, materialReflectivity),
	indexBufferOffset(indexBufferOffset),
	numberOfTriangles(numberOfTriangles)
{
}

ShadedModel3D::TriangleGroup& ShadedModel3D::TriangleGroup::operator=(TriangleGroup&& other)
{
	this->material = std::move(other.material);
	this->indexBufferOffset = other.indexBufferOffset;
	this->numberOfTriangles = other.numberOfTriangles;

	return *this;
}
