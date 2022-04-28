struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 shadowUV : TEXCOORD1;
};

Texture2D tex : register(t0);
// �e�̏�񂪊i�[���ꂽ�e�N�X�`��
Texture2D shadowMap : register(t1);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// �����ŕ`�����񂾏����擾���邽�߁A�Y���̃s�N�Z����
	// UV���W�̂ǂ̈ʒu���v�Z
	float2 uv = pin.shadowUV.xy / pin.shadowUV.w;
	// �������猩�����ɁA�Y���̃s�N�Z�����J��������ǂ̒��x����Ă��邩
	float uvDepth = pin.shadowUV.z / pin.shadowUV.w;

	// ���O�ɏ�������ł����J��������̋������擾
	float shadowDepth = shadowMap.Sample(samp, uv).r;
	if (0 < uv.x && uv.x < 1 && 0 < uv.y && uv.y < 1)
	{
		// �V���h�E�A�N�l
		// �[�x�o�b�t�@�V���h�E�����������Ƃ��ɁA�\�ʂɏo����g�ł����悤�Ȗ͗l
		// �J��������̋����̐��x�̓e�N�X�`���̉𑜓x�ɂ���ĈقȂ�(512x512��1024x1024�ł͑e�����قȂ�)
		// ���ۂ̕`��̓s�N�Z���V�F�[�_�ōׂ����v�Z����Ă��邽�߁A�v�Z���ꂽ�l�ƃe�N�X�`���ɕ`�����܂�Ă������ł͔����ȍ����o�Ă��܂��B
		// ���̌��ʁA�{���e���ł��Ȃ��ꏊ�ɔg�ł����悤�Ȗ͗l�������яオ��B
		// �����������邽�߂ɂ́A�J��������̋������������ɂ��炵�Ă����邾���ŉ����ł���B
		// ���ׂ����덷�Ő�����͗l�̂��߁A�덷�������Ȃ��悤�ɒl��傫��
		if (shadowDepth + 0.001f < uvDepth)
			color.rgb = 0;
	}
	//color.rgb = shadowDepth;
	return color;
}