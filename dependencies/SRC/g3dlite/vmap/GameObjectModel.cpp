/***
 * Demonstrike Core
 */

#include "../G3DAll.h"

using G3D::Vector3;
using G3D::Ray;
using G3D::AABox;

struct GameobjectModelData
{
    GameobjectModelData(const std::string& name_, const AABox& box) :
        bound(box), name(name_) {}

    AABox bound;
    std::string name;
};

typedef std::map<G3D::g3d_uint32, GameobjectModelData> ModelList;
ModelList model_list;
std::string VmapDir = ".";

void VMAP::VMapManager2::LoadGameObjectModelList(std::string vMapDirectory)
{
    VmapDir = vMapDirectory;
    FILE* model_list_file = fopen((VmapDir + "vmaps/" + VMAP::GAMEOBJECT_MODELS).c_str(), "rb");
    if (!model_list_file)
    {
        G3D::G3D_Log::common()->printf("Unable to open '%s' file.", VMAP::GAMEOBJECT_MODELS);
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
            G3D::G3D_Log::common()->printf("File '%s' seems to be corrupted!", VMAP::GAMEOBJECT_MODELS);
            break;
        }

        model_list.insert
        (
            ModelList::value_type( displayId, GameobjectModelData(std::string(buff, name_length), AABox(v1, v2)) )
        );
    }

    fclose(model_list_file);
    G3D::G3D_Log::common()->printf(">> Loaded %u GameObject models", G3D::g3d_uint32(model_list.size()));
}

GameObjectModel::~GameObjectModel()
{
    if (iModel)
        ((VMAP::VMapManager2*)VMAP::VMapFactory::createOrGetVMapManager())->releaseModelInstance(name);
}

bool GameObjectModel::initialize(G3D::g3d_uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, G3D::g3d_int32 phasemask)
{
    ModelList::const_iterator it = model_list.find(displayID);
    if (it == model_list.end())
        return false;

    G3D::AABox mdl_box(it->second.bound);
    // ignore models with no bounds
    if (mdl_box == G3D::AABox::zero())
    {
        G3D::G3D_Log::common()->printf("GameObject model %s has zero bounds, loading skipped", it->second.name.c_str());
        return false;
    }

    iModel = ((VMAP::VMapManager2*)VMAP::VMapFactory::createOrGetVMapManager())->acquireModelInstance(std::string(".") + "vmaps/", it->second.name);
    if (!iModel)
        return false;

    name = it->second.name;
    //flags = VMAP::MOD_M2;
    //adtId = 0;
    //ID = 0;
    iPos = Vector3(posX, posY, posZ);
    m_PhaseMask = phasemask;
    iScale = scale;
    iInvScale = 1.f / iScale;

    G3D::Matrix3 iRotation = G3D::Matrix3::fromEulerAnglesZYX(orientation, 0, 0);
    iInvRot = iRotation.inverse();
    // transform bounding box:
    mdl_box = AABox(mdl_box.low() * iScale, mdl_box.high() * iScale);
    AABox rotated_bounds;
    for (int i = 0; i < 8; ++i)
        rotated_bounds.merge(iRotation * mdl_box.corner(i));

    this->iBound = rotated_bounds + iPos;
#ifdef SPAWN_CORNERS
    // test:
    for (int i = 0; i < 8; ++i)
    {
        Vector3 pos(iBound.corner(i));
        if (Creature* c = const_cast<GameObject&>(go).SummonCreature(24440, pos.x, pos.y, pos.z, 0, TEMPSUMMON_MANUAL_DESPAWN))
        {
            c->setFaction(35);
            c->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.1f);
        }
    }
#endif

    return true;
}

GameObjectModel* GameObjectModel::Create(G3D::g3d_uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, G3D::g3d_int32 phasemask)
{
    ModelList::const_iterator it = model_list.find(displayID);
    if (it == model_list.end())
        return NULL;

    GameObjectModel* mdl = new GameObjectModel();
    if (!mdl->initialize(displayID, scale, posX, posY, posZ, orientation, phasemask))
    {
        delete mdl;
        return NULL;
    }

    return mdl;
}

bool GameObjectModel::intersectRay(const G3D::Ray& ray, float& MaxDist, bool StopAtFirstHit, G3D::g3d_uint32 ph_mask) const
{
    if(m_PhaseMask != -1 && ph_mask != -1)
        if (!(m_PhaseMask & ph_mask))
            return false;

    float time = ray.intersectionTime(iBound);
    if (time == G3D::inf())
        return false;

    // child bounds are defined in object space:
    Vector3 p = iInvRot * (ray.origin() - iPos) * iInvScale;
    Ray modRay(p, iInvRot * ray.direction());
    float distance = MaxDist * iInvScale;
    bool hit = iModel->IntersectRay(modRay, distance, StopAtFirstHit);
    if (hit)
    {
        distance *= iScale;
        MaxDist = distance;
    }
    return hit;
}
