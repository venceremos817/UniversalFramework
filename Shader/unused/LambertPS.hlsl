struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// ライトの情報
cbuffer Light : register(b0)
{
	float4 direction;	// 16B区切りでないといけない
}

float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//color = tex.Sample(samp, pin.uv);
	
	// 法線のデータが正しく渡せれているか確認する
	// 確認方法として色情報として出力する(法線以外のデータもこの方法
	//color.rgb = pin.normal;
	// マイナスの値は0に丸め込まれてしまうので、全て黒色で表示されてしまう。
	// プラスの情報からマイナス部分も憶測で確認できる場合もあるが、
	// 正確に確認するのであればマイナス(-1~1)をプラス(0~1)に変換してあげる
	//color.rgb = (color.rgb + 1.0f) * 0.5f;
	//color = direction;
	//color.a = 1.0f;

	// ランバートシェーディング
	// オブジェクトに影をつける計算
	// オブジェクトに影をつけるためには太陽(光源)と
	// オブジェクトの法線(表を向いている方向)の2つが必要
	// 太陽の光の向きと、法線の向きを比べてほぼ同じ方向を向いている部分には
	// 影がつき、反対方向を向いている部分は影がつかないようにする。
	float3 N = normalize(pin.normal);
	// 影の計算は二つのベクトルの内積から計算する。内積の結果は二つのベクトルが同じ方向を向いていると+、逆の方向を向いていると-になる。
	// 影がつく部分の法線と光の内積は+になるが、影は色が暗くなる=マイナスの値なので符号を反転しておく。
	float3 L = -direction.xyz;	
	
	// 内積
	// ・二つのベクトルが同じ方向を向いている... +1
	// ・二つのベクトルが反対方向を向いている... -1
	// ・二つのベクトルが直行... 0
	// ・単位ベクトルとの結果は、単位ベクトル上での長さを表す(ベクトルが落とす影の長さ)
	
	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb *= diffuse;
	
	return color;
}
