#--------------------------------------------------------------------------
# render
#
# All .h/.cpp files in this folder are found automatically.
#--------------------------------------------------------------------------
engine_library(render)

target_link_libraries(render PUBLIC foundation core glfw imgui assimp misc)
