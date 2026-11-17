#--------------------------------------------------------------------------
# exts - third party libraries
#--------------------------------------------------------------------------

#--------------------------------------------------------------------------
# glew
#--------------------------------------------------------------------------
set(CMAKE_FOLDER "exts/glew")
add_subdirectory(glew)

#--------------------------------------------------------------------------
# glfw
#--------------------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

set(CMAKE_FOLDER "exts/glfw")
add_subdirectory(glfw-3.5.1)

#--------------------------------------------------------------------------
# assimp
#--------------------------------------------------------------------------
set(ASSIMP_BUILD_TESTS OFF)        # on by default and slow to build
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_NO_EXPORT ON)           # we only import models
# To build faster, turn off all importers and enable only the formats you need:
# set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
# set(ASSIMP_BUILD_OBJ_IMPORTER ON)
# set(ASSIMP_BUILD_FBX_IMPORTER ON)
# set(ASSIMP_BUILD_GLTF_IMPORTER ON)

set(CMAKE_FOLDER "exts/assimp")
add_subdirectory(assimp)

#--------------------------------------------------------------------------
# spdlog (header only)
#--------------------------------------------------------------------------
set(CMAKE_FOLDER "exts/spdlog")
# EXCLUDE_FROM_ALL: we only use the header-only target, so the
# compiled spdlog library is not built unless something asks for it.
add_subdirectory(spdlog EXCLUDE_FROM_ALL)

#--------------------------------------------------------------------------
# Libraries built directly from source in this folder
#--------------------------------------------------------------------------
set(CMAKE_FOLDER "exts")

# imgui
add_library(imgui STATIC
	imgui/imgui.cpp
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui.h
	imgui/imgui_internal.h
	imgui/imconfig.h
	imgui/imstb_rectpack.h
	imgui/imstb_textedit.h
	imgui/imstb_truetype.h
	imgui/backends/imgui_impl_opengl3.h
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/backends/imgui_impl_opengl3_loader.h
	imgui/backends/imgui_impl_glfw.h
	imgui/backends/imgui_impl_glfw.cpp
	)
target_include_directories(imgui PUBLIC imgui imgui/backends)
# IMGUI_DEFINE_MATH_OPERATORS: enable ImVec2/ImVec4 math operators for everything
# that uses imgui. Must be defined before the first imgui.h include, so it is set
# project wide.
# IMGUI_USE_LEGACY_CRC32_ADLER: imgui 1.91.6 changed the ID hash, which would
# invalidate the docking layout saved in imgui.ini. Keep the old hash.
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS IMGUI_USE_LEGACY_CRC32_ADLER)
target_link_libraries(imgui PRIVATE glfw)

# ImGuizmo
add_library(imguizmo STATIC
	ImGuizmo/src/ImCurveEdit.cpp
	ImGuizmo/src/ImCurveEdit.h
	ImGuizmo/src/ImGradient.cpp
	ImGuizmo/src/ImGradient.h
	ImGuizmo/src/ImGuizmo.cpp
	ImGuizmo/src/ImGuizmo.h
	ImGuizmo/src/ImSequencer.cpp
	ImGuizmo/src/ImSequencer.h
	)
target_include_directories(imguizmo PUBLIC ImGuizmo/src)
target_link_libraries(imguizmo PUBLIC imgui)

# nativefiledialog
add_library(nativefiledialog STATIC
	nativefiledialog/src/include/nfd.h
	nativefiledialog/src/nfd_common.h
	nativefiledialog/src/nfd_common.c
	)
target_include_directories(nativefiledialog PUBLIC nativefiledialog/src/include)

if(WIN32)
	target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_win.cpp)
elseif(APPLE)
	target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_cocoa.m)
else()
	# Linux: use GTK3, found with pkg-config
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
	target_sources(nativefiledialog PRIVATE nativefiledialog/src/nfd_gtk.c)
	target_link_libraries(nativefiledialog PRIVATE PkgConfig::GTK3)
endif()

# misc: small single-file libraries (stb, tinyxml2, json, icons, ...)
add_library(misc STATIC
	misc/tinyxml2.h
	misc/tinyxml2.cpp
	misc/stb_image.h
	misc/stb_image_write.h
	misc/PerlinNoise.hpp
	misc/json.hpp
	misc/IconsFontAwesome4_c.h
	misc/IconsFontAwesome5_c.h
	)
target_include_directories(misc PUBLIC misc)

unset(CMAKE_FOLDER)
