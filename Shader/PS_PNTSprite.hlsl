#include "INCstructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);


float4 main(PS_PNT_Input pin) : SV_TARGET
{
	float4 light = Diffuse + Emissive;
	light.a = Diffuse.a;
	light = tex.Sample(samp, pin.tex) * light;
	return light;
}