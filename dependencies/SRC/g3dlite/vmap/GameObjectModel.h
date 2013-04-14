/***
 * Demonstrike Core
 */

#pragma once

#include "../G3D.h"

namespace VMAP
{
    class WorldModel;
}

class GameObject;
struct GameObjectDisplayInfoEntry;

class GameObjectModel /*, public Intersectable*/
{
    G3D::AABox iBound;
    G3D::Matrix3 iInvRot;
    G3D::Vector3 iPos;
    G3D::g3d_int32 m_PhaseMask;
    //G3D::Vector3 iRot;
    float iInvScale;
    float iScale;
    VMAP::WorldModel* iModel;

    GameObjectModel() : iModel(NULL) {}
    bool initialize(G3D::g3d_uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, G3D::g3d_int32 phasemask);

public:
    std::string name;

    const G3D::AABox& getBounds() const { return iBound; }

    ~GameObjectModel();

    const G3D::Vector3& getPosition() const { return iPos;}

    bool intersectRay(const G3D::Ray& Ray, float& MaxDist, bool StopAtFirstHit, G3D::g3d_uint32 ph_mask) const;

    static GameObjectModel* Create(G3D::g3d_uint32 displayID, float scale, float posX, float posY, float posZ, float orientation, G3D::g3d_int32 phasemask);
};
