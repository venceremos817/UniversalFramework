struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
};

// CPU����n���ꂽ�e�N�X�`����
// ���p���邽�߂̐錾
Texture2D tex : register(t0);
// �e�N�X�`���̌J��Ԃ��ݒ���K�v
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
	float4 color;
	// �@�P�F�ŕ\��
	color = float4(1, 0, 0, 1);
	color = pin.color;
	// �AUV��F���Ƃ��ďo��
	// color = float4(pin.uv, 0, 1);
	// �B�e�N�X�`���𒣂�t���ĕ\��
	//color = tex.Sample(samp, pin.uv);
	return color;
	/*
	float4 color = tex.Sample(samp, PIN.uv);
	color *= PIN.color;
	return color;
	*/
}
