/*****************************************************************
Filename: light_ps.hlsl
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
struct VSOutput
{
  float4 position : SV_POSITION;
};

cbuffer LightBuffer : register(b0)
{
  float4 atten[100];
  float4 col[100];
  float4 pos[100];
  int numLights;
};

float4 combineLights(float4 position, int i)
{
  float dist = length(position - pos[i]);
  float at = col[i].a / (atten[i].x + atten[i].y * dist + atten[i].z * dist * dist);
  float4 newCol = col[i] * at;
  newCol.w = 1.0f;
  return saturate(newCol);
}

float4 PSMain(VSOutput input) : SV_TARGET
{
  float4 outputCol = float4(0.0f, 0.0f, 0.0f, 0.0f);

  for (int i = 0; i < numLights; ++i)
    outputCol += combineLights(input.position, i);

  return saturate(outputCol);
}