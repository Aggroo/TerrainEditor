#--------------------------------------------------------------------------
# engine
#--------------------------------------------------------------------------

add_library(engine INTERFACE)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine INTERFACE
	glew
	OpenGL::GL
	spdlog::spdlog_header_only
	)

function(engine_library name)
	collect_sources(sources ${CMAKE_CURRENT_SOURCE_DIR})
	add_library(${name} STATIC ${sources})
	target_link_libraries(${name} PUBLIC engine)
	target_precompile_headers(${name} PRIVATE ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/config.h)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})
	set_target_properties(${name} PROPERTIES FOLDER "terraineditor/engine")
endfunction()

add_subdirectory(core)
add_subdirectory(foundation)
add_subdirectory(render)
add_subdirectory(application)
add_subdirectory(ui)
