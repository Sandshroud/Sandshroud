project(libmpq2)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.c")
source_group("${PROJECT_NAME}" FILES ${sources})
SET(SRCS ${SRCS} ${sources} )

include_directories( ${GLOBAL_INCLUDE_DIRS} ${prefix} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )

add_dependencies( ${PROJECT_NAME} pcre zlib bzip2 )
target_link_libraries( ${PROJECT_NAME} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} pcre zlib bzip2 )

#Set it to the project name.
set(LIBMPQ2_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME} Libraries")
SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Dependencies")
