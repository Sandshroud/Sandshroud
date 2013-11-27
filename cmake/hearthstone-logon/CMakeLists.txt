PROJECT(hearthstone-logonserver)

SET( prefix ${ROOT_PATH}/src/hearthstone-logonserver)

SET( sources
    AccountCache.cpp
    AuthSocket.cpp
    AutoPatcher.cpp
    LogonCommServer.cpp
    LogonConsole.cpp
    LogonOpcodes.cpp
    LogonStdAfx.cpp
    Main.cpp
)

 SET( headers
    AccountCache.h
    AuthSocket.h
    AuthStructs.h
    AutoPatcher.h
    LogonCommServer.h
    LogonConsole.h
    LogonOpcodes.h
    LogonStdAfx.h
    Main.h
    PeriodicFunctionCall_Thread.h
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

set( SRCS ${SRCS}
	${ROOT_PATH}/src/hearthstone-shared/resources.rc
	${ROOT_PATH}/src/hearthstone-shared/Icon.ico)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${EXTRA_LIBS_PATH} ${DEPENDENCY_LIBS} )
IF( IS_64BIT )
	link_directories( ${DEPENDENCY_DLLS64} )
ENDIF()

include(PCHSupport)

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} ${HEADERS} )

IF( CMAKE_GENERATOR MATCHES Visual*)
	ADD_NATIVE_PRECOMPILED_HEADER(${PROJECT_NAME} LogonStdAfx.h ${prefix}/LogonStdAfx.cpp)
ENDIF()

#Link our hearthstone-shared lib.
add_dependencies( ${PROJECT_NAME} hearthstone-shared g3dlite recast detour pcre zlib )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} hearthstone-shared g3dlite recast detour pcre zlib ${EXTRA_LIBS} )

#Set our resources
set_source_files_properties(Icon.ico resources.rc PROPERTIES LANGUAGE RC)
#Set our compile information
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS _LOGON)
