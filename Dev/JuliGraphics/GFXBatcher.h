// ------------------------------------------------------------------------------------
//  File     : GFXInstancer.h
//  Author   : Kai-Hwa Yao
//  Copyright: All content © 2013-2014 DigiPen (USA) Corporation, all rights reserved.
// ------------------------------------------------------------------------------------

#pragma once

#include "RenderContext.h"

#include "GFXHandle.h"
#include "GFXTypes.h"

namespace Graphics
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
