
add_subdirectory(consolelog)
add_subdirectory(zlib)
add_subdirectory(pcre)
add_subdirectory(g3dlite)
add_subdirectory(detour)
add_subdirectory(recast)

if(BUILD_TOOLS)
add_subdirectory(libmpq1)
add_subdirectory(bzip2)
add_subdirectory(libmpq2)
endif()
