bool GraphicsManager::Present(float stepInterpolation)
{
  m_stepInterpolation = stepInterpolation;
 
  // For each active viewframe
  for(unsigned i = 0; i < m_activeViewframes; ++i)
  {
    //
    // Draw Setup
    //
 
    // Set viewport for viewframe
    m_renderContext.SetViewport(0, 0, m_resolutions[m_viewframes[i].type]);
 
    // Clear Render Targets for viewframe
    ClearRenderTargets(m_viewframes[i]);
 
    // Update Per Frame Buffer
    UpdatePerFrameConstantBuffer(m_viewframes[i], stepInterpolation);
 
    //
    // Perform Draw Calls
    //
    for(auto layer = m_layers.rbegin(); layer != m_layers.rend(); ++layer)
    {
      //
      //  Sprites
      //
      BeginSpriteDraw(m_viewframes[i]);
      DrawSprites(*layer);
      EndSpriteDraw();
 
      //
      // Lighting
      //
      if(m_lighting)
      {
        BeginLightDraw(m_viewframes[i]);
        DrawLights(*layer);
        EndLightDraw();
      }
 
      //
      // Distortion Maps
      //
      BeginDistortionDraw(m_viewframes[i]);
      DrawDistortionMaps(*layer);
      EndDistortionDraw();
 
      //
      // God Rays
      //
      if(m_godRay)
      {
        BeginGodRayDraw(m_viewframes[i]);
        DrawGodRaySource(*layer);
        EndGodRayDraw();
      }
 
      //
      // Final Pass
      //
      LayerFinalPass(m_viewframes[i]);
    } // End layers
 
    //
    // Post Processing
    //
    if(m_godRay)
      ApplyGodRays(m_viewframes[i]);
 
    if(m_bloom)
      Bloom(m_viewframes[i]);
 
    Distort(m_viewframes[i]);
 
 
    // Copy final render target to screen target
    m_renderContext.SetViewport(m_viewframes[i].viewport);
    CopyRenderTarget(m_Screen_rt, m_renderTargets[m_finalRenderTarget].rt[m_viewframes[i].type], "Basic_ps", false);
 
  } // End Viewframes
 
  // You can do stuff to the final screen image here before it is drawn to the back buffer
 
  // Copy screen target to back buffer
  m_renderContext.SetBlendMode(BLEND_MODE_ALPHA);
  m_renderContext.SetViewport(m_mainViewport);
  CopyRenderTarget(m_renderContext.GetBackBuffer(), m_Screen_rt, "Basic_ps");
  m_renderContext.BindRenderTarget(m_renderContext.GetBackBuffer());
 
  //
  // Draw Text
  //
  DrawText();
 
  //
  // Draw Tweak Bar
  //
  TwDraw();
 
  //
  // Present Back Buffer To Front Buffer
  //
  m_renderContext.Present();
 
  //Success
  return true;
}*/