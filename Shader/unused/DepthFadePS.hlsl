// �f�v�X�t�F�[�h(�\�t�g�p�[�e�B�N��)
// �ʏ�A�p�[�e�B�N�����U�炷�Ǝ��ӂ̃|���S���Ɏh����A
// �|���S���ƃp�[�e�B�N���̋��ڂ��ڗ����Ă��܂����Ƃ�����B
// ���ڂ�ڗ����Ȃ����邽�߂ɁA���ڂɋ߂��قǓ����ɂȂ�悤�ȏ������s���B
// �����ɍۂ��ẮA�[�x�o�b�t�@���g���āA������`�悵�悤�Ƃ��Ă���I�u�W�F�N�g��
// �[�x�ƁA���ɑ~�����܂�Ă���I�u�W�F�N�g�̐[�x���߂���΂߂荞�ނƂ݂Ȃ��āA���ߕ\�����s���B

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 shadowPos : TEXCOOR0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 0.0f, 0.0f, 1.0f);

	// �X�N���[����֕ϊ��������W��UV���W�Ƃ��Ď擾����
	float2 uv = pin.shadowPos.xy / pin.shadowPos.w;
	// ����uv�� -1�`1 �ɂȂ��Ă���̂� 0�`1 �ɕϊ�
	uv = uv * 0.5f + 0.5f;
	uv.y = 1.0f - uv.y;
	// �X�N���[����֕ϊ��������W�̉��s�� 0�`1 �Ŏ擾
	// �����̂܂�z���g���ƒP���ȃJ��������̋���(1m,2m,3m,...)�Ƃ݂Ȃ���g���Â炢
	float objDepth = pin.shadowPos.z / pin.shadowPos.w;
	
	// �e�N�X�`���ɕ`������ł���[�x�l
	float renderDepth = tex.Sample(samp, uv);
	
	// ��̐[�x�l�̍����瓧�ߋ���v�Z(����������������)
	float fade = saturate(renderDepth - objDepth);	// 0�`1�Ɋۂ߂�

	// �[�x�l�̍��̓j�A�N���b�v(0)�ƃt�@�[�N���b�v(1)�قǗ���Ă��Ȃ���
	// �͂�����\������Ȃ��B(1-0)
	// �j�A�N���b�v�ɋ߂��I�u�W�F�N�g���m�̍����͂����肷��悤�ɐ������|���đ傫������
	// (���̐��l�ŕω��̋�������ł���)
	fade *= 1000.0f;
	
	color.a = fade;

	return color;
}