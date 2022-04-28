struct PS_IN
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 projPos : TEXCOORD2;
};

// カメラの位置
cbuffer CameraInfo : register(b0)
{
	float4 cameraPos;
};



float main(PS_IN pin):SV_Target
{
	// 影を付けるためには、これから影を付けようとするオブジェクトが手前か奥か判断したい
	// その比較をするため、カメラからの距離をテクスチャへ描き込む
	// ここでprojPosはスクリーンの座標系だが、z,w値にはそれぞれ必要な奥行きの情報が格納されている。
	// projPos.z は手前スクリーンから目的位置までの距離
	// projPos.w は手前クスリーンから最奥スクリーン間の距離		(手前スクリーン = ニアクリップ、最奥スクリーン = ファークリップ
	// そもそもの色情報は0～1に丸め込まれるので
	// w(全体の距離)でz(奥行き)を割ることで深度値(0～1)が計算できる
	float depth = 0.0f;
	
	depth = pin.projPos.z / pin.projPos.w;		

	return depth;
}