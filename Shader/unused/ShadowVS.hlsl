// 頂点データの受取先
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
// ピクセルシェーダに渡すデータ
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	// 影として遮るテクスチャのどの位置を参照するか
	float4 shadowUV : TEXCOORD2;
};

// 定数バッファの受取先
cbuffer World : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// 影カメラから見た時の座標に変換
cbuffer ShadowViewProj : register(b1)
{
	float4x4 shadowVPS;	// ビュープロジェクションスクリーン
}

VS_OUT main(VS_IN vin)
{
	/*
	影をつけるためには、メインカメラの見ている位置を
	影カメラ(別カメラ)から見た時の位置に変換する。
	メイン) ワールド	> ﾒｲﾝ ビュー	> ﾒｲﾝ プロジェクション
	別カメ) ワールド	> 別ｶﾒビュー	> 別ｶﾒプロジェクション
	メインで描こうとしているピクセルを別カメの位置に計算しなおすことで、別カメらから見たときに遮られるかどうかを判断する
	*/
	
	VS_OUT vout;

	float4 wPos = float4(vin.pos, 1.0f);
	wPos = mul(wPos, world);
	
	// メインカメラの変換
	vout.pos = mul(wPos, view);
	vout.pos = mul(vout.pos, proj);

	// 影カメラの変換
	vout.shadowUV = mul(wPos, shadowVPS);
	
	// UV
	vout.uv = vin.uv;
	// 法線
	vout.normal = vin.normal;
	

	return vout;
}
