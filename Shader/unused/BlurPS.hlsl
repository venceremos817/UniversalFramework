struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

cbuffer Parameter : register(b0)
{
	float2 texSize;
	float2 blurDir;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//--- ���ӂ̃s�N�Z�����Q�Ƃ��邽�߂̏�������
	// �s�N�Z���ōl����΁A��ƂȂ��1�s�N�Z���ړ������ꏊ�ƍl�����邪�A
	// �V�F�[�_�ł�UV(0~1)�ōl���Ȃ���΂Ȃ�Ȃ��B�傫��10�̉摜��1�s�N�Z���̈ړ��ʂ�0.1
	// �傫��100�̉摜��1�s�N�Z���̈ړ��ʂ�0.01 �ƈقȂ�B
	// (UV(0~1)��10�������邩�A100�������邩�̈Ⴂ)
	// ���̂��߁A�摜�T�C�Y�ɕ�����UV�̈ړ��ʂ����߂Ȃ���΂����Ȃ�
	float2 movePixel = 1.0f / texSize;
	// �������ɂڂ�����������̂��c�����ɂڂ�����������̂��́A�ʁX�̃V�F�[�_�����Ȃ��Ƃ��A
	// �p�����[�^�̍H�v�ň�̃V�F�[�_�ɂ܂Ƃ߂���
	// ���ꂼ��̕����ŁA�����Ȃ������Ɋւ��Ă͕K��0�ɂȂ�̂ŁA
	// ����������1,�����Ȃ�������0��ݒ肵�Ċ|���Z����΋��߂���B
	// ��(1,0) , �c(0,1)
	movePixel *= blurDir;

	// �ڂ���������������
	// �������g�̃s�N�Z��
	color = tex.Sample(samp, pin.uv);
	// ������(�E,��)�̃s�N�Z��
	color += tex.Sample(samp, pin.uv + movePixel * 1.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 2.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 3.0f);
	color += tex.Sample(samp, pin.uv + movePixel * 4.0f);
	// ������(��,��)�̃s�N�Z��
	color += tex.Sample(samp, pin.uv - movePixel * 1.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 2.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 3.0f);
	color += tex.Sample(samp, pin.uv - movePixel * 4.0f);
	// �������킹���s�N�Z���̕��ς����߂�
	color /= 9.0f;
	
	return color;
}