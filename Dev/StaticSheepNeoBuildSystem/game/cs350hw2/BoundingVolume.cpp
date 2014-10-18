/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "BoundingVolume.h"
#include "HalfEdgeMesh.h"
#include "Model.h"

namespace dit {

    using namespace mesh;

    //-----------------------------------//

    AxisAlignedBoundingBox::AxisAlignedBoundingBox(const BoundingSphere& bs)
    {
        float3 r = bs.radius * one<float3>();

        if (bs.radius < 0.f)
            r *= -1.f;

        Build(bs.center - r, bs.center + r);
    }

    //-----------------------------------//

    AxisAlignedBoundingBox::AxisAlignedBoundingBox(const float3& minPt, const float3& maxPt)
        : min(minPt)
        , max(maxPt)
        , span(maxPt - minPt)
        , center((minPt + maxPt) / 2.f)
    {}

    //-----------------------------------//

    void AxisAlignedBoundingBox::Build(const float3& minPt, const float3& maxPt)
    {
        min = minPt;
        max = maxPt;
        span = max - min;
        center = (minPt + maxPt) / 2.f;
    }

    //-----------------------------------//

    BoundingSphere::BoundingSphere()
        : center(zero<float3>())
        , radius(0.f)
    {}

    //-----------------------------------//

    BoundingSphere::BoundingSphere(const AxisAlignedBoundingBox& aabb, bool useExterior)
    {
        BuildFromAABB(aabb, useExterior);
    }

    //-----------------------------------//

    BoundingSphere::BoundingSphere(const BoundingSphere& model_bsphere, const float3& objPos, float objScale)
    {
        center = model_bsphere.center     *objScale;
        radius = model_bsphere.radius*objScale;

        center += objPos;
    }

    //-----------------------------------//

    BoundingSphere::BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs1)
    {
        float3 d = normalize(bs1.center - bs0.center);
        float3 edge0 = bs0.center - (d * bs0.radius);
        float3 edge1 = bs1.center + (d * bs1.radius);

        center = (edge0 + edge1) / 2.f;
        radius = length(center - edge1);
    }

    //-----------------------------------//

    unsigned BoundingSphere::FindFarthest(const float3 &vtx, const std::vector<float3>& pts)
    {
        unsigned maxDistIdx = 0;
        unsigned idx = 0;
        float maxDist = 0.f;
        for (const auto& p : pts)
        {
            float d = length(vtx - p);
            if (d > maxDist)
            {
                maxDist = d;
                maxDistIdx = idx;
            }

            idx += 1;
        }

        return maxDistIdx;
    }

    //-----------------------------------//

    bool BoundingSphere::PointInside(const float3& pt)
    {
        return(length2(center - pt) <= radius * radius);
    }

    //-----------------------------------//

    void BoundingSphere::BuildFromAABB(const AxisAlignedBoundingBox& aabb, bool useExterior)
    {
        if (useExterior)
        {
            // use a vector from center to max
            radius = length(aabb.max - aabb.center);
        }
        else
        {
            float val = aabb.span[0];
            unsigned idx = 0;
            for (unsigned i = 1; i < 3; ++i)
            {
                if (aabb.span[i] > val)
                {
                    val = aabb.span[i];
                    idx = i;
                }
            }

            radius = aabb.max[idx] - aabb.center[idx];
        }
    }

    //-----------------------------------//

    void BoundingSphere::BuildFromCentroid(const std::vector<float3>& pts)
    {
        float3 average = zero<float3>();
        for (unsigned i = 0; i < pts.size(); ++i)
            average += (const float3&) pts[i];

        center = average / (float) pts.size();

        float maxDistSq = 0.f;
        for (unsigned i = 0; i < pts.size(); ++i)
        {
            float d = length2(average - pts[i]);

            if (maxDistSq < d)
                maxDistSq = d;
        }

        radius = sqrt(maxDistSq);
    }

    //-----------------------------------//

    void BoundingSphere::BuildRittersMethod(const std::vector<float3>& pts)
    {
        if (pts.size() == 0)
            return;

        const float3& idx0 = pts[0];
        const float3& idx1 = pts[FindFarthest(idx0, pts)];
        const float3& idx2 = pts[FindFarthest(idx1, pts)];

        GrowToMin(idx1, idx2);
        GrowToContain(pts);
    }

    //-----------------------------------//

    void BoundingSphere::BuildLarssonMethod(const std::vector<float3>& verts, const std::vector<float3>& directions)
    {
        unsigned n = verts.size();
        unsigned s = directions.size();
        if (n == 0)
            return;

        const std::vector<float3>* pts = nullptr;
        std::vector<float3> extremalPts;

        // there must be at least 2 points per direction, else just approximate normally
        if (n > s * 2)
        {
            // find extremal points
            for (unsigned i = 0; i < s; ++i)
            {
                unsigned minIdx;
                unsigned maxIdx;

                FindFarthestAlong(directions[i], verts, minIdx, maxIdx);

                extremalPts.push_back(verts[minIdx]);
                extremalPts.push_back(verts[maxIdx]);
            }

            pts = &extremalPts;
        }
        else
        {
            pts = &verts;
        }

        BuildRittersMethod(*pts);
    }

    //-----------------------------------//

    // returns distance along normal, minPt and maxPt are output indexes
    float BoundingSphere::FindFarthestAlong(const float3& n, const std::vector<float3>& pts, unsigned& minPointIdx, unsigned& maxPointIdx)
    {
        unsigned minIdx = 0;
        unsigned maxIdx = 0;
        float minProj = std::numeric_limits<float>::min();
        float maxProj = std::numeric_limits<float>::max();

        float l = length(n);

        unsigned idx = 0;
        for (const auto& p : pts)
        {
            float d = dot(p, n) / l;

            if (d > maxProj)
            {
                maxIdx = idx;
                maxProj = d;
            }
            else if (d < minProj)
            {
                minIdx = idx;
                minProj = d;
            }

            idx += 1;
        }

        minPointIdx = minIdx;
        maxPointIdx = maxIdx;

        return maxProj - minProj;
    }

    //-----------------------------------//

    void BoundingSphere::GrowToContain(const std::vector<float3>& pts)
    {
        for (const auto& p : pts)
        {
            if (!PointInside(p))
                radius = length(center - p);
        }
    }

    //-----------------------------------//

    void BoundingSphere::GrowToMin(const float3& pt0, const float3& pt1)
    {
        center = (pt0 + pt1) / 2.f;
        radius = length(pt0 - pt1) / 2.f;
    }

    //-----------------------------------//

    float BoundingSphere::DistanceSqTo(const BoundingSphere& rhs)
    {
        return length2(center - rhs.center) - ((radius * radius) + (rhs.radius * rhs.radius));
    }

    //-----------------------------------//

}