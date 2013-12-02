/***
 * Demonstrike Core
 */

#pragma once

/* imports */
#define NO_WMO_HEIGHT -100000.0f
#define WMO_MAX_HEIGHT 100000.0f

extern VMAP::VMapManager* CollisionMgr;

class SERVER_DECL CCollideInterface
{
public:
	void Init();
	void DeInit();
	void UpdateAllMaps(uint32 p_time);
	void UpdateSingleMap(uint32 mapId, uint32 p_time);

	bool ActivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void DeactivateTile(uint32 mapId, uint32 tileX, uint32 tileY);
	bool IsActiveTile(uint32 mapId, uint32 tileX, uint32 tileY);
	void ActivateMap(uint32 mapId);
	void DeactivateMap(uint32 mapId);

	bool CheckLOS(uint32 mapId, uint32 instanceId, int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2);
	bool GetFirstPoint(uint32 mapId, uint32 instanceId, int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2, float & outx, float & outy, float & outz, float distmod);
	bool IsIndoor(uint32 mapId, float x, float y, float z);
	bool IsIncity(uint32 mapid, float x, float y, float z);
	uint32 GetVmapAreaFlags(uint32 mapId, float x, float y, float z);
	float GetHeight(uint32 mapId, uint32 instanceId, int32 m_phase, float x, float y, float z);

	void LoadGameobjectModel(uint64 Guid, uint32 mapId, uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, uint32 instanceId, int32 phasemask);
	void UpdateObjectModel(uint64 Guid, uint32 mapId, uint32 instanceId, uint32 displayID);
	void UnLoadGameobjectModel(uint64 Guid, uint32 mapId, uint32 instanceId);
};

extern SERVER_DECL CCollideInterface CollideInterface;
