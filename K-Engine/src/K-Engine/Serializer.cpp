#include "Serializer.h"

namespace K 
{
	Serializer::Serializer(K::Scene* scene)
	{
		std::ofstream outFile;
		std::string name = *scene->GetSceneName();
		name += ".JAWS";
		outFile.open(name.c_str());
		if (!outFile)
		{
			std::cerr << "Error - unable to open output file " << scene->GetSceneName()->c_str() << std::endl;
			exit(1);
		}
		int count = 0;
		int nOGameObjects = scene->GetNumberOfObjects() - 1;
		for (K::GameObject* g : scene->GetGameObjects()) 
		{
			if (count == 0) 
			{
				std::cout << "Saving..." << std::endl;
			}
			else 
			{
				K::Transform transform = *g->GetTransform();
				outFile << g->GetName();
				outFile << "," << transform.position->x << "," << transform.position->y << "," << transform.position->z;
				outFile << "," << transform.rotation->x << "," << transform.rotation->y << "," << transform.rotation->z;
				outFile << "," << transform.scale->x << "," << transform.scale->y << "," << transform.scale->z;
				std::cout << g->GetName();
				std::cout << " " << transform.position->x << "," << transform.position->y << "," << transform.position->z;
				std::cout << " " << transform.rotation->x << "," << transform.rotation->y << "," << transform.rotation->z;
				std::cout << " " << transform.scale->x << "," << transform.scale->y << "," << transform.scale->z << " ";
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
				if (count < nOGameObjects)
				{
					outFile << ",";
					outFile << '\n';
				}
				std::cout << std::endl;
			}
			count++;
		}
		outFile.close();
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location, K::Editor* editor) 
	{
		std::ifstream inFile;
		inFile.open(location);
		if (inFile)
		{
			std::string line;
			K::Component* currentComponent = nullptr;
			while (std::getline(inFile, line))
			{
				std::string name = "";
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
							name = val;
							temp = new K::GameObject(name.c_str(), transform, editor->GetMaterial());
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
							bool found = false;
							for (K::IFactory* comp : editor->lst)
							{
								K::Component* tempCo = comp->create();
								if (val == tempCo->GetName())
								{
									std::cout << "Creating " << tempCo->GetName() << std::endl;
									temp->AddComponent(tempCo);
									currentComponent = tempCo;
									componentCount = 0;
									found = true;
									break;
								}
								else
								{
									delete tempCo;
								}
							}
							if (!found)
							{
								std::cout << "Setting Values of " << currentComponent->GetName() << " to: " << val << std::endl;
								currentComponent->SetPropertyValues(val.c_str(), componentCount);
								componentCount++;
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
				newScene->Attach(temp);
			}
			inFile.close();
		}
		newScene->Init();
	}

	Deserializer::Deserializer(K::Scene* newScene, unsigned int resource, K::Editor* editor)
	{
		HMODULE hModule;
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR) & "main", &hModule);
		HRSRC hr = FindResource(hModule, MAKEINTRESOURCE(resource), "SCENE");
		int size = SizeofResource(hModule, hr);
		if (hr == NULL)
		{
			
		}
		else 
		{
			HGLOBAL temp = LoadResource(hModule, hr);
			LPVOID lp = LockResource(temp);
			std::stringstream inFile;
			const char* data = (const char*)lp;
			inFile << data;
			std::string line;
			K::Component* currentComponent = nullptr;
			while (std::getline(inFile, line))
			{
				std::string name = "";
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
							name = val;
							temp = new K::GameObject(name.c_str(), transform, editor->GetMaterial());
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
							bool found = false;
							for (K::IFactory* comp : editor->lst)
							{
								K::Component* tempCo = comp->create();
								if (val == tempCo->GetName())
								{
									std::cout << "Creating " << tempCo->GetName() << std::endl;
									temp->AddComponent(tempCo);
									currentComponent = tempCo;
									componentCount = 0;
									found = true;
									break;
								}
								else 
								{
									delete tempCo;
								}
							}
							if (!found)
							{
								std::cout << "Setting Values of " << currentComponent->GetName() << " to: " << val << std::endl;
								currentComponent->SetPropertyValues(val.c_str(), componentCount);
								componentCount++;
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
				newScene->Attach(temp);
			}
			inFile.clear();
			UnlockResource(temp);
		}
		newScene->Init();
	}
}