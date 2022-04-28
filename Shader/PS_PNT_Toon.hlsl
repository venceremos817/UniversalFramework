#include "INCStructs.hlsli"
#include "SimpleConstantBuffer.hlsli"

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);

SamplerState samp : register(s0);

float4 main(PS_PNT_Input pin) : SV_TARGET
{
	// �x�[�X�̃e�N�X�`������F�����߂�
	float4 color = tex.Sample(samp, pin.tex);
	color += pin.specular;
	color.a *= Diffuse.a;
	
	float diffuse = dot(pin.normal, -LightDir.xyz);
	diffuse = (diffuse + 1.0f) * 0.5f;
		
	// �摜�t�@�C������i�K�I�ȉe��\��
	// �����v�e�N�X�`���ƌĂ΂��摜���疾�邳���擾����B
	// �����Ɩ@������v�Z�������邳��1�`0�̊Ԃŕ\�����邽�߁A���̒l��UV�̎Q�ƒl�Ɏg����
	// �����v�e�N�X�`�����疾�邳���擾�ł���
	diffuse = rampTex.Sample(samp, float2(diffuse * 0.98f + 0.01f, 0.5f)).r;
	
	color.rgb *= diffuse;
	return color;
}