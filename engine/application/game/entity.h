#pragma once
#include "foundation/math/mat4.h"
#include "core/refcounted.h"

namespace Game
{
class Entity : public Core::RefCounted
{
	__DeclareClass(Entity);
public:
	Entity();
	~Entity();

	const uint& GetID() const { return ID; }

	///Activates this entity and all it's properties
	virtual void Activate();
	///Deactivates this entity and all it's properties
	virtual void Deactivate();

	///Updates this entity and calls all its properties FixedUpdate() functions. This is called much less frequently than Update()
	virtual void FixedUpdate();
	///Updates this entity and calls all its properties Update() functions.
	virtual void Update();

	///Shortcut for getting this entitys transform
	virtual Math::mat4 GetTransform();
	///Shortcut for setting this entitys transform
	virtual void SetTransform(const Math::mat4& nTransform);

	///Check if entity is active or not
	const bool& IsActive() const;

protected:
	uint ID;
	bool active;

	Math::mat4 transform;
};
}
