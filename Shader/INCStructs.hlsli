
struct VS_PNT_Input
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct VS_PNTTg_Input
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 tangent : TANGENT;
};

struct VS_PixelLightingTxTangent_Output
{
	float2 texCoord : TEXCOORD0;
	float4 positionWS : TEXCOORD1;
	float3 normalWS : TEXCOORD2;
	float3 tangentWS : TEXCOORD3;
	float4 diffuse : COLOR0;
	float4 positionPS : SV_POSITION;
};

struct PS_PNT_Input
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 specular : COLOR;
	float2 tex : TEXCOORD;
};

struct PS_PixelLightingTxTangent_Input
{
	float2 texCoord : TEXCOORD0;
	float4 positionWS : TEXCOORD1;
	float3 normalWS : TEXCOORD2;
	float3 tangentWS : TEXCOORD3;
	float4 diffuse : COLOR0;
};
	
struct PS_Bump_Input
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;
	float4 diffuse : COLOR0;
};