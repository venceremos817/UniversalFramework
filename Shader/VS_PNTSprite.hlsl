#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};


PS_PNT_Input main(VS_PNT_Input vin)
{
	PS_PNT_Input result;
	
	result.position = mul(vin.position, MatrixTransform);
	result.normal = vin.normal;
	result.tex = vin.tex;

	return result;
}
