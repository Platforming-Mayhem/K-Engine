#include "TriggerNextScene.h"

namespace K 
{
	TriggerNextScene::TriggerNextScene()
	{
		this->bounds[0] = K::Vector3();
		this->bounds[1] = K::Vector3();
		this->boundsModelMatrix[0] = K::Vector3();
		this->boundsModelMatrix[1] = K::Vector3();
	}

	TriggerNextScene::~TriggerNextScene()
	{

	}

	void TriggerNextScene::VisualizeTriggerZone()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
		K::Transform temp = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
		temp.PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &temp.modelMatrix.m[0][0]);

		glBegin(GL_LINE_LOOP);
		glVertex3f(this->boundsModelMatrix[0].x, this->boundsModelMatrix[0].y, this->boundsModelMatrix[0].z);
		glVertex3f(this->boundsModelMatrix[0].x, this->boundsModelMatrix[0].y, this->boundsModelMatrix[1].z);
		glVertex3f(this->boundsModelMatrix[1].x, this->boundsModelMatrix[1].y, this->boundsModelMatrix[1].z);
		glVertex3f(this->boundsModelMatrix[1].x, this->boundsModelMatrix[1].y, this->boundsModelMatrix[0].z);
		glEnd();

		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
	}

	void TriggerNextScene::Update() 
	{
		K::MultiplyMatrixVector(this->bounds[0], this->boundsModelMatrix[0], this->parent->GetTransform()->modelMatrix);
		K::MultiplyMatrixVector(this->bounds[1], this->boundsModelMatrix[1], this->parent->GetTransform()->modelMatrix);
		K::Collider* temp = nullptr;
		if (K::Physics::Hitbox(this->boundsModelMatrix[0], this->boundsModelMatrix[1], { K::Layer::LayerType::Enemy, K::Layer::LayerType::Ground }, &temp))
		{
			K::SceneManager::LoadNextScene();
		}
	}

	void TriggerNextScene::UpdateEditor()
	{
		if (ImGui::CollapsingHeader(this->GetName()))
		{
			ImGui::DragFloat3("Bottom Left", &this->bounds[0].x);
			ImGui::DragFloat3("Top Right", &this->bounds[1].x);
			this->VisualizeTriggerZone();
		}
	}

	void TriggerNextScene::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				break;
			}
		}
	}

	const char* TriggerNextScene::GetPropertyValues()
	{
		return this->properties.c_str();
	}
}