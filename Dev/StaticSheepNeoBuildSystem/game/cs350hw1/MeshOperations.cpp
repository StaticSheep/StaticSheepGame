/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#include "API.h"
#include "HashMap.h"
#include "Core.h"
#include "MeshOperations.h"
#include "HalfEdgeMesh.h"
#include "Model.h"
#include "Plane.h"

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>

namespace dit {

    using namespace mesh;

    //-----------------------------------//

    float IntersectPlaneRay(const Plane& p, const float3& p0, const float3& p1)
    {
        return dot(p.normal, p.center - p0) / dot(p.normal, p1 - p0);
    }

    //-----------------------------------//

    float DistancePointPlane(const Plane& plane, const float3& pt)
    {
        return dot((pt - plane.center), plane.normal) / length(plane.normal);
    }

    //-----------------------------------//

    void ComputeConnectivity(HalfEdgeMesh* mesh)
    {
        // make pretty all the names
        auto& vertices = mesh->vertexPositions;
        auto& vertexFaces = mesh->vertexFaces;
        auto& vertexNormals = mesh->vertexNormals;
        auto& vfNodes = mesh->vfNodes;
        auto& faces = mesh->faceVertices;
        auto& faceEdges = mesh->faceEdges;
        auto& feNodes = mesh->feNodes;
        auto& edges = mesh->halfEdges;

        // temporary data
        HashMap<uint32> edgeTraversals;

        vertexFaces.resize(mesh->vertices());
        faceEdges.resize(mesh->triangles());

        bool buildingNormals = vertexNormals.empty();
        if (buildingNormals)
            vertexNormals.resize(mesh->vertices());

        // walk each face
        walk(mesh::faces(*mesh), [&](FaceIterator& face) -> FaceIterator {

            // calculate the face's normal
            auto u = face.vtx(1) - face.vtx(0);
            auto v = face.vtx(2) - face.vtx(1);
            auto fn = cross(u, v);

            if (buildingNormals)
            {
                // if it is negative, reorder the verts
                if (length2(fn) < 0.f)
                {
                    std::swap(face.value().vtx[1], face.value().vtx[2]);

                    u = face.vtx(1) - face.vtx(0);
                    v = face.vtx(2) - face.vtx(1);

                    fn = cross(u, v);
                    assert(length2(fn) > 0.f);
                }
            }

            // walk the verts in cw order
            auto verts = mesh::vertices(face);
            walk(begin(verts), [&](VertexIterator& vtx) -> VertexIterator {

                // compute the vtx's normal
                if (buildingNormals)
                {
                    auto& vn = vtx.normal();
                    vn += fn;
                    vn = normalize(vn);
                }

                // insert the face into the vtx's face-list
                auto head = mesh->vertexFaces[vtx.idx];
                auto vfnIdx = vfNodes.size();
                vfNodes.push_back({ head.nodeIdx, face.idx });

                mesh->vertexFaces[vtx.idx] = VertexFace{ vfnIdx };

                // KEEP 'EM COMIN' SON
                return next(vtx);
            });

            // build the edge data
            std::array<std::array<uint32, 2>, 3> fedges{ {
                    { { 0, 1 } }
                    , { { 1, 2 } }
                    , { { 2, 0 } } } };

            std::array<uint32, 4> feNextNodeIdx{ { MeshNullIdx, MeshNullIdx, MeshNullIdx, MeshNullIdx } };

            // walk the edges
            for (uint32 i = 0; i < fedges.size(); ++i)
            {
                // construct the linked list of edges in reverse order, with the 0th edge being the head
                auto& fe = fedges[2 - i];

                // edge end-points and key-value
                std::array<uint32, 2> feIdx{ { face.value().vtx[fe[0]], face.value().vtx[fe[1]] } };
                HalfEdgeHash feKey{ feIdx[0], feIdx[1] };

                // insert the half edge structure
                HalfEdge hedge{ MeshNullIdx, face.idx, feIdx };
                uint32 heIdx = edges.size();
                edges.push_back(hedge);

                // copy face edge index into array
                auto feNodeIdx = feNodes.size();
                feNodes.push_back(FaceEdgeNode{ feNextNodeIdx[(2 - i) + 1] , heIdx });

                feNextNodeIdx[(2 - i)] = feNodeIdx;

                // this is the first traversal of the edge
                if (!edgeTraversals.has(feKey.key))
                {
                    edgeTraversals.set(feKey.key, heIdx);
                }
                else
                {
                    auto twinIdx = edgeTraversals.get(feKey.key, MeshNullIdx);

                    // this is a valid twin, and this is the second time we're walking it
                    if (twinIdx != MeshNullIdx)
                    {
                        edges[heIdx].twinIdx = twinIdx;
                        edges[twinIdx].twinIdx = heIdx;
                        edgeTraversals.set(feKey.key, MeshNullIdx);
                    }
                    // this is an invalid twin, we're traversing the same edge multiple times(???)
                    else
                    {
                        log("edge traversal set to null [\"%0, %1\"] {\"%2\"}", feIdx[0], feIdx[1], face.idx);
                    }
                }

            }

            // insert edge list head into the face
            faceEdges[face.idx].nodeIdx = { { feNextNodeIdx[0], feNextNodeIdx[1], feNextNodeIdx[2] } };

            return next(face);
        });
    }

    //-----------------------------------//

    int32 ComputeBalance(const Plane& plane, HalfEdgeMesh* mesh)
    {
        int32 tb = 0;

        walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator {

            int32 vb = 0;
            for (uint32 i = 0; i < 3; ++i)
            {
                float d = DistancePointPlane(plane, face.vtx(i));

                if (d > .000001f)
                    ++vb;
                else if (d < -.000001f)
                    --vb;
            }

            if (vb > 0)
                ++tb;
            else if (vb < 0)
                --tb;

            return next(face);
        });

        return tb;
    }

    //-----------------------------------//

    HalfEdgeMesh* Stitch(const std::vector<std::shared_ptr<Model>>& objs)
    {
        // create the bsp mesh
        auto mesh = new HalfEdgeMesh{};

        auto& verts = mesh->vertexPositions;
        auto& normals = mesh->vertexNormals;
        auto& tris = mesh->faceVertices;

        // stitch the object meshes together
        for (auto obj : objs)
        {
            auto mesh = obj->meshData.get();
            auto vtxCapacity = verts.capacity() + mesh->vertexPositions.size();
            auto triCapacity = tris.capacity() + mesh->faceVertices.size();

            verts.reserve(vtxCapacity);
            normals.reserve(vtxCapacity);
            tris.reserve(triCapacity);

            // bsp verts are in world space
            auto w = obj->world();
            auto wit = inverseTranspose(w);

            walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator {
                walk(vertices(face), [&](const VertexIterator& vtx) -> VertexIterator {
                    const auto& pos = vtx.pos();
                    const auto& nml = vtx.normal();
                    auto p = float4(pos.x, pos.y, pos.z, 1.f) * w;
                    auto n = float4(nml.x, nml.y, nml.z, 0.f) * wit;

                    verts.push_back(float3(p.x, p.y, p.z));
                    normals.push_back(float3(n.x, n.y, n.z));

                    return next(vtx);
                });

                tris.push_back(face.value());

                return next(face);
            });
        }

        return mesh;
    }

    //-----------------------------------//

    std::array<HalfEdgeMesh*, 2> Split(const Plane& plane, HalfEdgeMesh* mesh)
    {
        std::array<HalfEdgeMesh*, 2> childs = { { new HalfEdgeMesh{}, new HalfEdgeMesh{} } };

        walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator {
        
            std::array<float, 3> d = { { 0.f, 0.f, 0.f } };
            std::array<bool, 3> space = { { false, false, false } };

            // in'n'out tests
            for (uint32 i = 0; i < 3; ++i)
            {
                d[i] = DistancePointPlane(plane, face.vtx(i));
                space[i] = d[i] > 0.f;
            }

            // triangles are on the same side of the plane
            if (space[0] == space[1] == space[2])
            {
                // pick a side
                uint32 cidx = (d[0] > 0.f) ? 0 : 1;
                uint32 vidx = childs[cidx]->vertexPositions.size();

                // add the vtx positions and normals
                childs[cidx]->vertexPositions.push_back(face.vtx(0));
                childs[cidx]->vertexPositions.push_back(face.vtx(1));
                childs[cidx]->vertexPositions.push_back(face.vtx(2));

                childs[cidx]->vertexNormals.push_back(face.normal(0));
                childs[cidx]->vertexNormals.push_back(face.normal(1));
                childs[cidx]->vertexNormals.push_back(face.normal(2));

                // add the triangle
                childs[cidx]->faceVertices.push_back(Face{ { { cidx, cidx + 0, cidx + 1 } } });
            }
            // clip triangle against the plane
            else
            {
                // unify indexing order to my head's reference frame of vtx[ idx[0] ] and vtx[ idx[1] ] are positive wrt to the plane
                std::array<uint32, 2> cidx = { {
                    (d[0] > 0.f) ? 0 : 1
                    , (d[0] > 0.f) ? 1 : 0
                } };

                std::array<uint32, 3> vidx = { { 0, 0, 0 } };

                if (space[0] == space[1])
                {
                    vidx[0] = 0;
                    vidx[1] = 1;
                    vidx[2] = 2;
                }
                else if (space[1] == space[2])
                {
                    vidx[0] = 0;
                    vidx[1] = 2;
                    vidx[2] = 1;

                    cidx[0] = (cidx[0] + 1) % 2;
                    cidx[1] = (cidx[1] + 1) % 2;
                }
                else if (space[2] == space[0])
                {
                    vidx[0] = 2;
                    vidx[1] = 0;
                    vidx[2] = 1;

                    cidx[0] = (cidx[0] + 1) % 2;
                    cidx[1] = (cidx[1] + 1) % 2;
                }

                // compute intersection points
                std::array<float, 2> t = { {
                    IntersectPlaneRay(plane, face.vtx(vidx[1]), face.vtx(vidx[2]))
                    , IntersectPlaneRay(plane, face.vtx(vidx[2]), face.vtx(vidx[0]))
                } };

                std::array<float3, 2> clip = { {
                    t[0] * (face.vtx(vidx[2]) - face.vtx(vidx[1]))
                    , t[1] * (face.vtx(vidx[2]) - face.vtx(vidx[0]))
                } };

                // two verts and two triangles into "positive" space
                auto& verts = childs[cidx[0]]->vertexPositions;
                auto& nmls = childs[cidx[0]]->vertexNormals;
                auto& tris = childs[cidx[0]]->faceVertices;
                auto clipIdx = verts.size();

                verts.push_back(face.vtx(vidx[0]));
                verts.push_back(face.vtx(vidx[1]));
                verts.push_back(clip[0]);
                verts.push_back(clip[1]);

                tris.push_back(Face{ { { clipIdx        , clipIdx + 1   , clipIdx + 2 } } });
                tris.push_back(Face{ { { clipIdx + 1    , clipIdx + 2   , clipIdx + 3 } } });

                // two verts and one triangle into "negative" space
                verts = childs[cidx[1]]->vertexPositions;
                nmls = childs[cidx[1]]->vertexNormals;
                tris = childs[cidx[1]]->faceVertices;
                clipIdx = verts.size();

                verts.push_back(face.vtx(vidx[2]));
                verts.push_back(clip[1]);
                verts.push_back(clip[0]);

                tris.push_back(Face{ { { clipIdx        , clipIdx + 1   , clipIdx + 2 } } });
            }

            return next(face);
        });

        return childs;
    }

    //-----------------------------------//

    Plane FindSplitPlane(HalfEdgeMesh* mesh)
    {
        static const unsigned _MaxBalanceIntolerance = 2;

        Plane plane;
        int min = std::numeric_limits<int>::max();

        walk(faces(*mesh), [&](const FaceIterator& face) -> FaceIterator
        {
            // create the plane at vtx[0].pos with face.normal
            const auto& n = face.normal();
            auto p = Plane{ face.vtx(0), n };
            auto b = ComputeBalance(p, mesh);

            // kick out if satisfactory
            if (abs(b) < _MaxBalanceIntolerance)
                return end(face);

            // update bounds and move on
            if (abs(b) < abs(min))
            {
                plane = p;
                min = b;
            }

            return next(face);
        });

        return plane;
    }

    //-----------------------------------//

    HalfEdgeMesh* MeshFromOBJ(const String& file)
    {
        using namespace mesh;

        // Vertex meta data
        std::vector<float3> vertices;
        std::vector<float3> vertexNormals;
        std::vector<VertexFace> vertexFaces;
        std::vector<VertexFaceNode> vfNodes;

        // Face meta data
        std::vector<Face> faces;
        std::vector<FaceEdge> faceEdges;
        std::vector<FaceEdgeNode> feNodes;

        // Edge meta data
        HashMap<uint32> edgeTraversals;
        std::vector<HalfEdge> edges;

        // check the file
        std::ifstream in(file, std::ios::in);
        if (!in)
        {
            log("could not open file: %0", file.c_str());
            return nullptr;
        }

        // mesh data
        auto mesh = new HalfEdgeMesh{};

        std::array<float3, 2> exts{ { 
            float3{ std::numeric_limits<float>::max() }
            , float3{ std::numeric_limits<float>::min() } } };

        Timer loadClock;
        loadClock.Reset();

        // read the file in and create the vertex & face lists
        String line;
        try
        {
            while (std::getline(in, line))
            {
                if (line[0] == '#') continue;   // comment
                if (line.size() < 3) continue;   // blank line


                // parse the data using spaces as a delimiter
                boost::char_separator<char> sep(" \t\r\n");
                boost::tokenizer<boost::char_separator<char>> tokens(line, sep);
                auto tok = tokens.begin();

#pragma region vertex parsing
                // vertex data
                if (*tok == "v") {
                    float3 v; ++tok;
                    v.x = boost::lexical_cast<float>(*tok); ++tok;
                    v.y = boost::lexical_cast<float>(*tok); ++tok;
                    v.z = boost::lexical_cast<float>(*tok); ++tok;

                    assert(tok == tokens.end());

                    vertices.push_back(v);

                    // determine the extents
                    exts[0] = float3{ std::min(v.x, exts[0].x), std::min(v.y, exts[0].y), std::min(v.z, exts[0].z) };
                    exts[1] = float3{ std::max(v.x, exts[1].x), std::max(v.y, exts[1].y), std::max(v.z, exts[1].z) };

                }
                else if (*tok == "vn") {
                    float3 vn; ++tok;
                    vn.x = boost::lexical_cast<float>(*tok); ++tok;
                    vn.y = boost::lexical_cast<float>(*tok); ++tok;
                    vn.z = boost::lexical_cast<float>(*tok); ++tok;

                    vertexNormals.push_back(vn);

                    assert(tok == tokens.end());
                }
                else if (*tok == "vf") {
                    log("[ %0 ] not supported { \"%1\" }", (*tok).c_str(), line.c_str());
                }
                else if (*tok == "vt") {
                    log("[ %0 ] not supported { \"%1\" }", (*tok).c_str(), line.c_str());
                }
#pragma endregion
#pragma region triangle parsing
                else if (*tok == "f") { // triangle data
                    ++tok;
                    auto a = boost::lexical_cast<uint32>(*tok); ++tok;
                    auto b = boost::lexical_cast<uint32>(*tok); ++tok;
                    auto c = boost::lexical_cast<uint32>(*tok); ++tok;

                    assert(tok == tokens.end());

                    // create the face
                    a--; b--; c--;

                    Face f;
                    f.vtx = { { a, b, c } };

                    auto fi = (uint32) faces.size();
                    faces.push_back(f);

#pragma endregion

                }
                else
                {
                    log("[ %0 ] not supported { \"%1\" }", (*tok).c_str(), line.c_str());
                }
            }

            // move the temp data into the mesh
            mesh->vertexPositions = std::move(vertices);
            mesh->vertexNormals = std::move(vertexNormals);
            mesh->vertexFaces = std::move(vertexFaces);
            mesh->vfNodes = std::move(vfNodes);
            mesh->faceVertices = std::move(faces);
            mesh->faceEdges = std::move(faceEdges);
            mesh->halfEdges = std::move(edges);

            // build the topology graph
            ComputeConnectivity(mesh);

            mesh->exts = exts;
        }
        catch (const boost::bad_lexical_cast& e)
        {
            log(e.what());
        }
        catch (...)
        {
            return nullptr;
        }

#pragma endregion

        auto time = loadClock.GetElapsed();
        log("load time: %0s", time);

        return mesh;
    }

}