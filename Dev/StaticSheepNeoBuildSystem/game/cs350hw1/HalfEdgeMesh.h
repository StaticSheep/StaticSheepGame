/************************************************************************
*
* Corbin Hart © (2008-201x)
* All rights reserved.
*
************************************************************************/

#pragma once

#include "API.h"
#include "MathExt.h"

#include <array>
#include <vector>
#include <numeric>
#include <functional>

namespace dit {

    //-----------------------------------//
    // Global values

    const uint32 MeshNullIdx = std::numeric_limits<uint32>::max();

    //-----------------------------------//
    // Half-edge data structures

    struct VertexFace
    {
        VertexFace() = default;
        VertexFace(uint32 node) : nodeIdx(node) {}

        uint32 nodeIdx = MeshNullIdx;
    };

    //-----------------------------------//

    struct VertexFaceNode
    {
        VertexFaceNode() = default;
        VertexFaceNode(uint32 next, uint32 faceIdx);

        uint32 next = MeshNullIdx;
        uint32 faceIdx = MeshNullIdx;
    };

    //-----------------------------------//

    struct Face
    {
        Face() { vtx = { { MeshNullIdx, MeshNullIdx, MeshNullIdx } }; }
        Face(const std::array<uint32, 3>& vtx);

        std::array<uint32, 3> vtx;
    };

    //-----------------------------------//

    struct FaceEdge
    {
        FaceEdge() { nodeIdx = { { MeshNullIdx, MeshNullIdx, MeshNullIdx } }; }

        std::array<uint32, 3> nodeIdx;
    };

    //-----------------------------------//

    struct FaceEdgeNode
    {
        FaceEdgeNode() = default;
        FaceEdgeNode(uint32 next, uint32 edgeIdx);

        uint32 next = MeshNullIdx;
        uint32 edgeIdx = MeshNullIdx;
    };

    //-----------------------------------//

    struct HalfEdge
    {
        HalfEdge() { vtx = { { MeshNullIdx, MeshNullIdx } }; }
        HalfEdge(uint32 twinIdx, uint32 faceIdx, std::array<uint32, 2> vtx);

        uint32 twinIdx = MeshNullIdx;    // index of the twin edge 
        uint32 faceIdx = MeshNullIdx;    // index into the face index list
        std::array<uint32, 2> vtx;  // indices into the vertex list
    };

    //-----------------------------------//

    struct HalfEdgeHash
    {
        HalfEdgeHash() { v = { { MeshNullIdx, MeshNullIdx } }; }
        HalfEdgeHash(uint32 v0, uint32 v1);
        HalfEdgeHash(uint64 k);

        union
        {
            std::array<uint32, 2> v;
            uint64 key;
        };
    };

    //-----------------------------------//
    // Half-edge mesh structure

    class HalfEdgeMesh
    {
    public:
        HalfEdgeMesh() { exts = { { float3{ 0.f }, float3{ 0.f } } }; }
        HalfEdgeMesh(const HalfEdgeMesh&) = delete;
        HalfEdgeMesh(HalfEdgeMesh&& rhs);

        uint32 triangles() const { return faceVertices.size(); }
        uint32 vertices() const { return vertexPositions.size(); }
        uint32 edges() const { return halfEdges.size(); };

        // vertex data
        std::vector<float3> vertexPositions;
        std::vector<float3> vertexNormals;
        std::vector<VertexFace> vertexFaces;
        std::vector<VertexFaceNode> vfNodes;

        // face data
        std::vector<Face> faceVertices;
        std::vector<FaceEdge> faceEdges;
        std::vector<FaceEdgeNode> feNodes;

        // edge data
        std::vector<HalfEdge> halfEdges;

        // meta data
        std::array<float3, 2> exts;
    };

    //-----------------------------------//
    // Iterator patterns

    class MeshIterator
    {
    public:
        struct Context
        {
            union
            {
                const MeshIterator* parent;
                uint32 data;
            };

            Context() : data(MeshNullIdx) {}
            Context(uint32 val) : data(val) {}
            Context(const MeshIterator* ptr) : parent(ptr) {}
        };

    public:
        MeshIterator() = default;
        MeshIterator(Context ctx, uint32 idx, HalfEdgeMesh* mesh);

        virtual MeshIterator nextVertex(const MeshIterator*) const;
        virtual MeshIterator nextEdge(const MeshIterator*) const;
        virtual MeshIterator nextFace(const MeshIterator*) const;

    public:
        Context ctx;
        uint32  idx = MeshNullIdx;
        HalfEdgeMesh* mesh = nullptr;
    };

    //-----------------------------------//

    class VertexIterator : public MeshIterator
    {
    public:
        VertexIterator(Context ctx, uint32 idx, HalfEdgeMesh* mesh);

        float3& pos();
        const float3& pos() const;

        float& pos(uint32 idx);
        const float& pos(uint32 idx) const;

        float3& normal();
        const float3& normal() const;

        float& normal(uint32 idx);
        const float& normal(uint32 idx) const;

    private:
        virtual MeshIterator nextVertex(const MeshIterator* callee) const;
        virtual MeshIterator nextFace(const MeshIterator* callee) const;
        virtual MeshIterator nextEdge(const MeshIterator*) const;
    };

    //-----------------------------------//

    class EdgeIterator : public MeshIterator
    {
    public:
        EdgeIterator(Context ctx, uint32 idx, HalfEdgeMesh* mesh);

        HalfEdge& value();
        const HalfEdge& value() const;

        HalfEdge* twin();
        const HalfEdge* twin() const;

        float3& vtx(uint32 idx);
        const float3& vtx(uint32 idx) const;

        float3& normal(uint32 idx);
        const float3& normal(uint32 idx) const;

        Face& face();
        const Face& face() const;

    private:
        virtual MeshIterator nextVertex(const MeshIterator*) const;
        virtual MeshIterator nextEdge(const MeshIterator* callee) const;
        virtual MeshIterator nextFace(const MeshIterator*) const;
    };

    //-----------------------------------//

    class FaceIterator : public MeshIterator
    {
    public:
        FaceIterator(Context ctx, uint32 idx, HalfEdgeMesh* mesh);

        Face& value();
        const Face& value() const;

        float3& vtx(uint32 idx);
        const float3& vtx(uint32 idx) const;

        float3& normal(uint32 idx);
        const float3& normal(uint32 idx) const;

        float3 normal() const;

    private:
        virtual MeshIterator nextVertex(const MeshIterator* callee) const;
        virtual MeshIterator nextEdge(const MeshIterator* callee) const;
        virtual MeshIterator nextFace(const MeshIterator* callee) const;
    };

    //-----------------------------------//
    // mesh traversal dsel

    namespace mesh {

        //-----------------------------------//

        VertexIterator vertices(HalfEdgeMesh& mesh);
        VertexIterator vertices(const FaceIterator& face);

        //-----------------------------------//

        EdgeIterator edges(HalfEdgeMesh& mesh);
        EdgeIterator edges(const FaceIterator& face);
        EdgeIterator twin(const EdgeIterator& edge);

        //-----------------------------------//

        FaceIterator faces(HalfEdgeMesh& mesh);
        FaceIterator faces(const VertexIterator& vtx);

        //-----------------------------------//

        VertexIterator begin(VertexIterator& vertices);
        EdgeIterator begin(EdgeIterator& edges);
        FaceIterator begin(FaceIterator& faces);

        //-----------------------------------//

        VertexIterator next(const VertexIterator& vtx);
        EdgeIterator next(const EdgeIterator& edge);
        FaceIterator next(const FaceIterator& face);

        //-----------------------------------//

        bool null(const VertexIterator& vtx);
        bool null(const EdgeIterator& edge);
        bool null(const FaceIterator& face);

        //-----------------------------------//

        VertexIterator end(const VertexIterator&);
        EdgeIterator end(const EdgeIterator&);
        FaceIterator end(const FaceIterator&);

        //-----------------------------------//

        typedef std::function<VertexIterator(VertexIterator&)> VertexProcessor;
        typedef std::function<EdgeIterator(EdgeIterator&)> EdgeProcessor;
        typedef std::function<FaceIterator(FaceIterator&)> FaceProcessor;

        void walk(VertexIterator vertices, VertexProcessor func);
        void walk(EdgeIterator edges, EdgeProcessor func);
        void walk(FaceIterator faces, FaceProcessor func);
    }
}