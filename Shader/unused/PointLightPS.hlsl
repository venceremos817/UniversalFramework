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
};

cbuffer Lights : register(b0)
{
	Light lights[30];
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	/* 点光源
	通常の太陽の光は平行光源という。
	平行光源では全体に対して一律の方向から光が当たっている様を表現する。
	点光源では、ワールド空間に配置されている個々の光源から、
	描画しようとしているオブジェクトのピクセルに向かって光が差す。
	点光源は松明やろうそくのようなオブジェクトを表現するときに利用する。
	平行光源の光源は一方向しかないのに対し、点光源はありとあらゆる方向に向いている。
	この方向は光源の座標とピクセルの座標を引くことで求められる
	*/
	
	// すべてのライトに対して処理を行う
	float4 pointColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 30; ++i)
	{
		// ピクセルのワールド座標と、光源の座標から光のベクトルを計算
		float3 lightDir = pin.wPos.xyz - lights[i].pos;
		// ピクセル位置までの距離
		float distance = length(lightDir);
		// 点光源の有効範囲をもとに、ピクセル位置までにどれだけ
		// 光が弱くなるのか(減衰)計算する
		// 点光源に近いほど1(明るい),遠いほど0(暗い)
		// 距離L、有効範囲Rとした時 L/R で明るさを求める
		// 計算した値を1から引くことで正しい関係の数値を求められる
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		
		// 光は距離の2乗で減衰する(逆2乗の法則)ため、
		// 計算した減衰率を乗算する
		attenuation = pow(attenuation, 3.0f);
		
		// 減衰率と掛けて高原の光を計算
		float4 lightColor = lights[i].color * attenuation;
		// 点光源ごとの光を合算していく
		pointColor += lightColor;
	}

	// 合算した光を最終カラーとして出力
	color.rgb = pointColor.rgb;


	return color;
}