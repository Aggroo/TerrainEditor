#--------------------
#	Foundation
#-----------------
SET(files_foundation_math
	math/matrix3D.h	
	math/matrix4D.h	
	math/vector2D.h
	math/vector3D.h
	math/vector4D.h
	math/quaternions.h
	)
	
SET(files_foundation_util
	util/JsonParser.h
	util/JsonParser.cpp
	)
	
SOURCE_GROUP("math" FILES ${files_foundation_math})
SOURCE_GROUP("util" FILES ${files_foundation_util})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(foundation STATIC ${files_foundation_math} ${files_foundation_util} ${files_pch})
TARGET_PCH(foundation ../)
ADD_DEPENDENCIES(foundation glew misc core)
TARGET_LINK_LIBRARIES(foundation PUBLIC engine exts glew misc core)