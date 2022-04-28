struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	//color.rgb = step(0.65f, tex.Sample(samp, pin.uv).r);
	color = tex.Sample(samp, pin.uv);

	return color;
}
