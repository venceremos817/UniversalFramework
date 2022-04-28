struct PS_IN
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 projPos : TEXCOORD2;
};

// �J�����̈ʒu
cbuffer CameraInfo : register(b0)
{
	float4 cameraPos;
};



float main(PS_IN pin):SV_Target
{
	// �e��t���邽�߂ɂ́A���ꂩ��e��t���悤�Ƃ���I�u�W�F�N�g����O���������f������
	// ���̔�r�����邽�߁A�J��������̋������e�N�X�`���֕`������
	// ������projPos�̓X�N���[���̍��W�n�����Az,w�l�ɂ͂��ꂼ��K�v�ȉ��s���̏�񂪊i�[����Ă���B
	// projPos.z �͎�O�X�N���[������ړI�ʒu�܂ł̋���
	// projPos.w �͎�O�N�X���[������ŉ��X�N���[���Ԃ̋���		(��O�X�N���[�� = �j�A�N���b�v�A�ŉ��X�N���[�� = �t�@�[�N���b�v
	// ���������̐F����0�`1�Ɋۂߍ��܂��̂�
	// w(�S�̂̋���)��z(���s��)�����邱�ƂŐ[�x�l(0�`1)���v�Z�ł���
	float depth = 0.0f;
	
	depth = pin.projPos.z / pin.projPos.w;		

	return depth;
}