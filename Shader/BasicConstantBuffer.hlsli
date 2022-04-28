cbuffer ParametersBuffer : register(b0)
{
	float4x4 World : packoffset(c0);
	float3x3 WorldInverseTranspose : packoffset(c4);
	float4x4 WorldViewProjection : packoffset(c7);
	
	float4 Diffuse : packoffset(c11);
	float3 Emissive : packoffset(c12);
	float4 Specular : packoffset(c13);

	uint4 Activeflags : packoffset(c14);

	float3 EyePos : packoffset(c15);

	float4 LightPos : packoffset(c16);
	float3 LightDir : packoffset(c17);
	float3 LightDiffuse : packoffset(c18);
	float4 LightSpecular : packoffset(c19);
	float4x4 LightView : packoffset(c20);
	float4x4 LightProjection : packoffset(c24);
	float2 UVScale : packoffset(c28);
};
