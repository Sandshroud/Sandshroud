PROJECT(hearthstone-world)

SET( prefix ${ROOT_PATH}/src/hearthstone-world )

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.cpp"
"${prefix}/AI/*.h"
"${prefix}/AI/*.cpp"
"${prefix}/Spellfixes/*.h"
"${prefix}/Spellfixes/*.cpp"
"${prefix}/resources.rc"
"${prefix}/Icon.ico")

SET(SRCS ${SRCS} ${sources} )

include(PCHSupport)
include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )

add_executable( ${PROJECT_NAME} ${SRCS} )

IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} StdAfx.h ${prefix}/StdAfx.cpp)
ENDIF()

#Link our hearthstone-shared lib.
add_dependencies( ${PROJECT_NAME} hearthstone-shared g3dlite recast detour pcre zlib )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} hearthstone-shared g3dlite recast detour pcre zlib ${EXTRA_LIBS} )

#Set our resources
set_source_files_properties(Icon.ico resources.rc PROPERTIES LANGUAGE RC)
#Set our compile information
set_target_properties(${PROJECT_NAME} PROPERTIES ENABLE_EXPORTS TRUE)
