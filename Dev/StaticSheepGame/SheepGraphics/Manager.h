#pragma once

#include "GFXRenderer.h"
#include "Sprite.h"

namespace DirectSheep
{

  class GFXManager
  {
    public:
      GFXManager();
      ~GFXManager();

      bool Initialize(WindowSystem& Window);

      //Actually goes through and calls all draw calls
      //and then sends to screen
      bool Present(void);

      void UpdateWindowSize(LPARAM);
      
      GFXRenderer* m_renderer;

    private:
      
      TwBar *m_graphicsBar = nullptr;
      D3DXCOLOR m_clearColor = D3DXCOLOR(.508f, .508f, .508f, 1.0f);
      bool m_debugDraw = false;

      //To be called from Present function
      void BeginSpriteDraw(void);
      void DrawSprites(void);
      void EndSpriteDraw(void);

      Instance CreateInstance(Sprite& currentSprite);

      //May not be void in future
      //Constant buffers hold camera and world matrix
      //Proj, View, ViewProj Matrix
      //void UpdatePerFrameConstantBuffer(void);

      struct SpriteBatch
      {
        std::vector<Instance> instanceData;
        Handle texture;

        void ClearBatch(Handle& tHandle)
        {
          instanceData.clear();
          texture = tHandle;
        }
      };

      std::vector<Handle> m_handles;
      std::vector<Handle> m_instanceBuffers;
      std::vector<SpriteBatch> m_spriteBatches;

  };

} //namespace Graphics