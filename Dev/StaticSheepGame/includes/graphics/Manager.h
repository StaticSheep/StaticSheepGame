//#pragma once
//
//#include "Context.h"
////#include "Sprite.h"
//
//namespace DirectSheep
//{
//
//  class GFXManager
//  {
//    public:
//
//
//      //Actually goes through and calls all draw calls
//      //and then sends to screen
//
//      
//      GFXRenderer* m_renderer;
//
//    private:
//      
//      TwBar *m_graphicsBar = nullptr;
//      bool m_debugDraw = false;
//
//      //To be called from Present function
//
//
//      //May not be void in future
//      //Constant buffers hold camera and world matrix
//      //Proj, View, ViewProj Matrix
//
//      struct SpriteBatch
//      {
//        std::vector<Instance> instanceData;
//        Handle texture;
//      };
//
//      std::vector<Handle> m_handles;
//      std::vector<Handle> m_instanceBuffers;
//      std::vector<SpriteBatch> m_spriteBatches;
//
//  };
//
//} //namespace Graphics
