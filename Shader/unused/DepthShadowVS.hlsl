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
	float4 shadowUV : TEXCOORD1;
};

// いつも通り、ゲーム画面に表示する行列
cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
// 光源から見た景色で表示するための行列
cbuffer ShadowViewProj : register(b1)
{
	float4x4 shadowVPS;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	// ワールド行列計算後、ライト空間の行列をかける
	vout.shadowUV = mul(vout.pos, shadowVPS);
	// いつも通り、画面へ出力するためのカメラ行列をかける
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	return vout;
}