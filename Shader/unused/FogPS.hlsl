/*	�t�H�O
�J�������牓���ɗ����قǁA�F���W���Ȃ�悤�ȕ\��
���̒���A�����̎R���݂Ȃǂ�\������Ƃ��Ɏg����
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

cbuffer Camera : register(b1)
{
	float4 eyePos;
}


float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 backColor = float4(0.8f, 0.8f, 0.9f, 1.0f);
	

	// �J�����܂ł̋���
	float cameraDistance = distance(eyePos.xyz, pin.worldPos);
	// �t�H�O�̊J�n����
	float fogStartDistance = 100.0f;
	// �t�H�O�̓K�p�͈�
	float fogRange = 800.0f;
	
	// �t�H�O�̌v�Z
	// �@��ʒu����t�H�O���n�܂�悤�ɃJ�����Ƃ̋������C��
	cameraDistance -= fogStartDistance;
	// �A�����̏���K�p�͈͂Ŋ���
	cameraDistance /= fogRange;
	// �B1.0f ����A�̌��ʂ������ƃt�H�O�̒l���v�Z�ł���
	float fog = 1.0f - cameraDistance;

	// �w�i�̐F�Ɍ������ď��X�ɕω�����
	color.rgb = lerp(backColor.rgb, color.rgb, saturate(fog));
	
	return color;
}