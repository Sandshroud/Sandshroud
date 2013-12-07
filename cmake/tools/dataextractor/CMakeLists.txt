PROJECT(Extractor)

SET( prefix "${ROOT_PATH}/src/tools/data extractor")

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.cpp")
source_group("${PROJECT_NAME}" FILES ${sources})

SET( SRCS ${SRCS} ${sources} )
SET( SRCS ${SRCS} "${ROOT_PATH}/src/tools/Icon.ico" "${ROOT_PATH}/src/tools/resources.rc")

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )
add_executable( ${PROJECT_NAME} ${SRCS} )
add_dependencies( ${PROJECT_NAME} pcre zlib libmpq1 )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} pcre zlib libmpq1 )
#Set the solution folder to tools
SET_PROPERTY(TARGET Extractor PROPERTY FOLDER "Tools")
#Set the output folder to bin/tools
set_target_properties( Extractor PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}/Tools")
