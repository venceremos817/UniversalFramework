/*	�������C�g
�I�u�W�F�N�g�̌�납�狭�͂Ȍ��������������A�I�u�W�F�N�g�̉��̕����������Č�����悤�ȕ\��
*/
struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float3 worldPos : TEXCOORD2;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// ���C�g�̏��
cbuffer Light : register(b0)
{
	float4 direction;
}

cbuffer Camera : register(b1)
{
	float4 eyePos;
}

float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//color = tex.Sample(samp, pin.uv);

	//--- �v�Z�ɕK�v�ȃp�����[�^
	float3 L = -normalize(direction.xyz);
	float3 V = normalize(pin.worldPos - eyePos.xyz);
	float3 N = normalize(pin.normal);

	// �������C�g
	float lim = dot(L, V);
	lim = pow(lim, 5); // �J�����ƃ��C�g�����΂��Ă��鎞�Ɍ��ʂ������悤����
	float edge = dot(N, V) + 1.0f;

	//--- �����o�[�g�v�Z
	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb = lim * edge + diffuse;
	
	return color;
}
