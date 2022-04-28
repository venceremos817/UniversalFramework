struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 wPos : TEXCOORD0;
};

// ���C�g�̏��
struct Light
{
	float3 pos;
	float range;
	float4 color;
};

cbuffer Lights : register(b0)
{
	Light lights[30];
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	/* �_����
	�ʏ�̑��z�̌��͕��s�����Ƃ����B
	���s�����ł͑S�̂ɑ΂��Ĉꗥ�̕�����������������Ă���l��\������B
	�_�����ł́A���[���h��Ԃɔz�u����Ă���X�̌�������A
	�`�悵�悤�Ƃ��Ă���I�u�W�F�N�g�̃s�N�Z���Ɍ������Č��������B
	�_�����͏�����낤�����̂悤�ȃI�u�W�F�N�g��\������Ƃ��ɗ��p����B
	���s�����̌����͈���������Ȃ��̂ɑ΂��A�_�����͂���Ƃ���������Ɍ����Ă���B
	���̕����͌����̍��W�ƃs�N�Z���̍��W���������Ƃŋ��߂���
	*/
	
	// ���ׂẴ��C�g�ɑ΂��ď������s��
	float4 pointColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 30; ++i)
	{
		// �s�N�Z���̃��[���h���W�ƁA�����̍��W������̃x�N�g�����v�Z
		float3 lightDir = pin.wPos.xyz - lights[i].pos;
		// �s�N�Z���ʒu�܂ł̋���
		float distance = length(lightDir);
		// �_�����̗L���͈͂����ƂɁA�s�N�Z���ʒu�܂łɂǂꂾ��
		// �����キ�Ȃ�̂�(����)�v�Z����
		// �_�����ɋ߂��ق�1(���邢),�����ق�0(�Â�)
		// ����L�A�L���͈�R�Ƃ����� L/R �Ŗ��邳�����߂�
		// �v�Z�����l��1����������ƂŐ������֌W�̐��l�����߂���
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		
		// ���͋�����2��Ō�������(�t2��̖@��)���߁A
		// �v�Z��������������Z����
		attenuation = pow(attenuation, 3.0f);
		
		// �������Ɗ|���č����̌����v�Z
		float4 lightColor = lights[i].color * attenuation;
		// �_�������Ƃ̌������Z���Ă���
		pointColor += lightColor;
	}

	// ���Z���������ŏI�J���[�Ƃ��ďo��
	color.rgb = pointColor.rgb;


	return color;
}