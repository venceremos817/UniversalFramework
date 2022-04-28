struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// ライト
struct Light
{
	float3 pos;
	float range;
	float4 color;
};
cbuffer Lights : register(b0)
{
	Light lights[30];
};

cbuffer Matrix : register(b1)
{
	float4x4 invVPS;
};

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D depthTex : register(t2);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// GBufferから各種情報を取得
	color = albedoTex.Sample(samp, pin.uv);
	float3 N = normalTex.Sample(samp, pin.uv).rgb;
	float depth = depthTex.Sample(samp, pin.uv).r;
	
	// 深度値からワールド座標を再計算
	// もともと深度値はワールド座標をスクリーン上の座標に変換したときに、あるピクセルの奥行情報を保存している。
	// 画面に表示されるまでは、 ビュー > プロジェクション > スクリーン と行列で変換を行っている。
	// スクリーン上の点に対してこの変換処理の逆手順(逆行列)を行ってやれば、ワールド座標を求めることができる。
	// このとき、あるピクセルの座標はディファードで画面全体に表示するので、UVの値を座標と置くことができる
	float4 worldPos = float4(pin.uv, depth, 1.0f);
	worldPos = mul(worldPos, invVPS);
	worldPos.xyz /= worldPos.w;

	// 計算したワールド座標をもとに点光源の処理を行う
	float4 pointColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 30; ++i)
	{
		float3 lightDir = worldPos.xyz - lights[i].pos;
		float distance = length(lightDir);
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		attenuation = pow(attenuation, 3.0f);
		
		float4 lightColor = lights[i].color * attenuation;
		pointColor += lightColor;
	}
	color.rgb = pointColor.rgb;
	

	return color;
}