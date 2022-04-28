/*	�f�B�]���u
A�̉摜��B�֕ω�������\��(�ꉞ�A�P���ȍ���F�̃t�F�[�h���Y������)
���܂ł̓I�u�W�F�N�g�S�̂̓��߂��ꗥ�ŕς��ăt�F�[�h�����Ă������A
�V�F�[�_�ƑJ�ڂ�\������e�N�X�`����p���邱�ƂŁA�P���łȂ��t�F�[�h��\���ł���B
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// �p�����[�^
cbuffer Parameter : register(b2)
{
	float value;
};

Texture2D baseTex : register(t0); // �x�[�X�̃e�N�X�`��
Texture2D dissolveTex : register(t1); // �f�B�]���u�e�N�X�`��

SamplerState samp : register(s0); // �e�N�X�`���̌J��Ԃ��ݒ�


float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	color = baseTex.Sample(samp, pin.uv);
	
	// �f�B�]���u
	// �t�F�[�h�̂ݍs�� = ���ߐF�����ω�������
	//color.a = value.x; // �ꊇ�ŕω����Ă��܂�
	
	// �e�N�X�`���̐F(��:0/��:1)�Ƃ������Ƃ�
	// �o�ߎ��ԂƔ�r���āA�������l�͕\������āA�傫���l�͕\������Ȃ��Ɣ���ł���΁A�摜��p�����t�F�[�h���ł���
	// ��) �o�ߎ���:0.2 �Ȃ�� ��(0.2��菬�����l)�͕\������邪�A��(0.2���傫���l)�͂܂��\������Ȃ�
	// ���̔����f���Ɏ�������̂ł����if��������
	// �������A1280*720�̉�ʂɑ΂��ăt�F�[�h���s���Ƃ���� 921600����̔�����s�����ƂɂȂ�
	// �v�Z�����ɔ�ׂĔ���̏����͒x���̂ŁA�o�����if���͎g��Ȃ��ق����ǂ�
	// �܂��͌v�Z�Ŏ����ł��Ȃ����l���Ă���if�����g��
	float dissolve = dissolveTex.Sample(samp, pin.uv).r;
	// step() ��1��������2�������傫�����0,���������1��Ԃ�
	color.a = step(dissolve, value.x);
	//color.a = 1.0f - step(dissolve, value.x);
	
	
	return color;
}
