cbuffer ConstantBuffer
{
    float4x4 matFinal;
}

Texture2D Texture;
SamplerState ss;

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;    // texture coordinates
};

VOut VShader(float3 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
    VOut output;

    output.position = mul(matFinal, float4(position, 1));

    output.color = color;

    output.texcoord = texcoord;    // set the texture coordinates, unmodified

    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float4 newcolor = color * Texture.Sample(ss, texcoord);
    return newcolor;
}
