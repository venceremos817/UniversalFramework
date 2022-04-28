/*	1つのモデルに複数枚のテクスチャ
傷や衣服の汚れなどを表現する際、あらかじめ描き込まれているテクスチャを用意するのではなく、
傷のみでテクスチャを用意する。
シェーダ内で複数のテクスチャから色を読み取り、それぞれを合成した結果を出力する。
(キャラメイクのペイント・タトゥーなど
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D baseTex : register(t0);	// ベースのテクスチャ
Texture2D subTex : register(t1);	// 張り合わせるテクスチャ

SamplerState samp : register(s0);	// テクスチャの繰り返し設定


float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// テクスチャの重ね合わせ
	// color = subTex,Sample(samp, pin.uv);		// サブで上書きしてるだけ
	// color += subTex.Sample(samp, pin.uv);	// 加算合成
	// 正しく重ね合わせを行うためには、自前でアルファブレンドを行う
	// アルファブレンド = dst.rgb * (1.0f - src.a) + src.rgb * src.a;
	float4 base = baseTex.Sample(samp, pin.uv);
	float4 sub = subTex.Sample(samp, pin.uv);
	color = base;
	color.rgb = base.rgb * (1.0f - sub.a) + sub.rgb * sub.a;

	
	return color;
}
