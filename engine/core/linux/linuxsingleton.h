#pragma once
//------------------------------------------------------------------------------
/**
    @file core/posix/posixsingleton.h

    Provides helper macros to implement singleton objects:
    
    - __DeclareSingleton      put this into class declaration
    - __ImplementSingleton    put this into the implemention file
    - __ConstructSingleton    put this into the constructor
    - __DestructSingleton     put this into the destructor

    Get a pointer to a singleton object using the static Instance() method:

    Core::Server* coreServer = Core::Server::Instance();
    
    (C) 2007 Radon Labs GmbH
    (C) 2013 Individual contributors, see AUTHORS file
*/
#include "core/types.h"

//------------------------------------------------------------------------------
#define __DeclareSingleton(type) \
private: \
	type(); \
public: \
	static type* Instance() { static type instance; return &instance; }\
	type(const type&) = delete; \
	void operator=(const type&) = delete; \
private:
//------------------------------------------------------------------------------
