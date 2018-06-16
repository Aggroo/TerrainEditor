#include "config.h"
#include "entitymanager.h"
#include <ctime>
#include "GLFW/glfw3.h"
#include "application/game/entity.h"


namespace BaseGameFeature
{

EntityManager::EntityManager()
{
	//UPDATE_MULTIPLIER = iniParser::getUpdateMultiplier();

	this->lastUpdateTime = clock();
	double lastTime = glfwGetTime();

}

EntityManager::~EntityManager()
{
}

uint EntityManager::GetNewEntityID()
{
	return this->entityCounter++;
}

void EntityManager::RegisterEntity(Ptr<Game::Entity> entity)
{
	this->entityList.insert(std::pair<int, Ptr<Game::Entity>>(entity->GetID(), entity));
}

void EntityManager::UnregisterEntity(const int& ID)
{
	if (entityList.size() > 0 && entityList.count(ID) > 0)
	{
		entityList.erase(ID);
	}
}

void EntityManager::Update()
{
	double currentTime = glfwGetTime();

	deltaTime = (float)fabs(currentTime - lastTime);
	//Check delta time and update if it exceeds update frequency
	if (TimeSinceLastUpdate() >= UPDATE_FREQUENCY)
	{
		//update last update time to current clock-ticks
		lastUpdateTime = clock();

		for (std::unordered_map<int, Ptr<Game::Entity>>::iterator it = entityList.begin(); it != entityList.end(); it++)
		{
			Ptr<Game::Entity> entity = it->second;
			if (entity->IsActive())
				entity->FixedUpdate();
		}
	}

	for (std::unordered_map<int, Ptr<Game::Entity>>::iterator it = this->entityList.begin(); it != entityList.end(); it++)
	{
		Ptr<Game::Entity> entity = it->second;
		if (entity->IsActive())
			entity->Update();
	}

	lastTime = currentTime;
}

double EntityManager::DeltaTime()
{
	return deltaTime < 1.0 ? (deltaTime < 0.000001 ? 0.0 : deltaTime) : 1.0;
}

double EntityManager::TimeSinceLastUpdate()
{
	return (((float)clock() - lastUpdateTime) / CLOCKS_PER_SEC);
}

Ptr<Game::Entity> EntityManager::GetEntityByID(const uint& id)
{
	assert(this->entityList.count(id) > 0, "ERROR: No Entity with this ID exists!");
	return this->entityList[id];
}
}
