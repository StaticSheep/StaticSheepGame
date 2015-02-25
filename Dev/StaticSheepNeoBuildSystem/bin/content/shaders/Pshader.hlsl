/*****************************************************************
Filename: Pshader.hlsl
Project:
Author(s): Scott Nelson (Primary)

All content � 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
cbuffer perFrame: register(b0)
{
  float4 cAmbientColor;
  float4 cAmbientIntensity;
}

struct PSInput
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;    // texture coordinates
};

Texture2D    g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

float4 PSMain(PSInput input) : SV_TARGET0
{
  float4 posCheck = input.position * 3;
  float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, input.texCoord);
  return vDiffuse + posCheck * 0;
}