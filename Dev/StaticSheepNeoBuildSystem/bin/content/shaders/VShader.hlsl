cbuffer DefaultBuffer: register(b0)
{
  float4x4 cWorld;
  float4x4 cWorldViewProj;
  float4 cBlendColor;
  float2 cuvBegin;
  float2 cuvEnd;
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
  output.position = mul(cWorldViewProj, float4(input.position, 1));
  output.texCoord = lerp(cuvBegin, cuvEnd, input.texCoord);    // set the texture coordinates, unmodified

  return output;
}
