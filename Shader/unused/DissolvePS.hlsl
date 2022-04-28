/*	ディゾルブ
Aの画像をBへ変化させる表現(一応、単純な黒一色のフェードも該当する)
今まではオブジェクト全体の透過を一律で変えてフェードさせていたが、
シェーダと遷移を表現するテクスチャを用いることで、単調でないフェードを表現できる。
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// パラメータ
cbuffer Parameter : register(b2)
{
	float value;
};

Texture2D baseTex : register(t0); // ベースのテクスチャ
Texture2D dissolveTex : register(t1); // ディゾルブテクスチャ

SamplerState samp : register(s0); // テクスチャの繰り返し設定


float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	color = baseTex.Sample(samp, pin.uv);
	
	// ディゾルブ
	// フェードのみ行う = 透過色だけ変化させる
	//color.a = value.x; // 一括で変化してしまう
	
	// テクスチャの色(黒:0/白:1)とおいたとき
	// 経過時間と比較して、小さい値は表示されて、大きい値は表示されないと判定できれば、画像を用いたフェードができる
	// 例) 経過時間:0.2 ならば 黒(0.2より小さい値)は表示されるが、白(0.2より大きい値)はまだ表示されない
	// この判定を素直に実装するのであればif文をつかう
	// ただし、1280*720の画面に対してフェードを行うとすると 921600回もの判定を行うことになる
	// 計算や代入に比べて判定の処理は遅いので、出来ればif文は使わないほうが良い
	// まずは計算で実現できないか考えてからif文を使う
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// step() 第1引数が第2引数より大きければ0,小さければ1を返す
	color.a = step(dissolve, value.x);
	//color.a = 1.0f - step(dissolve, value.x);
	
	
	return color;
}
