/*	1�̃��f���ɕ������̃e�N�X�`��
����ߕ��̉���Ȃǂ�\������ہA���炩���ߕ`�����܂�Ă���e�N�X�`����p�ӂ���̂ł͂Ȃ��A
���݂̂Ńe�N�X�`����p�ӂ���B
�V�F�[�_���ŕ����̃e�N�X�`������F��ǂݎ��A���ꂼ��������������ʂ��o�͂���B
(�L�������C�N�̃y�C���g�E�^�g�D�[�Ȃ�
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D baseTex : register(t0);	// �x�[�X�̃e�N�X�`��
Texture2D subTex : register(t1);	// ���荇�킹��e�N�X�`��

SamplerState samp : register(s0);	// �e�N�X�`���̌J��Ԃ��ݒ�


float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// �e�N�X�`���̏d�ˍ��킹
	// color = subTex,Sample(samp, pin.uv);		// �T�u�ŏ㏑�����Ă邾��
	// color += subTex.Sample(samp, pin.uv);	// ���Z����
	// �������d�ˍ��킹���s�����߂ɂ́A���O�ŃA���t�@�u�����h���s��
	// �A���t�@�u�����h = dst.rgb * (1.0f - src.a) + src.rgb * src.a;
	float4 base = baseTex.Sample(samp, pin.uv);
	float4 sub = subTex.Sample(samp, pin.uv);
	color = base;
	color.rgb = base.rgb * (1.0f - sub.a) + sub.rgb * sub.a;

	
	return color;
}
