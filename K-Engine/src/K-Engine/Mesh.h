#pragma once
#include "build.h"
#include "Transform.h"
#include "Material.h"
#include "Component.h"

namespace K 
{
	K_API struct Vertex 
	{
		K::Vector3 position = K::Vector3(0.0f, 0.0f, 0.0f);
		K::Vector2 uv = K::Vector2(0.0f, 0.0f);
		K::Vector3 normal = K::Vector3(0.0f, -1.0f, 0.0f);
		Vertex(Vector3 position, Vector2 uv, Vector3 normal)
		{
			this->position = position;
			this->uv = uv;
			this->normal = normal;
		}

		Vertex(Vector3 position, Vector2 uv)
		{
			this->position = position;
			this->uv = uv;
		}

		~Vertex()
		{
			std::cout << "Vertex Destructor..." << std::endl;
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

	class K_API Mesh : public K::Component
	{
	private:
		K::Material material;
		ImGui::FileBrowser file;
		std::string filename;
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
	public:
		//Model Vertices
		std::vector<K::Vertex> vertices = K::Quad;

		//Model Indices
		std::vector<int> indices = 
		{ 
			0, 3, 1,
			0, 2, 3
		};

		//Constructor
		Mesh();

		//Destructor
		~Mesh();

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

		bool LoadModelsAssimp(std::string File);

		void processMesh(aiMesh* mesh, const aiScene* scene);
		
		void processNode(aiNode* node, const aiScene* scene);

		void Init() override;

		void Update() override;

		void UpdateEditor() override;

		void Bind() override;

		void Unbind()  override;

		void SetPropertyValues(const char* value, int valueIndex) override;

		const char* GetPropertyValues() override;

		const char* GetName() override;

		const char* GetFilePath();
	};
}