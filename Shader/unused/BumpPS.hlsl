// ���_�V�F�[�_����n�����f�[�^
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 texSpaceLight : TEXCOORD2;	// �e�N�X�`����ԏ�̃��C�g�̌���
};

// �e�N�X�`��
Texture2D baseTex : register(t0);
Texture2D normalTex : register(t1);
SamplerState samp : register(s0);



float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	// �o���v�}�b�s���O�̓e�N�X�`���ɕۑ�����Ă���f�[�^��@���x�N�g���Ƃ��Ĉ���
	float3 N = normalTex.Sample(samp, pin.uv);
	N = N * 2.0f - 1.0f;	// -1 ~ 1 �ɕϊ�
	// �{���ł���΁A�@���}�b�v�̖@�������[���h��Ԃɕϊ����邪�A
	// �s�N�Z���V�F�[�_�ł��̏������s���ƌv�Z�ʂ�������B
	// ���̂��߁A���[���h��Ԃ̃��C�g���e�N�X�`����ԂɈړ������Čv�Z���s���B
	// ���C�g���e�N�X�`����ԂɈړ������鏈���͒��_�V�F�[�_�ōs�����߁A�v�Z�ʂ����Ȃ��Ȃ�B
	float3 L = normalize(pin.texSpaceLight);
	
	// �����o�[�g�̌v�Z�ŉe������
	float diffuse = dot(N, -L);	
	diffuse = (diffuse + 1.0f) * 0.5f;
	color.rgb *= diffuse;
	
	return color;
}