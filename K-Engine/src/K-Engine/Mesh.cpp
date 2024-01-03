#include "Mesh.h"
#include "GameObject.h"

namespace K 
{
	Mesh::Mesh() 
	{
		
	}

	Mesh::~Mesh() 
	{
		std::cout << "Begin Mesh Destruction..." << std::endl;
		this->vertices.clear();
		this->indices.clear();
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
		std::cout << "End Mesh Destruction..." << std::endl;
	}

	void Mesh::Init() 
	{
		if (this->vertices.size() <= 0) 
		{
			this->vertices = K::Quad;
		}
		if (this->indices.size() <= 0) 
		{
			this->indices = { 0, 3, 1, 0, 2, 3 };
		}

		glCreateVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		this->Bind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int), &this->indices[0], GL_DYNAMIC_DRAW);

		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(K::Vertex), &this->vertices[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

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
		glDrawElements(GL_TRIANGLES, this->indices.size() * sizeof(int), GL_UNSIGNED_INT, nullptr);
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
					this->filename = location;
				}
				file.ClearSelected();
			}
			ImGui::Text("Vertices: %i", this->vertices.size());
			ImGui::Text("Indices: %i", this->indices.size());
		}
	}

	void Mesh::Unbind() 
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Mesh::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				if (this->LoadModelsAssimp(temp.c_str()))
				{
					this->filename = temp.c_str();
				}
				break;
			}
		}
	}

	const char* Mesh::GetPropertyValues()
	{
		return this->GetFilePath();
	}

	const char* Mesh::GetFilePath() 
	{
		return this->filename.c_str();
	}

	const char* Mesh::GetName()
	{
		return typeid(K::Mesh).name();
	}

	//IMPLEMENT THIS LATER
	void Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			
		}
		// process indices

		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			
		}
	}

	//IMPLEMENT THIS LATER
	void Mesh::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	bool Mesh::LoadModelsAssimp(std::string File)
	{
		this->vertices.clear();
		this->vertices.shrink_to_fit();
		this->indices.clear();
		this->indices.shrink_to_fit();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(File, aiProcess_Triangulate | aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			return false;
		}
		for (int j = 0; j < scene->mNumMeshes; j++)
		{
			aiMesh* mesh = scene->mMeshes[j];
			for (int i = 0; i < mesh->mNumVertices; i++)
			{
				float u = 0.0f;
				float v = 0.0f;
				if (mesh->HasTextureCoords(0))
				{
					u = mesh->mTextureCoords[0][i].x;
					v = mesh->mTextureCoords[0][i].y;
				}
				K::Vector3 normal = K::Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				this->vertices.push_back(K::Vertex(K::Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z), K::Vector2(u, v), normal));
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