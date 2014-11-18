cbuffer DefaultBuffer: register(b0)
{
  float4x4 cProj;
  float4x4 cView;
  float4x4 cWorld;
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
  output.position = mul(float4(input.position, 1), cWorld);
  output.position = mul(output.position, cView);
  output.position = mul(output.position, cProj);
  output.texCoord = lerp(cuvBegin, cuvEnd, input.texCoord);    // set the texture coordinates, unmodified

  return output;
}
