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
  float4 atten;
  float4 col;
  float4 pos;
};

float4 PSMain(VSOutput input) : SV_TARGET
{
  float dist = length(input.position - pos);
  float at = col.a / (atten.x + atten.y * dist + atten.z * dist * dist);
  float4 newColor = col * at;

  newColor.w = 1;

  return saturate(newColor);
}