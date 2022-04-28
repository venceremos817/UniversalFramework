struct PS_IN
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 shadowUV : TEXCOORD2;
};

Texture2D shadowMap : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// 3Dの場合遠くの景色ほど小さくなるので、wで割ることで正しいパラメータをとれる
	float2 uv = pin.shadowUV.xy / pin.shadowUV.w;

	if (0.0f <= uv.x && uv.x < 1.0f &&
		0.0f <= uv.y && uv.y <= 1.0f)
	{
		/*
		投影テクスチャシャドウ
		テクスチャに書き込まれているか、そうでないかのみを判断して、オブジェクトに影を付ける手法
		複雑な計算もないため、影を付ける処理では軽い手法である
		ただし、正確に計算しないため、キャラクターの足元だけでなく、周辺の壁や天井にも同様の影が表示されることがある。
		屋外などの開けたゲームであれば利用できる
		*/
		color = shadowMap.Sample(samp, uv);
		color.rgb = step(color.a, 0.8f);
		color.a = 1.0f;
	}
	return color;
}
