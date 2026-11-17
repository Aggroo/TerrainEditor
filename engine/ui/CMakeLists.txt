#--------------------------------------------------------------------------
# ui
#
# All .h/.cpp files in this folder are found automatically.
#--------------------------------------------------------------------------
engine_library(ui)

target_link_libraries(ui PUBLIC application render foundation core glfw imgui imguizmo nativefiledialog misc)
