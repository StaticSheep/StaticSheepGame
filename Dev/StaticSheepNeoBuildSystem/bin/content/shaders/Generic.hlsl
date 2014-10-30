cbuffer DefaultBuffer
{
  float4x4 cWorld;
  float4x4 cWorldViewProj;
  float4 cBlendColor;
  float2 cuvBegin;
  float2 cuvEnd;
}

float4 AmbientColor = float4(1, 1, 1, 1);
float AmbientIntensity = 0.1;

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
  output.texCoord = lerp(cuvBegin, cuvEnd, input.texCoord);    // set the texture coordinates, unmodified

  return output;
}

float4 PShader(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_TARGET0
{
  return gTexture.Sample(gSampler, texCoord) * AmbientColor * AmbientIntensity;
}

technique Ambient
{
  pass Pass1
  {
    VertexShader = compile vs_4_0 VShader();
    PixelShader = compile ps_4_0 PShader();
  }
}
