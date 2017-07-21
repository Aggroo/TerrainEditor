project (glew)

set (GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR})
find_package (OpenGL REQUIRED)
set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})
include_directories (${GLEW_DIR}/include)

set(GLEW_HEAD 
	${GLEW_DIR}/include/GL/glew.h
	${GLEW_DIR}/include/GL/wglew.h
	${GLEW_DIR}/include/GL/glxew.h)

set(GLEW_SRC
	${GLEW_DIR}/src/glew.c
)

add_definitions(-DGLEW_STATIC)
add_library (glew STATIC ${GLEW_SRC} ${GLEW_HEAD})
#target_link_libraries (glew ${GLEW_LIBRARIES})