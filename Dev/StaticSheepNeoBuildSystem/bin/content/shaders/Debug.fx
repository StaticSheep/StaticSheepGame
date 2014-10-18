// Tutorial code
/*Texture2D SpriteSheet : register(t0);

SamplerState SpriteSampler : register(s0);

cbuffer CBText : register (b0)
{  
    float2 TextureSize : packoffset(c0.x);
    float2 ViewportSize : packoffset(c0.z);
}

struct VSInput
{
    // Vertex Data	float2 Position	: POSITION0;
    // Instance Data	float4 Color	: COLOR0;
    float4 Source	: SRCRECT;
    float4 Destination : DSTRECT;
};

struct PSInput
{
  float4 Position : SV_Position;
	float2 TexCoord : TEXCOORD;	
  float4 Color : COLOR;
};

PSInput VSText(VSInput input)
{
  PSInput output = (PSInput)0;
	float4 SourceRect = float4(input.Source.xy / TextureSize, input.Source.zw / TextureSize);
	float4 DestinationRect = float4(input.Destination.xy / ViewportSize, input.Destination.zw / ViewportSize);
	float2 OutPos = input.Position * DestinationRect.zw + DestinationRect.xy;	
  OutPos = OutPos * 2.0 - 1.0;
	OutPos.y = -OutPos.y;
	output.Position = float4(OutPos, 0, 1);
	output.TexCoord = input.Position * SourceRect.zw + SourceRect.xy;
	output.Color = input.Color;	
  return output;
}

float4 PSText(PSInput input) : SV_TARGET
{
	float4 Color = SpriteSheet.Sample(SpriteSampler, input.TexCoord);
	Color = Color * input.Color;
	Color.rgb *= Color.a;
	return Color;
}*/