// ���_�f�[�^�̎���
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
// �s�N�Z���V�F�[�_�ɓn���f�[�^
struct VS_OUT
{
	float4 pos : SV_POSITION;
};

// �萔�o�b�t�@�̎���
cbuffer World : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
// �J�������
cbuffer Camera : register(b1)
{
	float4 eyePos;
};

// �G���g���|�C���g
VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	
	vout.pos = float4(vin.pos, 1.0f);
	
	// �J�����Ƃ̋����𒲂ׂ邽�߂ɁA���O�Ƀ��[���h���W���v�Z
	float4 worldPos = mul(vout.pos, world);
	// �J�����Ƃ̋���
	float dist = length(worldPos.xyz - eyePos.xyz);
	
	// �֊s���Ƃ��ĕ\�����邽�߁A�@�������Ƀ��f����傫������
	// ���g�k�ł͌��_���痣���قǁA�Y�����\���ɂȂ��Ă��܂�
	// �܂��A�J�����Ƃ̋����ɉ����Ė@�������ւ̈ړ��ʂ�ς��邱�ƂŁA�J����������Ă����̑傫���ŗ֊s�����\�������
	float margin = dist / 200.0f;
	vout.pos.xyz += normalize(vin.normal) * margin;
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	
	
	return vout;
}
