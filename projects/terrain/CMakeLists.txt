#--------------------------------------------------------------------------
# terrain project
#--------------------------------------------------------------------------

PROJECT(terrain)

SET(files_terrain 
code/CGLab.cc 
code/CGLab.h
code/main.cc
code/userinterface.h
code/userinterface.cpp
code/uisettings.h
code/imguiextra.h
)
SOURCE_GROUP("terraineditor" FILES ${files_terrain})

ADD_EXECUTABLE(terrain ${files_terrain})
TARGET_LINK_LIBRARIES(terrain core render foundation application)
ADD_DEPENDENCIES(terrain core render foundation application)

SET_TARGET_PROPERTIES(terrain PROPERTIES FOLDER "terraineditor/projects")