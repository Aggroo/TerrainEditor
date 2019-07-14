#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_ui_utilities
	iconhelper.cpp
	iconhelper.h
	uiserver.cpp
	uiserver.h
	imguiextra.h
	)

SET(files_ui_widgets
	widgets/widget.h
	widgets/widgetinspector.cpp
	widgets/widgetinspector.h
	widgets/widgetmenubar.cpp
	widgets/widgetmenubar.h
	widgets/widgetperlingenerator.cpp
	widgets/widgetperlingenerator.h
	widgets/widgetterrainsettings.cpp
	widgets/widgetterrainsettings.h
	widgets/widgettoolbar.cpp
	widgets/widgettoolbar.h
	widgets/widgetview.cpp
	widgets/widgetview.h
	)

SOURCE_GROUP("utilities" FILES ${files_ui_utilities})
SOURCE_GROUP("widgets" FILES ${files_ui_widgets})


SET(files_ui
	${files_ui_utilities}
	${files_ui_widgets})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(ui STATIC ${files_ui} ${files_pch})
TARGET_PCH(ui ../)	
ADD_DEPENDENCIES(ui glew glfw misc)
TARGET_LINK_LIBRARIES(ui PUBLIC core engine exts glew glfw ${OPENGL_LIBS} misc)