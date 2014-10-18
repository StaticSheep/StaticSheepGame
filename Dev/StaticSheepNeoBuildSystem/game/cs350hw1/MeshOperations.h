/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "MathExt.h"
#include "StringExt.h"

#include <memory>
#include <array>
#include <vector>

namespace dit {

    //-----------------------------------//

    class Model;
    class Plane;
    class HalfEdgeMesh;

    //-----------------------------------//

    float IntersectPlaneRay(const Plane& plane, const float3& p0, const float3& p1);
    float DistancePointPlane(const Plane& plane, const float3& pt);

    void ComputeConnectivity(HalfEdgeMesh* mesh);

    int32 ComputeBalance(const Plane& plane, HalfEdgeMesh* mesh);
    Plane FindSplitPlane(HalfEdgeMesh* mesh);

    HalfEdgeMesh* Stitch(const std::vector<std::shared_ptr<Model>>& models);
    std::array<HalfEdgeMesh*, 2> Split(const Plane& plane, HalfEdgeMesh* mesh);

    //-----------------------------------//

    HalfEdgeMesh* MeshFromOBJ(const String& file);

    //-----------------------------------//

}