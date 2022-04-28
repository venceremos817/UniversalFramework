struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

// CPUから渡されたテクスチャを
// 利用するための宣言
Texture2D tex : register(t0);
// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
	float4 color;
	// ①単色で表示
	color = float4(1, 0, 0, 1);
	color = pin.color;
	// ②UVを色情報として出力
	// color = float4(pin.uv, 0, 1);
	// ③テクスチャを張り付けて表示
	//color = tex.Sample(samp, pin.uv);
	return color;
	/*
	float4 color = tex.Sample(samp, PIN.uv);
	color *= PIN.color;
	return color;
	*/
}
