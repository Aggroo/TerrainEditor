#--------------------------------------------------------------------------
# Terrain Editor
#--------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
project(terrain_editor LANGUAGES C CXX)

# Language standard for every target in the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Group targets into folders in the Visual Studio solution
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(MSVC)
	# Compile the files within a project in parallel
	add_compile_options(/MP)
	# Silence warnings about "unsafe" C functions like fopen/sprintf
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

find_package(OpenGL REQUIRED)

# collect_sources(<out_var> <dir>)
#
# Finds every .h/.hpp/.inl/.c/.cc/.cpp file under <dir> (including
# subfolders) and stores the list in <out_var>.
#  - CONFIGURE_DEPENDS makes CMake re-check the folder on every build, so a
#    file you add or delete is picked up automatically on the next build.
#  - Files in folders named "windows" or "linux" are only included when
#    building for that platform.
function(collect_sources out_var dir)
	file(GLOB_RECURSE files CONFIGURE_DEPENDS
		"${dir}/*.h"
		"${dir}/*.hpp"
		"${dir}/*.inl"
		"${dir}/*.c"
		"${dir}/*.cc"
		"${dir}/*.cpp"
		)
	if(WIN32)
		list(FILTER files EXCLUDE REGEX "/linux/")
	else()
		list(FILTER files EXCLUDE REGEX "/windows/")
	endif()
	set(${out_var} ${files} PARENT_SCOPE)
endfunction()

add_subdirectory(exts)
add_subdirectory(engine)
add_subdirectory(projects)

# Must be set on the top-level directory to take effect
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT terrain)
