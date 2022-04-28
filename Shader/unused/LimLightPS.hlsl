/*	リムライト
オブジェクトの後ろから強力な光が当たった時、オブジェクトの縁の部分が光って見えるような表現
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// ライトの情報
cbuffer Light : register(b0)
{
	float4 direction;
}

cbuffer Camera : register(b1)
{
	float4 eyePos;
}

float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//color = tex.Sample(samp, pin.uv);

	//--- 計算に必要なパラメータ
	float3 L = -normalize(direction.xyz);
	float3 V = normalize(pin.worldPos - eyePos.xyz);
	float3 N = normalize(pin.normal);

	// リムライト
	float lim = dot(L, V);
	lim = pow(lim, 5); // カメラとライトが正対している時に効果が現れるよう調整
	float edge = dot(N, V) + 1.0f;

	//--- ランバート計算
	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb = lim * edge + diffuse;
	
	return color;
}
