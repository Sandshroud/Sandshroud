/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef ADT_H
#define ADT_H

#include "mpq_libmpq04.h"
#include "wmo.h"
#include "model.h"
#include "liquidmodel.h"

#define TILESIZE (533.33333f)
#define CHUNKSIZE ((TILESIZE) / 16.0f)
#define UNITSIZE (CHUNKSIZE / 8.0f)

class Liquid;

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
    float v9[16*8+1][16*8+1];
    float v8[16*8][16*8];
}Cell;

typedef struct
{
    double v9[9][9];
    double v8[8][8];
    uint16 area_id;
    //Liquid *lq;
    float waterlevel[9][9];
    uint8 flag;
}chunk;

typedef struct
{
    chunk ch[16][16];
}mcell;

struct MapChunkHeader
{
    uint32 flags;
    uint32 ix;
    uint32 iy;
    uint32 nLayers;
    uint32 nDoodadRefs;
    uint32 ofsHeight;
    uint32 ofsNormal;
    uint32 ofsLayer;
    uint32 ofsRefs;
    uint32 ofsAlpha;
    uint32 sizeAlpha;
    uint32 ofsShadow;
    uint32 sizeShadow;
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
    uint32 ofsSndEmitters;
    uint32 nSndEmitters;
    uint32 ofsLiquid;
    uint32 sizeLiquid;
    float  zpos;
    float  xpos;
    float  ypos;
    uint32 textureId;
    uint32 props;
    uint32 effectId;
};


class ADTFile
{
private:
    MPQFile ADT;
    string Adtfilename;

public:
    uint32 MapId, TileX, TileY;

public:
    ADTFile(char* filename, uint32 mapid, uint32 tilex, uint32 tiley);
    ~ADTFile();

    bool init();
};

const char * GetPlainName(const char * FileName);
char * GetPlainName(char * FileName);
char * GetExtension(char * FileName);
void fixnamen(char *name, size_t len);
void fixname2(char *name, size_t len);
//void fixMapNamen(char *name, size_t len);

#endif
