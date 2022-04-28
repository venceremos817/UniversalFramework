#include "INCStructs.hlsli"
#include "SimpleConstantBuffer.hlsli"


PS_PNT_Input main(VS_PNT_Input vin)
{
	PS_PNT_Input result;
	// ���_�̈ʒu��ϊ�
	float4 pos = float4(vin.position.xyz, 1.0f);
	// ���[���h�ϊ�
	pos = mul(pos, World);
	// �r���[�ϊ�
	pos = mul(pos, View);
	// �ˉe�ϊ�
	pos = mul(pos, Projection);

	// �s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	// �@��
	result.normal = mul(vin.normal, (float3x3) World);
	result.normal = normalize(result.normal);
	// �X�y�L�����[
	float3 H = normalize(normalize(-LightDir.xyz) + normalize(EyePos.xyz - pos.xyz));
	result.specular = Specular * dot(result.normal, H);
	result.specular *= Specular.a;
	// �e�N�X�`��UV
	result.tex = vin.tex /** UVScale*/;
	
	return result;
}