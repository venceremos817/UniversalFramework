// 頂点シェーダから渡されるデータ
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;	// テクスチャ空間上のライトの向き
};

// テクスチャ
Texture2D baseTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samp : register(s0);



float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// バンプマッピングはテクスチャに保存されているデータを法線ベクトルとして扱う
	float3 N = normalTex.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	// -1 ~ 1 に変換
	// 本来であれば、法線マップの法線をワールド空間に変換するが、
	// ピクセルシェーダでその処理を行うと計算量が増える。
	// そのため、ワールド空間のライトをテクスチャ空間に移動させて計算を行う。
	// ライトをテクスチャ空間に移動させる処理は頂点シェーダで行うため、計算量が少なくなる。
	float3 L = normalize(pin.texSpaceLight);
	
	// ランバートの計算で影をつける
	float diffuse = dot(N, -L);	
	diffuse = (diffuse + 1.0f) * 0.5f;
	color.rgb *= diffuse;
	
	return color;
}