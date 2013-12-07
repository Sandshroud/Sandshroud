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
#include "adtfile.h"
#include "format.h"

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

ADTFile::ADTFile(char* filename, uint32 mapid, uint32 tilex, uint32 tiley): ADT(filename), MapId(mapid), TileX(tilex), TileY(tiley)
{
    Adtfilename.append(filename);
}

ADTFile::~ADTFile()
{
    ADT.close();
}

bool ADTFile::init()
{
    FILE *dirfile = NULL;
    if(ADT.isEof ())
        return false;

    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    fopen_s(&dirfile, dirname.c_str(), "ab");
    if(dirfile == NULL)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    uint32 size;
    string *WmoInstansName = NULL, *ModelInstansName = NULL;
    while (!ADT.isEof())
    {
        char fourcc[5];
        ADT.read(&fourcc, 4);
        ADT.read(&size, 4);
        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = ADT.getPos() + size;
        if (!strcmp(fourcc,"MHDR"))
        {
        }
        else if (!strcmp(fourcc,"MVER"))
        {
        }
        else if (!strcmp(fourcc,"MCIN"))
        {
        }
        else if (!strcmp(fourcc,"MTEX"))
        {
        }
        else if (!strcmp(fourcc,"MWID")) // offsets for strings in MWMO
        {
        }
        else if (!strcmp(fourcc,"MMID")) // offsets for strings in MMDX
        {
        }
        else if (!strcmp(fourcc,"MTEX"))
        {
        }
        else if (!strcmp(fourcc,"MTXF"))
        {
        }
        else if (!strcmp(fourcc,"MFBO"))
        {
        }
        else if (!strcmp(fourcc,"MMDX"))
        {
        }
        else if (!strcmp(fourcc,"MWMO"))
        {
        }
        else if (!strcmp(fourcc,"MDDF"))
        {
        }
        else if (!strcmp(fourcc,"MODF"))
        {
        }
        else if (!strcmp(fourcc,"MCNK"))
        {
        }
        else if (!strcmp(fourcc,"MH2O"))
        {
        }
        else
        {
            printf("Unknown Chunk type: %s\n", (char*)fourcc);
        }
        ADT.seek(nextpos);
    }

    //======================

    //======================
    ADT.close();
    fclose(dirfile);
    return true;
}
