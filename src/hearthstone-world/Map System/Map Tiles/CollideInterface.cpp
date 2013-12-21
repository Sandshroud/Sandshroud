/***
 * Demonstrike Core
 */

#include "StdAfx.h"

struct CollisionMap
{
    uint32 m_loadCount;
    uint32 m_tileLoadCount[64][64];
    Mutex m_lock;
};

SERVER_DECL CCollideInterface CollideInterface;
VMAP::VMapManager* CollisionMgr;
CollisionMap *m_mapLocks[NUM_MAPS];
Mutex m_mapCreateLock;

void CCollideInterface::Init()
{
    sLog.Notice("CollideInterface", "Init");
    CollisionMgr = new VMAP::VMapManager(sWorld.vMapPath);
    for(uint32 i = 0; i < NUM_MAPS; i++)
        m_mapLocks[i] = NULL;
    CollisionMgr->LoadGameObjectModelList();
}

void CCollideInterface::UpdateAllMaps(uint32 p_time)
{
    if( !CollisionMgr )
        return;

    CollisionMgr->updateDynamicMapTrees(p_time);
}

void CCollideInterface::UpdateSingleMap(uint32 mapId, uint32 p_time)
{
    if( !CollisionMgr )
        return;

    CollisionMgr->updateDynamicMapTree(mapId, p_time);
}

void CCollideInterface::ActivateMap(uint32 mapId)
{
    if( !CollisionMgr )
        return;

    m_mapCreateLock.Acquire();
    if( m_mapLocks[mapId] == NULL )
    {
        m_mapLocks[mapId] = new CollisionMap();
        m_mapLocks[mapId]->m_loadCount = 1;
        memset(&m_mapLocks[mapId]->m_tileLoadCount, 0, sizeof(uint32)*64*64);
    }
    else
        m_mapLocks[mapId]->m_loadCount++;

    m_mapCreateLock.Release();
}

void CCollideInterface::DeactivateMap(uint32 mapId)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return;

    m_mapCreateLock.Acquire();
    --m_mapLocks[mapId]->m_loadCount;
    if( m_mapLocks[mapId]->m_loadCount == 0 )
    {
        // no instances using this anymore
        delete m_mapLocks[mapId];
        CollisionMgr->unloadMap(mapId);
        m_mapLocks[mapId] = NULL;
    }
    m_mapCreateLock.Release();
}

bool CCollideInterface::ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return false;

    // acquire write lock
    m_mapLocks[mapId]->m_lock.Acquire();
    if( m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY] == 0 )
    {
        if(CollisionMgr->loadMap(mapId, tileX, tileY))
            sLog.outDebug("Loading VMap [%u/%u] successful", tileX, tileY);
        else
        {
            sLog.outDebug("Loading VMap [%u/%u] unsuccessful", tileX, tileY);
            m_mapLocks[mapId]->m_lock.Release();
            return false;
        }
    }

    // increment count
    m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY]++;

    // release lock
    m_mapLocks[mapId]->m_lock.Release();
    return true;
}

void CCollideInterface::DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return;

    // get write lock
    m_mapLocks[mapId]->m_lock.Acquire();
    if( (--m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY]) == 0 )
        CollisionMgr->unloadMap(mapId, tileX, tileY);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();
}

bool CCollideInterface::IsActiveTile(uint32 mapId, uint32 tileX, uint32 tileY)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return false;

    bool isactive = false;

    // acquire write lock
    m_mapLocks[mapId]->m_lock.Acquire();
    if(m_mapLocks[mapId]->m_tileLoadCount[tileX][tileY])
        isactive = true;
    m_mapLocks[mapId]->m_lock.Release(); // release lock

    return isactive;
}

bool CCollideInterface::CheckLOS(uint32 mapId, uint32 instanceId, int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return true;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    // get data
    bool res = CollisionMgr ? CollisionMgr->isInLineOfSight(mapId, instanceId, m_phase, x1, y1, z1, x2, y2, z2) : true;

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    // return
    return res;
}

bool CCollideInterface::GetFirstPoint(uint32 mapId, uint32 instanceId, int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return false;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    // get data
    bool res = (CollisionMgr ? CollisionMgr->getObjectHitPos(mapId, instanceId, m_phase, x1, y1, z1, x2, y2, z2, outx, outy, outz, distmod) : false);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    // return
    return res;
}

float CCollideInterface::GetHeight(uint32 mapId, uint32 instanceId, int32 m_phase, float x, float y, float z)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return NO_WMO_HEIGHT;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    // get data
    float res = CollisionMgr ? CollisionMgr->getHeight(mapId, instanceId, m_phase, x, y, z, 10.0f) : NO_WMO_HEIGHT;

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    // return
    return res;
}

// DBC ids are stored in vmaps currently, so convert to terrain water flags
// We could use DBC file, but this workaround is sufficient.
uint16 convertWaterIDToFlags(uint16 wmoType)
{
    switch(wmoType)
    {
        // Mask these to Regular Water
    case 1: case 5: case 9: case 13: case 17:
    case 41: case 61: case 81: case 181:
        return 0x01;
        // Mask these to Ocean Water
    case 2: case 6: case 10:
    case 14: case 100:
        return 0x02;
        // Mask these to Regular Magma
    case 3: case 7: case 11: case 15:
    case 19: case 121: case 141:
        return 0x04;
        // Mask these to Regular Slime
    case 4: case 8: case 12:
    case 20: case 21:
        return 0x08;
    }
    return 0;
}

float CCollideInterface::GetWaterHeight(uint32 mapId, float x, float y, float z, uint16 &outType)
{
    float res = NO_WMO_HEIGHT;
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return res;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    // get data
    uint16 waterDBCId = 0;
    CollisionMgr->GetLiquidData(mapId, x, y, z, waterDBCId, res);
    if(waterDBCId)
        outType = convertWaterIDToFlags(waterDBCId);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    // return
    return res;
}

/* Crow: Systematic calculations based on Mangos, a big thank you to them! */
bool CCollideInterface::IsIndoor(uint32 mapId, float x, float y, float z)
{
    bool res = false;
    ASSERT(m_mapLocks[mapId] != NULL);
    if(!CollisionMgr)
        return res;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    uint32 flags = 0;
    int32 adtId = 0, rootId = 0, groupid = 0;
    if(CollisionMgr->getAreaInfo(mapId, x, y, z, flags, adtId, rootId, groupid))
    {
        WMOAreaTableEntry * WMOEntry = GetWorldMapOverlayEntry(adtId, rootId, groupid);
        if(WMOEntry != NULL)
        {
            AreaTable* ate = dbcArea.LookupEntry(WMOEntry->adtId);
            if(ate != NULL)
            {
                if((ate->AreaFlags & AREA_INSIDE) && !(ate->AreaFlags & AREA_OUTSIDE))
                    res = true;
            }
            res = res || ((WMOEntry->Flags & 2) && !(WMOEntry->Flags & 4));
        }

        res = res || ((flags & VA_FLAG_INDOORS) &&
            !(flags & VA_FLAG_OUTSIDE) &&
            !(flags & VA_FLAG_IN_CITY) &&
            !(flags & VA_FLAG_IN_CITY2) &&
            !(flags & VA_FLAG_IN_CITY3));
    }

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    return res;
}

bool CCollideInterface::IsIncity(uint32 mapId, float x, float y, float z)
{
    bool res = false;
    ASSERT(m_mapLocks[mapId] != NULL);
    if(!CollisionMgr)
        return res;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    uint32 flags = 0;
    int32 adtId = 0, rootId = 0, groupid = 0;
    if(CollisionMgr->getAreaInfo(mapId, x, y, z, flags, adtId, rootId, groupid))
    {
        WMOAreaTableEntry * WMOEntry = GetWorldMapOverlayEntry(adtId, rootId, groupid);
        if(WMOEntry != NULL)
        {
            AreaTable* ate = dbcArea.LookupEntry(WMOEntry->areaId);
            if(ate != NULL)
            {
                if(ate->AreaFlags & AREA_CITY_AREA || ate->AreaFlags & AREA_CITY)
                    res = true;
            }
        }

        if((flags & VA_FLAG_IN_CITY) || (flags & VA_FLAG_IN_CITY2) || (flags & VA_FLAG_IN_CITY3))
            res = true;
    }

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    return res;
}

uint16 CCollideInterface::GetAreaID(uint32 mapId, float x, float y, float z)
{
    uint16 res = 0xFFFF;
    ASSERT(m_mapLocks[mapId] != NULL);
    if(!CollisionMgr)
        return res;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    uint32 flags = 0;
    int32 adtId = 0, rootId = 0, groupid = 0;
    if(CollisionMgr->getAreaInfo(mapId, x, y, z, flags, adtId, rootId, groupid))
    {
        WMOAreaTableEntry * WMOEntry = GetWorldMapOverlayEntry(adtId, rootId, groupid);
        if(WMOEntry != NULL)
            res = WMOEntry->areaId;
    }

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    return res;
}

uint32 CCollideInterface::GetVmapAreaFlags(uint32 mapId, float x, float y, float z)
{
    ASSERT(m_mapLocks[mapId] != NULL);
    if( !CollisionMgr )
        return 0;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    // get data
    uint32 flags = CollisionMgr ? CollisionMgr->GetVmapFlags(mapId, x, y, z) : 0;

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();

    // return
    return flags;
}

void CCollideInterface::LoadGameobjectModel(uint64 Guid, uint32 mapId, uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, uint32 instanceId, int32 phasemask)
{
    if( !CollisionMgr )
        return;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    CollisionMgr->loadObject(Guid, mapId, displayID, scale, posX, posY, posZ, orientation, instanceId, phasemask);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();
}

void CCollideInterface::UpdateObjectModel(uint64 Guid, uint32 mapId, uint32 instanceId, uint32 displayID)
{
    if( !CollisionMgr )
        return;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    CollisionMgr->changeObjectModel(Guid, mapId, instanceId, displayID);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();
}

void CCollideInterface::UnLoadGameobjectModel(uint64 Guid, uint32 instanceId, uint32 mapId)
{
    if( !CollisionMgr )
        return;

    // get read lock
    m_mapLocks[mapId]->m_lock.Acquire();

    CollisionMgr->unloadObject(mapId, instanceId, Guid);

    // release write lock
    m_mapLocks[mapId]->m_lock.Release();
}

void CCollideInterface::DeInit()
{
    // bleh.
}
