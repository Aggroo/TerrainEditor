#--------------------------------------------------------------------------
# glew - built as a static library
#--------------------------------------------------------------------------
add_library(glew STATIC
	src/glew.c
	include/GL/glew.h
	include/GL/wglew.h
	include/GL/glxew.h
	)

# PUBLIC: anything that links glew gets the include path and GLEW_STATIC
target_include_directories(glew PUBLIC include)
target_compile_definitions(glew PUBLIC GLEW_STATIC)
target_link_libraries(glew PUBLIC OpenGL::GL)
