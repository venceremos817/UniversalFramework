struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 wPos : TEXCOORD0;
};

// ライトの情報
struct Light
{
	float3 pos;
	float range;
	float4 color;
	// スポットライトに追加されたパラメータ
	float3 dir; // 向き
	float angle; // 照らす範囲
};

cbuffer Lights : register(b0)
{
	Light lights[30];
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 spotColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 30; ++i)
	{
		// 点光源の処理と同じく、距離や減衰率を求める
		float3 lightDir = pin.wPos.xyz - lights[i].pos;
		float distance = length(lightDir);
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		
		// 光は距離の2乗で減衰する(逆2乗の法則)ため、
		// 計算した減衰率を乗算する
		attenuation = pow(attenuation, 3.0f);
		
		//--- 点光源で求めた減衰率にスポットライトの処理を適用する
		// スポットライトの範囲におさまっているか
		// または、範囲外は減衰率を0にする(光が届かない
		
		// 点光源からピクセルに向かうベクトルと、スポットライトの向きのベクトル
		// 二つのベクトルの間の角度がテラス範囲内であればスポットライトで照らせる範囲であるといえる
		// 内積の公式を使って、二つのベクトルのなす角を求める
		// a・b (二つのベクトルの内積結果)
		// |a| (ベクトルaの長さ)
		// Θ (二つのベクトルのなす角)
		// a・b = |a| * |b| * cosΘ
		// a・b / (|a| * |b|) = cosΘ
		// acos( a・b / ( |a| * |b| ) ) = Θ
		// aとbのベクトルの長さが1であれば
		// acos( a・b / ( 1 * 1 ) ) = Θ
		// acos( a・b ) = Θ
		float3 L = normalize(lights[i].dir);
		float3 V = normalize(lightDir);
		float vecAngle = acos(dot(L, V));

		// スポットライトの角度は端から端までの範囲
		// 二つのベクトルの角度はスポットライトの向きからベクトルまで
		// スポットライトの向き(中心)から端までの角度で計算したいので、
		// 照らせる範囲は半分にして計算する
		float angleAttenuation = vecAngle / (lights[i].angle * 0.5f);
		angleAttenuation = saturate(angleAttenuation);
		angleAttenuation = 1.0f - pow(angleAttenuation, 5.0f);
		attenuation *= angleAttenuation;

		// 照らす、照らさないで計算するので、自然には見えない
		//if (lights[i].angle * 0.5f < vecAngle)
		//	attenuation = 0.0f;
		
		// 各スポットライトを加算していく
		float4 lightColor = lights[i].color * attenuation;
		spotColor += lightColor;
	}
	
	color.rgb = spotColor.rgb;

	return color;
}