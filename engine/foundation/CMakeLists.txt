#--------------------------------------------------------------------------
# foundation
#
# All .h/.cpp files in this folder are found automatically.
#--------------------------------------------------------------------------
engine_library(foundation)

target_link_libraries(foundation PUBLIC core glfw imgui misc)
