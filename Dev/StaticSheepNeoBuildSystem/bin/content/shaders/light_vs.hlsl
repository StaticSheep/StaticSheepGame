struct VSInput
{
  float4 : POSITION;
};

struct VSOutput
{
  float4 position : SV_POSITION;
};

cbuffer MatrixBuffer : register(cb0)
{
  matrix proj;
  matrix view;
  matrix world;
};

VSOutput VSMain(VSInput input)
{
  output.position = mul(input.position, world);
  output.position = mul(output.position, view);
  output.position = mul(output.position, projection);

  return output;
}