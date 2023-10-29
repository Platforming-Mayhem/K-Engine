#include "Mesh.h"
#include "GameObject.h"

namespace K 
{
	Mesh::Mesh() 
	{
		this->vertices = K::Quad;
	}

	Mesh::~Mesh() 
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
	}

	void Mesh::Init() 
	{
		glCreateVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		this->Bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_DYNAMIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(K::Vertex), &this->vertices[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		this->Unbind();
	}

	void Mesh::Bind() 
	{
		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(K::Vertex), &this->vertices[0], GL_DYNAMIC_DRAW);
	}

	void Mesh::Update() 
	{
		this->Bind();

		glDrawElements(GL_TRIANGLES, this->indices.size() * sizeof(int), GL_UNSIGNED_INT, nullptr);

		this->Unbind();
	}

	void Mesh::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Mesh Renderer Settings")) 
		{
			if (ImGui::Button("Load Model"))
			{
				file.SetTitle("Load Model");
				file.SetTypeFilters({ ".FBX", ".OBJ" });
				file.Open();
			}
			file.Display();
			if (file.HasSelected())
			{
				std::string location = file.GetSelected().string();
				if (this->LoadModelsAssimp(location))
				{
					std::cout << "Found model" << std::endl;
					this->meshUpdate = true;
				}
				file.ClearSelected();
			}
		}
	}

	void Mesh::Unbind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	const char* Mesh::GetName()
	{
		return typeid(K::Mesh).name();
	}

	bool Mesh::LoadModelsAssimp(std::string File)
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
}