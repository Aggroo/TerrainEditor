#pragma once
//------------------------------------------------------------------------------
/**
    @class Core::ExitHandler
    
    ExitHandlers are static objects which register themselves automatically
    once at startup and are called back from the Core::SysFunc::Exit()
    static method which is called right before the application exists.  
    Please note that the Vortex runtime usually doesn't yet exist when
    the ExitHandler is created or destroyed, so don't put anything complex
    into the constructor or destructor of the class!
    
    @copyright	See LICENCE file
*/    
#include "core/types.h"

//------------------------------------------------------------------------------
namespace Core
{
class ExitHandler
{
public:
    /// constructor
    ExitHandler();
    /// destructor
    virtual ~ExitHandler();
    /// virtual method called from SysFunc::Exit()
    virtual void OnExit() const;
    /// get pointer to next exit handler in forward linked list
    const ExitHandler* Next() const;

private:
    const ExitHandler* nextExitHandler; // for forward linking...
};

} // namespace Core
//------------------------------------------------------------------------------
