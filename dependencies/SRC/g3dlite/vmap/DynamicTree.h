/***
 * Demonstrike Core
 */

#pragma once

class GameobjectModelInstance;
struct DynTreeImpl;

class DynamicMapTree
{
    DynTreeImpl *impl;

public:

    DynamicMapTree();
    ~DynamicMapTree();

    bool isInLineOfSight(float x1, float y1, float z1, float x2, float y2,
                         float z2, G3D::g3d_uint32 phasemask) const;

    bool getIntersectionTime(G3D::g3d_uint32 phasemask, const G3D::Ray& ray,
                             const G3D::Vector3& endPos, float& maxDist) const;

    bool getObjectHitPos(G3D::g3d_uint32 phasemask, const G3D::Vector3& pPos1,
                         const G3D::Vector3& pPos2, G3D::Vector3& pResultHitPos,
                         float pModifyDist) const;

    float getHeight(float x, float y, float z, float maxSearchDist, G3D::g3d_uint32 phasemask) const;

    void insert(const VMAP::GameobjectModelInstance&);
    void remove(const VMAP::GameobjectModelInstance&);
    bool contains(const VMAP::GameobjectModelInstance&) const;
    int size() const;

    void balance();
    void update(G3D::g3d_uint32 diff);
};
