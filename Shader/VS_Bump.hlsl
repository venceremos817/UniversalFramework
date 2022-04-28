#include "INCStructs.hlsli"
#include "BasicConstantBuffer.hlsli"

PS_Bump_Input main(VS_PNTTg_Input vin)
{
	PS_Bump_Input vout;
	
	vout.pos = float4(vin.position);
	vout.pos = mul(vout.pos, WorldViewProjection);
	
	vout.tex = vin.tex * UVScale;
	vout.normal = vin.normal;
	
	float3 N = normalize(vin.normal);
	float3 T = normalize(vin.tangent);
	float3 B = normalize(cross(N, T));
	
	float3x3 invTexToWorld =
	float3x3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z
	);
	
	float3 L = normalize(LightDir);
	vout.texSpaceLight = mul(L, invTexToWorld);
	
	vout.diffuse = Diffuse; //	float4(1.0f, 1.0f, 1.0f, Diffuse.a);
	
	return vout;
}
