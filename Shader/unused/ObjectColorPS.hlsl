struct PS_IN
{
	float4 pos : SV_POSITION;
};
cbuffer Color : register(b0)
{
	float4 color;
};

float4 main(PS_IN pin) : SV_TARGET
{
	return color;
}