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
	resources/surface.h
	resources/surface.cpp
	resources/rasterizer.h
	resources/rasterizer.cpp
	resources/model.h
	resources/model.cpp
	resources/material.h
	resources/material.cpp
	resources/vertexcomponent.h
	resources/materialcomponents.h
	)
	
SET(files_render_camera
	camera/camera.h
	camera/camera.cpp)
	
SET(files_render_render
	render/renderer.h
	render/renderer.cpp
	render/shadervariables.h
	render/shadersemantics.h
	render/skybox.h
	render/skybox.cpp
	)

SET(files_render_frames
	frames/framepass.h
	frames/framepass.cpp
	frames/depthpass.h
	frames/depthpass.cpp
	frames/drawpass.h
	frames/drawpass.cpp
	frames/lightcullingpass.h
	frames/lightcullingpass.cpp
	frames/flatgeometrylitpass.h
	frames/flatgeometrylitpass.cpp
	frames/postprocessingpass.h
	frames/postprocessingpass.cpp
	frames/ssaopass.h
	frames/ssaopass.cpp
	frames/iblpass.h
	frames/iblpass.cpp
	)

SET(files_render_terrain
	terrain/terrain.h
	terrain/terrain.cpp
	terrain/terrainheightpass.h
	terrain/terrainheightpass.cpp
	terrain/terrainsplatpass.h
	terrain/terrainsplatpass.cpp
	)

SET(files_render_server
	server/resourceserver.h
	server/resourceserver.cpp
	server/frameserver.h
	server/frameserver.cpp
	server/shaderserver.h
	server/shaderserver.cpp
	server/lightserver.h
	server/lightserver.cpp
	server/pickingServer.h
	server/pickingServer.cpp
	)

SOURCE_GROUP("display" FILES ${files_render_display})

SOURCE_GROUP("resources" FILES ${files_render_resources})

SOURCE_GROUP("camera" FILES ${files_render_camera})

SOURCE_GROUP("render" FILES ${files_render_render})

SOURCE_GROUP("frames" FILES ${files_render_frames})

SOURCE_GROUP("terrain" FILES ${files_render_terrain})

SOURCE_GROUP("server" FILES ${files_render_server})

SET(files_render
	${files_render_display}
	${files_render_resources}
	${files_render_camera}
	${files_render_render}
	${files_render_frames}
	${files_render_terrain}
	${files_render_server}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw misc)
TARGET_LINK_LIBRARIES(render PUBLIC core engine exts glew glfw ${OPENGL_LIBS} misc)