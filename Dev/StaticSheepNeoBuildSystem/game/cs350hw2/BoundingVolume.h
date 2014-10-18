/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "HalfEdgeMesh.h"
#include "MathExt.h"

namespace dit
{
    //-----------------------------------//

    class BoundingSphere;

    //-----------------------------------//

    class AxisAlignedBoundingBox
    {
    public:
        AxisAlignedBoundingBox() = default;
        AxisAlignedBoundingBox(const AxisAlignedBoundingBox& rhs) = default;

        AxisAlignedBoundingBox(const float3& minPt, const float3& maxPt);
        AxisAlignedBoundingBox(const BoundingSphere& bs);

        void Build(const float3& minPt, const float3& maxPt);

    public:
        float3 min{ zero<float3>() };
        float3 max{ zero<float3>() };
        float3 span{ zero<float3>() };
        float3 center{ zero<float3>() };
    };

    //-----------------------------------//

    class BoundingSphere
    {
    public:
        BoundingSphere();
        BoundingSphere(const AxisAlignedBoundingBox& aabb, bool useExterior);
        BoundingSphere(const BoundingSphere& bs, const float3& pt, float scale);
        BoundingSphere(const BoundingSphere& bs0, const BoundingSphere& bs2);

        // geometric tests
        bool PointInside(const float3& pt);
        float DistanceSqTo(const BoundingSphere& rhs);

        // construction methods
        void BuildFromAABB(const AxisAlignedBoundingBox& aabb, bool useExterior);
        void BuildFromCentroid(const std::vector<float3>& pts);
        void BuildRittersMethod(const std::vector<float3>& pts);
        void BuildLarssonMethod(const std::vector<float3>& pts, const std::vector<float3>& directions);
        void GrowToMin(const float3& pt0, const float3& pt1);
        void GrowToContain(const std::vector<float3>& pts);

    public:
        float3 center{zero<float3>()};
        float radius = 0.f;

    private:
        unsigned FindFarthest(const float3& vtx, const std::vector<float3>& pts);
        float    FindFarthestAlong(const float3& normal, const std::vector<float3>& pts, unsigned& minPointIdx, unsigned& maxPointIdx);
    };

}