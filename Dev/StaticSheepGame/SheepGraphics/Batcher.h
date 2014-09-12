#pragma once

#include "Context.h"

#include "Handle.h"
#include "DataTypes.h"

namespace DirectSheep
{
  template<typename DATA>
  class Batcher
  {
  public:
    Batcher(void);

    void Initialize(RenderContext *renderContext, unsigned maxInstances);
    void Uninitialize(void);

    void SetVertexBuffer(const Handle& vbHandle);
    void SetVertexCount(unsigned count);
    void SetVertexStride(unsigned stride);

    void Begin(void);
    void Draw(DATA& instance);
    void End(void);

  private:
    RenderContext *m_renderContext;

    unsigned m_vertexCount;
    unsigned m_vertexStride;

    Handle            m_vertexBuffer;
    Handle            m_instanceBuffer;
    std::vector<DATA> m_instances;
    unsigned          m_maxInstances;
  };

#include "GFXBatcher.cpp"

} // namespace Graphics
