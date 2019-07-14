#--------------------------------------------------------------------------
# terrain project
#--------------------------------------------------------------------------

PROJECT(sandbox)

SET(files_sandbox 
code/application.cc 
code/application.h
code/main.cc
)
SOURCE_GROUP("sandbox" FILES ${files_sandbox})

ADD_EXECUTABLE(sandbox ${files_sandbox})
TARGET_LINK_LIBRARIES(sandbox core render foundation application ui)
ADD_DEPENDENCIES(sandbox core render foundation application ui)

SET_TARGET_PROPERTIES(sandbox PROPERTIES FOLDER "terraineditor/projects")
set_target_properties(sandbox PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")