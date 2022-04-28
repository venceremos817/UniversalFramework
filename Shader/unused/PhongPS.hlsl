/*	Phongシェーディング
Lambertシェーディングに鏡面反射(光の反射)を加えたシェーディング
光が反射してまぶしく見えるためには、その光を観測する人が必要(反射する角度によってはまぶしく感じない)
3Dゲームではこの観測する人はカメラのことを差す。
光の反射なので、太陽の光は一度オブジェクトにぶつかって跳ね返らないといけない。
跳ね返り方はオブジェクトのぶつかった位置によって異なる。
そのため、表示されるピクセルごとに反射した光を求める。
この反射した光のベクトルと、カメラの視線のベクトルの向きがまっすぐすれ違うような場合に、オブジェクトが光を反射してまぶしく見える。
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

// ライト
cbuffer Light : register(b0)
{
	float4 direction; // 16B区切りでないといけない
};

// カメラ
cbuffer Camera : register(b1)
{
	float4 eyePos;
};



float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	color = tex.Sample(samp, pin.uv);
	
	// 計算に必要なデータ
	float3 L = normalize(direction.xyz); // 光源の向き
	float3 V = normalize(eyePos.xyz - pin.worldPos);
	
	// 反射ベクトル計算
	float3 R = reflect(L, normalize(pin.normal));
	R = normalize(R);

	//--- 鏡面反射の計算
	// 光が当たってまぶしく見える部分…スペキュラー(specular
	float specular = dot(V, R) * 0.5f + 0.5f;
	
	// そのままでは広範囲が明るく見えてしまうので、抑える必要がある。
	// 線形で明るさが変化するのではなく、カーブを描くような変化が望ましい
	// カーブを描く累乗を利用する。
	//color.rgb = pow(specular, 20);
	specular = pow(specular, 20);
	
	//--- ランバート計算
	float3 N = pin.normal;
	float diffuse = dot(N, -L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb = specular + diffuse;
	
	return color;
}
