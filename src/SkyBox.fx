Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);
cbuffer cbNeverChanges : register(b0)
{
	matrix viewMatrix;
};
cbuffer cbChangeOnResize : register(b1)
{
	matrix projMatrix;
};
struct VS_Input
{
	float4 pos : POSITION;
	float2 tex0 : TEXCOORD0;
};
struct PS_Input
{
	float4 pos : SV_POSITION;
	float2 tex0 : TEXCOORD0;
};
PS_Input VS_Main(VS_Input vertex)
{
	PS_Input vsOut = (PS_Input)0;
	vsOut.pos = vertex.pos;
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);
	vsOut.tex0 = vertex.tex0;
	return vsOut;
}
float4 PS_Main(PS_Input frag) : SV_TARGET
{
	return colorMap_.Sample(colorSampler_, frag.tex0);
}