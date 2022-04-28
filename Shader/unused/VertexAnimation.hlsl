/*
���Ƃ��ƕ`�悵�Ă��������Ȃǂ͂��̂܂ܕ\��������(�A�j���[�V�����͕K�v�Ȃ�
VertexShader.hlsl�����������ăA�j���[�V�����̏�����ǉ�����ƁA
�A�j���[�V�������Ȃ����f���̕\�������������Ȃ邽�߁A����������
�@�\���ƂɃV�F�[�_�[�t�@�C���𕪂���(���̃t�@�C���̓A�j���[�V�����@�\
*/
#include "InputLayout.hlsli"

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 color : TEXCOORD2;
	float4 wPos : TEXCOORD3;
	float4 vPos : TEXCOORD4;
};

// ���f���̕ϊ��s��(���f�����ƂɈقȂ邽�ߍX�V�p�x������
cbuffer World : register(b0)
{
	float4x4 world;
};
// �J�����̕ϊ��s��(1�x�̕`��Ŏg�p�����J�����͊�{�P��ނ̂��߁A
// �X�V�p�x�͒Ⴂ
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};
// ���f���̃A�j���[�V�����p�̕ϊ��s��
// ���f�����ƂɎp�����قȂ邽�߁A�X�V�p�x�������B
// ������Ƃ�����World�ɑS�Ă܂Ƃ߂�ƁA�V�F�[�_�[��؂�ւ����Ƃ���
// �萔�o�b�t�@�̕ύX�������邽�߁A�萔�o�b�t�@�𑼂̃V�F�[�_�[�Ɠ���
// ��؂�ɂ��Ă����Ɛ؂�ւ��̎�Ԃ����Ȃ��Ȃ�
cbuffer Animation : register(b2)
{
	float4x4 bone[250];
};


VS_OUT main(IL_ANIME vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1);

	// �{�[���̉e�����v�Z���āA�A�j���[�V�����������W�ʒu�����߂�
	float4x4 anime =
		bone[vin.index.x] * vin.weight.x + 
		bone[vin.index.y] * vin.weight.y + 
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
	vout.pos = mul(vout.pos, anime);

	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	vout.color.xyz = vin.normal;
	vout.color.a = 1.0f;

	return vout;
}