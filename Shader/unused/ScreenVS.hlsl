struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer Matrix : register(b0)
{
	float4x4 screen;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, screen);

	vout.uv = vin.uv;

	return vout;
}