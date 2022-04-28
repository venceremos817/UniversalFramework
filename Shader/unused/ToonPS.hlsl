struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
	float4 direction;
};


float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float diffuse = dot(pin.normal, -direction.xyz);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	/* トゥーンレンダリング(セルシェーディング)
	実際は3Dだが、あたかも2Dのように見せかける手法。
	表示されるオブジェクトの影をアニメ塗りのように表現する。(日本の作品に多く見られる表現)
	今までの表現では明るい部分(1)から暗い部分(0)へ徐々に数字が変化していたが、トゥーンレンダリングでは
	徐々に変化させずに、段階的(2～4段階)になるように数字を計算する
	*/
	
	// ①ピクセルシェーダ内で段階的な数字に計算
	// step... 指定された数字より大きければ 1
	//float highLight = step(0.8f, diffuse) * 0.3f;
	//float baseLight = step(0.4f, diffuse) * 0.4f;
	//float shadowLight = step(0.0f, diffuse) * 0.3f;
	// diffuse = highLight + baseLight + shadowLight;
	
	
	
	// ②画像ファイルから段階的な影を表現
	// ランプテクスチャと呼ばれる画像から明るさを取得する。
	// 光源と法線から計算した明るさは1～0の間で表現するため、この値をUVの参照値に使うと
	// ランプテクスチャから明るさが取得できる
	diffuse = rampTex.Sample(samp, float2(diffuse * 0.98 + 0.01f, 0.5f)).r;
	
	color.rgb *= diffuse;
	return color;
}