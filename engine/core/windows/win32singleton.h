#pragma once
//------------------------------------------------------------------------------
/**
    @file core/win32/win32singleton.h

    Provides helper macros to implement singleton objects:

    - __DeclareSingleton      Put this into class declaration
							  This implements constructor and Instance methods

    Get a pointer to a singleton object using the static Instance() method:

    Core::Server* coreServer = Core::Server::Instance();
    
    @copyright	See LICENCE file
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
