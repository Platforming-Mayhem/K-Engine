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

		outFile.write((char*)scene, sizeof(K::Scene));
		outFile.close();
	}

	Deserializer::Deserializer(K::Scene* newScene, std::string location) 
	{
		std::ifstream inFile;
		inFile.open(location);
		if (!inFile)
		{
			std::cerr << "Error - unable to open input file " << location << std::endl;
			exit(1);
		}
		inFile.read((char*)newScene, sizeof(K::Scene));
		inFile.close();
	}
}