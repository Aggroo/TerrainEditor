#include "config.h"
#include "JsonParser.h"
#include <fstream>

namespace Util
{
JsonParser::JsonParser()
{
}

JsonParser::~JsonParser()
{
}

	bool JsonParser::ParseJson(std::string fileName, std::string jsonName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}
	json j;
	file >> j;
	jsonContainers[jsonName] = j;

	return true;
}

bool JsonParser::SaveJson(std::string fileName, std::string jsonName)
{
	//TODO: Add a way of saving the json file
	return true;
}

void JsonParser::AddJson(std::string jsonName, json j)
{
	if (jsonContainers.find(jsonName) == jsonContainers.end()) {
		jsonContainers[jsonName] = j;
	}
	else {
		jsonContainers[jsonName].insert(j.begin(), j.end());
	}
	
}

bool JsonParser::GetJson(std::string jsonName, json& j)
{
	if (jsonContainers.find(jsonName) != jsonContainers.end())
	{
		j = jsonContainers[jsonName];
		return true;
	}
	return false;
}
}
