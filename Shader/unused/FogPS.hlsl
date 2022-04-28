/*	フォグ
カメラから遠くに離れるほど、色が淡くなるような表現
霧の中や、遠くの山並みなどを表現するときに使える
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

cbuffer Camera : register(b1)
{
	float4 eyePos;
}


float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 backColor = float4(0.8f, 0.8f, 0.9f, 1.0f);
	

	// カメラまでの距離
	float cameraDistance = distance(eyePos.xyz, pin.worldPos);
	// フォグの開始距離
	float fogStartDistance = 100.0f;
	// フォグの適用範囲
	float fogRange = 800.0f;
	
	// フォグの計算
	// ①基準位置からフォグが始まるようにカメラとの距離を修正
	cameraDistance -= fogStartDistance;
	// ②距離の情報を適用範囲で割る
	cameraDistance /= fogRange;
	// ③1.0f から②の結果を引くとフォグの値が計算できる
	float fog = 1.0f - cameraDistance;

	// 背景の色に向かって徐々に変化する
	color.rgb = lerp(backColor.rgb, color.rgb, saturate(fog));
	
	return color;
}