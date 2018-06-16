#include "config.h"
#include "entity.h"
#include "application/basegamefeatures/entitymanager.h"

namespace Game
{
	__ImplementClass(Game::Entity, 'NTTY', Core::RefCounted);
	Entity::Entity()
	{
		this->transform = Math::mat4::identity();
		this->ID = BaseGameFeature::EntityManager::Instance()->GetNewEntityID();
	}

	Entity::~Entity()
	{
	}

	void Entity::Activate()
	{
		BaseGameFeature::EntityManager::Instance()->RegisterEntity(this);

		this->active = true;
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
