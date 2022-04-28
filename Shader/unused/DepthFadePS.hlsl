// デプスフェード(ソフトパーティクル)
// 通常、パーティクルを散らすと周辺のポリゴンに刺さり、
// ポリゴンとパーティクルの境目が目立ってしまうことがある。
// 境目を目立たなくするために、境目に近いほど透明になるような処理を行う。
// 処理に際しては、深度バッファを使って、今から描画しようとしているオブジェクトの
// 深度と、既に掻き込まれているオブジェクトの深度が近ければめり込むとみなして、透過表現を行う。

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 shadowPos : TEXCOOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);

	// スクリーン上へ変換した座標をUV座標として取得する
	float2 uv = pin.shadowPos.xy / pin.shadowPos.w;
	// 今回uvが -1～1 になっているので 0～1 に変換
	uv = uv * 0.5f + 0.5f;
	uv.y = 1.0f - uv.y;
	// スクリーン上へ変換した座標の奥行を 0～1 で取得
	// ※そのままzを使うと単純なカメラからの距離(1m,2m,3m,...)とみなされ使いづらい
	float objDepth = pin.shadowPos.z / pin.shadowPos.w;
	
	// テクスチャに描き込んである深度値
	float renderDepth = tex.Sample(samp, uv);
	
	// 二つの深度値の差から透過具合を計算(差が小さい程透明)
	float fade = saturate(renderDepth - objDepth);	// 0～1に丸める

	// 深度値の差はニアクリップ(0)とファークリップ(1)ほど離れていないと
	// はっきり表示されない。(1-0)
	// ニアクリップに近いオブジェクト同士の差がはっきりするように数字を掛けて大きくする
	// (この数値で変化の具合も調整できる)
	fade *= 1000.0f;
	
	color.a = fade;

	return color;
}