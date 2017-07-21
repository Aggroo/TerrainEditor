#--------------------------------------------------------------------------
# projects
#--------------------------------------------------------------------------
FILE(GLOB children RELATIVE ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/*)
FOREACH(child ${children})
	IF(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${child})
		ADD_SUBDIRECTORY(${child})
	ENDIF()
ENDFOREACH()
