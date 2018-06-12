#--------------------------------------------------------------------------
# engine
#--------------------------------------------------------------------------

ADD_LIBRARY(engine INTERFACE)
TARGET_INCLUDE_DIRECTORIES(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(engine INTERFACE ${OPENGL_LIBS})
ADD_SUBDIRECTORY(core)
ADD_SUBDIRECTORY(foundation)
ADD_SUBDIRECTORY(render)
ADD_SUBDIRECTORY(application)

SET_TARGET_PROPERTIES(core PROPERTIES FOLDER "terraineditor/engine")
SET_TARGET_PROPERTIES(foundation PROPERTIES FOLDER "terraineditor/engine")
SET_TARGET_PROPERTIES(render PROPERTIES FOLDER "terraineditor/engine")
SET_TARGET_PROPERTIES(application PROPERTIES FOLDER "terraineditor/engine")
