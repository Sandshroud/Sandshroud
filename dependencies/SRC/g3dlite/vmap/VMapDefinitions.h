/***
 * Demonstrike Core
 */

#pragma once

#define LIQUID_TILE_SIZE (533.333f / 128.f)

namespace VMAP
{
	const char VMAP_MAGIC[] = "VMAP_3.0";

	// defined in TileAssembler.cpp currently...
	bool readChunk(FILE *rf, char *dest, const char *compare,G3D::g3d_uint32 len);
}

#ifndef _GAME

#if defined MMAP_GENERATOR
	#include <assert.h>
	#define MANGOS_ASSERT(x) assert(x)
	#define DEBUG_LOG(...) 0
	#define DETAIL_LOG(...) 0
	#define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while(0)
#else
//	#include <assert.h>
//	#define ASSERT(x) assert(x)
	#define DEBUG_LOG(info, ...) do{ printf(info); printf(__VA_ARGS__); printf("\n"); } while(0)
	#define DETAIL_LOG(...) do{ printf(__VA_ARGS__); printf("\n"); } while(0)
	#define ERROR_LOG(...) do{ printf("ERROR:"); printf(__VA_ARGS__); printf("\n"); } while(0)
#endif

#endif
