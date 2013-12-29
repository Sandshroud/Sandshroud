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

WDTFile::WDTFile(char* file_name, char* file_name1) : WDT(file_name), gWmoInstansName(NULL), gnWMO(0), wdtFlags(0)
{
    filename.append(file_name1,strlen(file_name1));
}

bool WDTFile::init(char* /*map_id*/, unsigned int mapID)
{
    if (WDT.isEof())
    {
        //printf("Can't find WDT file.\n");
        return false;
    }

    char fourcc[5];
    uint32 size;

    std::string dirname = std::string(szWorkDirWmo) + "/dir_bin";
    FILE *dirfile;
    dirfile = fopen(dirname.c_str(), "ab");
    if(!dirfile)
    {
        printf("Can't open dirfile!'%s'\n", dirname.c_str());
        return false;
    }

    while (!WDT.isEof())
    {
        WDT.read(fourcc,4);
        WDT.read(&size, 4);

        flipcc(fourcc);
        fourcc[4] = 0;

        size_t nextpos = WDT.getPos() + size;

        if(!strcmp(fourcc, "MPHD"))
        {
            WDT.read(&wdtFlags, 4);
            //WDT.read(&wdtMainOffset, 4); Not used
        }
        else if (!strcmp(fourcc,"MAIN"))
        {
            // MAIN is used to define what ADT is available on what tiles, areaflag & 0x01 means adt exists, but the other field is unknown
            // No flag other than 0x01 exists in wotlk
            /*uint64 mainData[4096], lastValue = 0xFFFFFFFFFFFFFFFF;
            WDT.read(&mainData, size);
            for(uint32 x = 0; x < 64; x++)
            {
                for(uint32 y = 0; y < 64; y++)
                {
                    uint32 areaFlag = uint32(mainData[x*64+y]), m_unk = uint32(mainData[x*64+y] >> 32);
                    if(areaFlag == 0 && m_unk == 0)
                        continue;
                    printf("[%u|%u|AF:%u|UNK:%u]\r", x, y, areaFlag, m_unk);
                    if(lastValue != 0xFFFFFFFFFFFFFFFF && lastValue != mainData[x*64+y])
                        printf("\n");
                    lastValue = mainData[x*64+y];
                }
            }
            printf("Done parsing MAIN flags\r\n");*/
        }
        else if (!strcmp(fourcc,"MWMO"))
        {
            // global map objects
            if (size)
            {
                char *buf = new char[size];
                WDT.read(buf, size);
                char *p=buf;
                int q = 0;
                gWmoInstansName = new string[size];
                while (p < buf + size)
                {
                    char* s=wdtGetPlainName(p);
                    fixnamen(s,strlen(s));
                    p=p+strlen(p)+1;
                    gWmoInstansName[q++] = s;
                }
                delete[] buf;
            }
        }
        else if (!strcmp(fourcc, "MODF"))
        {
            // global wmo instance data
            if (size)
            {
                gnWMO = (int)size / 64;

                for (int i = 0; i < gnWMO; ++i)
                {
                    int id;
                    WDT.read(&id, 4);
                    WMOInstance inst(WDT,gWmoInstansName[id].c_str(), mapID, 65, 65, dirfile);
                }

                delete[] gWmoInstansName;
            }
        }
        WDT.seek((int)nextpos);
    }

    WDT.close();
    fclose(dirfile);
    return true;
}

WDTFile::~WDTFile(void)
{
    WDT.close();
}

ADTFile* WDTFile::GetMap(int x, int z)
{
    if(!(x>=0 && z >= 0 && x<64 && z<64))
        return NULL;

    char name[512];

    sprintf(name,"World\\Maps\\%s\\%s_%d_%d.adt", filename.c_str(), filename.c_str(), x, z);
    return new ADTFile(name);
}
