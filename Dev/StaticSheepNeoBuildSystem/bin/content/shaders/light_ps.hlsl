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

cbuffer LightBuffer : register(cb0)
{
  float4 atten;
  float4 col;
  float4 pos;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
  float diff = length(input.position - pos);
  float at = 1. / (atten.x + atten.y * diff + atten.z * diff * diff);
  float4 newColor = col * at;

  newColor.w = 1.f;

  return saturate(newColor);
}