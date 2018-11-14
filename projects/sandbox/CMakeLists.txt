#--------------------------------------------------------------------------
# terrain project
#--------------------------------------------------------------------------

PROJECT(sandbox)

SET(files_sandbox 
code/application.cc 
code/application.h
code/main.cc
code/userinterface.h
code/userinterface.cpp
code/uisettings.h
code/imguiextra.h
)
SOURCE_GROUP("sandbox" FILES ${files_sandbox})

ADD_EXECUTABLE(sandbox ${files_sandbox})
TARGET_LINK_LIBRARIES(sandbox core render foundation application)
ADD_DEPENDENCIES(sandbox core render foundation application)

SET_TARGET_PROPERTIES(sandbox PROPERTIES FOLDER "terraineditor/projects")