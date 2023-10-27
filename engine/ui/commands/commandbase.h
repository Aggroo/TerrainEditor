#pragma once
#include "core/refcounted.h"
#include "core/types.h"

namespace UI {

class CommandBase : public Core::RefCounted
{
__DeclareAbstractClass(CommandBase)
public:
	virtual ~CommandBase() {};
	virtual void Execute() = 0;
	virtual void Undo() = 0;
	virtual bool MergeWith(CommandBase* other) = 0;

	void SetNoMerge() { canMerge = false; }
	bool CanMerge() const { return canMerge; }

protected:
	bool canMerge = true;

private:
	static int64 id;

};
}