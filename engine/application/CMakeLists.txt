#--------------------
#	Application
#-----------------
SET(files_application_game
	game/entity.h
	game/entity.cpp
	game/terrain.h
	game/terrain.cpp)
	
SET(files_application_basegamefeatures
	basegamefeatures/entitymanager.h
	basegamefeatures/entitymanager.cpp
	)
	
SOURCE_GROUP("game" FILES ${files_application_game})
SOURCE_GROUP("basegamefeatures" FILES ${files_application_basegamefeatures})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(application STATIC ${files_application_game} ${files_application_basegamefeatures} ${files_pch})
TARGET_PCH(application ../)
ADD_DEPENDENCIES(application glew misc core)
TARGET_LINK_LIBRARIES(application PUBLIC engine exts glew misc core)