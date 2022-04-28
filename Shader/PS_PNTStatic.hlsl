#include "INCStructs.hlsli"
#include "SimpleConstantBuffer.hlsli"

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_PNT_Input pin) : SV_TARGET
{
	// 法線ライティング
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(pin.normal);
	float4 light = (saturate(dot(N1, -lightdir)) * Diffuse) + Emissive;
	light += pin.specular;
	light.a = Diffuse.a;
	if (Activeflags.x)
	{
		light = tex.Sample(samp, pin.tex) * light;
	}
	
	return light;
}