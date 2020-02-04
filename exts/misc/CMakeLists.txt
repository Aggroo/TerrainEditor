project (misc)
set (MISC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(MISC_HEAD 
	${MISC_DIR}/tinyxml2.h
	${MISC_DIR}/stb_image.h
	${MISC_DIR}/stb_image_write.h
	${MISC_DIR}/PerlinNoise.hpp
	${MISC_DIR}/json.hpp)
	
set(MISC_SRC
	${MISC_DIR}/tinyxml2.cpp
)

add_library (misc STATIC ${MISC_SRC} ${MISC_HEAD})