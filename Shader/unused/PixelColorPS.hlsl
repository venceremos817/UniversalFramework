struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

float4 main(PS_IN PIN) : SV_Target
{
	return PIN.color;
}
