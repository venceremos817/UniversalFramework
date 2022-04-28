// 頂点データの受取先
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
};
// ピクセルシェーダに渡すデータ
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;	 
	float3 texSpaceLight : TEXCOORD2; //テクスチャ空間上のライトの向き
};

// 定数バッファの受取先
cbuffer World : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// ライトの情報
cbuffer Light : register(b1)
{
	float4 direction;
};

// エントリポイント
VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.uv = vin.uv;
	vout.normal = vin.normal;
	
	//--- ワールド空間上のライトをテクスチャ空間に置きなおす
	// 法線とベクトルから従法線を求める
	float3 N = normalize(vin.normal);
	float3 T = normalize(vin.tangent);
	float3 B = normalize(cross(N, T));
	// テクスチャ空間からワールド空間に変換する姿勢行列を求め、ライトに対してその行列をかける
	// 姿勢行列の逆行列は転地で求めることができる
	float3x3 invTexToWorld = float3x3(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
		);
	// ライトをテクスチャ空間に変換
	float3 L = normalize(direction.xyz);
	vout.texSpaceLight = mul(L, invTexToWorld);

	
	return vout;
}
