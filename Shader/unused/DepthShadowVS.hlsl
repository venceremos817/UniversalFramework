struct VS_IN
{
	float3 pos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};
struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 shadowUV : TEXCOORD1;
};

// �����ʂ�A�Q�[����ʂɕ\������s��
cbuffer Matrix : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};
// �������猩���i�F�ŕ\�����邽�߂̍s��
cbuffer ShadowViewProj : register(b1)
{
	float4x4 shadowVPS;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout;
	vout.pos = float4(vin.pos, 1.0f);
	vout.pos = mul(vout.pos, world);
	// ���[���h�s��v�Z��A���C�g��Ԃ̍s���������
	vout.shadowUV = mul(vout.pos, shadowVPS);
	// �����ʂ�A��ʂ֏o�͂��邽�߂̃J�����s���������
	vout.pos = mul(vout.pos, view);
	vout.pos = mul(vout.pos, proj);
	vout.uv = vin.uv;
	return vout;
}