#include "INCStructs.hlsli"
#include "SimpleConstantBuffer.hlsli"

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);

SamplerState samp : register(s0);

float4 main(PS_PNT_Input pin) : SV_TARGET
{
	// ベースのテクスチャから色を決める
	float4 color = tex.Sample(samp, pin.tex);
	color += pin.specular;
	color.a *= Diffuse.a;
	
	float diffuse = dot(pin.normal, -LightDir.xyz);
	diffuse = (diffuse + 1.0f) * 0.5f;
		
	// 画像ファイルから段階的な影を表現
	// ランプテクスチャと呼ばれる画像から明るさを取得する。
	// 光源と法線から計算した明るさは1～0の間で表現するため、この値をUVの参照値に使うと
	// ランプテクスチャから明るさが取得できる
	diffuse = rampTex.Sample(samp, float2(diffuse * 0.98f + 0.01f, 0.5f)).r;
	
	color.rgb *= diffuse;
	return color;
}