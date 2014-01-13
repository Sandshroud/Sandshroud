/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ADT_H
#define ADT_H

#include "mpq_libmpq04.h"
#include "wmo.h"
#include "model.h"

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)
#define UNITSIZE (CHUNKSIZE / 8.0f)

typedef struct
{
    float x;
    float y;
    float z;
}svec;

struct vec
{
    double x;
    double y;
    double z;
};

struct triangle
{
    vec v[3];
};

typedef struct
{
    uint32 light;
    float  height;
}liquid_data;

typedef struct
{
    uint8 dataFlags;
    float v9[9][9];
    float v8[8][8];
    uint16 area_id;
    uint16 flags;
    float waterlevel;
    liquid_data liquid[9][9];
    uint8 liquidflags[8][8];
}chunk;

typedef struct
{
    chunk ch[16][16];
}mTile;

struct MapChunkHeader
{
    uint32 flags;
    uint32 ix;
    uint32 iy;
    uint32 nLayers;
    uint32 nDoodadRefs;
    uint32 ofsMCVT;
    uint32 ofsMCNR;
    uint32 ofsMCLY;
    uint32 ofsMCRF;
    uint32 ofsMCAL;
    uint32 lenMCAL;
    uint32 ofcMCSH;
    uint32 lenMCSH;
    uint32 areaid;
    uint32 nMapObjRefs;
    uint32 holes;
    uint16 s1;
    uint16 s2;
    uint32 d1;
    uint32 d2;
    uint32 d3;
    uint32 predTex;
    uint32 nEffectDoodad;
    uint32 ofsMCSE;
    uint32 lenMCSE;
    uint32 ofcMCLQ;
    uint32 lenMCLQ;
    float  zpos;
    float  xpos;
    float  ypos;
    uint32 ofsMCCV;
    uint32 ofsMCLV;
    uint32 buff;
};

class ADT_Tile_Instance
{
private:
    std::string fileName;
    uint16 _mapId, _tileX, _tileY;
public:
    ADT_Tile_Instance(uint32 mapId, uint32 tileX, uint32 tileY);
    void HandleInput(mTile *tile);
};

class ADTFile
{
private:
    MPQFile ADT;
    string Adtfilename;
public:
    ADTFile(char* filename);
    ~ADTFile();
    int nWMO;
    int nMDX;
    string* WmoInstansName;
    string* ModelInstansName;
    bool init(uint32 map_num, uint32 tileX, uint32 tileY);
};

const char * GetPlainName(const char * FileName);
char * GetPlainName(char * FileName);
char * GetExtension(char * FileName);
void fixnamen(char *name, size_t len);
void fixname2(char *name, size_t len);
//void fixMapNamen(char *name, size_t len);

#endif
