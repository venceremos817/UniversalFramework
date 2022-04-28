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
	// �X�|�b�g���C�g�ɒǉ����ꂽ�p�����[�^
	float3 dir; // ����
	float angle; // �Ƃ炷�͈�
};

cbuffer Lights : register(b0)
{
	Light lights[30];
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 spotColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < 30; ++i)
	{
		// �_�����̏����Ɠ������A�����⌸���������߂�
		float3 lightDir = pin.wPos.xyz - lights[i].pos;
		float distance = length(lightDir);
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		
		// ���͋�����2��Ō�������(�t2��̖@��)���߁A
		// �v�Z��������������Z����
		attenuation = pow(attenuation, 3.0f);
		
		//--- �_�����ŋ��߂��������ɃX�|�b�g���C�g�̏�����K�p����
		// �X�|�b�g���C�g�͈̔͂ɂ����܂��Ă��邩
		// �܂��́A�͈͊O�͌�������0�ɂ���(�����͂��Ȃ�
		
		// �_��������s�N�Z���Ɍ������x�N�g���ƁA�X�|�b�g���C�g�̌����̃x�N�g��
		// ��̃x�N�g���̊Ԃ̊p�x���e���X�͈͓��ł���΃X�|�b�g���C�g�ŏƂ点��͈͂ł���Ƃ�����
		// ���ς̌������g���āA��̃x�N�g���̂Ȃ��p�����߂�
		// a�Eb (��̃x�N�g���̓��ό���)
		// |a| (�x�N�g��a�̒���)
		// �� (��̃x�N�g���̂Ȃ��p)
		// a�Eb = |a| * |b| * cos��
		// a�Eb / (|a| * |b|) = cos��
		// acos( a�Eb / ( |a| * |b| ) ) = ��
		// a��b�̃x�N�g���̒�����1�ł����
		// acos( a�Eb / ( 1 * 1 ) ) = ��
		// acos( a�Eb ) = ��
		float3 L = normalize(lights[i].dir);
		float3 V = normalize(lightDir);
		float vecAngle = acos(dot(L, V));

		// �X�|�b�g���C�g�̊p�x�͒[����[�܂ł͈̔�
		// ��̃x�N�g���̊p�x�̓X�|�b�g���C�g�̌�������x�N�g���܂�
		// �X�|�b�g���C�g�̌���(���S)����[�܂ł̊p�x�Ōv�Z�������̂ŁA
		// �Ƃ点��͈͔͂����ɂ��Čv�Z����
		float angleAttenuation = vecAngle / (lights[i].angle * 0.5f);
		angleAttenuation = saturate(angleAttenuation);
		angleAttenuation = 1.0f - pow(angleAttenuation, 5.0f);
		attenuation *= angleAttenuation;

		// �Ƃ炷�A�Ƃ炳�Ȃ��Ōv�Z����̂ŁA���R�ɂ͌����Ȃ�
		//if (lights[i].angle * 0.5f < vecAngle)
		//	attenuation = 0.0f;
		
		// �e�X�|�b�g���C�g�����Z���Ă���
		float4 lightColor = lights[i].color * attenuation;
		spotColor += lightColor;
	}
	
	color.rgb = spotColor.rgb;

	return color;
}