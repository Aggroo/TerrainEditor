#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------
IF(WIN32)
	SET(files_core_OS
		windows/win32singleton.h
		windows/win32sysfunc.h
		windows/win32sysfunc.cpp
		)
ELSE(WIN32)
	SET(files_core_OS
		linux/linuxsingleton.h
		linux/linuxsysfunc.h
		linux/linuxsysfunc.cpp
		)
ENDIF(WIN32)

SET(files_core
	app.h
	app.cc
	ptr.h
	factory.h
	factory.cpp
	refcounted.h
	refcounted.cpp
	refcountedlist.h
	refcountedlist.cpp
	rtti.h
	rtti.cpp
	rttimacros.h
	singleton.h
	sysfunc.h
	weakptr.h
	debug.h
	debug.cpp
	exithandler.h
	exithandler.cpp
	${files_core_OS}
	)
SOURCE_GROUP("core" FILES ${files_core})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(core STATIC ${files_core} ${files_pch})
TARGET_PCH(core ../)
ADD_DEPENDENCIES(core glew)
TARGET_LINK_LIBRARIES(core PUBLIC engine exts glew)
