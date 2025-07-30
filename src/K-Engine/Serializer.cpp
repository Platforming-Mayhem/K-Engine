#include "K-Engine/Serializer.h"
#include "K-Engine/Editor.h"

namespace K 
{
	Serializer::Serializer(K::Scene* scene, std::string location)
	{
		std::string name = location;
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
				outFile << "," << component->GetPropertyValues();
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
		std::vector<int> sizes;
		std::vector<K::SerialiseObject> objs;
		for (auto it : scene->GetGameObjects()) 
		{
			K::SerialiseObject obj(it.second);
			sizes.push_back(obj.GetNumberOfInts());
			sizes.push_back(obj.GetNumberOfFloats());
			sizes.push_back(obj.GetNumberOfBools());
			sizes.push_back(obj.GetNumberOfStrings());
			objs.push_back(obj);
			//sizes.push_back(obj.GetNumberOfStrings());
		}
		for (auto size : sizes) 
		{
			outFile << size << ",";
		}
		outFile << "\n";
		for (auto obj : objs) 
		{
			outFile << obj;
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
			case 1: // index
				break;
			case 2: // x Pos
				position->x = std::stof(val);
				break;
			case 3: // y Pos
				position->y = std::stof(val);
				break;
			case 4: // z Pos
				position->z = std::stof(val);
				break;
			case 5: // x Rot
				rotation->x = std::stof(val);
				break;
			case 6: // y Rot
				rotation->y = std::stof(val);
				break;
			case 7: // z Rot
				rotation->z = std::stof(val);
				break;
			case 8: // x Scale
				scale->x = std::stof(val);
				break;
			case 9: // y Scale
				scale->y = std::stof(val);
				break;
			case 10: // z Scale
				scale->z = std::stof(val);
				break;
			case 11: // local x Pos
				temp->GetTransform()->localPosition->x = std::stof(val);
				break;
			case 12: // local y Pos
				temp->GetTransform()->localPosition->y = std::stof(val);
				break;
			case 13: // local z Pos
				temp->GetTransform()->localPosition->z = std::stof(val);
				break;
			case 14: // local x Rot
				temp->GetTransform()->localRotation->x = std::stof(val);
				break;
			case 15: // local y Rot
				temp->GetTransform()->localRotation->y = std::stof(val);
				break;
			case 16: // local z Rot
				temp->GetTransform()->localRotation->z = std::stof(val);
				break;
			case 17: // local x Scale
				temp->GetTransform()->localScale->x = std::stof(val);
				break;
			case 18: // local y Scale
				temp->GetTransform()->localScale->y = std::stof(val);
				break;
			case 19: // local z Scale
				temp->GetTransform()->localScale->z = std::stof(val);
				break;
			case 20: // parent index
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
					std::unordered_map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(val);
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
			std::unordered_map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(data[i]);
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
		std::unordered_map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(datum);
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
		EditorDeserializer(newScene, location);
		std::cout << ASSET_DIR + location << std::endl;
		newScene->SetSceneName(location);
		newScene->SetLocation(location);
		newScene->Init();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
		std::cout << "Time To Deserialize Scene (" << newScene->GetSceneName() << "): " << elapsed << std::endl;
	}

	void Deserializer::EditorDeserializer(K::Scene* newScene, std::string location) 
	{
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
				std::unordered_map<std::string, std::chrono::milliseconds> times;
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
	}

	void Deserializer::RunTimeDeserializer(K::Scene* newScene, std::string location)
	{
		std::string file = location;
		std::ifstream inFile;
		file.replace(file.begin() + file.find(".JAWS"), file.end(), ".BJAWS");
		inFile.open(ASSET_DIR + file, std::ios::binary);
		if (inFile)
		{
			std::string data;
			std::getline(inFile, data);
			int numberOfGameObjects = std::count(data.begin(), data.end(), ',') / 4;
			for (int i = 0; i < numberOfGameObjects; i++) 
			{
				K::SerialiseObject obj = K::SerialiseObject();
				obj.AppendInt(i);
				inFile >> obj;
				obj.CreateGameObject(this);
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
			std::string val = other->GetComponent(i)->GetName();
			val += "," + std::string(other->GetComponent(i)->GetPropertyValues()) + ",";
			this->AppendString(val);
		}
	}

	void SerialiseObject::CreateGameObject(Deserializer* deserializer)
	{
		K::GameObject* temp = new K::GameObject(this->GetString(0).c_str(), new K::Transform(), this->GetInt(0));

		temp->GetTransform()->position->x = this->GetFloat(0);
		temp->GetTransform()->position->y = this->GetFloat(1);
		temp->GetTransform()->position->z = this->GetFloat(2);

		temp->GetTransform()->rotation->x = this->GetFloat(3);
		temp->GetTransform()->rotation->y = this->GetFloat(4);
		temp->GetTransform()->rotation->z = this->GetFloat(5);

		temp->GetTransform()->scale->x = this->GetFloat(6);
		temp->GetTransform()->scale->y = this->GetFloat(7);
		temp->GetTransform()->scale->z = this->GetFloat(8);

		temp->GetTransform()->localPosition->x = this->GetFloat(9);
		temp->GetTransform()->localPosition->y = this->GetFloat(10);
		temp->GetTransform()->localPosition->z = this->GetFloat(11);

		temp->GetTransform()->localRotation->x = this->GetFloat(12);
		temp->GetTransform()->localRotation->y = this->GetFloat(13);
		temp->GetTransform()->localRotation->z = this->GetFloat(14);

		temp->GetTransform()->localScale->x = this->GetFloat(15);
		temp->GetTransform()->localScale->y = this->GetFloat(16);
		temp->GetTransform()->localScale->z = this->GetFloat(17);

		if (this->GetInt(1) != -1)
		{
			deserializer->parents.insert({temp, this->GetInt(1)});
		}

		for (int i = 1; i < this->GetNumberOfStrings(); i++)
		{
			std::string data = this->GetString(i);
			K::Component* selectedComponent = nullptr;
			int componentDataCount = 0;
			while (data.find(',') != std::string::npos)
			{
				std::string datum = data.substr(0, data.find(','));
				std::unordered_map<std::string, K::IFactory*>::iterator pos = K::Editor::lst().find(datum);
				if (pos != K::Editor::lst().end())
				{
					selectedComponent = pos->second->create();
					temp->AddComponent(selectedComponent);
					componentDataCount = 0;
				}
				else if(selectedComponent != nullptr)
				{
					selectedComponent->SetPropertyValues(datum.c_str(), componentDataCount);
					componentDataCount++;
				}
				data.erase(0, data.find(',') + 1);
			}
		}
	}

	SerialiseObject::SerialiseObject() 
	{

	}

	std::istream& operator>>(std::istream& is, K::SerialiseObject& sO)
	{
		std::string data;
		std::vector<int> sizes;
		int currentPos = is.tellg();
		is.seekg(0, std::ios::beg);
		std::getline(is, data);
		is.seekg(currentPos, std::ios::beg);
		for (int i = 0; i < sO.GetInt(0); i++) 
		{
			for (int j = 0; j < 4; j++)
			{
				data.erase(0, data.find(',') + 1);
			}
		}
		sO.RemoveInt(0);
		for(int k = 0; k < 4; k++)
		{
			std::string datum = data.substr(0, data.find(','));
			int size = std::stoi(datum);
			sizes.push_back(size);
			data.erase(0, data.find(',') + 1);
		}
		int type = 0;
		for (int size : sizes) 
		{
			for (int i = 0; i < size; i++) 
			{
				char datumI[sizeof(int)];
				char datumF[sizeof(float)];
				char datumB[sizeof(int)];
				std::string datumS;
				switch (type)
				{
				case 0:
					is.read(datumI, sizeof(int));
					//std::cout << *(int*)datumI << std::endl;
					sO.AppendInt(*(int*)&datumI);
					break;
				case 1:
					is.read(datumF, sizeof(float));
					//std::cout << *(float*)datumF << std::endl;
					sO.AppendFloat(*(float*)&datumF);
					break;
				case 2:
					is.read(datumB, sizeof(int));
					//std::cout << *(bool*)datumB << std::endl;
					sO.AppendBool(*(bool*)&datumB);
					break;
				case 3:
					std::getline(is, datumS);
					//std::cout << datumS << std::endl;
					sO.AppendString(datumS);
					break;
				}
			}
			if (type < 3) 
			{
				type++;
			}
			else 
			{
				type = 0;
			}
		}
		//std::cout << "Finished " << sO.GetString(0) << std::endl;
		return is;
	}

	std::ostream& operator<<(std::ostream& os, K::SerialiseObject& sO)
	{
		for (int i = 0; i < sO.GetNumberOfInts(); i++)
		{
			int datum = sO.GetInt(i);
			os.write((char*)&datum, sizeof(datum));
		}
		for (int j = 0; j < sO.GetNumberOfFloats(); j++) 
		{
			float datum = sO.GetFloat(j);
			os.write((char*)&datum, sizeof(datum));
		}
		for (int k = 0; k < sO.GetNumberOfBools(); k++)
		{
			int datum = sO.GetBool(k);
			os.write((char*)&datum, sizeof(datum));
		}
		for (int l = 0; l < sO.GetNumberOfStrings(); l++)
		{
			std::string value = sO.GetString(l);
			os.write(&value[0], value.size() * sizeof(char));
			os << "\n";
		}
		return os;
	}
}
