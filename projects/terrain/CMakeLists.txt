#--------------------------------------------------------------------------
# terrain project
#--------------------------------------------------------------------------

PROJECT(terrain_proj)

SET(files_terrain 
code/CGLab.cc 
code/CGLab.h
code/main.cc
code/userinterface.h
code/userinterface.cpp
code/imguiextra.h
)
SOURCE_GROUP("terraineditor" FILES ${files_terrain})

ADD_EXECUTABLE(terrain_proj ${files_terrain})
TARGET_LINK_LIBRARIES(terrain_proj core render foundation application)
ADD_DEPENDENCIES(terrain_proj core render foundation application)

SET_TARGET_PROPERTIES(terrain_proj PROPERTIES FOLDER "terraineditor/projects")