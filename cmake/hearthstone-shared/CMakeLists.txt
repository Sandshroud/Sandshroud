PROJECT(hearthstone-shared)

SET( prefix ${ROOT_PATH}/src/hearthstone-shared )

SET( sources
    CallBack.cpp
    CoreMemoryAllocator.cpp
    CoreMemoryAllocator_Core.cpp
    CrashHandler.cpp
    crc32.cpp
    hashmap.cpp
    hearthstone_getopt.cpp
    Log.cpp
    MemoryLeaks.cpp
    MersenneTwister.cpp
    StackWalker.cpp
    Util.cpp
    Threading/Mutex.cpp
    Threading/ThreadPool.cpp
    Auth/BigNumber.cpp
    Auth/HMAC.cpp
    Auth/MD5.cpp
    Auth/Sha1.cpp
    Auth/WowCrypt.cpp
    Config/IniFiles.cpp
    DataStorage/DBC/DBC.cpp
    DataStorage/Database.cpp
    DataStorage/DatabaseEngine.cpp
    DataStorage/DBC/DBCStores.cpp
    DataStorage/DBC/dbcfile.cpp
    Network/SocketEngine.cpp
    Network/SocketEngine_epoll.cpp
    Network/SocketEngine_iocp.cpp
    Network/SocketEngine_kqueue.cpp
    Network/SocketEngine_poll.cpp
    Network/SocketEngine_select.cpp
    Network/TcpSocket.cpp
    Console/CConsole.cpp
)

set( headers 
    AuthCodes.h
    ByteBuffer.h
    ByteConverter.h
    CallBack.h
    CircularQueue.h
    Common.h
    CrashHandler.h
    crc32.h
    Errors.h
    FastQueue.h
    format.h
    hashmap.h
    hearthstone_getopt.h
    hearthstoneConfig.h
    int24.h
    Log.h
    MemoryLeaks.h
    MersenneTwister.h
    NGLog.h
    PerfCounters.h
    PreallocatedQueue.h
    RC4Engine.h
    Singleton.h
    Storage.h
    svn_revision.h
    Timer.h
    Util.h
    WorldPacket.h
    WoWGuid.h
    Threading/Threading.h
    Threading/Condition.h
    Threading/Guard.h
    Threading/LockedQueue.h
    Threading/RWLock.h
    Threading/Mutex.h
    Threading/ThreadPool.h
    Threading/ThreadStarter.h
    Auth/BigNumber.h
    Auth/HMAC.h
    Auth/MD5.h
    Auth/Sha1.h
    Auth/WowCrypt.h
    Config/IniFiles.h
    DataStorage/DBC/DBC.h
    DataStorage/Database.h
    DataStorage/DatabaseEngine.h
    DataStorage/DatabaseEnv.h
    DataStorage/DBC/DBCStores.h
    DataStorage/Field.h
    DataStorage/DBC/DataStore.h
    DataStorage/DBC/dbcfile.h
    Network/BaseBuffer.h
    Network/BaseSocket.h
    Network/CircularBuffer.h
    Network/ListenSocket.h
    Network/Network.h
    Network/SocketEngine.h
    Network/SocketEngine_epoll.h
    Network/SocketEngine_iocp.h
    Network/SocketEngine_kqueue.h
    Network/SocketEngine_poll.h
    Network/SocketEngine_select.h
    Network/StraightBuffer.h
    Network/TcpSocket.h
    Console/BaseConsole.h
    Console/CConsole.h
)

IF(UNIX)
  set( headers ${headers}
	unix/ini.c
	unix/ini.h )
ENDIF()

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET(HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${GLOBAL_INCLUDE_DIRS} )
link_directories( ${DEPENDENCY_DLLS} )
link_directories( ${DEPENDENCY_LIBS} )

ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SRCS} ${HEADERS} )

add_dependencies( ${PROJECT_NAME} g3dlite recast detour pcre zlib )
target_link_libraries( ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MYSQL_LIBRARY} ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES} ${PCRE_LIBRARIES} g3dlite recast detour pcre zlib ${EXTRA_LIBS} )

#Set our compile information
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS SHARED_LIB_BUILDER)
