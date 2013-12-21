
if(BUILD_TOOLS)
add_subdirectory(bzip2)
endif()

add_subdirectory(consolelog)
add_subdirectory(database)
add_subdirectory(dbclib)
add_subdirectory(detour)
add_subdirectory(g3dlite)

if(BUILD_TOOLS)
add_subdirectory(libmpq1)
add_subdirectory(libmpq2)
endif()

add_subdirectory(network)
add_subdirectory(pcre)
add_subdirectory(recast)
add_subdirectory(threading)
add_subdirectory(zlib)
