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
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	// �e�Ƃ��ĎՂ�e�N�X�`���̂ǂ̈ʒu���Q�Ƃ��邩
	float4 shadowUV : TEXCOORD2;
};

// �萔�o�b�t�@�̎���
cbuffer World : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// �e�J�������猩�����̍��W�ɕϊ�
cbuffer ShadowViewProj : register(b1)
{
	float4x4 shadowVPS;	// �r���[�v���W�F�N�V�����X�N���[��
}

VS_OUT main(VS_IN vin)
{
	/*
	�e�����邽�߂ɂ́A���C���J�����̌��Ă���ʒu��
	�e�J����(�ʃJ����)���猩�����̈ʒu�ɕϊ�����B
	���C��) ���[���h	> Ҳ� �r���[	> Ҳ� �v���W�F�N�V����
	�ʃJ��) ���[���h	> �ʶ҃r���[	> �ʶ҃v���W�F�N�V����
	���C���ŕ`�����Ƃ��Ă���s�N�Z����ʃJ���̈ʒu�Ɍv�Z���Ȃ������ƂŁA�ʃJ���炩�猩���Ƃ��ɎՂ��邩�ǂ����𔻒f����
	*/
	
	VS_OUT vout;

	float4 wPos = float4(vin.pos, 1.0f);
	wPos = mul(wPos, world);
	
	// ���C���J�����̕ϊ�
	vout.pos = mul(wPos, view);
	vout.pos = mul(vout.pos, proj);

	// �e�J�����̕ϊ�
	vout.shadowUV = mul(wPos, shadowVPS);
	
	// UV
	vout.uv = vin.uv;
	// �@��
	vout.normal = vin.normal;
	

	return vout;
}
