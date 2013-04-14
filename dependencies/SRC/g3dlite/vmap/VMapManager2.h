/***
 * Demonstrike Core
 */

#pragma once

//===========================================================

#define MAP_FILENAME_EXTENSION2 ".vmtree"

#define FILENAMEBUFFER_SIZE 500

/**
This is the main Class to manage loading and unloading of maps, line of sight, height calculation and so on.
For each map or map tile to load it reads a directory file that contains the ModelContainer files used by this map or map tile.
Each global map or instance has its own dynamic BSP-Tree.
The loaded ModelContainers are included in one of these BSP-Trees.
Additionally a table to match map ids and map names is used.
*/

//===========================================================

namespace VMAP
{
    enum VMAPLoadResult
    {
        VMAP_LOAD_RESULT_ERROR,
        VMAP_LOAD_RESULT_OK,
        VMAP_LOAD_RESULT_IGNORED,
    };

    #define VMAP_INVALID_HEIGHT        -100000.0f            // for check
    #define VMAP_MAX_HEIGHT             100000.0f            // for other checks

    class StaticMapTree;
    class WorldModel;

    class ManagedModel
    {
        public:
            ManagedModel(): iModel(0), iRefCount(0) {}
            void setModel(WorldModel *model) { iModel = model; }
            WorldModel *getModel() { return iModel; }
            void incRefCount() { ++iRefCount; }
            int decRefCount() { return --iRefCount; }
        protected:
            WorldModel *iModel;
            int iRefCount;
    };

    typedef std::map<G3D::g3d_uint32 , StaticMapTree *> InstanceTreeMap;
    typedef std::map<std::string, ManagedModel*> ModelFileMap;

    class VMapManager2
    {
        protected:
            // Tree to check collision
            ModelFileMap iLoadedModelFiles;
            InstanceTreeMap iInstanceMapTrees;
            // UNORDERED_MAP<unsigned int , bool> iMapsSplitIntoTiles;
            std::map<unsigned int , bool> iIgnoreMapIds;

            bool _loadMap(G3D::g3d_uint32 pMapId, const std::string &basePath, G3D::g3d_uint32 tileX, G3D::g3d_uint32 tileY);
            /* void _unloadMap(g3d_uint32 pMapId,G3D::g3d_uint32 x,G3D::g3d_uint32 y); */

        public:
            // public for debug
            G3D::Vector3 convertPositionToInternalRep(float x, float y, float z) const;
            G3D::Vector3 convertPositionToMangosRep(float x, float y, float z) const;
            static std::string getMapFileName(unsigned int pMapId);

            VMapManager2();
            ~VMapManager2(void);

            VMAPLoadResult loadMap(const char* pBasePath, unsigned int pMapId, int x, int y);

            void unloadMap(unsigned int pMapId, int x, int y);
            void unloadMap(unsigned int pMapId);

            bool isInLineOfSight(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2) ;
            /**
            fill the hit pos and return true, if an object was hit
            */
            bool getObjectHitPos(unsigned int pMapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float pModifyDist);
            float getHeight(unsigned int pMapId, float x, float y, float z, float maxSearchDist = 10.0f);
            G3D::g3d_uint32 GetVmapFlags(unsigned int mapid, float x, float y, float z);

            bool processCommand(char *pCommand) { return false; }        // for debug and extensions

            void preventMapsFromBeingUsed(const char* pMapIdString);
            bool getAreaInfo(unsigned int pMapId, float x, float y, float &z, G3D::g3d_uint32 &flags, G3D::g3d_int32 &adtId, G3D::g3d_int32 &rootId, G3D::g3d_int32 &groupId) const;
            bool GetLiquidLevel(G3D::g3d_uint32 pMapId, float x, float y, float z, G3D::g3d_uint8 ReqLiquidType, float &level, float &floor, G3D::g3d_uint32 &type) const;

            WorldModel* acquireModelInstance(const std::string &basepath, const std::string &filename);
            void releaseModelInstance(const std::string &filename);

            // what's the use of this? o.O
            virtual std::string getDirFileName(unsigned int pMapId, int x, int y) const
            {
                return getMapFileName(pMapId);
            }
            virtual bool existsMap(const char* pBasePath, unsigned int pMapId, int x, int y);

#ifdef MMAP_GENERATOR
        public:
            void getInstanceMapTree(InstanceTreeMap &instanceMapTree);
#endif
    };
}
