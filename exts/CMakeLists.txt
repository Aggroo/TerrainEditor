#--------------------------------------------------------------------------
# exts
#--------------------------------------------------------------------------
ADD_LIBRARY(exts INTERFACE)

ADD_SUBDIRECTORY(glew)
SET_TARGET_PROPERTIES(glew PROPERTIES FOLDER "exts/glew")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glew/include)

SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
SET(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
SET(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")		  

ADD_SUBDIRECTORY(glfw-3.3.2)
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "exts/glfw")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE glfw-3.3.2/include)

ADD_SUBDIRECTORY(assimp)
SET_TARGET_PROPERTIES(assimp PROPERTIES FOLDER "exts/assimp")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE assimp/include)
target_link_libraries(exts INTERFACE assimp)

OPTION(DEFINE_GLEW "Define IMGUI GLEW" ON)

IF(DEFINE_GLEW)
    ADD_DEFINITIONS(-DIMGUI_IMPL_OPENGL_LOADER_GLEW)
	add_definitions(-DGLFW_EXPOSE_NATIVE_WGL)
ENDIF(DEFINE_GLEW)

ADD_LIBRARY(imgui STATIC 
			imgui/imgui.cpp 
			imgui/imgui_demo.cpp 
			imgui/imgui_draw.cpp 
			imgui/imgui.h 
			imgui/imgui_internal.h 
			imgui/imstb_rectpack.h
			imgui/imstb_textedit.h
			imgui/imstb_truetype.h
			imgui/examples/imgui_impl_opengl3.h
			imgui/examples/imgui_impl_opengl3.cpp
			imgui/examples/imgui_impl_glfw.h
			imgui/examples/imgui_impl_glfw.cpp
			imgui/imconfig.h
			imgui/imgui_widgets.cpp
			ImGuizmo/ImCurveEdit.cpp
			ImGuizmo/ImCurveEdit.h
			ImGuizmo/ImGradient.cpp
			ImGuizmo/ImGradient.h
			ImGuizmo/ImGuizmo.cpp
			ImGuizmo/ImGuizmo.h
			ImGuizmo/ImSequencer.cpp
			ImGuizmo/ImSequencer.h
			)

TARGET_LINK_LIBRARIES(imgui PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(imgui PUBLIC imgui imgui/examples/)
SET_TARGET_PROPERTIES(imgui PROPERTIES FOLDER "exts/imgui")
TARGET_LINK_LIBRARIES(exts INTERFACE imgui)

ADD_LIBRARY(imguizmo STATIC 
			ImGuizmo/ImCurveEdit.cpp
			ImGuizmo/ImCurveEdit.h
			ImGuizmo/ImGradient.cpp
			ImGuizmo/ImGradient.h
			ImGuizmo/ImGuizmo.cpp
			ImGuizmo/ImGuizmo.h
			ImGuizmo/ImSequencer.cpp
			ImGuizmo/ImSequencer.h
			)

TARGET_LINK_LIBRARIES(imguizmo PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(imguizmo PUBLIC imguizmo)
SET_TARGET_PROPERTIES(imguizmo PROPERTIES FOLDER "exts/ImGuizmo")
TARGET_LINK_LIBRARIES(exts INTERFACE imguizmo)

IF(CMAKE_COMPILER_IS_GNUCXX)

	# Use the package PkgConfig to detect GTK+ headers/library files
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
	add_definitions(${GTK3_CFLAGS_OTHER})

	SET(files_nativefiledialog
	nativefiledialog/src/nfd_gtk.c
	)
ENDIF()

IF(MSVC)
	SET(files_nativefiledialog
	nativefiledialog/src/nfd_win.cpp
	)
ENDIF()	

ADD_LIBRARY(nativefiledialog STATIC
			nativefiledialog/src/include/nfd.h
			nativefiledialog/src/nfd_common.h
			nativefiledialog/src/nfd_common.c
			${files_nativefiledialog}
			)

TARGET_LINK_LIBRARIES(nativefiledialog PUBLIC exts ${GTK3_LIBRARIES})
SET_TARGET_PROPERTIES(nativefiledialog PROPERTIES FOLDER "exts/nativefiledialog")
TARGET_INCLUDE_DIRECTORIES(exts INTERFACE nativefiledialog nativefiledialog/src/include)
TARGET_LINK_LIBRARIES(exts INTERFACE nativefiledialog)

ADD_LIBRARY(misc STATIC 
			misc/tinyxml2.h
			misc/stb_image.h
			misc/stb_image_write.h
			misc/PerlinNoise.hpp
			misc/json.hpp
			misc/IconsFontAwesome4_c.h
			misc/IconsFontAwesome5_c.h
			misc/tinyxml2.cpp
			)

TARGET_LINK_LIBRARIES(misc PUBLIC exts)
TARGET_INCLUDE_DIRECTORIES(misc PUBLIC misc)
SET_TARGET_PROPERTIES(misc PROPERTIES FOLDER "exts/misc")
TARGET_LINK_LIBRARIES(exts INTERFACE misc)

