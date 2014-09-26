cbuffer DefaultBuffer
{
  float4x4 cWorld;
  float4x4 cWorldViewProj;
  float4 cBlendColor;
}

Texture2D gTexture : register(t0);

SamplerState gSampler : register(s0);

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

PSInput VShader(VSInput input)
{
  PSInput output;
  output.position = mul(cWorldViewProj, float4(input.position,1));
  output.texCoord = input.texCoord;    // set the texture coordinates, unmodified

  return output;
}

float4 PShader(PSInput input) : SV_TARGET
{
  return cBlendColor * gTexture.Sample(gSampler, input.texCoord);
}
