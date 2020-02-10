#include "config.h"
#include "entitybase.h"
#include "application/basegamefeatures/entitymanager.h"

namespace Game
{
	__ImplementClass(Game::EntityBase, 'NTTY', Core::RefCounted);
	EntityBase::EntityBase()
	{
		this->transform = Math::mat4::identity();
		this->ID = BaseGameFeature::EntityManager::Instance()->GetNewEntityID();
	}

	EntityBase::~EntityBase()
	{
	}

	void EntityBase::Activate()
	{
		BaseGameFeature::EntityManager::Instance()->RegisterEntity(this);

		this->active = true;
	}

	void EntityBase::Deactivate()
	{
		BaseGameFeature::EntityManager::Instance()->UnregisterEntity(this->ID);
	}

	void EntityBase::FixedUpdate()
	{
	}

	void EntityBase::Update()
	{
	}

	void EntityBase::OnUI()
	{
	}

	Math::mat4 EntityBase::GetTransform()
	{
		return this->transform;
	}

	void EntityBase::SetTransform(const Math::mat4& nTransform)
	{
		this->transform = nTransform;
	}

	const bool& EntityBase::IsActive() const
	{
		return this->active;
	}
}
