/*
もともと描画していた直線などはそのまま表示したい(アニメーションは必要ない
VertexShader.hlslを書き換えてアニメーションの処理を追加すると、
アニメーションしないモデルの表示がおかしくなるため、実現したい
機能ごとにシェーダーファイルを分ける(このファイルはアニメーション機能
*/
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

// モデルの変換行列(モデルごとに異なるため更新頻度が高い
cbuffer World : register(b0)
{
	float4x4 world;
};
// カメラの変換行列(1度の描画で使用されるカメラは基本１種類のため、
// 更新頻度は低い
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};
// モデルのアニメーション用の変換行列
// モデルごとに姿勢が異なるため、更新頻度が高い。
// だからといってWorldに全てまとめると、シェーダーを切り替えたときに
// 定数バッファの変更がかかるため、定数バッファを他のシェーダーと同じ
// 区切りにしておくと切り替えの手間が少なくなる
cbuffer Animation : register(b2)
{
	float4x4 bone[250];
};


VS_OUT main(IL_ANIME vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1);

	// ボーンの影響を計算して、アニメーションした座標位置を求める
	float4x4 anime =
		bone[vin.index.x] * vin.weight.x + 
		bone[vin.index.y] * vin.weight.y + 
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);

	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.color.xyz = vin.normal;
	vout.color.a = 1.0f;

	return vout;
}