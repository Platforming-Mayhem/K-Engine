#include "Serializer.h"
#include "Editor.h"

namespace K 
{
	Serializer::Serializer(K::Scene* scene, std::string location)
	{
		std::string name = location;
		if (!name.contains(".JAWS"))
		{
			name += ".JAWS";
		}
		std::ofstream outFile;
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
		RunTimeSerialize(scene, location);
	}

	void Serializer::RunTimeSerialize(K::Scene* scene, std::string location)
	{
		std::string name = location;
		name.replace(name.begin() + name.find(".JAWS"), name.end(), ".BJAWS");
		std::ofstream outFile;
		outFile.open(name.c_str(), std::ios::binary | std::ofstream::trunc);
		if (!outFile)
		{
			std::cerr << "Error - unable to open output file " << name.c_str() << std::endl;
			exit(1);
		}
		for (auto it : scene->GetGameObjects()) 
		{
			K::SerialiseObject obj(it.second);
			outFile << obj << '\n';
		}
		outFile.close();
	}

	void Deserializer::CreateGameObject(std::string gameObject) 
	{
		K::Vector3* position = new K::Vector3(0.0f, 0.0f, 0.0f);
		K::Vector3* rotation = new K::Vector3(0.0f, 0.0f, 0.0f);
		K::Vector3* scale = new K::Vector3(1.0f, 1.0f, 1.0f);

		K::Transform* transform = new K::Transform(position, rotation, scale);

		std::string name = "";

		int componentCount = 0;

		K::GameObject* temp = nullptr;
		K::Component* currentComponent = nullptr;
		int pos = 0;
		int number = 0;
		while ((pos = gameObject.find(',')) != std::string::npos)
		{
			std::string val = gameObject.substr(0, pos);
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
					this->parents.insert({ temp, std::stoi(val) });
				}
				break;
			}
			if (number > 20)
			{
				if (K::Editor::lst().count(val) > 0)
				{
					std::map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(val);
					if (pos != K::Editor::lst().end())
					{
						currentComponent = pos->second->create();
						temp->AddComponent(currentComponent);
						componentCount = 0;
						//std::cout << "Creating " << currentComponent->GetName() << std::endl;
					}
					else 
					{
						std::cerr << val << ": CLASS DOESN'T EXIST" << std::endl;
						throw;
					}
				}
				else 
				{
					//std::cout << "Setting Values of " << currentComponent->GetName() << " to: " << val << std::endl;
					currentComponent->SetPropertyValues(val.c_str(), componentCount);
					componentCount++;
				}
			}
			number++;
			gameObject.erase(0, pos + sizeof(char));
		}
	}

	void Deserializer::CreateGameObjectFast(std::vector<std::string>& data)
	{
		K::GameObject* tempGameObject = new K::GameObject(data[0].c_str(), new K::Transform(new K::Vector3(std::stof(data[2]), std::stof(data[3]), std::stof(data[4])), new K::Vector3(std::stof(data[5]), std::stof(data[6]), std::stof(data[7])), new K::Vector3(std::stof(data[8]), std::stof(data[9]), std::stof(data[10]))), std::stoi(data[1]));
		tempGameObject->GetTransform()->localPosition = new K::Vector3(std::stof(data[11]), std::stof(data[12]), std::stof(data[13]));
		tempGameObject->GetTransform()->localRotation = new K::Vector3(std::stof(data[14]), std::stof(data[15]), std::stof(data[16]));
		tempGameObject->GetTransform()->localScale = new K::Vector3(std::stof(data[17]), std::stof(data[18]), std::stof(data[19]));
		if (std::stoi(data[20]) != -1)
		{
			this->parents.insert({ tempGameObject, std::stoi(data[20]) });
		
		}
		this->selectedGameObject = tempGameObject;
	}

	void Deserializer::CreateComponent(K::GameObject* tempGameObject, std::vector<std::string>& data)
	{
		K::Component* component = nullptr;
		int count = 0;
		//For Loop within a For loop is what is slowing it down
		for (int i = 21; i < data.size(); i++)
		{
			std::map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(data[i]);
			if (pos != K::Editor::lst().end())
			{
				//pos->second->create is slow

				component = pos->second->create();
				tempGameObject->AddComponent(component);
				count = 0;
			}
			else
			{
				//SetPropertyValues is slow

				component->SetPropertyValues(data[i].c_str(), count);
				count++;
			}
		}
	}

	void Deserializer::CreateComponentFast(std::string datum) 
	{
		std::map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(datum);
		if (pos != K::Editor::lst().end())
		{
			this->selectedComponent = pos->second->create();
			this->selectedGameObject->AddComponent(this->selectedComponent);
			this->componentDataCount = 0;
		}
		else
		{
			if (this->selectedComponent != nullptr) 
			{
				this->selectedComponent->SetPropertyValues(datum.c_str(), this->componentDataCount);
				this->componentDataCount++;
			}
		}
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location) 
	{
		auto start = std::chrono::steady_clock::now();
		std::ifstream inFile;
		inFile.open(ASSET_DIR + location);
		if (inFile)
		{
			std::vector<char> characters;

			inFile.seekg(0, std::ios::end);
			int length = inFile.tellg();

			inFile.seekg(0, std::ios::beg);

			if (length > 0) 
			{
				characters.resize(length);
				inFile.read(&characters[0], length);

				std::string line;
				std::vector<std::string> dataArray;
				std::map<std::string, std::chrono::milliseconds> times;
				int count = 0;
				for (auto char0 : characters)
				{
					if (char0 == '\n')
					{
						count = 0;
						line.clear();
						dataArray.clear();
						this->selectedGameObject = nullptr;
						this->selectedComponent = nullptr;
						this->componentDataCount = 0;
					}
					else if (char0 == ',')
					{
						dataArray.push_back(line);
						line.clear();
						count++;
						if (count == 21)
						{
							this->CreateGameObjectFast(dataArray);
						}
						else if (count > 21)
						{
							this->CreateComponentFast(dataArray[count - 1]);
						}
					}
					else
					{
						line += char0;
					}
				}
			}

			inFile.close();

			if (!parents.empty())
			{
				for (auto temp : parents)
				{
					temp.first->SetParent(newScene->GetGameObjects().at(temp.second));
				}
				parents.clear();
			}
		}
		//RunTimeDeserializer(newScene, location);
		std::cout << ASSET_DIR + location << std::endl;
		newScene->SetSceneName(location);
		newScene->SetLocation(location);
		newScene->Init();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
		std::cout << "Time To Deserialize Scene (" << newScene->GetSceneName() << "): " << elapsed << std::endl;
	}
	void Deserializer::RunTimeDeserializer(K::Scene* newScene, std::string location)
	{
		std::string file = location;
		std::ifstream inFile;
		file.replace(file.begin() + file.find(".JAWS"), file.end(), ".BJAWS");
		inFile.open(ASSET_DIR + file, std::ios::binary);
		if (inFile)
		{
			K::SerialiseObject obj = K::SerialiseObject();
			inFile >> obj;
			for (int i = 0; i < obj.GetNumberOfInts(); i++) 
			{
				std::cout << obj.GetInt(i) << std::endl;
			}
		}
		inFile.close();
	}

	SerialiseObject::SerialiseObject(K::GameObject* other)
	{
		this->AppendString(other->GetName());
		this->AppendInt(other->GetIndex());
		this->AppendVector3(*other->GetTransform()->position);
		this->AppendVector3(*other->GetTransform()->rotation);
		this->AppendVector3(*other->GetTransform()->scale);
		this->AppendVector3(*other->GetTransform()->localPosition);
		this->AppendVector3(*other->GetTransform()->localRotation);
		this->AppendVector3(*other->GetTransform()->localScale);
		if (other->parent == nullptr)
			this->AppendInt(-1);
		else
			this->AppendInt(other->parent->GetIndex());
		for (int i = 0; i < other->GetNumberOfComponents(); i++) 
		{
			this->AppendString(other->GetComponent(i)->GetPropertyValues());
		}
	}

	SerialiseObject::SerialiseObject() 
	{

	}

	std::istream& operator>>(std::istream& is, K::SerialiseObject& sO)
	{
		is.seekg(0, std::ios::end);
		int size = is.tellg();
		is.seekg(0, std::ios::beg);
		for (int i = 0; i < size / sizeof(char); i++)
		{
			char* cValue = new char[sizeof(char)];
			is.read(cValue, sizeof(char));
			int value = (int)*cValue;
			sO.AppendInt(value);
		}
		return is;
	}

	std::ostream& operator<<(std::ostream& os, K::SerialiseObject& sO)
	{
		os << sO.GetNumberOfInts() << "." << sO.GetNumberOfFloats() << "\n";
		for (int i = 0; i < sO.GetNumberOfInts(); i++)
		{
			char c = sO.GetInt(i);
			os.write(&c, sizeof(char));
		}
		os << '\n';
		for (int j = 0; j < sO.GetNumberOfFloats(); j++) 
		{
			float value = sO.GetFloat(j);
			char* c = reinterpret_cast<char*>(&value);
			os.write(c, sizeof(float));
		}
		return os;
	}
}