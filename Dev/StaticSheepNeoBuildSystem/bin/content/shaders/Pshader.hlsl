cbuffer perFrame: register(b0)
{
  float4 cAmbientColor;
  float4 cAmbientIntensity;
}

struct PSInput
{
  float4 position : SV_POSITION;
  float2 texCoord : TEXCOORD0;    // texture coordinates
};

Texture2D    g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

float4 PSMain(PSInput input) : SV_TARGET0
{
  //input.texCoord.x = 1 - input.texCoord.x;
  float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, input.texCoord);
  return cAmbientColor * cAmbientIntensity.r * vDiffuse;
}