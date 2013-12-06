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

#include "vmapexport.h"
#include "wdtfile.h"
#include "adtfile.h"
#include <cstdio>

char * wdtGetPlainName(char * FileName)
{
    char * szTemp;

    if((szTemp = strrchr(FileName, '\\')) != NULL)
        FileName = szTemp + 1;
    return FileName;
}

WDTFile::WDTFile(char* file_name, char* adtname, char* map_id, uint32 mapID) : filename(file_name), adtName(adtname), MapEntry(map_id), MapId(mapID), WDT(file_name)
{
    memset(&_mphdChunk, 0, 4);
    memset(&_mphdChunk, 0, 32);
    memset(&_modfChunk, 0, 64);
    memset(&_mainChunk.m_chunkData, 0, 64*64*8);
    _mwmoChunk.hasWMO = false;
}

WDTFile::~WDTFile(void)
{
    WDT.close();
}

bool WDTFile::init()
{
    if (WDT.isEof())
        return false;
    return true;
}

void WDTFile::readandprocess()
{
    // WDT is literally MVER->MPHD->Main->MWMO->MODF, we'll keep this structure though
    uint32 size;
    char fourcc[5];
    while (!WDT.isEof())
    {
        WDT.read(fourcc,4);
        WDT.read(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = WDT.getPos()+size;
        if (!strcmp(fourcc,"MVER"))
        {
            if(size > 4)
                size = 4;
            WDT.read(&_mverChunk, size);
        }
        else if (!strcmp(fourcc,"MPHD"))
        {
            if(size > 32)
                size = 32;
            WDT.read(&_mphdChunk, size);
        }
        else if (!strcmp(fourcc,"MAIN"))
        {
            if(size > 32768)
                size = 32768;
            // Data offset used for compression, but nonexistent till 4.x
            WDT.read(&_mainChunk.m_chunkData+_mphdChunk.DataOffset, size);
            for(uint8 x = 0; x < 64; x++)
            {
                for(uint8 y = 0; y < 64; y++)
                {
                    if(_mainChunk.m_chunkData[x][y].m_flags & MPHD_FLAG_ADT_BASED)
                        ADTTiles.push_back(std::make_pair(x,y));
                }
            }
        }
        else if (!strcmp(fourcc,"MWMO"))
        {
            _mwmoChunk.hasWMO = size > 1;
            if(_mwmoChunk.hasWMO)
            {
                if(size > 256)
                    size = 256;
                _mwmoChunk.WMOName = (char*)malloc(size+1);
                WDT.read(_mwmoChunk.WMOName, size);
            }
        }
        else if (!strcmp(fourcc, "MODF"))
        {
            if(_mwmoChunk.hasWMO)
            {
                if(size > 64)
                    size = 64;
                WDT.read(&_modfChunk, size);
            }
        }
        WDT.seek((int)nextpos);
    }
}

void WDTFile::close()
{
    WDT.close();
}

ADTFile* WDTFile::GetADT()
{
    if(ADTTiles.empty())
        return NULL;

    std::pair<uint32, uint32> xY = ADTTiles.front();
    ADTTiles.pop_front();

    char buff[512];
    sprintf_s(buff, 512, "World\\Maps\\%s\\%s_%d_%d.adt", adtName.c_str(), adtName.c_str(), xY.first, xY.second);
    return new ADTFile(buff, MapId, xY.first, xY.second);
}

WMOInstance* WDTFile::GetWMO()
{
    if(!_mwmoChunk.hasWMO)
        return NULL;

    return new WMOInstance(MapId, WDT, _mwmoChunk.WMOName, _modfChunk);
}
