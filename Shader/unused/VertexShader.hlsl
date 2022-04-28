#include "InputLayout.hlsli"

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
	float4 vPos : TEXCOORD4;
};

cbuffer World : register(b0)
{
	float4x4 world;
};
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};

VS_OUT main(IL_PCUN vin)
{
	VS_OUT vout;

	// float4に格納するため、
	// float3のデータをfloat4に変換
	vout.pos = float4(vin.pos, 1);

	// ローカル座標からスクリーン座標への変換
	// ローカル→ワールド→カメラ→ﾌﾟﾛｼﾞｪｸｼｮﾝ→ｽｸﾘｰﾝ
	// 定数バッファの中身はグローバル変数のように扱える
	// mul ... 3D座標と行列の掛け算を行う
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// CPUから受け取った頂点情報を
	// ピクセルシェーダに渡すためにコピー
	vout.uv = vin.uv;

	return vout;
}