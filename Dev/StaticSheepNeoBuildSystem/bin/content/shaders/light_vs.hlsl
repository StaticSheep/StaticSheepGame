/*****************************************************************
Filename: light_vs.hlsl
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
struct VSInput
{
  float3 position : POSITION;
};

struct VSOutput
{
  float4 position : SV_POSITION;
  float4 worldPosition : POSITION0;
};

cbuffer MatrixBuffer : register(b0)
{
  matrix proj;
  matrix view;
  matrix world;
};

VSOutput VSMain(VSInput input)
{
  VSOutput output;
  output.position = mul(float4(input.position, 1), world);
  output.worldPosition = output.position;
  output.position = mul(output.position, view);
  output.position = mul(output.position, proj);

  return output;
}