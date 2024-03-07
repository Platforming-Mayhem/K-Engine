#include "Mesh.h"
#include "GameObject.h"
#include "Editor.h"

namespace K 
{
	Mesh::Mesh() 
	{
		
	}

	Mesh::~Mesh() 
	{
		std::cout << "Begin Mesh Destruction..." << std::endl;
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
		this->vertices.clear();
		this->indices.clear();
		std::cout << "End Mesh Destruction..." << std::endl;
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
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), colourTint[0], colourTint[1], colourTint[2]);
	}

	void Mesh::Update() 
	{
		glDrawElements(GL_TRIANGLES, this->indices.size() * sizeof(int), GL_UNSIGNED_INT, nullptr);
	}

	void Mesh::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Mesh Renderer Settings")) 
		{
			ImGui::Text("Vertices: %i", this->vertices.size());
			ImGui::Text("Indices: %i", this->indices.size());
			ImGui::ColorPicker3("Colour Tint", this->colourTint);
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
				this->colourTint[0] = std::stof(temp);
				break;
			case 1:
				this->colourTint[1] = std::stof(temp);
				break;
			case 2:
				this->colourTint[2] = std::stof(temp);
				break;
			}
		}
	}

	const char* Mesh::GetPropertyValues()
	{
		this->properties = std::to_string(this->colourTint[0]) + ",";
		this->properties += std::to_string(this->colourTint[1]) + ",";
		this->properties += std::to_string(this->colourTint[2]);
		return this->properties.c_str();
	}

	const char* Mesh::GetFilePath() 
	{
		return this->filename.c_str();
	}

	const char* Mesh::GetName()
	{
		return typeid(K::Mesh).name();
	}
}