#--------------------------------------------------------------------------
# terrain project
#--------------------------------------------------------------------------

PROJECT(terrain)

SET(files_terrain 
code/application.cc 
code/application.h
code/main.cc
)
SOURCE_GROUP("terraineditor" FILES ${files_terrain})

ADD_EXECUTABLE(terrain ${files_terrain})
TARGET_LINK_LIBRARIES(terrain core render foundation application ui)
ADD_DEPENDENCIES(terrain core render foundation application ui)

SET_TARGET_PROPERTIES(terrain PROPERTIES FOLDER "terraineditor/projects")
set_target_properties(terrain PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")