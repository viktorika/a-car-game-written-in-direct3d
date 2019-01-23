Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);
cbuffer worldCb : register(b0)
{
	matrix worldMatrix;
};
cbuffer viewCb : register(b1)
{
	matrix viewMatrix;
};
cbuffer projCb : register(b2)
{
	matrix projMatrix;
};
cbuffer textureCb:register(b3) {
	matrix textMatirx;
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
	vsOut.pos = mul(vertex.pos, worldMatrix);
	vsOut.pos = mul(vsOut.pos, viewMatrix);
	vsOut.pos = mul(vsOut.pos, projMatrix);
	vsOut.tex0 = mul(textMatirx, float4(vertex.tex0, 0.0f, 1.0f));
	return vsOut;
}
