project(pcre)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.c")
source_group("${PROJECT_NAME}" FILES ${sources})
SET(SRCS ${SRCS} ${sources} )

include_directories( ${prefix} )
add_library(${PROJECT_NAME} STATIC ${SRCS} )

#Set it to the project name.
set(RECAST_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "pcre Libraries")