// ���_�f�[�^�̎���
struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
};
// �s�N�Z���V�F�[�_�ɓn���f�[�^
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;	 
	float3 texSpaceLight : TEXCOORD2; //�e�N�X�`����ԏ�̃��C�g�̌���
};

// �萔�o�b�t�@�̎���
cbuffer World : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

// ���C�g�̏��
cbuffer Light : register(b1)
{
	float4 direction;
};

// �G���g���|�C���g
VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);

	vout.uv = vin.uv;
	vout.normal = vin.normal;
	
	//--- ���[���h��ԏ�̃��C�g���e�N�X�`����Ԃɒu���Ȃ���
	// �@���ƃx�N�g������]�@�������߂�
	float3 N = normalize(vin.normal);
	float3 T = normalize(vin.tangent);
	float3 B = normalize(cross(N, T));
	// �e�N�X�`����Ԃ��烏�[���h��Ԃɕϊ�����p���s������߁A���C�g�ɑ΂��Ă��̍s���������
	// �p���s��̋t�s��͓]�n�ŋ��߂邱�Ƃ��ł���
	float3x3 invTexToWorld = float3x3(
			T.x, B.x, N.x,
			T.y, B.y, N.y,
			T.z, B.z, N.z
		);
	// ���C�g���e�N�X�`����Ԃɕϊ�
	float3 L = normalize(direction.xyz);
	vout.texSpaceLight = mul(L, invTexToWorld);

	
	return vout;
}
