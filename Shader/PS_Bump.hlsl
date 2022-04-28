#include "INCStructs.hlsli"
#include "BasicConstantBuffer.hlsli"


Texture2D<float4> AlbedoTexture : register(t0);
Texture2D<float3> NormalTexture : register(t1);
sampler Sampler : register(s0);


float4 main(PS_Bump_Input pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	if (Activeflags.y > 0)
	{
		color = AlbedoTexture.Sample(Sampler, pin.tex) * pin.diffuse;
	}

	if (Activeflags.x > 0)
	{
		float3 N = NormalTexture.Sample(Sampler, pin.tex);
		N = N * 2.0f - 1.0f;
		float3 L = normalize(pin.texSpaceLight);
		float3 V = normalize(EyePos.xyz - pin.pos.xyz);
		float3 R = normalize(reflect(L, N));

		float diffusePow = dot(N, -L);
		//float3 specular = dot(V, R) * 0.5f + 0.5f;
		//specular = pow(specular, Specular.w);
		//specular = mul(specular, LightSpecular.xyz) * Specular.xyz;
		
		diffusePow = (diffusePow + 1.0f) * 0.5f;
		color.rgb *= diffusePow * LightDiffuse;
		//color.rgb += specular * color.a;
	}
	return color;
}