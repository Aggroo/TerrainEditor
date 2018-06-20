#--------------------
#	Foundation
#-----------------
SET(files_foundation_math
	math/mat3.h	
	math/mat4.h	
	math/vec2.h
	math/vec3.h
	math/vec4.h
	math/quaternions.h
	math/math.h
	)
	
SET(files_foundation_util
	util/JsonParser.h
	util/JsonParser.cpp
	util/string.h
	util/string.cpp
	util/array.h
	util/hashtable.h
	util/dictionary.h
	util/fourcc.h
	util/pair.h
	util/fixedarray.h
	util/list.h
	util/curve.hpp
	util/threadpool.h
	)
	
IF(WIN32)
	SET(files_foundation_memory_OS
		memory/windows/winmemory.h
		memory/windows/winmemoryconfig.h
		memory/windows/winmemoryconfig.cpp
		)
ELSE(WIN32)
	SET(files_foundation_memory_OS
		memory/linux/linuxmemory.h
		memory/linux/linuxmemoryconfig.h
		memory/linux/linuxmemoryconfig.cpp
		)
ENDIF(WIN32)

SET(files_foundation_memory_base
	memory/memory.h
	${files_foundation_memory_OS}
)
	
SOURCE_GROUP("math" FILES ${files_foundation_math})
SOURCE_GROUP("util" FILES ${files_foundation_util})
SOURCE_GROUP("memory" FILES ${files_foundation_memory_base})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(foundation STATIC ${files_foundation_math} ${files_foundation_util} ${files_foundation_memory_base} ${files_pch})
TARGET_PCH(foundation ../)
ADD_DEPENDENCIES(foundation glew misc core)
TARGET_LINK_LIBRARIES(foundation PUBLIC engine exts glew misc core)