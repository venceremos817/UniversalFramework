struct IL_PCUN
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
struct IL_PU
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
};
struct IL_ANIME
{
	float3 pos : POSITION0;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float4 weight : WEIGHT0;
	uint4 index : INDEX0;
};