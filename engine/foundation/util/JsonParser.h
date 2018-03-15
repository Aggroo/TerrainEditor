#pragma once
#include "json.hpp"
#include <map>

namespace Util
{
using json = nlohmann::json;

class JsonParser
{

public:
	JsonParser();
	~JsonParser();

	bool ParseJson(std::string fileName, std::string jsonName);
	bool SaveJson(std::string fileName, std::string jsonName);
	void AddJson(std::string jsonName, json j);
	bool GetJson(std::string jsonName, json& j);

	
private:

	std::map<std::string, json> jsonContainers;
};

}