/***
 * Demonstrike Core
 */

#include "../G3DAll.h"

using VMAP::ModelInstance;

namespace {

int CHECK_TREE_PERIOD = 200;

} // namespace

template<> struct HashTrait< VMAP::GameobjectModelInstance>{
    static size_t hashCode(const VMAP::GameobjectModelInstance& g) { return (size_t)(void*)&g; }
};

template<> struct PositionTrait< VMAP::GameobjectModelInstance> {
    static void getPosition(const VMAP::GameobjectModelInstance& g, G3D::Vector3& p) { p = g.getPosition(); }
};

template<> struct BoundsTrait< VMAP::GameobjectModelInstance> {
    static void getBounds(const VMAP::GameobjectModelInstance& g, G3D::AABox& out) { out = g.getBounds();}
    static void getBounds2(const VMAP::GameobjectModelInstance* g, G3D::AABox& out) { out = g->getBounds();}
};

template<class Node>
struct NodeCreator{
    static Node * makeNode(int /*x*/, int /*y*/) { return new Node();}
};
#define CELL_NUMBER 64
#define HGRID_MAP_SIZE  (533.33333f * 64.f)     // shouldn't be changed
#define CELL_SIZE       float(HGRID_MAP_SIZE/(float)CELL_NUMBER)

struct Cell
{
    int x, y;
    bool operator == (const Cell& c2) const { return x == c2.x && y == c2.y;}

    static Cell ComputeCell(float fx, float fy)
    {
        Cell c = { int(fx * (1.f/CELL_SIZE) + (CELL_NUMBER/2)), int(fy * (1.f/CELL_SIZE) + (CELL_NUMBER/2)) };
        return c;
    }

    bool isValid() const { return x >= 0 && x < CELL_NUMBER && y >= 0 && y < CELL_NUMBER;}
};

struct TimeTrackerSmall
{
    public:
        TimeTrackerSmall(G3D::g3d_uint32 expiry = 0) : i_expiryTime(expiry) {}
        void Update(G3D::g3d_int32 diff)
        {
            i_expiryTime -= diff;
        }

        bool Passed() const
        {
            return i_expiryTime <= 0;
        }

        void Reset(G3D::g3d_uint32 interval)
        {
            i_expiryTime = interval;
        }

        G3D::g3d_int32 GetExpiry() const
        {
            return i_expiryTime;
        }

    private:

        G3D::g3d_int32 i_expiryTime;
};

struct DynTreeImpl
{
    typedef BIHWrap<VMAP::GameobjectModelInstance> ModelWrap;
    typedef G3D::Table<const VMAP::GameobjectModelInstance*, ModelWrap*> MemberTable;
    typedef VMAP::GameobjectModelInstance Model;

    DynTreeImpl() :
        rebalance_timer(CHECK_TREE_PERIOD),
        unbalanced_times(0)
    {
        memset(nodes, NULL, sizeof(nodes));
    }

    ~DynTreeImpl()
    {
        for (int x = 0; x < CELL_NUMBER; ++x)
            for (int y = 0; y < CELL_NUMBER; ++y)
                delete nodes[x][y];
    }

    ModelWrap& getGrid(int x, int y)
    {
        assert(x < CELL_NUMBER && y < CELL_NUMBER);
        if (!nodes[x][y])
            nodes[x][y] = NodeCreator<ModelWrap>::makeNode(x, y);
        return *nodes[x][y];
    }

    ModelWrap& getGridFor(float fx, float fy)
    {
        Cell c = Cell::ComputeCell(fx, fy);
        return getGrid(c.x, c.y);
    }

    void insert(const Model& mdl)
    {
        G3D::Vector3 pos;
        PositionTrait<Model>::getPosition(mdl, pos);
        ModelWrap& node = getGridFor(pos.x, pos.y);
        node.insert(mdl);
        memberTable.set(&mdl, &node);
        ++unbalanced_times;
    }

    void remove(const Model& mdl)
    {
        memberTable[&mdl]->remove(mdl);
        // Remove the member
        memberTable.remove(&mdl);
        ++unbalanced_times;
    }

    void balance()
    {
        for (int x = 0; x < CELL_NUMBER; ++x)
            for (int y = 0; y < CELL_NUMBER; ++y)
                if (ModelWrap* n = nodes[x][y])
                    n->balance();
        unbalanced_times = 0;
    }

    void update(G3D::g3d_uint32 difftime)
    {
        if (!size())
            return;

        rebalance_timer.Update(difftime);
        if (rebalance_timer.Passed())
        {
            rebalance_timer.Reset(CHECK_TREE_PERIOD);
            if (unbalanced_times > 0)
                balance();
        }
    }

    template<typename RayCallback>
    void intersectRay(const G3D::Ray& ray, RayCallback& intersectCallback, float max_dist)
    {
        intersectRay(ray, intersectCallback, max_dist, ray.origin() + ray.direction() * max_dist);
    }

    template<typename RayCallback>
    void intersectRay(const G3D::Ray& ray, RayCallback& intersectCallback, float& max_dist, const G3D::Vector3& end)
    {
        Cell cell = Cell::ComputeCell(ray.origin().x, ray.origin().y);
        if (!cell.isValid())
            return;

        Cell last_cell = Cell::ComputeCell(end.x, end.y);

        if (cell == last_cell)
        {
            if (ModelWrap* node = nodes[cell.x][cell.y])
                node->intersectRay(ray, intersectCallback, max_dist);
            return;
        }

        float voxel = (float)CELL_SIZE;
        float kx_inv = ray.invDirection().x, bx = ray.origin().x;
        float ky_inv = ray.invDirection().y, by = ray.origin().y;

        int stepX, stepY;
        float tMaxX, tMaxY;
        if (kx_inv >= 0)
        {
            stepX = 1;
            float x_border = (cell.x+1) * voxel;
            tMaxX = (x_border - bx) * kx_inv;
        }
        else
        {
            stepX = -1;
            float x_border = (cell.x-1) * voxel;
            tMaxX = (x_border - bx) * kx_inv;
        }

        if (ky_inv >= 0)
        {
            stepY = 1;
            float y_border = (cell.y+1) * voxel;
            tMaxY = (y_border - by) * ky_inv;
        }
        else
        {
            stepY = -1;
            float y_border = (cell.y-1) * voxel;
            tMaxY = (y_border - by) * ky_inv;
        }

        //int Cycles = std::max((int)ceilf(max_dist/tMaxX),(int)ceilf(max_dist/tMaxY));
        //int i = 0;

        float tDeltaX = voxel * fabs(kx_inv);
        float tDeltaY = voxel * fabs(ky_inv);
        do
        {
            if (ModelWrap* node = nodes[cell.x][cell.y])
            {
                //float enterdist = max_dist;
                node->intersectRay(ray, intersectCallback, max_dist);
            }
            if (cell == last_cell)
                break;
            if (tMaxX < tMaxY)
            {
                tMaxX += tDeltaX;
                cell.x += stepX;
            }
            else
            {
                tMaxY += tDeltaY;
                cell.y += stepY;
            }
            //++i;
        } while (cell.isValid());
    }

    template<typename IsectCallback>
    void intersectPoint(const G3D::Vector3& point, IsectCallback& intersectCallback)
    {
        Cell cell = Cell::ComputeCell(point.x, point.y);
        if (!cell.isValid())
            return;
        if (ModelWrap* node = nodes[cell.x][cell.y])
            node->intersectPoint(point, intersectCallback);
    }

    // Optimized verson of intersectRay function for rays with vertical directions
    template<typename RayCallback>
    void intersectZAllignedRay(const G3D::Ray& ray, RayCallback& intersectCallback, float& max_dist)
    {
        Cell cell = Cell::ComputeCell(ray.origin().x, ray.origin().y);
        if (!cell.isValid())
            return;
        if (ModelWrap* node = nodes[cell.x][cell.y])
            node->intersectRay(ray, intersectCallback, max_dist);
    }

    TimeTrackerSmall rebalance_timer;

    int unbalanced_times;
    bool contains(const Model& value) const { return memberTable.containsKey(&value); }
    int size() const { return memberTable.size(); }
    MemberTable memberTable;
    ModelWrap* nodes[CELL_NUMBER][CELL_NUMBER];
};

DynamicMapTree::DynamicMapTree() : impl(new DynTreeImpl())
{

}

DynamicMapTree::~DynamicMapTree()
{
    delete impl;
}

void DynamicMapTree::insert(const VMAP::GameobjectModelInstance& mdl)
{
    impl->insert(mdl);
}

void DynamicMapTree::remove(const VMAP::GameobjectModelInstance& mdl)
{
    impl->remove(mdl);
}

bool DynamicMapTree::contains(const VMAP::GameobjectModelInstance& mdl) const
{
    return impl->contains(mdl);
}

void DynamicMapTree::balance()
{
    impl->balance();
}

int DynamicMapTree::size() const
{
    return impl->size();
}

void DynamicMapTree::update(G3D::g3d_uint32 t_diff)
{
    impl->update(t_diff);
}

struct DynamicTreeIntersectionCallback
{
    bool did_hit;
    G3D::g3d_int32 phase_mask;
    G3D::g3d_uint32 instanceId;
    DynamicTreeIntersectionCallback(G3D::g3d_int32 instanceid, G3D::g3d_int32 phasemask) : did_hit(false), instanceId(instanceid), phase_mask(phasemask) {}
    bool operator()(const G3D::Ray& r, const VMAP::GameobjectModelInstance& obj, float& distance)
    {
        did_hit = obj.intersectRay(r, distance, true, instanceId, phase_mask);
        return did_hit;
    }
    bool didHit() const { return did_hit;}
};

struct DynamicTreeIntersectionCallback_WithLogger
{
    bool did_hit;
    G3D::g3d_int32 phase_mask;
    G3D::g3d_uint32 instanceId;
    DynamicTreeIntersectionCallback_WithLogger(G3D::g3d_int32 instanceid, G3D::g3d_uint32 phasemask) : did_hit(false), instanceId(instanceid), phase_mask(phasemask)
    {
#ifdef _DEBUG
        printf("Dynamic Intersection log\n");
#endif
    }
    bool operator()(const G3D::Ray& r, const VMAP::GameobjectModelInstance& obj, float& distance)
    {
#ifdef _DEBUG
        printf("testing intersection with %s\n", obj.name.c_str());
#endif
        bool hit = obj.intersectRay(r, distance, true, instanceId, phase_mask);
        if (hit)
        {
            did_hit = true;
#ifdef _DEBUG
            printf("result: intersects\n");
#endif
        }
        return hit;
    }
    bool didHit() const { return did_hit;}
};

bool DynamicMapTree::getIntersectionTime(G3D::g3d_uint32 instanceId, G3D::g3d_int32 phasemask, const G3D::Ray& ray,
                                         const G3D::Vector3& endPos, float& maxDist) const
{
    float distance = maxDist;
    DynamicTreeIntersectionCallback callback(instanceId, phasemask);
    impl->intersectRay(ray, callback, distance, endPos);
    if (callback.didHit())
        maxDist = distance;
    return callback.didHit();
}

bool DynamicMapTree::getObjectHitPos(G3D::g3d_uint32 instanceId, G3D::g3d_int32 phasemask, const G3D::Vector3& startPos,
                                     const G3D::Vector3& endPos, G3D::Vector3& resultHit,
                                     float modifyDist) const
{
    bool result = false;
    float maxDist = (endPos - startPos).magnitude();
    // valid map coords should *never ever* produce float overflow, but this would produce NaNs too
    assert(maxDist < std::numeric_limits<float>::max());
    // prevent NaN values which can cause BIH intersection to enter infinite loop
    if (maxDist < 1e-10f)
    {
        resultHit = endPos;
        return false;
    }
    G3D::Vector3 dir = (endPos - startPos)/maxDist;              // direction with length of 1
    G3D::Ray ray(startPos, dir);
    float dist = maxDist;
    if (getIntersectionTime(instanceId, phasemask, ray, endPos, dist))
    {
        resultHit = startPos + dir * dist;
        if (modifyDist < 0)
        {
            if ((resultHit - startPos).magnitude() > -modifyDist)
                resultHit = resultHit + dir*modifyDist;
            else
                resultHit = startPos;
        }
        else
            resultHit = resultHit + dir*modifyDist;

        result = true;
    }
    else
    {
        resultHit = endPos;
        result = false;
    }
    return result;
}

bool DynamicMapTree::isInLineOfSight(float x1, float y1, float z1, float x2, float y2, float z2, G3D::g3d_uint32 instanceId, G3D::g3d_int32 phasemask) const
{
    G3D::Vector3 v1(x1, y1, z1), v2(x2, y2, z2);

    float maxDist = (v2 - v1).magnitude();

    if (!G3D::fuzzyGt(maxDist, 0) )
        return true;

    G3D::Ray r(v1, (v2-v1) / maxDist);
    DynamicTreeIntersectionCallback callback(instanceId, phasemask);
    impl->intersectRay(r, callback, maxDist, v2);

    return !callback.did_hit;
}

float DynamicMapTree::getHeight(float x, float y, float z, float maxSearchDist, G3D::g3d_uint32 instanceId, G3D::g3d_int32 phasemask) const
{
    G3D::Vector3 v(x, y, z);
    G3D::Ray r(v, G3D::Vector3(0, 0, -1));
    DynamicTreeIntersectionCallback callback(instanceId, phasemask);
    impl->intersectZAllignedRay(r, callback, maxSearchDist);

    if (callback.didHit())
        return v.z - maxSearchDist;
    else
        return G3D::inf();
}
