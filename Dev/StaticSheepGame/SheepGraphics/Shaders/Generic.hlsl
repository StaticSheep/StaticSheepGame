cbuffer ConstantBuffer
{
    float4x4 matFinal;
}

Texture2D Texture : register(t0);
Texture2D boop : register(t1);

SamplerState ss : register(s0);

struct VertexShaderInput
{
  float3 position : POSITION0;
  float2 texcoord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;    // texture coordinates
};

PixelShaderInput VShader(VertexShaderInput input)
{
    PixelShaderInput output;

    output.position = mul(matFinal, float4(input.position, 1));

    output.texcoord = input.texcoord;    // set the texture coordinates, unmodified

    return output;
}

float4 PShader(PixelShaderInput input) : SV_TARGET
{
    float4 newcolor = Texture.Sample(ss, input.texcoord);
    return newcolor;
}
