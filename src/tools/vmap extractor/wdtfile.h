#ifndef WDTFILE_H
#define WDTFILE_H

#include "mpq_libmpq04.h"
#include "wmo.h"
#include <string>
#include "stdlib.h"

class ADTFile;

enum MPHD_flags
{
    MPHD_FLAG_ADT_BASED = 0x01, //    Use global map object definition.
    MPHD_FLAG_WMO_BASED = 0x02, //    CMapRenderChunk::SetVertexFormat( 2 ); (else: 1) -- Use vertex shading (ADT.MCNK.MCCV)
    MPHD_F_3 = 0x04, //    Decides whether to use _env terrain shaders or not: funky and if MCAL has 4096 instead of 2048(?)
    MPHD_F_4 = 0x08, //    Disables something. No idea what. Another rendering thing. Someone may check all them in wild life..
    MPHD_F_5 = 0x10, //    if( flags & 0x02 && CMap::enableTerrainShaderVertex ) CMapRenderChunk::SetVertexFormat( 16 );  -- Cataclysm : Use vertex lighting (ADT.MCNK.MCLV)
    MPHD_F_6 = 0x20  //    Flips the ground display upside down to create a ceiling (Cataclysm)
};

struct WDT_MVER
{
    uint32 version;
};

struct WDT_MPHD
{
    uint32 MWMOflags;
    uint32 DataOffset;
    uint32 unk[6];
};

struct WDT_MAIN
{
    struct
    {
        uint32 m_flags;
        uint32 buffer;
    }m_chunkData[64][64];
};

struct WDT_MWMO
{
    bool hasWMO;
    char *WMOName;
};

struct WDT_MODF
{
    uint32 NameID;      // Index for MWID
    uint32 AdtID;       // Unique WMO ID
    float Pos[3];       // Position(X,Y,Z)
    float Rot[3];       // Rotation(A,B,C)
    float UBnd[3];      // Upper boundaries
    float LBnd[3];      // Lower Boundaries
    uint16 MODFFlags;   // WMO placement flags
    uint16 DoodadSetId; // Doodad set index
    uint16 WMOSetId;    // Unsure
    char padding[2];    // file padding
};

class WDTFile
{
private:
    MPQFile WDT;

private:
    string filename, adtName, MapEntry;
    uint32 MapId;

    std::deque< std::pair<uint32, uint32> > ADTTiles;

public:
    WDT_MVER _mverChunk;
    WDT_MPHD _mphdChunk;
    WDT_MAIN _mainChunk;
    WDT_MWMO _mwmoChunk;
    WDT_MODF _modfChunk;

public:
    WDTFile(char* file_name, char* adtname, char *map_id, uint32 mapID);
    ~WDTFile(void);

    bool init();
    void readandprocess();
    void close();

    ADTFile* GetADT();
    WMOInstance* GetWMO();

    bool hasWMO() { return _mwmoChunk.hasWMO; }
    uint32 getADTCount() { return ADTTiles.size(); }
};

#endif
