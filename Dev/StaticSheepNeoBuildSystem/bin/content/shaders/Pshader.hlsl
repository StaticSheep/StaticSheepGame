cbuffer perFrame: register(b1)
{
  float4 cAmbientColor;
  float cAmbientIntensity;
}
Texture2D    g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

float4 PSMain(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_TARGET0
{
  float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, texCoord);
  return cAmbientColor * cAmbientIntensity * color * vDiffuse;
}