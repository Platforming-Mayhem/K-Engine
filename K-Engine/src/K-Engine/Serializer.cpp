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
		for (int i = 0; i < scene->GetNumberOfObjects(); i++) 
		{
			if (i == 0) 
			{
				std::cout << "Saving to " << name << std::endl;
			}
			else 
			{
				K::GameObject* g = scene->GetGameObjects()[i];
				K::Transform* transform = g->GetTransform();
				outFile << g->GetName();
				outFile << "," << transform->position->x << "," << transform->position->y << "," << transform->position->z;
				outFile << "," << transform->rotation->x << "," << transform->rotation->y << "," << transform->rotation->z;
				outFile << "," << transform->scale->x << "," << transform->scale->y << "," << transform->scale->z;
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
			}
		}
		outFile.close();
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location) 
	{
		std::ifstream inFile;
		inFile.open(ASSET_DIR + location);
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
				int componentCount = 0;
				int number = 0;
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] == ',')
					{
						switch (number)
						{
						case 0:
							temp = new K::GameObject(val.c_str(), transform);
							break;
						case 1:
							position->x = std::stof(val);
							break;
						case 2:
							position->y = std::stof(val);
							break;
						case 3:
							position->z = std::stof(val);
							break;
						case 4:
							rotation->x = std::stof(val);
							break;
						case 5:
							rotation->y = std::stof(val);
							break;
						case 6:
							rotation->z = std::stof(val);
							break;
						case 7:
							scale->x = std::stof(val);
							break;
						case 8:
							scale->y = std::stof(val);
							break;
						case 9:
							scale->z = std::stof(val);
							break;
						}
						if (number > 9)
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
		}
		std::cout << ASSET_DIR + location;
		std::cout << "..." << std::endl;
		newScene->SetSceneName(location);
		newScene->SetLocation(location);
		newScene->Init();
	}
}