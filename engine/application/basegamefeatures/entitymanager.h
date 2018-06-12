#pragma once
#include <memory>
#include <unordered_map>


namespace Game
{
class Entity;
}

#define UPDATE_FREQUENCY 0.1f //Update frequency per second for Fixed Update

namespace BaseGameFeature
{
class EntityManager
{
public: 
	static EntityManager* Instance() { static EntityManager instance; return &instance; }
	~EntityManager();
	EntityManager(const EntityManager&) = delete;
	void operator=(const EntityManager&) = delete;

	//Returns a unique ID
	uint GetNewEntityID();

	void RegisterEntity(std::shared_ptr<Game::Entity> entity);

	//Deletes an entity by ID. Unregisters it from gamehandler. 
	void UnregisterEntity(const int& ID);

	//Calls all entities update and fixedUpdate functions.
	void Update();

	//Returns time since last update multiplied with UPDATE_MULTIPLIER
	double DeltaTime();

	std::shared_ptr<Game::Entity> GetEntityByID(const uint& id);

	//Update frequency multiplier for deltaTime, put this to 1.0f if you want normal speed.
	//float UPDATE_MULTIPLIER;

	//Returns std::map of registered entities.
	std::unordered_map<int, std::shared_ptr<Game::Entity>>& GetEntityList() { return entityList; }

private:
	EntityManager();

	//Incrementing index for making sure entities has unique IDs. 
	//Never change this explicitly if you don't know what you're doing!
	uint entityCounter = 20000; //HACK: Start at 20k because we need to reserve low numbers...		

	//Used for calculating DeltaTime
	//Time variables
	double deltaTime;
	double lastTime;

	double lastUpdateTime;

	//Returns time since last update in seconds
	double TimeSinceLastUpdate();

	//Holds all units mapped: unique ID as key and pointer to their baseclass as mapped value
	std::unordered_map<int, std::shared_ptr<Game::Entity>> entityList;

};
}