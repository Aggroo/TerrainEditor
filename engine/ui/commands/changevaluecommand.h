#pragma once
//------------------------------------------------------------------------------
/**
    Command Class for undo and redo value changes

    (C) 2021 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "commandbase.h"
#include "foundation/util/variant.h"
namespace UI
{
class ChangeValueCommand : public CommandBase
{
__DeclareClass(ChangeValueCommand)
public:
    ChangeValueCommand() {};
    ChangeValueCommand(Util::Variant& originalValue, Util::Variant& newValue) : value(originalValue), newValue(newValue), oldValue(new Util::Variant()) {};
    ~ChangeValueCommand() {};

    void SetValues(const Util::Variant& originalValue, const Util::Variant& newValue)
    {
        this->value = originalValue;
        this->newValue = newValue;
    }

    virtual void Execute() override 
    {
        oldValue = value;
        value = newValue;
    }

    virtual void Undo() override 
    {
        value = oldValue;
    }

    virtual bool MergeWith(CommandBase* other) override
    {
        Ptr<CommandBase> otherPtr(other);
        Ptr<ChangeValueCommand> changeValueCommand = otherPtr.downcast<ChangeValueCommand>();

        if (changeValueCommand != nullptr) 
        {
            if (&changeValueCommand->value == &this->value) 
            {
                changeValueCommand->newValue = this->newValue;
                return true;
            }
        }
        return false;
    }

private:
    Util::Variant newValue;
    Util::Variant value;
    Util::Variant oldValue;

};
} // namespace UI
