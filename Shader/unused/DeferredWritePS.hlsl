struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 depth : TEXCOORD2;
};
/*	�}���`�����_�[�^�[�Q�b�g
��x�̕`��ŕ����̃����_�[�^�[�Q�b�g(�e�N�X�`��)�ɕ`�����݂��s���B
���炩����CPU�ŕ����̃����_�[�^�[�Q�b�g��ݒ肵�Ă������ƂƁA
�s�N�Z���V�F�[�_�ŕ����̃^�[�Q�b�g�ɏo�͂ł���悤�ɋL�q����K�v������B
*/
struct PS_OUT
{
	float4 albedo : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 depth : SV_TARGET2;
};

PS_OUT main(PS_IN pin)
{
	PS_OUT pout;
	pout.albedo = float4(1, 1, 1, 1);
	pout.normal = float4(pin.normal, 1.0f);
	pout.depth = float4(pin.depth.z / pin.depth.w, 0.0f, 0.0f, 1.0f);

	return pout;
}