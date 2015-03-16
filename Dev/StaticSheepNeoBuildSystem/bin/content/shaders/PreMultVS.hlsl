/*****************************************************************
Filename: VShader.hlsl
Project:
Author(s): Scott Nelson (Primary)

All content © 2014 DigiPen (USA) Corporation, all rights reserved.
*****************************************************************/
cbuffer DefaultBuffer: register(b0)
{
  float4x4 cScale;
}

struct VSInput
{
  float3 position : POSITION;
  float2 texCoord : TEXCOORD0;
};

struct PSInput
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;    // texture coordinates
};

PSInput VSMain(VSInput input)
{
  PSInput output;
  output.position = mul(float4(input.position, 1), cScale);
  output.texCoord = input.texCoord;    // set the texture coordinates, unmodified
  
  return output;
}
