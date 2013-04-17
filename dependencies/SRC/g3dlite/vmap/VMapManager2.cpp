/***
 * Demonstrike Core
 */

#include "../G3DAll.h"

#include <iomanip>

using G3D::Vector3;

namespace VMAP
{
    VMapManager2::VMapManager2(std::string vMapDir)
    {
        vmapDir = vMapDir+"/"; // FUUU
    }

    VMapManager2::~VMapManager2(void)
    {
        for (InstanceTreeMap::iterator i = iInstanceMapTrees.begin(); i != iInstanceMapTrees.end(); ++i)
        {
            delete i->second;
        }
        for (DynamicTreeMap::iterator i = iDynamicMapTrees.begin(); i != iDynamicMapTrees.end(); ++i)
        {
            delete i->second;
        }
        for (ModelFileMap::iterator i = iLoadedModelFiles.begin(); i != iLoadedModelFiles.end(); ++i)
        {
            delete i->second.getModel();
        }
    }

    Vector3 VMapManager2::convertPositionToInternalRep(float x, float y, float z) const
    {
        Vector3 pos;
        const float mid = 0.5f * 64.0f * 533.33333333f;
        pos.x = mid - x;
        pos.y = mid - y;
        pos.z = z;

        return pos;
    }

    // move to MapTree too?
    std::string VMapManager2::getMapFileName(unsigned int mapId)
    {
        std::stringstream fname;
        fname.width(3);
        fname << std::setfill('0') << mapId << std::string(MAP_FILENAME_EXTENSION2);

        return fname.str();
    }

    int VMapManager2::loadMap(unsigned int mapId, int x, int y)
    {
        int result = VMAP_LOAD_RESULT_IGNORED;
        if (_loadMap(mapId, x, y))
            result = VMAP_LOAD_RESULT_OK;
        else
            result = VMAP_LOAD_RESULT_ERROR;

        return result;
    }

    // load one tile (internal use only)
    bool VMapManager2::_loadMap(unsigned int mapId, G3D::g3d_uint32 tileX, G3D::g3d_uint32 tileY)
    {
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree == iInstanceMapTrees.end())
        {
            std::string mapFileName = getMapFileName(mapId);
            StaticMapTree* newTree = new StaticMapTree(mapId, vmapDir);
            if (!newTree->InitMap(mapFileName, this))
            {
                delete newTree;
                return false;
            }
            instanceTree = iInstanceMapTrees.insert(InstanceTreeMap::value_type(mapId, newTree)).first;
        }

        return instanceTree->second->LoadMapTile(tileX, tileY, this);
    }

    // load one tile (internal use only)
    int VMapManager2::loadObject(G3D::g3d_uint64 guid, unsigned int mapId, G3D::g3d_uint32 DisplayID, float scale, float x, float y, float z, float o, G3D::g3d_int32 m_phase)
    {
        int result = VMAP_LOAD_RESULT_IGNORED;
        DynamicTreeMap::iterator DIT = iDynamicMapTrees.find(mapId);
        if (DIT == iDynamicMapTrees.end())
        {
            DynamicMapTree* Tree = new DynamicMapTree();
            DIT = iDynamicMapTrees.insert(DynamicTreeMap::value_type(mapId, Tree)).first;
        }
        if(DIT->second == NULL)
            return result; // Shouldn't happen.

        if(_loadObject(DIT->second, guid, mapId, DisplayID, scale, x, y, z, o, m_phase))
            result = VMAP_LOAD_RESULT_OK;
        else
            result = VMAP_LOAD_RESULT_ERROR;
        return result;
    }

    // Load our object into our dynamic tree(Internal only please!)
    bool VMapManager2::_loadObject(DynamicMapTree* tree, G3D::g3d_uint64 guid, unsigned int mapId, G3D::g3d_uint32 DisplayID, float scale, float x, float y, float z, float o, G3D::g3d_int32 m_phase)
    {
        GOModelSpawnList::const_iterator it = GOModelList.find(DisplayID);
        if (it == GOModelList.end())
            return false;

        G3D::AABox mdl_box(it->second.BoundBase);
        if (mdl_box == G3D::AABox::zero())
        {
            // ignore models with no bounds
#ifdef _DEBUG
            printf("GameObject model %s has zero bounds, loading skipped\n", it->second.name.c_str());
#endif
            return false;
        }

        WorldModel* model = acquireModelInstance(it->second.name);
        if(model == NULL)
            return false; // Shouldn't happen.
        GameobjectModelInstance* Instance = new GameobjectModelInstance(it->second, model, m_phase);
        if(Instance == NULL)
        {
            releaseModelInstance(it->second.name);
            return false; // Shouldn't happen.
        }

        Instance->SetData(mdl_box, x, y, z, o, scale);
        GOMapGuides* guides = NULL;
        GOModelInstanceByGUID::iterator itr = GOModelTracker.find(mapId);
        if(GOModelTracker.find(mapId) == GOModelTracker.end())
        {
            guides = new GOMapGuides();
            GOModelTracker.insert(GOModelInstanceByGUID::value_type(mapId, guides));
        }
        else
            guides = GOModelTracker.at(mapId);

        guides->ModelsByGuid.insert(ModelGUIDEs::value_type(guid, Instance));
        tree->insert(*Instance);
        return true;
    }

    void VMapManager2::changeObjectModel(G3D::g3d_uint64 guid, unsigned int mapId, G3D::g3d_uint32 DisplayID)
    {
        DynamicTreeMap::iterator DIT = iDynamicMapTrees.find(mapId);
        if (DIT == iDynamicMapTrees.end())
            return;
        GOModelInstanceByGUID::iterator itr = GOModelTracker.find(mapId);
        if(itr == GOModelTracker.end())
            return;

        GOMapGuides* guides = GOModelTracker.at(mapId);
        ModelGUIDEs::iterator itr2 = guides->ModelsByGuid.find(guid);
        if(itr2 != guides->ModelsByGuid.end())
        {
            GameobjectModelInstance *Instance = itr2->second;
            GOModelSpawnList::const_iterator it = GOModelList.find(DisplayID);
            if (it == GOModelList.end())
            {
                DIT->second->remove(*Instance);
                Instance->setUnloaded();
                DIT->second->insert(*Instance);
                return;
            }
            else
            {
                G3D::AABox mdl_box(it->second.BoundBase);
                if (mdl_box == G3D::AABox::zero())
                {
                    DIT->second->remove(*Instance);
                    Instance->setUnloaded();
                    DIT->second->insert(*Instance);
                    return;
                }

                WorldModel* model = acquireModelInstance(it->second.name);
                if(model == NULL)
                {
                    DIT->second->remove(*Instance);
                    Instance->setUnloaded();
                    DIT->second->insert(*Instance);
                    return;
                }

                DIT->second->remove(*Instance);
                G3D::Vector3 pos = Instance->getPosition();
                Instance->LoadModel(model, mdl_box);
                Instance->SetData(mdl_box, pos.x, pos.y, pos.z, Instance->GetOrientation(), Instance->GetScale());
                DIT->second->insert(*Instance);
            }
        }
    }

    void VMapManager2::unloadObject(unsigned int mapId, G3D::g3d_uint64 guid)
    {
        GOModelInstanceByGUID::iterator itr = GOModelTracker.find(mapId);
        if(itr == GOModelTracker.end())
            return;
        GOMapGuides* guides = GOModelTracker.at(mapId);
        DynamicTreeMap::iterator DynamicTree = iDynamicMapTrees.find(mapId);
        if (DynamicTree != iDynamicMapTrees.end())
        {
            if(guides->ModelsByGuid.find(guid) != guides->ModelsByGuid.end())
            {
                GameobjectModelInstance* Instance = guides->ModelsByGuid.at(guid);
                DynamicTree->second->remove(*Instance);
                releaseModelInstance(Instance->name);
                guides->ModelsByGuid.erase(guid);
                delete Instance;
            }
        }
        if(guides->ModelsByGuid.size() == 0)
        {
            GOModelTracker.erase(mapId);
            delete guides;
        }
    }

    void VMapManager2::unloadMap(unsigned int mapId)
    {
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            instanceTree->second->UnloadMap(this);
            if (instanceTree->second->numLoadedTiles() == 0)
            {
                delete instanceTree->second;
                iInstanceMapTrees.erase(mapId);
            }
        }
    }

    void VMapManager2::unloadMap(unsigned int mapId, int x, int y)
    {
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            instanceTree->second->UnloadMapTile(x, y, this);
            if (instanceTree->second->numLoadedTiles() == 0)
            {
                delete instanceTree->second;
                iInstanceMapTrees.erase(mapId);
            }
        }
    }

    bool VMapManager2::isInLineOfSight(unsigned int mapId, G3D::g3d_int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2)
    {
		bool result = true;
        if(x1 == x2 && y1 == y2 && z1 == z2)
            return result; // Save us some time.
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            Vector3 pos1 = convertPositionToInternalRep(x1, y1, z1);
            Vector3 pos2 = convertPositionToInternalRep(x2, y2, z2);
            result = instanceTree->second->isInLineOfSight(pos1, pos2);
        }

        if(result)
        {
            DynamicTreeMap::iterator DynamicTree = iDynamicMapTrees.find(mapId);
            if (DynamicTree != iDynamicMapTrees.end())
                result = DynamicTree->second->isInLineOfSight(x1, y1, z1, x2, y2, z2, m_phase);
        }

        return result;
    }

    g3d_uint32 VMapManager2::GetVmapFlags(unsigned int mapid, float x, float y, float z)
    {
        g3d_uint32 result;
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapid);
        if (instanceTree != iInstanceMapTrees.end())
        {
            Vector3 position = convertPositionToInternalRep(x,y,z);
            result = instanceTree->second->GetVmapFlags(position);
        }
        return result;
    }

    /**
    get the hit position and return true if we hit something
    otherwise the result pos will be the dest pos
    */
    bool VMapManager2::getObjectHitPos(unsigned int mapId, G3D::g3d_int32 m_phase, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float &ry, float& rz, float modifyDist)
    {
        bool result = false;
        rx = x2;
        ry = y2;
        rz = z2;
        if(x1 == x2 && y1 == y2 && z1 == z2)
            return result; // Save us some time.

        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            Vector3 pos1 = convertPositionToInternalRep(x1, y1, z1);
            Vector3 pos2 = convertPositionToInternalRep(x2, y2, z2);
            Vector3 resultPos;
            bool result = instanceTree->second->getObjectHitPos(pos1, pos2, resultPos, modifyDist);
            resultPos = convertPositionToInternalRep(resultPos.x, resultPos.y, resultPos.z);
            rx = resultPos.x;
            ry = resultPos.y;
            rz = resultPos.z;
        }

        if(!result)
        {
            DynamicTreeMap::iterator DynamicTree = iDynamicMapTrees.find(mapId);
            if (DynamicTree != iDynamicMapTrees.end())
            {
                Vector3 pos1 = Vector3(x1, y1, z1);
                Vector3 pos2 = Vector3(x2, y2, z2);
                Vector3 resultPos;
                bool result = DynamicTree->second->getObjectHitPos(m_phase, pos1, pos2, resultPos, modifyDist);
                resultPos = convertPositionToInternalRep(resultPos.x, resultPos.y, resultPos.z);
                rx = resultPos.x;
                ry = resultPos.y;
                rz = resultPos.z;
            }
        }

        return result;
    }

    /**
    get height or INVALID_HEIGHT if no height available
    */

    float VMapManager2::getHeight(unsigned int mapId, G3D::g3d_int32 m_phase, float x, float y, float z, float maxSearchDist)
    {
        float height = VMAP_INVALID_HEIGHT;
        InstanceTreeMap::iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            Vector3 pos = convertPositionToInternalRep(x, y, z);
            float height2 = instanceTree->second->getHeight(pos, maxSearchDist);
            if (!G3D::fuzzyEq(height2, G3D::inf()))
                height = height2; // No height
        }

        DynamicTreeMap::iterator DynamicTree = iDynamicMapTrees.find(mapId);
        if (DynamicTree != iDynamicMapTrees.end())
        {
            float height2 = DynamicTree->second->getHeight(x, y, z, maxSearchDist, m_phase);
            if (!G3D::fuzzyEq(height2, G3D::inf()))
                height = height2; // No height
        }

        return height;
    }

    bool VMapManager2::getAreaInfo(unsigned int mapId, float x, float y, float& z, G3D::g3d_uint32& flags, G3D::g3d_int32& adtId, G3D::g3d_int32& rootId, G3D::g3d_int32& groupId) const
    {
        InstanceTreeMap::const_iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            Vector3 pos = convertPositionToInternalRep(x, y, z);
            bool result = instanceTree->second->getAreaInfo(pos, flags, adtId, rootId, groupId);
            // z is not touched by convertPositionToInternalRep(), so just copy
            z = pos.z;
            return result;
        }

        return false;
    }

    bool VMapManager2::GetLiquidLevel(G3D::g3d_uint32 mapId, float x, float y, float z, G3D::g3d_uint8 reqLiquidType, float& level, float& floor, G3D::g3d_uint32& type) const
    {
        InstanceTreeMap::const_iterator instanceTree = iInstanceMapTrees.find(mapId);
        if (instanceTree != iInstanceMapTrees.end())
        {
            LocationInfo info;
            Vector3 pos = convertPositionToInternalRep(x, y, z);
            if (instanceTree->second->GetLocationInfo(pos, info))
            {
                floor = info.ground_Z;
                assert(floor < std::numeric_limits<float>::max());
                type = info.hitModel->GetLiquidType();  // entry from LiquidType.dbc
//                if (reqLiquidType && !(GetLiquidFlags(type) & reqLiquidType))
//                    return false;
                if (info.hitInstance->GetLiquidLevel(pos, info, level))
                    return true;
            }
        }

        return false;
    }

    WorldModel* VMapManager2::acquireModelInstance(const std::string& filename)
    {
        //! Critical section, thread safe access to iLoadedModelFiles
        LoadedModelFilesLock.lock();

        ModelFileMap::iterator model = iLoadedModelFiles.find(filename);
        if (model == iLoadedModelFiles.end())
        {
            WorldModel* worldmodel = new WorldModel();
            if (!worldmodel->readFile(vmapDir + filename + ".vmo"))
            {
                LoadedModelFilesLock.unlock();
#ifdef _DEBUG
                printf("VMapManager2: could not load '%s%s.vmo'!\n", vmapDir.c_str(), filename.c_str());
#endif
                delete worldmodel;
                return NULL;
            }

#ifdef _DEBUG
            printf("VMapManager2: loading file '%s%s'\n", vmapDir.c_str(), filename.c_str());
#endif
            model = iLoadedModelFiles.insert(std::pair<std::string, ManagedModel>(filename, ManagedModel())).first;
            model->second.setModel(worldmodel);
        }
        model->second.incRefCount();
        LoadedModelFilesLock.unlock();
        return model->second.getModel();
    }

    void VMapManager2::releaseModelInstance(const std::string &filename)
    {
        //! Critical section, thread safe access to iLoadedModelFiles
        LoadedModelFilesLock.lock();

        ModelFileMap::iterator model = iLoadedModelFiles.find(filename);
        if (model == iLoadedModelFiles.end())
        {
            LoadedModelFilesLock.unlock();
#ifdef _DEBUG
            printf("VMapManager2: trying to unload non-loaded file '%s'\n", filename.c_str());
#endif
            return;
        }

        if (model->second.decRefCount() == 0)
        {
#ifdef _DEBUG
            printf("VMapManager2: unloading file '%s'\n", filename.c_str());
#endif
            delete model->second.getModel();
            iLoadedModelFiles.erase(model);
        }
        LoadedModelFilesLock.unlock();
    }

    bool VMapManager2::existsMap(const char* basePath, unsigned int mapId, int x, int y)
    {
        return StaticMapTree::CanLoadMap(std::string(basePath), mapId, x, y);
    }

    void VMapManager2::updateDynamicMapTrees(G3D::g3d_uint32 t_diff)
    {
        for(DynamicTreeMap::iterator itr = iDynamicMapTrees.begin(); itr != iDynamicMapTrees.end(); itr++)
            itr->second->update(t_diff);
    }

    void VMapManager2::updateDynamicMapTree(G3D::g3d_uint32 mapid, G3D::g3d_uint32 t_diff)
    {
        DynamicTreeMap::iterator DynamicTree = iDynamicMapTrees.find(mapid);
        if (DynamicTree != iDynamicMapTrees.end())
            DynamicTree->second->update(t_diff);
    }

    void VMapManager2::LoadGameObjectModelList()
    {
        FILE* model_list_file = fopen((vmapDir + VMAP::GAMEOBJECT_MODELS).c_str(), "rb");
        if (!model_list_file)
        {
#ifdef _DEBUG
            printf("Unable to open '%s' file.\n", VMAP::GAMEOBJECT_MODELS);
#endif
            return;
        }

        G3D::g3d_uint32 name_length, displayId;
        char buff[500];
        while (true)
        {
            Vector3 v1, v2;
            if (fread(&displayId, sizeof(G3D::g3d_uint32), 1, model_list_file) != 1)
                if (feof(model_list_file))  // EOF flag is only set after failed reading attempt
                    break;

            if (fread(&name_length, sizeof(G3D::g3d_uint32), 1, model_list_file) != 1
                || name_length >= sizeof(buff)
                || fread(&buff, sizeof(char), name_length, model_list_file) != name_length
                || fread(&v1, sizeof(Vector3), 1, model_list_file) != 1
                || fread(&v2, sizeof(Vector3), 1, model_list_file) != 1)
            {
#ifdef _DEBUG
                printf("File '%s' seems to be corrupted!\n", VMAP::GAMEOBJECT_MODELS);
#endif
                break;
            }

            GameobjectModelSpawn iModel;
            iModel.name = std::string(buff, name_length);
            iModel.BoundBase = AABox(v1, v2);
            GOModelList.insert(GOModelSpawnList::value_type(displayId, iModel));
        }

        fclose(model_list_file);
#ifdef _DEBUG
        printf(">> Loaded %u GameObject models\n", G3D::g3d_uint32(GOModelList.size()));
#endif
    }

} // namespace VMAP
