#include "INCStructs.hlsli"
#include "SimpleConstantBuffer.hlsli"


PS_PNT_Input main(VS_PNT_Input vin)
{
	PS_PNT_Input result;
	// 頂点の位置を変換
	float4 pos = float4(vin.position.xyz, 1.0f);
	// ワールド変換
	pos = mul(pos, World);
	// ビュー変換
	pos = mul(pos, View);
	// 射影変換
	pos = mul(pos, Projection);

	// ピクセルシェーダに渡す変数に設定
	result.position = pos;
	// 法線
	result.normal = mul(vin.normal, (float3x3) World);
	result.normal = normalize(result.normal);
	// スペキュラー
	float3 H = normalize(normalize(-LightDir.xyz) + normalize(EyePos.xyz - pos.xyz));
	result.specular = Specular * dot(result.normal, H);
	result.specular *= Specular.a;
	// テクスチャUV
	result.tex = vin.tex /** UVScale*/;
	
	return result;
}