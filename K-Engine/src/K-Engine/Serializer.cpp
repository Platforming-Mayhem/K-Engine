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
		for (K::GameObject* g : scene->GetGameObjects()) 
		{
			if (count == 0) 
			{

			}
			else 
			{
				K::Transform transform = *g->GetTransform();
				outFile << g->GetName();
				outFile << "," << transform.position->x << "," << transform.position->y << "," << transform.position->z;
				outFile << "," << transform.rotation->x << "," << transform.rotation->y << "," << transform.rotation->z;
				outFile << "," << transform.scale->x << "," << transform.scale->y << "," << transform.scale->z;
				for (int i = 0; i < g->GetNumberOfComponents(); i++)
				{
					K::Component* component = g->GetComponent(i);
					outFile << "," << component->GetName();
					outFile << "," << component->GetPropertyValues();
				}
				outFile << ",";
				outFile << "\n";
			}
			count++;
		}
		outFile.close();
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location, K::Editor* editor) 
	{
		std::ifstream inFile;
		inFile.open(location);
		if (!inFile)
		{
			std::cerr << "Error - unable to open input file " << location << std::endl;
			exit(1);
		}
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
			int number = 0;
			for (int i = 0; i < line.size(); i++) 
			{
				if (line[i] == ',') 
				{
					switch (number) 
					{
					case 0:
						name = val;
						temp = new K::GameObject(name.c_str(), transform);
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
								temp->AddComponent(tempCo);
								currentComponent = tempCo;
								found = true;
								break;
							}
						}
						if (!found) 
						{
							currentComponent->SetPropertyValues(val.c_str());
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
}