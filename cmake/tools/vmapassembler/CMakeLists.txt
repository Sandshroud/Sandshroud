PROJECT(VMapAssembler)

SET( prefix "${ROOT_PATH}/src/tools/vmap assembler")

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.cpp")
source_group("${PROJECT_NAME}" FILES ${sources})

SET( SRCS ${SRCS} ${sources} )

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )
add_executable( ${PROJECT_NAME} ${SRCS} )
add_dependencies( ${PROJECT_NAME} pcre zlib g3dlite )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} pcre zlib g3dlite )
