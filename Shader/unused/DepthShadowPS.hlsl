struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 shadowUV : TEXCOORD1;
};

Texture2D tex : register(t0);
// 影の情報が格納されたテクスチャ
Texture2D shadowMap : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// 光源で描きこんだ情報を取得するため、該当のピクセルが
	// UV座標のどの位置か計算
	float2 uv = pin.shadowUV.xy / pin.shadowUV.w;
	// 光源から見た時に、該当のピクセルがカメラからどの程度離れているか
	float uvDepth = pin.shadowUV.z / pin.shadowUV.w;

	// 事前に書き込んでいたカメラからの距離を取得
	float shadowDepth = shadowMap.Sample(samp, uv).r;
	if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
	{
		// シャドウアクネ
		// 深度バッファシャドウを実装したときに、表面に出来る波打ったような模様
		// カメラからの距離の精度はテクスチャの解像度によって異なる(512x512と1024x1024では粗さが異なる)
		// 実際の描画はピクセルシェーダで細かく計算されているため、計算された値とテクスチャに描き込まれていた情報では微妙な差が出てしまう。
		// その結果、本来影ができない場所に波打ったような模様が浮かび上がる。
		// これを回避するためには、カメラからの距離を少し奥にずらしてあげるだけで解決できる。
		// ※細かい誤差で生じる模様のため、誤差が生じないように値を大きく
		if (shadowDepth + 0.001f < uvDepth)
			color.rgb = 0;
	}
	//color.rgb = shadowDepth;
	return color;
}