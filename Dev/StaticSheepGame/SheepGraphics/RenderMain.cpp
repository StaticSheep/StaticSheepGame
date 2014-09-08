
using namespace DirectX;

DirectSheep::ShapeStates SHAPESTATES;
DirectSheep::Camera *CAMERA = NULL;


#define RGBTOBGR(color) (color & 0xFF000000) | ((color & 0xFF0000) >> 16) | (color & 0x00ff00) | ((color & 0x0000ff) << 16)
namespace DirectSheep
{

  void SetupMatrices(void);

  GFX_API void StartFrame(float dt)
  {
    UNREFERENCED_PARAMETER(dt);

    CORE->devcon->ClearRenderTargetView(CORE->backbuffer, (D3DXCOLOR)Colors::Black);
    CORE->devcon->ClearDepthStencilView(CORE->depthbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);


    SetupMatrices();
  }

  void SetupMatrices(void)
  {
    float cameraX = 0.0f;
    float cameraY = 0.0f;
    float cameraZ = -400.0f;

    Vec3 eyepoint(cameraX, cameraY, cameraZ);

    Vec3 lootAtPoint(cameraX, cameraY, 0.0f);

    Vec3 upVector( 0.0f, 1.0f, 0.0f);

    Mat4 matView;
    D3DXMatrixLookAtLH(&matView, &eyepoint, &lootAtPoint, &upVector);

    CAMERA->ViewMatrix = matView;

    Mat4 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, (FLOAT)D3DXToRadian(75), CAMERA->ScreenDimensions.x / CAMERA->ScreenDimensions.y, 1.0f, 1000.0f);

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

    CORE->pVertexShader = newShader.VS;
    CORE->pPixelShader = newShader.PS;
    CORE->pInputLayout = newShader.Input;

    // set the shader objects
    CORE->devcon->VSSetShader(CORE->pVertexShader, 0, 0);
    CORE->devcon->PSSetShader(CORE->pPixelShader, 0, 0);
    CORE->devcon->IASetInputLayout(CORE->pInputLayout);
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

    D3DXMatrixTranslation(&transMat, floor(SHAPESTATES.position.x), floor(SHAPESTATES.position.y), floor(0.0f));


    D3DXMatrixMultiply(&scaleMat, &scaleMat, &transMat);

    matFinal = scaleMat * CAMERA->ViewProjMatrix;

    CORE->devcon->RSSetState(STATES->RState);

    CORE->devcon->PSSetSamplers(0, 1, &STATES->SamplerState);

    CORE->devcon->OMSetBlendState(STATES->BlendState, 0, 0xffffffff);

    UINT stride = sizeof(Vertex2D);
    UINT offset = 0;
    CORE->devcon->IASetVertexBuffers(0, 1, &QUAD->VertexBuffer, &stride, &offset);

    CORE->devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    CORE->devcon->UpdateSubresource(CORE->pCBuffer, 0, 0, &matFinal, 0, 0);

    CORE->devcon->PSSetShaderResources(0, 1, &TEXTUREMAP->TextureVec[0]);
    CORE->devcon->PSSetShaderResources(1, 1, &TEXTUREMAP->TextureVec[1]);

    CORE->devcon->Draw(6,0);
  }

void DrawSpriteText(const char * text, float size, const char * font)
{
    Mat4 matFinal;

    Mat4 rotMat, transMat;

    D3DXMatrixIdentity(&rotMat);
    D3DXMatrixIdentity(&transMat);

    D3DXMatrixRotationYawPitchRoll(&rotMat, 0.0f, -D3DX_PI, -SHAPESTATES.rotation);

    D3DXMatrixTranslation(&transMat, floor(0), floor(0), floor(0.0f));

    D3DXMatrixMultiply(&rotMat, &rotMat, &transMat);

    matFinal = rotMat * CAMERA->ViewProjMatrix;

    FW1_RECTF rect;
		rect.Left = rect.Right = 0.0f;
		rect.Top = rect.Bottom = 0.0f;

    std::string boop(text);

    std::wstring test(boop.begin(), boop.end());

    std::string sfont(font);
    std::wstring WFont(sfont.begin(), sfont.end());

    CORE->pFontWrapper->DrawString(
    CORE->devcon,
    test.c_str(),// String
    WFont.c_str(),
    size,
    &rect,
    RGBTOBGR(D3DXCOLOR(Colors::Purple)),// Text color, 0xAaBbGgRr
    NULL,
    matFinal,
    FW1_RESTORESTATE | FW1_CENTER | FW1_VCENTER | FW1_NOWORDWRAP
    );
}


};

