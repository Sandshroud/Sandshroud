project(bzip2)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.c")
source_group("${PROJECT_NAME}" FILES ${sources})
SET(SRCS ${SRCS} ${sources} )

include_directories( ${prefix} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )

#Set it to the project name.
set(BZIP2_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "bzip2 Libraries")
SET_PROPERTY(TARGET bzip2 PROPERTY FOLDER "Dependencies")
