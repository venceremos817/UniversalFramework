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

cbuffer World : register(b0)
{
	float4x4 world;
};
cbuffer ViewProj : register(b1)
{
	float4x4 view;
	float4x4 proj;
};

VS_OUT main(IL_PCUN vin)
{
	VS_OUT vout;

	// float4�Ɋi�[���邽�߁A
	// float3�̃f�[�^��float4�ɕϊ�
	vout.pos = float4(vin.pos, 1);

	// ���[�J�����W����X�N���[�����W�ւ̕ϊ�
	// ���[�J�������[���h���J��������ۼު���݁���ذ�
	// �萔�o�b�t�@�̒��g�̓O���[�o���ϐ��̂悤�Ɉ�����
	// mul ... 3D���W�ƍs��̊|���Z���s��
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	// CPU����󂯎�������_����
	// �s�N�Z���V�F�[�_�ɓn�����߂ɃR�s�[
	vout.uv = vin.uv;

	return vout;
}