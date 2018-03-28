#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc)

SET(files_render_resources
	resources/graphicsnode.h
	resources/graphicsnode.cpp
	resources/halfedgemesh.h
	resources/halfedgemesh.cpp
	resources/lightnode.h
	resources/lightnode.cpp
	resources/meshresource.h
	resources/meshresource.cpp
	resources/shaderobject.h
	resources/shaderobject.cpp
	resources/textureresource.h
	resources/textureresource.cpp
	resources/texturenode.h
	resources/texturenode.cpp
	resources/rasterizer.h
	resources/rasterizer.cpp
	resources/stb_image.h
	resources/stb_image_write.h
	resources/inputmanager.h
	resources/inputmanager.cpp
	)
	
SET(files_render_camera
	camera/camera.h
	camera/camera.cpp)
	
SET(files_render_render
	render/renderer.h
	render/renderer.cpp)

SOURCE_GROUP("display" FILES ${files_render_display})

SOURCE_GROUP("resources" FILES ${files_render_resources})

SOURCE_GROUP("camera" FILES ${files_render_camera})

SOURCE_GROUP("render" FILES ${files_render_render})

SET(files_render
	${files_render_display}
	${files_render_resources}
	${files_render_camera}
	${files_render_render}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw misc)
TARGET_LINK_LIBRARIES(render PUBLIC core engine exts glew glfw ${OPENGL_LIBS} misc)