struct PS_IN
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
	float4 shadowUV : TEXCOORD2;
};

Texture2D shadowMap : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// 3D�̏ꍇ�����̌i�F�قǏ������Ȃ�̂ŁAw�Ŋ��邱�ƂŐ������p�����[�^���Ƃ��
	float2 uv = pin.shadowUV.xy / pin.shadowUV.w;

	if (0.0f <= uv.x && uv.x < 1.0f &&
		0.0f <= uv.y && uv.y <= 1.0f)
	{
		/*
		���e�e�N�X�`���V���h�E
		�e�N�X�`���ɏ������܂�Ă��邩�A�����łȂ����݂̂𔻒f���āA�I�u�W�F�N�g�ɉe��t�����@
		���G�Ȍv�Z���Ȃ����߁A�e��t���鏈���ł͌y����@�ł���
		�������A���m�Ɍv�Z���Ȃ����߁A�L�����N�^�[�̑��������łȂ��A���ӂ̕ǂ�V��ɂ����l�̉e���\������邱�Ƃ�����B
		���O�Ȃǂ̊J�����Q�[���ł���Η��p�ł���
		*/
		color = shadowMap.Sample(samp, uv);
		color.rgb = step(color.a, 0.8f);
		color.a = 1.0f;
	}
	return color;
}
