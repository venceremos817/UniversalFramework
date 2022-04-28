struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	color = tex.Sample(samp, pin.uv);

	// �Z�s�A�ϊ�
	float mono = color.r * 0.3f + color.g * 0.6f + color.b * 0.3f;
	color.rgb = mono;

	color.r *= 1.0f;	// �ԐF�͂��̂܂�
	color.g *= 0.7f;	// ���̑��̐��������炷
	color.b *= 0.4f;	// �F�͈Â��F�ŕω����킩��Â炢�̂ő傫�����炷
	
	return color;
}
