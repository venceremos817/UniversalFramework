struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	color = tex.Sample(samp, pin.uv);

	// セピア変換
	float mono = color.r * 0.3f + color.g * 0.6f + color.b * 0.3f;
	color.rgb = mono;

	color.r *= 1.0f;	// 赤色はそのまま
	color.g *= 0.7f;	// その他の成分を減らす
	color.b *= 0.4f;	// 青色は暗い色で変化がわかりづらいので大きく減らす
	
	return color;
}
