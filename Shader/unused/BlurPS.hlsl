struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer Parameter : register(b0)
{
	float2 texSize;
	float2 blurDir;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//--- 周辺のピクセルを参照するための情報を準備
	// ピクセルで考えれば、一つとなりは1ピクセル移動した場所と考えられるが、
	// シェーダではUV(0~1)で考えなければならない。大きさ10の画像で1ピクセルの移動量は0.1
	// 大きさ100の画像で1ピクセルの移動量は0.01 と異なる。
	// (UV(0~1)を10分割するか、100分割するかの違い)
	// そのため、画像サイズに併せてUVの移動量を求めなければいけない
	float2 movePixel = 1.0f / texSize;
	// 横方向にぼかしをかけるのか縦方向にぼかしをかけるのかは、別々のシェーダを作らなくとも、
	// パラメータの工夫で一つのシェーダにまとめられる
	// それぞれの方向で、動かない方向に関しては必ず0になるので、
	// 動く方向に1,動かない方向に0を設定して掛け算すれば求められる。
	// 横(1,0) , 縦(0,1)
	movePixel *= blurDir;

	// ぼかし処理をかける
	// 自分自身のピクセル
	color = tex.Sample(samp, pin.uv);
	// 正方向(右,上)のピクセル
	color += tex.Sample(samp, pin.uv + movePixel * 1.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 2.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 3.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 4.0f);
	// 負方向(左,下)のピクセル
	color += tex.Sample(samp, pin.uv - movePixel * 1.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 2.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 3.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 4.0f);
	// 足し合わせたピクセルの平均を求める
	color /= 9.0f;
	
	return color;
}