project(zlib)

SET( prefix ${DEPENDENCY_SOURCES}/${PROJECT_NAME} )

SET(sources
adler32.c
compress.c
crc32.c
deflate.c
example.c
gzio.c
infback.c
inffast.c
inflate.c
inftrees.c
trees.c
uncompr.c
zutil.c)

SET(headers
crc32.h
deflate.h
inffast.h
inffixed.h
inflate.h
inftrees.h
trees.h
zconf.h
zlib.h
zutil.h)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories( ${prefix} )
add_library(${PROJECT_NAME} STATIC ${SRCS} ${HEADERS} )
#Set it to the project name.
set(ZLIB_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "zlib Libraries")