#include "Serializer.h"
#include "Editor.h"

namespace K 
{
	Serializer::Serializer(K::Scene* scene, std::string location)
	{
		std::ofstream outFile;
		std::string name = location;
		if (location.c_str() == nullptr)
		{
			name = location.c_str();
			name += ".JAWS";
		}
		outFile.open(name.c_str());
		if (!outFile)
		{
			std::cerr << "Error - unable to open output file " << name.c_str() << std::endl;
			exit(1);
		}
		int i = 0;
		for (auto it : scene->GetGameObjects())
		{
			if (i == 0) 
			{
				std::cout << "Saving to " << name << std::endl;
			}
			K::GameObject* g = it.second;
			K::Transform* transform = g->GetTransform();
			outFile << g->GetName();
			outFile << "," << g->GetIndex();
			outFile << "," << transform->position->x << "," << transform->position->y << "," << transform->position->z;
			outFile << "," << transform->rotation->x << "," << transform->rotation->y << "," << transform->rotation->z;
			outFile << "," << transform->scale->x << "," << transform->scale->y << "," << transform->scale->z;
			outFile << "," << transform->localPosition->x << "," << transform->localPosition->y << "," << transform->localPosition->z;
			outFile << "," << transform->localRotation->x << "," << transform->localRotation->y << "," << transform->localRotation->z;
			outFile << "," << transform->localScale->x << "," << transform->localScale->y << "," << transform->localScale->z;
			if (g->parent == nullptr)
				outFile << "," << "-1";
			else
				outFile << "," << g->parent->GetIndex();
			std::cout << g->GetName();
			std::cout << " " << transform->position->x << "," << transform->position->y << "," << transform->position->z;
			std::cout << " " << transform->rotation->x << "," << transform->rotation->y << "," << transform->rotation->z;
			std::cout << " " << transform->scale->x << "," << transform->scale->y << "," << transform->scale->z << " ";
			int numberOfComponents = g->GetNumberOfComponents();
			for (int i = 0; i < numberOfComponents; i++)
			{
				K::Component* component = g->GetComponent(i);
				outFile << "," << component->GetName();
				if (component->GetPropertyValues()[0] != '\0')
				{
					outFile << "," << component->GetPropertyValues();
				}
			}
			if (i < scene->GetNumberOfObjects())
			{
				outFile << ",";
				outFile << '\n';
			}
			std::cout << std::endl;
			i++;
		}
		outFile.close();
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location) 
	{
		std::ifstream inFile;
		inFile.open(ASSET_DIR + location);
		std::map<K::GameObject* , int> parents;
		if (inFile)
		{
			std::string line;
			K::Component* currentComponent = nullptr;
			while (std::getline(inFile, line))
			{
				K::Vector3* position = new K::Vector3(0.0f, 0.0f, 0.0f);
				K::Vector3* rotation = new K::Vector3(0.0f, 0.0f, 0.0f);
				K::Vector3* scale = new K::Vector3(1.0f, 1.0f, 1.0f);
				K::Transform* transform = new K::Transform(position, rotation, scale);
				std::string val = "";
				K::GameObject* temp = nullptr;
				std::string name = "";
				int componentCount = 0;
				int number = 0;
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] == ',')
					{
						switch (number)
						{
						case 0:
							name = val;
							break;
						case 1:
							temp = new K::GameObject(name.c_str(), transform, std::stoi(val));
							break;
						case 2:
							position->x = std::stof(val);
							break;
						case 3:
							position->y = std::stof(val);
							break;
						case 4:
							position->z = std::stof(val);
							break;
						case 5:
							rotation->x = std::stof(val);
							break;
						case 6:
							rotation->y = std::stof(val);
							break;
						case 7:
							rotation->z = std::stof(val);
							break;
						case 8:
							scale->x = std::stof(val);
							break;
						case 9:
							scale->y = std::stof(val);
							break;
						case 10:
							scale->z = std::stof(val);
							break;
						case 11:
							temp->GetTransform()->localPosition->x = std::stof(val);
							break;
						case 12:
							temp->GetTransform()->localPosition->y = std::stof(val);
							break;
						case 13:
							temp->GetTransform()->localPosition->z = std::stof(val);
							break;
						case 14:
							temp->GetTransform()->localRotation->x = std::stof(val);
							break;
						case 15:
							temp->GetTransform()->localRotation->y = std::stof(val);
							break;
						case 16:
							temp->GetTransform()->localRotation->z = std::stof(val);
							break;
						case 17:
							temp->GetTransform()->localScale->x = std::stof(val);
							break;
						case 18:
							temp->GetTransform()->localScale->y = std::stof(val);
							break;
						case 19:
							temp->GetTransform()->localScale->z = std::stof(val);
							break;
						case 20:
							if (std::stoi(val) != -1) 
							{
								parents.insert({ temp, std::stoi(val) });
							}
							break;
						}
						if (number > 20)
						{
							std::map<std::string, K::IFactory*>::iterator pos = K::Editor::lst.find(val);
							if (pos == K::Editor::lst.end())
							{
								std::cout << "Setting Values of " << currentComponent->GetName() << " to: " << val << std::endl;
								currentComponent->SetPropertyValues(val.c_str(), componentCount);
								componentCount++;
							}
							else
							{
								currentComponent = pos->second->create();
								std::cout << "Creating " << currentComponent->GetName() << std::endl;
								temp->AddComponent(currentComponent);
								componentCount = 0;
							}
						}
						line.erase(0, i);
						i = 0;
						val = "";
						number++;
					}
					else
					{
						val += line[i];
					}
				}
			}
			inFile.close();
			if (!parents.empty())
			{
				for (auto temp : parents)
				{
					if (temp.first->SetParent(K::Editor::GetCurrentScene()->GetGameObjects().at(temp.second)))
					{
						std::cout << "Parent: " << K::Editor::GetCurrentScene()->GetGameObjects().at(temp.second)->GetName() << "," << " Child: " << temp.first->GetName() << std::endl;
					}
					else
					{
						std::cout << "Failed Setting Parent: " << K::Editor::GetCurrentScene()->GetGameObjects().at(temp.second)->GetName() << "," << " Child: " << temp.first->GetName() << std::endl;
					}
				}
				parents.clear();
			}
			std::cout << "Finished setting parents..." << std::endl;
		}
		std::cout << ASSET_DIR + location;
		std::cout << "..." << std::endl;
		newScene->SetSceneName(location);
		newScene->SetLocation(location);
		newScene->Init();
	}
}