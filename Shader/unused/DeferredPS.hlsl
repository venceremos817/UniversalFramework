struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

// ���C�g
struct Light
{
	float3 pos;
	float range;
	float4 color;
};
cbuffer Lights : register(b0)
{
	Light lights[30];
};

cbuffer Matrix : register(b1)
{
	float4x4 invVPS;
};

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D depthTex : register(t2);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// GBuffer����e������擾
	color = albedoTex.Sample(samp, pin.uv);
	float3 N = normalTex.Sample(samp, pin.uv).rgb;
	float depth = depthTex.Sample(samp, pin.uv).r;
	
	// �[�x�l���烏�[���h���W���Čv�Z
	// ���Ƃ��Ɛ[�x�l�̓��[���h���W���X�N���[����̍��W�ɕϊ������Ƃ��ɁA����s�N�Z���̉��s����ۑ����Ă���B
	// ��ʂɕ\�������܂ł́A �r���[ > �v���W�F�N�V���� > �X�N���[�� �ƍs��ŕϊ����s���Ă���B
	// �X�N���[����̓_�ɑ΂��Ă��̕ϊ������̋t�菇(�t�s��)���s���Ă��΁A���[���h���W�����߂邱�Ƃ��ł���B
	// ���̂Ƃ��A����s�N�Z���̍��W�̓f�B�t�@�[�h�ŉ�ʑS�̂ɕ\������̂ŁAUV�̒l�����W�ƒu�����Ƃ��ł���
	float4 worldPos = float4(pin.uv, depth, 1.0f);
	worldPos = mul(worldPos, invVPS);
	worldPos.xyz /= worldPos.w;

	// �v�Z�������[���h���W�����Ƃɓ_�����̏������s��
	float4 pointColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	for (int i = 0; i < 30; ++i)
	{
		float3 lightDir = worldPos.xyz - lights[i].pos;
		float distance = length(lightDir);
		float attenuation = 1.0f - distance / lights[i].range;
		attenuation = saturate(attenuation);
		attenuation = pow(attenuation, 3.0f);
		
		float4 lightColor = lights[i].color * attenuation;
		pointColor += lightColor;
	}
	color.rgb = pointColor.rgb;
	

	return color;
}