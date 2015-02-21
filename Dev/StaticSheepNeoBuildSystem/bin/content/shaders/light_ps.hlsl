/*****************************************************************
Filename: light_ps.hlsl
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
struct VSOutput
{
  float4 position : SV_POSITION;
  float4 worldPosition : POSITION0;
};

cbuffer LightBuffer : register(b0)
{
  float4 atten[100];
  float4 col[100];
  float4 pos[100];
  float4 screenScale;
  int numLights;
};

float4 combineLights(float4 position, int i)
{
  //float lightAngle = atan2(pos[i].y, pos[i].x) - atan2(position.y, position.x);
  //float3 dist = float3(0, 0, length(position - pos[i]));
  //dist.x = dist.z * cos(lightAngle) * screenScale.r;
  //dist.y = dist.z * sin(lightAngle) * screenScale.g;
  //dist.z = sqrt(dist.x * dist.x + dist.y * dist.y);
  float dist = length(position - pos[i]);
  float at = col[i].a / (atten[i].x + (atten[i].y * dist)
    + (atten[i].z * dist * dist));
  float4 newCol = col[i] * at;
  newCol.w = 1.0f;
  return saturate(newCol);
}

float4 PSMain(VSOutput input) : SV_TARGET
{
  float4 outputCol = float4(0.0f, 0.0f, 0.0f, 0.0f);

  for (int i = 0; i < numLights; ++i)
  {
    outputCol += combineLights(input.position, i);
  }

  outputCol.w = 1.0f;

  return saturate(outputCol);
}