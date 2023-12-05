#include "Collider.h"
#include "Time.h"
#include "InputManager.h"
#include "PhysicsEngine.h"

namespace K 
{
	Collider::Collider() 
	{
		
	}

	Collider::~Collider()
	{
		std::cout << "Begin Collider Destruction..." << std::endl;
		K::Physics::Remove(this);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
		this->linePoints.clear();
		std::cout << "End Collider Destruction..." << std::endl;
	}

	void Collider::Init()
	{
		K::Physics::Attach(this);
	}

	void Collider::UpdateEditor()
	{
		if (ImGui::CollapsingHeader("Collider Settings")) 
		{
			ImGui::Checkbox("Is Static", &this->isStatic);
			if (this->colliderType == ColliderType::Circle)
			{
				if (ImGui::BeginCombo("Collider Type", "Circle"))
				{
					if (ImGui::Selectable("Circle"))
					{
						this->colliderType = ColliderType::Circle;
					}
					else if (ImGui::Selectable("Line"))
					{
						this->colliderType = ColliderType::Line;
					}
					ImGui::EndCombo();
				}
				this->CircleVisualDebug();
				this->CircleEditor();
			}
			else if (this->colliderType == ColliderType::Line)
			{
				if (ImGui::BeginCombo("Collider Type", "Line"))
				{
					if (ImGui::Selectable("Circle"))
					{
						this->colliderType = ColliderType::Circle;
					}
					else if (ImGui::Selectable("Line"))
					{
						this->colliderType = ColliderType::Line;
					}
					ImGui::EndCombo();
				}
				this->LineVisualDebug();
				this->LineEditor();
			}
		}
	}

	void Collider::LineEditor() 
	{
		if (ImGui::Button("Add Line")) 
		{
			this->linePoints.push_back(K::Line(K::Vector3(0.0f, 0.0f, 0.0f), K::Vector3(1.0f, 0.0f, 0.0f)));
			this->selectedLine = this->linePoints.size() - 1;
		}
		if (this->linePoints.size() > 0) 
		{
			if (ImGui::BeginListBox("Line Points")) 
			{
				for (int i = 0; i < this->linePoints.size(); i++)
				{
					if (ImGui::Selectable(std::to_string(i).c_str()))
					{
						this->selectedLine = i;
					}
				}
				ImGui::EndListBox();
			}

			ImGui::DragFloat("X1:", &this->linePoints[this->selectedLine].point[0].x);
			ImGui::DragFloat("Y1:", &this->linePoints[this->selectedLine].point[0].y);
			ImGui::DragFloat("X2:", &this->linePoints[this->selectedLine].point[1].x);
			ImGui::DragFloat("Y2:", &this->linePoints[this->selectedLine].point[1].y);
			if (ImGui::Button("Delete Line")) 
			{
				this->linePoints.erase(this->linePoints.begin() + this->selectedLine);
				this->selectedLine = 0;
			}
		}
	}

	void Collider::CircleEditor()
	{
		ImGui::DragFloat("Circle Radius", &this->radius);
		ImGui::DragFloat3("Offset", (float*)&this->offset);
	}

	void Collider::CircleColliderStatic() 
	{
		
	}

	void Collider::CircleVisualDebug() 
	{
		//Draw Circle
		glClear(GL_DEPTH_BUFFER_BIT);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
		glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
		K::Vector3 position = *this->GetPosition();
		K::Transform transform = K::Transform(&position, new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
		transform.PassModelMatrix();
		glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &transform.modelMatrix.m[0][0]);
		float theta = 360.0f / 16.0f;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 16; i++)
		{
			glVertex3f(this->radius * cosf((i * theta) / 57.2958f), 0.0f, this->radius * sinf((i * theta) / 57.2958f));
		}
		glEnd();
		//Draw contact points
		if (K::Physics::CanGetClosestPoint()) 
		{
			//DEBUGGING
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			for (K::Vector3 pointOnLine : K::Physics::GetClosestPoints(position))
			{
				if ((pointOnLine - position).magnitude() < this->GetRadius())
				{
					glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 0.0f, 0.0f);
				}
				else 
				{
					glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 0.0f, 1.0f);
				}
				K::Transform* transform = new K::Transform(&pointOnLine, new K::Vector3(), new K::Vector3(0.1f, 0.1f, 0.1f));
				transform->PassModelMatrix();
				glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &transform->modelMatrix.m[0][0]);
				glBegin(GL_QUADS);
				glVertex3f(-1.0f, 0.0f, 1.0f);
				glVertex3f(-1.0f, 0.0f, -1.0f);
				glVertex3f(1.0f, 0.0f, -1.0f);
				glVertex3f(1.0f, 0.0f, 1.0f);
				glEnd();
			}
		}
		glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
	}

	float Collider::GetRadius() 
	{
		return this->radius;
	}

	void Collider::LineVisualDebug()
	{
		if (this->linePoints.size() > 0)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			K::Transform transform = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
			transform.PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &transform.modelMatrix.m[0][0]);
			for (int i = 0; i < this->linePoints.size(); i++)
			{
				if (i == this->selectedLine) 
				{
					glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 0.0f, 1.0f);
				}
				else 
				{
					glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
				}
				glBegin(GL_LINES);
				glVertex3f(this->linePoints[i].point[0].x, 0.0f, this->linePoints[i].point[0].y);
				glVertex3f(this->linePoints[i].point[1].x, 0.0f, this->linePoints[i].point[1].y);
				glEnd();
			}
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
		}
	}

	void Collider::LineColliderStatic() 
	{
		
	}

	void Collider::CircleCollider() 
	{
		if (K::Physics::CanGetClosestPoint()) 
		{
			K::Vector3* temp = K::Physics::GetCollisionResolution(this);
			*this->parent->GetTransform()->position += temp;
		}
	}

	K::Vector3* Collider::ClosestPointLineCollider(K::Vector3 P) 
	{
		int closestIndex = 0;
		float distance = INFINITY;
		for (int i = 0; i < this->linePoints.size(); i++) 
		{
			K::Vector3 J = K::Vector3(this->PointOnLine(this->linePoints[i].point[0], this->linePoints[i].point[1], P)->x, P.y, this->PointOnLine(this->linePoints[i].point[0], this->linePoints[i].point[1], P)->y);
			K::Vector3 PJ = J - P;
			if (PJ.magnitude() < distance && PJ.magnitude() > 0.0f) 
			{
				distance = PJ.magnitude();
				closestIndex = i;
			}
			else if (PJ.magnitude() <= 0.0f) 
			{
				distance = 0.0f;
				closestIndex = i;
				break;
			}
		}
		K::Vector3 J = K::Vector3(this->PointOnLine(this->linePoints[closestIndex].point[0], this->linePoints[closestIndex].point[1], P)->x, 0.0f, this->PointOnLine(this->linePoints[closestIndex].point[0], this->linePoints[closestIndex].point[1], P)->y);
		return &J;
	}

	K::Vector3* Collider::PointOnLine(K::Vector3 A, K::Vector3 B, K::Vector3 P)
	{
		K::Vector3 N = K::Vector3(-(B.y - A.y), B.x - A.x, 0.0f);
		float C = (N.x*P.z) - (N.y*P.x);
		float p = (-((A.x * B.y - B.x * A.y) * N.x) - ((B.x - A.x) * C))/(((B.x - A.x) * N.y) + ((A.y - B.y) * N.x));
		float q = (C * (A.y - B.y) - N.y * (A.x * B.y - B.x * A.y)) / (N.y * (B.x - A.x) + (A.y - B.y) * (N.x));
		K::Vector3 J = K::Vector3(p, q, 0.0f);
		K::Vector3 AJ = J - A;
		K::Vector3 BJ = J - B;
		K::Vector3 AB = A - B;
		float U = K::Vector3::DotProduct(BJ, AB);
		float V = K::Vector3::DotProduct(AJ, AB);
		if (V >= 0.0f) 
		{
			return &A;
		}
		else if (U <= 0.0f) 
		{
			return &B;
		}
		else if (U > 0.0f && V < 0.0f)
		{
			return &J;
		}
	}

	K::Line* Collider::GetLine(int index) 
	{
		return &this->linePoints[index];
	}

	K::Vector3* Collider::GetOffset() 
	{
		return &this->offset;
	}

	K::Vector3* Collider::GetPosition() 
	{
		K::Vector3 pos = *this->parent->GetTransform()->position;
		pos += this->offset;
		return &pos;
	}

	int Collider::GetNumberOfPoints() 
	{
		return this->linePoints.size();
	}

	void Collider::SetIsColliding(bool value) 
	{
		this->isColliding = value;
	}

	bool Collider::IsColliding() 
	{
		return this->isColliding;
	}

	bool Collider::IsStatic() 
	{
		return this->isStatic;
	}

	void Collider::LineCollider() 
	{
		
	}

	void Collider::Update()
	{
		if (this->isStatic)
		{
			switch (this->colliderType)
			{
			case ColliderType::Circle:
				CircleColliderStatic();
				break;
			case ColliderType::Line:
				LineColliderStatic();
				break;
			}
		}
		else
		{
			switch (this->colliderType)
			{
			case ColliderType::Circle:
				CircleCollider();
				break;
			case ColliderType::Line:
				LineCollider();
				break;
			}
		}
	}

	void Collider::Unbind()
	{
		
	}

	void Collider::Bind()
	{
		
	}

	int pointNumber = 0;

	void Collider::SetPoints(const char* value)
	{
		float temp = std::stof(value);
		pointNumber++;
		if (pointNumber % 2 == 0) 
		{
			//EVEN
			if (pointNumber % 4 == 0) 
			{
				this->linePoints[this->selectedLine].point[1].y = temp;
			}
			else 
			{
				this->linePoints[this->selectedLine].point[0].y = temp;
			}
		}
		else 
		{
			//ODD
			if ((pointNumber - 1) % 4 == 0) 
			{
				this->linePoints.push_back(K::Line(K::Vector3(), K::Vector3()));
				this->selectedLine = this->linePoints.size() - 1;
				//Set Value
				this->linePoints[this->selectedLine].point[0].x = temp;
			}
			else 
			{
				this->linePoints[this->selectedLine].point[1].x = temp;
			}
		}
	}

	void Collider::SetPropertyValues(const char* value, int valueIndex)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			switch (valueIndex)
			{
			case 0:
				if (temp == "true")
				{
					this->SetStaticState(true);
				}
				else if (temp == "false")
				{
					this->SetStaticState(false);
				}
				break;
			case 1:
				if (temp == std::to_string((int)ColliderType::Circle))
				{
					this->colliderType = ColliderType::Circle;
				}
				else if (temp == std::to_string((int)ColliderType::Line))
				{
					this->colliderType = ColliderType::Line;
				}
				break;
			}

			if (valueIndex > 1)
			{
				if (this->colliderType == ColliderType::Circle) 
				{
					this->radius = std::stof(temp);
				}
				else if (this->colliderType == ColliderType::Line) 
				{
					this->SetPoints(temp.c_str());
				}
			}
		}
	}

	void Collider::SetStaticState(bool state) 
	{
		this->isStatic = state;
	}

	const char* Collider::GetPropertyValues() 
	{
		std::string temp;
		if (this->isStatic)
		{
			temp = "true";
		}
		else
		{
			temp = "false";
		}
		this->properties = temp;
		this->properties += "," + std::to_string((int)this->colliderType);
		if (this->colliderType == ColliderType::Circle)
		{
			this->properties += "," + std::to_string(this->radius);
		}
		else if (this->colliderType == ColliderType::Line) 
		{
			for (K::Line l : this->linePoints)
			{
				this->properties += "," + std::to_string(l.point[0].x);
				this->properties += "," + std::to_string(l.point[0].y);
				this->properties += "," + std::to_string(l.point[1].x);
				this->properties += "," + std::to_string(l.point[1].y);
			}
		}
		return this->properties.c_str();
	}

	const char* Collider::GetName()
	{
		return typeid(K::Collider).name();
	}
}