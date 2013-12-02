project(Hearthstone)

#Do system checking.
cmake_minimum_required(VERSION 2.8)

#set our system paths
SET(ROOT_PATH  ${CMAKE_SOURCE_DIR}/..)
SET(GLOBAL_DEFINES "-DHAVE_CONFIG_H")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT_PATH}/bin" )

#check platform version.
IF(CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[xX]86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "[aA][mM][dD]64" OR CMAKE_GENERATOR MATCHES Win64*)
	#define for 64bit
	SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} -"DX64")
	SET(IS_64BIT TRUE)
	IF(CMAKE_GENERATOR MATCHES NMake*)
		SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "-bigobj")
	ENDIF()
ENDIF()

IF( CMAKE_GENERATOR MATCHES Visual*)
	SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "/Od /Ob2 /MP2 /W1")
ENDIF()

#Set the visual studio precompiled header buffer
SET(VISUALSTUDIO_COMPILERHEAPLIMIT 450 CACHE STRING "Visual Studio compiler heap limit. Ignore on darwin and unix platforms." )

IF(IS_64BIT AND CMAKE_GENERATOR MATCHES Visual*)
SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "/bigobj")
ENDIF()

SET(IS_MAC FALSE)
#mac osx
IF( APPLE )
	SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DHAVE_DARWIN")
	SET(IS_MAC TRUE)
ENDIF()

IF(UNIX AND NOT IS_MAC)
	#mac has unix defined too but if 'if(APPLE)' fails, then it's not apple.

	#check for freebsd
	IF( ( CMAKE_SYSTEM_NAME  STREQUAL "FreeBSD" ) OR ( CMAKE_SYSTEM_NAME STREQUAL kFreeBSD ) )
		SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_KQUEUE")
		SET(IS_FREEBSD TRUE)
	ELSE()
		SET(GLOBAL_DEFINES ${GLOBAL_DEFINES} "-DUSE_EPOLL")
		SET(IS_LINUX TRUE)
	ENDIF()
ENDIF()

#We have our own custom modules that we use. This tells cmakes where to find them.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ROOT_PATH}/cmake_modules )
#Include the directories for our dependencies
SET(DEPENDENCY_ROOT ${ROOT_PATH}/dependencies )
SET(DEPENDENCY_SOURCES ${DEPENDENCY_ROOT}/SRC)

IF(IS_64BIT)
	SET(DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/LIBS/amd64)
ELSE()
	SET(DEPENDENCY_LIBS ${DEPENDENCY_ROOT}/LIBS)
ENDIF()

IF(IS_64BIT)
	SET(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}"/DLL Files/amd64")
ELSE()
	SET(DEPENDENCY_DLLS ${DEPENDENCY_ROOT}"/DLL Files")
ENDIF()

SET(BUILD_INFORMATION "${CMAKE_SYSTEM_PROCESSOR} - ${CMAKE_GENERATOR}" CACHE PATH "What the cmake generator string is")

#Grab our ini files
SET(INIFILE_PATH ${ROOT_PATH}/src CACHE PATH "Path where the hearthstone ini files are.")

include(CompilerVersion)
if( CMAKE_GENERATOR MATCHES Unix* )
	add_definitions(-Wno-deprecated)
	#Extract Revision
	execute_process(WORKING_DIRECTORY "${ROOT_PATH}/dependencies/SH" COMMAND sh extract_git_version.sh)

	#Check support for unordered_map/set
	GetCompilerVersion(gcc_version)
	IF( DEFINED gcc_version AND ( ${gcc_version} VERSION_EQUAL "4.3" OR ${gcc_version} VERSION_GREATER "4.3" ) AND NOT APPLE )
		add_definitions(-DHAS_CXX0X -std=gnu++0x)
	ENDIF()
ELSEIF( CMAKE_GENERATOR MATCHES Visual* )
	#Extract revision
	execute_process(WORKING_DIRECTORY "${ROOT_PATH}/dependencies/SH" COMMAND extract_git_version.bat)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS /EHa )

	#Check support for unordered_map/set
	GetCompilerVersion( msvc_version)
	IF(DEFINED ${msvc_version} AND ${msvc_version} GREATER "1599" )
		add_definitions(-DHAS_CXX0X)
	ENDIF()

	#This fixes PCH issues 'Inconsistent values for /Ym'
	# 2 steps. First remove any /Zm option that may exist and then add our own one
	STRING( REGEX REPLACE "(/Zm)([0-9]+)" "" CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} )
	SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}" )
	STRING( REGEX REPLACE "(/Zm)([0-9]+)" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zm${VISUALSTUDIO_COMPILERHEAPLIMIT}" )
	# put flags in cmake cache
	SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING "CMAKE_CXX_FLAGS" FORCE)
	SET( CMAKE_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "CMAKE_C_FLAGS" FORCE)
ENDIF()

#Pass our configurable variables to the C++ preprocessor.
add_definitions( ${GLOBAL_DEFINES} -DCONFDIR=\"${INIFILE_PATH}\")

#These are set by find_package or the zlib and pcre subprojects.
set(ZLIB_LIBRARIES "" CACHE INTERNAL "zlib libraries.")
set(PCRE_LIBRARIES "" CACHE INTERNAL "pcre libraries.")
set(RECAST_LIBRARIES "" CACHE INTERNAL "recast libraries.")
set(DETOUR_LIBRARIES "" CACHE INTERNAL "detour libraries.")
set(G3DLITE_LIBRARIES "" CACHE INTERNAL "g3dlite libraries.")
#mark them as advanced so they don't show up on the gui.
mark_as_advanced(ZLIB_LIBRARIES PCRE_LIBRARIES RECAST_LIBRARIES DETOUR_LIBRARIES G3DLITE_LIBRARIES)

set(GLOBAL_INCLUDE_DIRS 
${ROOT_PATH}/src/hearthstone-shared
${ROOT_PATH}/src/hearthstone-world
${ROOT_PATH}/src/hearthstone-logonserver
${DEPENDENCY_SOURCES}
${DEPENDENCY_SOURCES}/g3dlite
${DEPENDENCY_SOURCES}/g3dlite/vmap
${DEPENDENCY_SOURCES}/pcre
${DEPENDENCY_SOURCES}/zlib
${DEPENDENCY_SOURCES}/recast
${DEPENDENCY_SOURCES}/detour )

SET(HEARTHSTONE_SOURCE_DIRECTORIES
"${ROOT_PATH}/src/hearthstone-world/AI"
"${ROOT_PATH}/src/hearthstone-world/Battlegrounds"
"${ROOT_PATH}/src/hearthstone-world/Battlegrounds/Expansion0"
"${ROOT_PATH}/src/hearthstone-world/Battlegrounds/Expansion1"
"${ROOT_PATH}/src/hearthstone-world/Battlegrounds/Expansion2"
"${ROOT_PATH}/src/hearthstone-world/Chat"
"${ROOT_PATH}/src/hearthstone-world/Chat/Commands"
"${ROOT_PATH}/src/hearthstone-world/Client Communications"
"${ROOT_PATH}/src/hearthstone-world/Client Communications/Packet Handlers"
"${ROOT_PATH}/src/hearthstone-world/Intersystem"
"${ROOT_PATH}/src/hearthstone-world/Intersystem/Logon"
"${ROOT_PATH}/src/hearthstone-world/Intersystem/Management"
"${ROOT_PATH}/src/hearthstone-world/Intersystem/Management/Guild"
"${ROOT_PATH}/src/hearthstone-world/Intersystem/Voice"
"${ROOT_PATH}/src/hearthstone-world/Mainframe"
"${ROOT_PATH}/src/hearthstone-world/Map System"
"${ROOT_PATH}/src/hearthstone-world/Map System/Environment"
"${ROOT_PATH}/src/hearthstone-world/Map System/Map Interface"
"${ROOT_PATH}/src/hearthstone-world/Map System/Map Managers"
"${ROOT_PATH}/src/hearthstone-world/Map System/Map Tiles"
"${ROOT_PATH}/src/hearthstone-world/Objects"
"${ROOT_PATH}/src/hearthstone-world/Objects/Units"
"${ROOT_PATH}/src/hearthstone-world/Mainframe"
"${ROOT_PATH}/src/hearthstone-world/Player Systems"
"${ROOT_PATH}/src/hearthstone-world/ScriptInterface"
"${ROOT_PATH}/src/hearthstone-world/Spell"
"${ROOT_PATH}/src/hearthstone-world/Spell/Spellfixes")

#Find our needed libs
IF( NOT WIN32 )
	find_package(ZLIB REQUIRED)
	find_package(PCRE REQUIRED)
	find_package(OpenSSL REQUIRED)
	find_package(Threads REQUIRED)
	find_package(MySQL REQUIRED)
ENDIF()

IF(WIN32)	#On windows, we have to specify the includes for libeay and mysql libs.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} 
	${DEPENDENCY_SOURCES}/openssl
	${DEPENDENCY_SOURCES}/mysql )
ELSE()	#On apple n linux, mysql.h should have been found.
	SET( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${MYSQL_INCLUDE_DIR} )
ENDIF()

#For windows, link to libeay and libmysql
SET( EXTRA_LIBS)
if(WIN32)
	SET( EXTRA_LIBS ${EXTRA_LIBS} "Mswsock.lib" )
	SET( MYSQL_LIBRARY ${MYSQL_LIBRARY} "libmysql.lib" )
	SET( OPENSSL_LIBRARIES ${OPENSSL_LIBRARIES} "libeay32.lib" )

	#Needed for socket stuff and crash handler
	SET(EXTRA_LIBS ${EXTRA_LIBS} ws2_32.lib dbghelp.lib )
elseif(APPLE)
	SET( EXTRA_LIBS ${EXTRA_LIBS} "-framework Carbon" )
elseif(IS_LINUX)
	SET( EXTRA_LIBS ${EXTRA_LIBS} dl )
else()
endif()

IF( APPLE )
	IF( NOT IS_64BIT )
		SET( EMPTY "" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS}" )		
		STRING( REGEX REPLACE ".*-m64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
		STRING( REGEX REPLACE ".*-m 64" "" CMAKE_MODULE_LINKER_FLAGS  "${CMAKE_MODULE_LINKER_FLAGS}" )
		ADD_DEFINITIONS( -m32 )
		SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32" )
		SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32 -undefined dynamic_lookup" )
		SET( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m32" )
	ENDIF( NOT IS_64BIT )
ENDIF( APPLE )

add_subdirectory(hearthstone-shared)
add_subdirectory(hearthstone-logon)
add_subdirectory(hearthstone-world)
add_subdirectory(lacrimi)
add_subdirectory(zlib)
add_subdirectory(pcre)
add_subdirectory(g3dlite)
add_subdirectory(detour)
add_subdirectory(recast)
