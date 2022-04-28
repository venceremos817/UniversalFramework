struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 shadowPos : TEXCOOR0;
};
float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1,0,0,1);
	color.r = pin.shadowPos.z / pin.shadowPos.w;
	return color;
}