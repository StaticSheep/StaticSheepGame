
using namespace DirectX;

DirectSheep::ShapeStates SHAPESTATES;
DirectSheep::Camera *CAMERA = NULL;

namespace DirectSheep
{

  void SetupMatrices(void);

  GFX_API void StartFrame(float dt)
  {
    UNREFERENCED_PARAMETER(dt);

    CORE->devcon->ClearRenderTargetView(CORE->backbuffer, (D3DXCOLOR)Colors::Black);
    CORE->devcon->ClearDepthStencilView(CORE->zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

    SetupMatrices(); //Do we even need to do this every frame?
  }

  void SetupMatrices(void)
  {
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = -10.0f;

    Vec3 eyepoint(cameraX, cameraY, cameraZ);

    Vec3 lootAtPoint(cameraX, cameraY, 0.0f);

    Vec3 upVector( 0.0f, 1.0f, 0.0f);

    Mat4 matView;
    D3DXMatrixLookAtLH(&matView, &eyepoint, &lootAtPoint, &upVector);

    CAMERA->ViewMatrix = matView;

    Mat4 matProj;
    D3DXMatrixOrthoLH(&matProj, CAMERA->ScreenDimensions.x, CAMERA->ScreenDimensions.y, 1.0f, 100.0f);
    //XMMatrixOrthographicOffCenterLH(0, ScreenWidth, ScreenHeight, 0, 1.0f, 100.0f);

    CAMERA->ProjMatrix = matProj;

    CAMERA->ViewProjMatrix = matView * matProj;
  }

  GFX_API void FinishFrame()
  {
    CORE->swapchain->Present(0, 0);
  }

  GFX_API void SetShader(const std::string& name)
  {
    DXShader& newShader = SHADERMAP[name];

    CORE->pVS = newShader.VS;
    CORE->pPS = newShader.PS;
    CORE->pLayout = newShader.Input;

    // set the shader objects
    CORE->devcon->VSSetShader(CORE->pVS, 0, 0);
    CORE->devcon->PSSetShader(CORE->pPS, 0, 0);
    CORE->devcon->IASetInputLayout(CORE->pLayout);
  }


  GFX_API void SetPosition(float x, float y)
  {
    SHAPESTATES.position = Vec2(x, y);
  }

  GFX_API void SetRotation(float theta)
  {
    SHAPESTATES.rotation = theta;
  }

  GFX_API void SetSize(float x, float y)
  {
    SHAPESTATES.scale = Vec2(x, y);
  }

  GFX_API void SetTexture(int ID)
  {
    SHAPESTATES.TexID = ID;
  }

  GFX_API void SetUseCamera(bool useCam)
  {
    SHAPESTATES.useCamera = useCam;
  }
  
  GFX_API void DrawSprite()
  {
    Mat4 matFinal;

    D3DXMATRIX scaleMat, rotMat, transMat;

    D3DXMatrixIdentity(&rotMat);
    D3DXMatrixIdentity(&transMat);
    D3DXMatrixIdentity(&scaleMat);

    D3DXMatrixScaling(&scaleMat,SHAPESTATES.scale.x, SHAPESTATES.scale.y, 1.0f);
    D3DXMatrixRotationZ(&rotMat, SHAPESTATES.rotation);
    D3DXMatrixMultiply(&scaleMat, &scaleMat, &rotMat);

    if (SHAPESTATES.useCamera)
      D3DXMatrixTranslation(&transMat, floor(SHAPESTATES.position.x), floor(SHAPESTATES.position.y), floor(0.0f));
    else
      D3DXMatrixTranslation(&transMat, floor(SHAPESTATES.position.x) - ScreenWidth / 2, floor(-SHAPESTATES.position.y) + ScreenHeight / 2, floor(0.0f));

    D3DXMatrixMultiply(&scaleMat, &scaleMat, &transMat);

    matFinal = scaleMat * CAMERA->ViewProjMatrix;

    CORE->devcon->RSSetState(STATES->RSDefault);

    CORE->devcon->PSSetSamplers(0, 1, &STATES->SS);

    CORE->devcon->OMSetBlendState(STATES->BS, 0, 0xffffffff);

    UINT stride = sizeof(Vertex2D);
    UINT offset = 0;
    CORE->devcon->IASetVertexBuffers(0, 1, &QUAD->vBuffer, &stride, &offset);

    CORE->devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CORE->devcon->UpdateSubresource(CORE->pCBuffer, 0, 0, &matFinal, 0, 0);
    CORE->devcon->PSSetShaderResources(0, 1, &TEXTUREMAP->TextureVec[SHAPESTATES.TexID]);
    CORE->devcon->Draw(6, 0);
  }

  


};