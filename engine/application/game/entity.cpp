#include "config.h"
#include "entity.h"
#include "application/basegamefeatures/entitymanager.h"

namespace Game
{
	Entity::Entity()
	{
		this->ID = BaseGameFeature::EntityManager::Instance()->GetNewEntityID();
	}

	Entity::~Entity()
	{
	}

	void Entity::Activate()
	{
		BaseGameFeature::EntityManager::Instance()->RegisterEntity(std::make_shared<Game::Entity>(*this));
	}

	void Entity::Deactivate()
	{
		BaseGameFeature::EntityManager::Instance()->UnregisterEntity(this->ID);
	}

	void Entity::FixedUpdate()
	{
	}

	void Entity::Update()
	{
	}

	Math::mat4 Entity::GetTransform()
	{
		return this->transform;
	}

	void Entity::SetTransform(const Math::mat4& nTransform)
	{
		this->transform = nTransform;
	}

	const bool& Entity::IsActive() const
	{
		return this->active;
	}
}
