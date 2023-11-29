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
	}

	void Collider::CircleColliderStatic() 
	{
		
	}

	void Collider::LineVisualDebug()
	{
		if (this->linePoints.size() > 0)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
			K::Transform transform = K::Transform(new K::Vector3(), new K::Vector3(), new K::Vector3(1.0f, 1.0f, 1.0f));
			transform.PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &transform.modelMatrix.m[0][0]);
			for (int i = 0; i < this->linePoints.size(); i++)
			{
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
		this->LineVisualDebug();
		/*if (K::Physics::CanGetClosestPoint())
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "canChromaKey"), false);
			glUniform1i(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "hasTexture"), false);
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 0.0f, 1.0f);
			K::Vector3 point = K::Vector3();
			K::Vector3* pointOnLine = K::Physics::GetClosestPoint(point);
			K::Transform* transform = new K::Transform(new K::Vector3(pointOnLine->x, 0.0f, pointOnLine->y), new K::Vector3(), new K::Vector3(0.1f, 0.1f, 0.1f));
			transform->PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &transform->modelMatrix.m[0][0]);
			glBegin(GL_QUADS);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glEnd();
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 0.0f, 1.0f, 0.0f);
			K::Transform* actualPoint = new K::Transform(&point, new K::Vector3(), new K::Vector3(0.1f, 0.1f, 0.1f));
			actualPoint->PassModelMatrix();
			glUniformMatrix4fv(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "modelMatrix"), 1, GL_FALSE, &actualPoint->modelMatrix.m[0][0]);
			glBegin(GL_QUADS);
			glVertex3f(-1.0f, 0.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, -1.0f);
			glVertex3f(1.0f, 0.0f, -1.0f);
			glVertex3f(1.0f, 0.0f, 1.0f);
			glEnd();
			glUniform3f(glGetUniformLocation(this->parent->GetMaterial()->GetShader()->shader, "colorTint"), 1.0f, 1.0f, 1.0f);
		}*/
	}

	void Collider::CircleCollider() 
	{
		if (K::Physics::CanGetClosestPoint()) 
		{
			K::Vector3 J = *K::Physics::GetClosestPoint(K::Vector3(this->parent->GetTransform()->position->x, 0.0f, this->parent->GetTransform()->position->z));
			K::Vector3 originToJ = J - K::Vector3(this->parent->GetTransform()->position->x, 0.0f, this->parent->GetTransform()->position->z);
			K::Vector3 jToOrigin = K::Vector3(this->parent->GetTransform()->position->x, 0.0f, this->parent->GetTransform()->position->z) - J;
			if (originToJ.magnitude() <= this->radius)
			{
				jToOrigin.normalise();
				K::Vector3 contactResolution = originToJ + (jToOrigin * this->radius);
				*this->parent->GetTransform()->position += contactResolution;
			}
		}
	}

	K::Vector3* Collider::ClosestPointLineCollider(K::Vector3 P) 
	{
		int closestIndex = 0;
		float distance = INFINITY;
		for (int i = 0; i < this->linePoints.size(); i++) 
		{
			K::Vector3 PJ = *this->PointOnLine(this->linePoints[i].point[0], this->linePoints[i].point[1], P) - P;
			if (PJ.magnitude() < distance) 
			{
				distance = PJ.magnitude();
				closestIndex = i;
			}
		}
		return this->PointOnLine(this->linePoints[closestIndex].point[0], this->linePoints[closestIndex].point[1], P);
	}

	K::Vector3* Collider::PointOnLine(K::Vector3 A, K::Vector3 B, K::Vector3 P)
	{
		K::Vector3 N = K::Vector3(-(B.y - A.y), B.x - A.x, 0.0f);
		float C = (N.x*P.y) - (N.y*P.x);
		float p = (-((A.x * B.y - B.x * A.y) * N.x) - ((B.x - A.x) * C))/(((B.x - A.x) * N.y) + ((A.y - B.y) * N.x));
		float q = (C * (A.y - B.y) - N.y * (A.x * B.y - B.x * A.y)) / (N.y * (B.x - A.x) + (A.y - B.y) * (N.x));
		K::Vector3 J = K::Vector3(p, 0.0f, q);
		float AJMag = (J - A).magnitude();
		float BJMag = (J - B).magnitude();
		K::Vector3 AJ = (J - A).normalise();
		K::Vector3 BJ = (J - B).normalise();
		if (roundf(K::Vector3::DotProduct(AJ, BJ)) > 0.0f) 
		{
			if (AJMag > BJMag) 
			{
				return &B;
			}
			else 
			{
				return &A;
			}
		}
		else 
		{
			return &J;
		}
	}

	int Collider::GetNumberOfPoints() 
	{
		return this->linePoints.size();
	}

	void Collider::LineCollider() 
	{
		this->LineVisualDebug();
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

	void Collider::SetPropertyValues(const char* value)
	{
		if (value[0] != '\0' && value != nullptr)
		{
			std::string temp = value;
			std::string a(1, temp.back());
			int propertyNumber = std::stoi(a);
			temp.pop_back();
			switch (propertyNumber)
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
			case 2:
				this->radius = std::stof(temp);
				break;
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
		this->properties += "," + std::to_string(this->radius);
		this->properties += ",{";
		for (K::Line l : this->linePoints) 
		{
			this->properties += "," + std::to_string(l.point[0].x);
			this->properties += "," + std::to_string(l.point[0].y);
			this->properties += "," + std::to_string(l.point[1].x);
			this->properties += "," + std::to_string(l.point[1].y);
		}
		this->properties += "}";
		return this->properties.c_str();
	}

	const char* Collider::GetName()
	{
		return typeid(K::Collider).name();
	}
}