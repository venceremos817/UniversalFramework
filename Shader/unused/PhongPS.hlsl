/*	Phong�V�F�[�f�B���O
Lambert�V�F�[�f�B���O�ɋ��ʔ���(���̔���)���������V�F�[�f�B���O
�������˂��Ă܂Ԃ��������邽�߂ɂ́A���̌����ϑ�����l���K�v(���˂���p�x�ɂ���Ă͂܂Ԃ��������Ȃ�)
3D�Q�[���ł͂��̊ϑ�����l�̓J�����̂��Ƃ������B
���̔��˂Ȃ̂ŁA���z�̌��͈�x�I�u�W�F�N�g�ɂԂ����Ē��˕Ԃ�Ȃ��Ƃ����Ȃ��B
���˕Ԃ���̓I�u�W�F�N�g�̂Ԃ������ʒu�ɂ���ĈقȂ�B
���̂��߁A�\�������s�N�Z�����Ƃɔ��˂����������߂�B
���̔��˂������̃x�N�g���ƁA�J�����̎����̃x�N�g���̌������܂���������Ⴄ�悤�ȏꍇ�ɁA�I�u�W�F�N�g�����𔽎˂��Ă܂Ԃ���������B
*/

struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// ���C�g
cbuffer Light : register(b0)
{
	float4 direction; // 16B��؂�łȂ��Ƃ����Ȃ�
};

// �J����
cbuffer Camera : register(b1)
{
	float4 eyePos;
};



float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
//	color = tex.Sample(samp, pin.uv);
	
	// �v�Z�ɕK�v�ȃf�[�^
	float3 L = normalize(direction.xyz); // �����̌���
	float3 V = normalize(eyePos.xyz - pin.worldPos);
	
	// ���˃x�N�g���v�Z
	float3 R = reflect(L, normalize(pin.normal));
	R = normalize(R);

	//--- ���ʔ��˂̌v�Z
	// �����������Ă܂Ԃ��������镔���c�X�y�L�����[(specular
	float specular = dot(V, R) * 0.5f + 0.5f;
	
	// ���̂܂܂ł͍L�͈͂����邭�����Ă��܂��̂ŁA�}����K�v������B
	// ���`�Ŗ��邳���ω�����̂ł͂Ȃ��A�J�[�u��`���悤�ȕω����]�܂���
	// �J�[�u��`���ݏ�𗘗p����B
	//color.rgb = pow(specular, 20);
	specular = pow(specular, 20);
	
	//--- �����o�[�g�v�Z
	float3 N = pin.normal;
	float diffuse = dot(N, -L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb = specular + diffuse;
	
	return color;
}
