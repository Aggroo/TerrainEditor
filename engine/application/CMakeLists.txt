#--------------------------------------------------------------------------
# application
#
# All .h/.cpp files in this folder are found automatically.
#--------------------------------------------------------------------------
engine_library(application)

target_link_libraries(application PUBLIC render foundation core glfw imgui imguizmo misc)
