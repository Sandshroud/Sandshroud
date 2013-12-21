PROJECT(hearthstone-shared)

SET( prefix ${ROOT_PATH}/src/hearthstone-shared )

FILE(GLOB auth_files
"${prefix}/Auth/*.h"
"${prefix}/Auth/*.cpp")
source_group("Auth" FILES ${auth_files})

SET(SRCS ${SRCS} ${auth_files})

FILE(GLOB config_files
"${prefix}/Config/*.h"
"${prefix}/Config/*.cpp")
source_group("Config" FILES ${config_files})
SET(SRCS ${SRCS} ${config_files} )

IF(TRUE)
FILE(GLOB unixconfig
"${prefix}/Config/unix/*.c"
"${prefix}/Config/unix/*.h")
source_group("Config" FILES ${unixconfig})

SET(SRCS ${SRCS} ${unixconfig} )
ENDIF()

FILE(GLOB console_files
"${prefix}/Console/*.h"
"${prefix}/Console/*.cpp")
source_group("Console" FILES ${console_files})
SET(SRCS ${SRCS} ${console_files} )

FILE(GLOB datastore_files
"${prefix}/DataStorage/*.h"
"${prefix}/DataStorage/*.cpp")
source_group("Database" FILES ${datastore_files})
SET(SRCS ${SRCS} ${datastore_files} )

FILE(GLOB datastore_dbc_files
"${prefix}/DataStorage/DBC/*.h"
"${prefix}/DataStorage/DBC/*.cpp")
source_group("Database\\DBC" FILES ${datastore_dbc_files})
SET(SRCS ${SRCS} ${datastore_dbc_files} )

FILE(GLOB network_files
"${prefix}/Network/*.h"
"${prefix}/Network/*.cpp")
source_group("Network" FILES ${network_files})
SET(SRCS ${SRCS} ${network_files} )

FILE(GLOB threading_files
"${prefix}/Threading/*.h"
"${prefix}/Threading/*.cpp")
source_group("Threading" FILES ${threading_files})
SET(SRCS ${SRCS} ${threading_files} )

FILE(GLOB sources
"${prefix}/*.h"
"${prefix}/*.cpp")
source_group("Util" FILES ${sources})
SET(SRCS ${SRCS} ${sources})

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_DLLS} ${DEPENDENCY_LIBS} )

ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRCS} )

add_dependencies( ${PROJECT_NAME} g3dlite recast detour pcre zlib consolelog database dbclib network threading )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} g3dlite recast detour pcre zlib consolelog database dbclib network threading ${EXTRA_LIBS} )

#Set our compile information
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS SHARED_LIB_BUILDER)
