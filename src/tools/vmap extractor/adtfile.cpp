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

#include "vmapexport.h"
#include "adtfile.h"

#include <algorithm>
#include <cstdio>

#ifdef WIN32
#define snprintf _snprintf
#endif

char const* GetPlainName(char const* FileName)
{
    const char * szTemp;

    if((szTemp = strrchr(FileName, '\\')) != NULL)
        FileName = szTemp + 1;
    return FileName;
}

char* GetPlainName(char* FileName)
{
    char * szTemp;

    if((szTemp = strrchr(FileName, '\\')) != NULL)
        FileName = szTemp + 1;
    return FileName;
}

void fixnamen(char* name, size_t len)
{
    for (size_t i = 0; i < len-3; i++)
    {
        if (i > 0 && name[i] >= 'A' && name[i] <= 'Z' && isalpha(name[i-1]))
            name[i] |= 0x20;
        else if ((i == 0 || !isalpha(name[i-1])) && name[i]>='a' && name[i]<='z')
            name[i] &= ~0x20;
    }
    //extension in lowercase
    for (size_t i = len - 3; i < len; i++)
        name[i] |= 0x20;
}

void fixname2(char* name, size_t len)
{
    for (size_t i=0; i<len-3; i++)
    {
        if(name[i] == ' ')
        name[i] = '_';
    }
}

char* GetExtension(char* FileName)
{
    if (char* szTemp = strrchr(FileName, '.'))
        return szTemp;
    return NULL;
}

ADTFile::ADTFile(char* filename): ADT(filename), nWMO(0), nMDX(0), WmoInstansName(NULL), ModelInstansName(NULL)
{
    Adtfilename.append(filename);
}

inline uint32 fillchunkData(MPQFile & mf, uint32 ch_x, uint32 ch_y, mTile *Cell)
{
    uint32 size = 0;
    uint32 result = 0;
    mf.seekRelative(4);
    mf.read(&size, 4);
    size_t lastpos = mf.getPos() + size;
    MapChunkHeader header;
    mf.read(&header, 0x80);
    Cell->ch[ch_x][ch_y].area_id = header.areaid;
    Cell->ch[ch_x][ch_y].flags = header.flags;

    while (mf.getPos() < lastpos)
    {
        char fourcc[5];
        uint32 fourcc2;
        mf.read(&fourcc2, 4);
        mf.read(&size, 4);
        if(size == 0 && fourcc2 == 0)
            continue;

        memcpy(&fourcc, &fourcc2, 4);
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = mf.getPos()+size;
        if (!strcmp(fourcc, "MCVT"))
        {
            result |= 0x01;
            for(uint8 i = 0; i < 9; i++)
            {
                mf.read(&Cell->ch[ch_x][ch_y].v9[i], 9*sizeof(float));
                if(i == 8)
                    continue;
                mf.read(&Cell->ch[ch_x][ch_y].v8[i], 8*sizeof(float));
            }

            // Adjust the height map with the zbase
            for (int y = 0; y < 9; y++)
            {
                for (int x = 0; x < 9; x++)
                {
                    Cell->ch[ch_x][ch_y].v9[x][y] += header.zpos;
                    if(y == 8 || x == 8)
                        continue;
                    Cell->ch[ch_x][ch_y].v8[x][y] += header.zpos;
                }
            }
        }
        else if (!strcmp(fourcc, "MCLV"))
        {

        }
        else if (!strcmp(fourcc, "MCCV"))
        {

        }
        else if(!strcmp(fourcc, "MCNR"))
        {
            nextpos = mf.getPos() + 0x1C0; // size fix
        }
        else if (!strcmp(fourcc, "MCLY"))
        {

        }
        else if (!strcmp(fourcc, "MCRF"))
        {

        }
        else if (!strcmp(fourcc, "MCSH"))
        {

        }
        else if (!strcmp(fourcc, "MCAL"))
        {

        }
        else if(!strcmp(fourcc, "MCLQ")) //MCLQ
        {
            if(header.lenMCLQ)
            {
                size = header.lenMCLQ-8; // MCLQ+size
                if(size)
                {
                    result |= 0x02;
                    mf.read(&Cell->ch[ch_x][ch_y].waterlevel, sizeof(float));
                    mf.seekRelative(sizeof(float));
                    mf.read(&Cell->ch[ch_x][ch_y].liquid, sizeof(liquid_data)*9*9);
                    mf.read(&Cell->ch[ch_x][ch_y].liquidflags, sizeof(uint8)*8*8);
                }
            }
        }
        else if(!strcmp(fourcc, "MCSE")) //MCSE
        {

        }
        else
        {
            if(fourcc2 == 0xefb88b70) // Fix for ICC map
                nextpos = mf.getPos()+0x1199;
        }

        mf.seek(nextpos);
    }

    return result;
}

inline void LoadH2OChunk(MPQFile & mf, uint32 ch_x, uint32 ch_y, mTile *Cell, uint32 base)
{
/*  MH2Oinformation header;
    mf.read(&header, sizeof(MH2Oinformation));

    uint64 mask;
    if(header.offsData2a)
    {
        mf.seek(base+header.offsData2a);
        mf.read(&mask, sizeof(uint64));
    }
    else mask = 0xFFFFFFFFFFFFFFFFLL;

    int count = 0;
    for (int y = 0; y < header.Height;y++)
    {
        int cy = ch_x*8 + y + header.yOffset;
        for (int x = 0; x < header.Width; x++)
        {
            int cx = ch_y*8 + x + header.xOffset;
            if (mask& 1)
            {
                Cell->liquid_show[cy][cx] = true;
                ++count;
            }
            mask>>=1;
        }
    }

    bool UseHeight = false;
    if(!(header.flags & 0x02) && header.offsData2b)
    {
        mf.seek(base+header.offsData2b);
        UseHeight = true;
    }

    int pos = 0;
    for (int y = 0; y <= header.Height; y++)
    {
        int cy = ch_x*8 + y + header.yOffset;
        for (int x = 0; x <= header.Width; x++)
        {
            int cx = ch_y*8 + x + header.xOffset;
            if (UseHeight)
                mf.read(&Cell->liquid_height[cy][cx], sizeof(float));
            else
                Cell->liquid_height[cy][cx] = header.levels[0];
            pos++;
        }
    }

    uint32 type = LiqType[header.type];
    switch(type)
    {
    case 0:
        {
            Cell->LiquidInfo[ch_x*16+ch_y] |= 0x01;
        }break;
    case 1:
        {
            Cell->LiquidInfo[ch_x*16+ch_y] |= 0x02;
            if(header.flags & 0x01 || !header.offsData2b)
                Cell->LiquidInfo[ch_x*16+ch_y] |= 0x10;
        }break;
    case 2:
        {
            Cell->LiquidInfo[ch_x*16+ch_y] |= 0x04;
        }break;
    case 3:
        {
            Cell->LiquidInfo[ch_x*16+ch_y] |= 0x08;
        }break;
    default:
        {
            printf("Unknown H20 Type: %u\n", header.type);
        }break;
    }
    if(header.flags & 0x02)
        Cell->LiquidInfo[ch_x*16+ch_y] |= 0x20;

    if (!count && Cell->LiquidInfo[ch_x*16+ch_y])
        printf("Wrong liquid detect in MH2O chunk\n");*/
}

bool ADTFile::init(uint32 map_num, uint32 tileX, uint32 tileY)
{
    if(ADT.isEof ())
        return false;

    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    FILE *dirfile = NULL;
    fopen_s(&dirfile, dirname.c_str(), "ab");
    if(!dirfile)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    bool data = false;
    uint32 size;
    char fourcc[5];
    size_t MCNK_offsets[256], MCNK_sizes[256], MH20_offset = 0;
    while (!ADT.isEof())
    {
        ADT.read(&fourcc,4);
        ADT.read(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = ADT.getPos() + size;
        if (!strcmp(fourcc,"MCIN"))
        {
            for (int i = 0; i < 256; i++)
            {
                data = true;
                ADT.read(&MCNK_offsets[i], 4);
                ADT.read(&MCNK_sizes[i], 4);
                ADT.seekRelative(8);
            }
        }
        else if (!strcmp(fourcc,"MH2O"))
            MH20_offset = ADT.getPos();
        else if (!strcmp(fourcc,"MMDX"))
        {
            if (size)
            {
                char *buf = new char[size];
                ADT.read(buf, size);
                char *p=buf;
                int t=0;
                ModelInstansName = new string[size];
                while (p<buf+size)
                {
                    fixnamen(p,strlen(p));
                    char* s = GetPlainName(p);
                    fixname2(s,strlen(s));

                    ModelInstansName[t++] = s;

                    string path(p);
                    ExtractSingleModel(path);

                    p = p+strlen(p)+1;
                }
                delete[] buf;
            }
        }
        else if (!strcmp(fourcc,"MWMO"))
        {
            if (size)
            {
                char* buf = new char[size];
                ADT.read(buf, size);
                char* p=buf;
                int q = 0;
                WmoInstansName = new string[size];
                while (p<buf+size)
                {
                    char* s = GetPlainName(p);
                    fixnamen(s, strlen(s));
                    fixname2(s, strlen(s));
                    p += strlen(p) + 1;
                    WmoInstansName[q++] = s;
                }
                delete[] buf;
            }
        } //======================
        else if (!strcmp(fourcc,"MDDF"))
        {
            if (size)
            {
                nMDX = (int)size / 36;
                for (int i=0; i<nMDX; ++i)
                {
                    uint32 id;
                    ADT.read(&id, 4);
                    ModelInstance inst(ADT,ModelInstansName[id].c_str(), map_num, tileX, tileY, dirfile);
                }
                delete[] ModelInstansName;
            }
        }
        else if (!strcmp(fourcc,"MODF"))
        {
            if (size)
            {
                nWMO = (int)size / 64;
                for (int i=0; i<nWMO; ++i)
                {
                    uint32 id;
                    ADT.read(&id, 4);
                    WMOInstance inst(ADT,WmoInstansName[id].c_str(), map_num, tileX, tileY, dirfile);
                }
                delete[] WmoInstansName;
            }
        } //======================
        ADT.seek(nextpos);
    }

    if(data)
    {
        mTile tile;
        memset(&tile, 0, sizeof(mTile));
        uint32 pos = 0;
        for (int j=0; j<16; j++)
        {
            for (int i=0; i<16; i++)
            {
                ADT.seek((int)MCNK_offsets[j*16+i]);
                tile.ch[j][i].dataFlags = fillchunkData(ADT, j, i, &tile);
                /*if(MH20_offset)
                {
                    if (mh2oheader[j*16+i].layerCount > 0)
                    {
                        mf.seek((int)mh2oheader[j*16+i].ofsInformation+mh2o_base);
                        LoadH2OChunk(mf, j, i, mcells, mh2o_base);
                    }
                }*/
            }
        }

//#define DUMP_HEIGHT_DATA
#ifdef DUMP_HEIGHT_DATA
        ADT_Tile_Instance m_tileInstance(map_num, tileX, tileY);
        m_tileInstance.HandleInput(&tile);
#endif
    }

    ADT.close();
    fclose(dirfile);
    return true;
}

ADTFile::~ADTFile()
{
    ADT.close();
}

ADT_Tile_Instance::ADT_Tile_Instance(uint32 mapId, uint32 tileX, uint32 tileY) : _mapId(mapId), _tileX(tileX), _tileY(tileY)
{
    char tempname[512];
    sprintf(tempname, "%s/%03u_%02u_%02u.hvm", szWorkDirWmo, mapId, tileX, tileY);
    fileName = std::string(tempname);
}

void ADT_Tile_Instance::HandleInput(mTile *tile)
{
    char tempname[512];
    sprintf(tempname, "%s/%03u.hvm", szWorkDirWmo, _mapId);
    FILE *output = NULL;
    fopen_s(&output, tempname, "rb");
    if(output == NULL)
        fopen_s(&output, tempname, "wb");
    fclose(output);

    fopen_s(&output, fileName.c_str(), "w+b");
    if(output==NULL)
        return; // Never happens..
    fwrite(&_mapId, sizeof(uint16), 1, output);
    fwrite(&_tileX, sizeof(uint16), 1, output);
    fwrite(&_tileY, sizeof(uint16), 1, output);

    uint32 offsets[16][16];
    memset(&offsets, 0, sizeof(uint32)*16*16);
    fseek (output, 0, SEEK_END);
    size_t offsetPos = ftell(output);
    fwrite(&offsets, sizeof(uint32), 16*16, output);
    // Note, G3D uses a different type of storage than the client does, so we don't need to convert this since vmap will do it for us
    float TileStartX = (float(_tileX)*TILESIZE);//-(32.0f*TILESIZE);
    float TileStartY = (float(_tileY)*TILESIZE);//-(32.0f*TILESIZE);
    // Fill process from chunks is precise, so we do regular flush
    for(uint8 x = 0; x < 16; x++)
    {
        for(uint8 y = 0; y < 16; y++)
        {
            if(!(tile->ch[x][y].dataFlags & 0x01))
                continue;

            // We're at the end anyway, but we need to seek regardless
            fseek (output, 0, SEEK_END);
            // Current pos since we haven't written data
            offsets[x][y] = ftell(output);
            // Create a 2 byte chunk id for storage
            uint16 chunkId = uint16(uint16(x)<<8|uint16(y));
            // We can probably get this into 8 bits, but it's fine for now
            fwrite(&chunkId, sizeof(uint16), 1, output);
            // Append the chunk area id
            fwrite(&tile->ch[x][y].area_id, sizeof(uint16), 1, output);
            // Append the chunk flags
            fwrite(&tile->ch[x][y].flags, sizeof(uint16), 1, output);
            // Our world position as a chunk in a tile
            float chunkStartX = TileStartX+(float(x)*CHUNKSIZE);
            float chunkStartY = TileStartY+(float(y)*CHUNKSIZE);
            uint32 count = 9*9+8*8;
            fwrite(&count, sizeof(uint32), 1, output);
            for(uint8 cx = 0; cx < 9; cx++)
            {
                for(uint8 cy = 0; cy < 9; cy++)
                {
                    // x 0 is the top corner, where the chunk starts, and x 9 is where it ends
                    // so break each chunk up based on how long each step is, for how long we step
                    float xOffset = cx*(CHUNKSIZE/9.0f), yOffset = cy*(CHUNKSIZE/9.0f);
                    Vec3D cornerPoint(chunkStartX+xOffset, chunkStartY+yOffset, tile->ch[x][y].v9[cx][cy]);
                    fwrite(&cornerPoint.x, sizeof(float), 1, output);
                    fwrite(&cornerPoint.y, sizeof(float), 1, output);
                    fwrite(&cornerPoint.z, sizeof(float), 1, output);
                }

                if(cx == 8)
                    continue;
                for(uint8 cy = 0; cy < 8; cy++)
                {
                    // For center points, offset the x and y positions for each index by half of the distance between each regular point.
                    float xOffset = ((cy*(CHUNKSIZE/9.0f))+(CHUNKSIZE/18.0f)), yOffset = ((cy*(CHUNKSIZE/9.0f))+(CHUNKSIZE/18.0f));
                    Vec3D centerPoint(chunkStartX+xOffset, chunkStartY+yOffset, tile->ch[x][y].v8[cx][cy]);
                    fwrite(&centerPoint.x, sizeof(float), 1, output);
                    fwrite(&centerPoint.y, sizeof(float), 1, output);
                    fwrite(&centerPoint.z, sizeof(float), 1, output);
                }
            }
        }
    }
    fseek(output, offsetPos, SEEK_SET);
    fwrite(&offsets, sizeof(uint32), 16*16, output);
    fclose(output);
}
