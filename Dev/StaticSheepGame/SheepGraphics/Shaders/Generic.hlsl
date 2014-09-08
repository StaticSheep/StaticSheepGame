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
  float4 worldPos : POSITION;
};

PSInput VShader(VSInput input)
{
  PSInput output;

  output.position = mul(cWorldViewProj, float4(input.position, 1));
  output.worldPos = mul(cWorld, float4(input.position, 1));
  output.texCoord = input.texCoord;    // set the texture coordinates, unmodified

  return output;
}

float4 PShader(PSInput input) : SV_TARGET
{
  //float3 normal = float3(0, 0, 1);
  //float3 lightDir = normalize(input.worldPos.xyz - cLightPos.xyz);
  //float4 diff = saturate(dot(normal, -lightDir));
  

  //float diffuseLighting = saturate(dot(normal, -lightDir));

  //diffuseLighting *= (1 / dot(lightDir, lightDir));

  //float3 h = normalize(normalize(cCameraPos - input.worldPos.xyz) - lightDir);
  
  float4 texCol = gTexture.Sample(gSampler, input.texCoord);

  //return float4(lightDir, 1);
  //return float4(texCol.xyz * diffuseLighting, 1);

  /*return float4(saturate(float4(0, 0, 0, 1) + (texCol *
  float4(1, 1, 1, 1) * diffuseLighting * 0.6)));*/

  return texCol * cBlendColor;
}
