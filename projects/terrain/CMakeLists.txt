#--------------------------------------------------------------------------
# terrain project
#
# All .h/.cpp files in the code/ folder are found automatically.
#--------------------------------------------------------------------------
collect_sources(sources ${CMAKE_CURRENT_SOURCE_DIR}/code)
add_executable(terrain ${sources})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

target_link_libraries(terrain PRIVATE core foundation render application ui)

set_target_properties(terrain PROPERTIES
	FOLDER "terraineditor/projects"
	# Run from the repo root so relative paths like "resources/..." resolve
	VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
	)
