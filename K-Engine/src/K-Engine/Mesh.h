#pragma once
#include "build.h"
#include "Transform.h"

namespace K 
{
	K_API struct Vertex 
	{
		K::Vector3 position = K::Vector3(0.0f, 0.0f, 0.0f);
		K::Vector2 uv = K::Vector2(0.0f, 0.0f);
		Vertex(Vector3 position, Vector2 uv)
		{
			this->position = position;
			this->uv = uv;
		}

		K::Vertex &operator += (const K::Vector3 *other)
		{
			this->position.x += other->x;
			this->position.y += other->y;
			this->position.z += other->z;
			return *this;
		}
	};

	const std::vector<K::Vertex> Quad = {K::Vertex(K::Vector3(1.0f, 0.0f, 1.0f), K::Vector2(1.0f, 1.0f)),
										 K::Vertex(K::Vector3(1.0f, 0.0f, -1.0f), K::Vector2(1.0f, 0.0f)),
										 K::Vertex(K::Vector3(-1.0f, 0.0f, 1.0f), K::Vector2(0.0f, 1.0f)),
										 K::Vertex(K::Vector3(-1.0f, 0.0f, -1.0f), K::Vector2(0.0f, 0.0f))};

	class K_API Mesh
	{
	public:
		//Model Vertices
		std::vector<K::Vertex> vertices;
		std::vector<K::Vector3> normals;

		//Model Indices
		std::vector<int> indices = { 
			0, 3, 1,
			0, 2, 3
		};

		//Model Shader
		K::Shader* shader;

		//Constructor
		Mesh()
		{
			this->vertices = K::Quad;
		}

		//Destructor
		~Mesh()
		{

		}

		//Operators

		std::vector<K::Vertex> &operator += (const K::Vector3 *other) 
		{
			for (int i = 0; i < this->vertices.size(); i++)
			{
				this->vertices[i].position += *other;
			}
			return this->vertices;
		}

		std::vector<K::Vertex>& operator *= (const K::Vector3* other)
		{
			for (int i = 0; i < this->vertices.size(); i++)
			{
				this->vertices[i].position *= *other;
			}
			return this->vertices;
		}

		std::vector<K::Vertex>& operator *= (K::Quaternion* other)
		{
			K::Matrix4x4 mat = *other->QuaternionToMatrix();
			for (int i = 0; i < this->vertices.size(); i++)
			{
				K::Vector3* temp = new K::Vector3(0.0f, 0.0f, 0.0f);
				MultiplyMatrixVector(this->vertices[i].position, *temp, mat);
				this->vertices[i].position = *temp;
			}
			return this->vertices;
		}

		K::Vertex &GetVertexPositions() 
		{
			return this->vertices[0];
		}

		bool LoadModelsAssimp(std::string File)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_GenNormals);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
				return false;
			}
			for (int j = 0; j < (scene->mNumMeshes); j++)
			{
				aiMesh* mesh = scene->mMeshes[j];
				this->vertices.clear();
				this->indices.clear();
				for (int i = 0; i < mesh->mNumVertices; i++)
				{
					float u, v;
					if (mesh->HasTextureCoords(0))
					{
						u = mesh->mTextureCoords[0][i].x;
						v = mesh->mTextureCoords[0][i].y;
					}
					else
					{
						u = 0.0f;
						v = 0.0f;
					}
					this->normals.push_back(K::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
					this->vertices.push_back(K::Vertex(K::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), K::Vector2(u, v)));
				}
				for (int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					for (int j = 0; j < face.mNumIndices; j++)
					{
						int index = face.mIndices[j];
						this->indices.push_back(index);
					}
				}
			}
			return true;
		}
	};
}