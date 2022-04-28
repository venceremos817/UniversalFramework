struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

Texture2D tex : register(t0);
Texture2D rampTex : register(t1);
SamplerState samp : register(s0);

cbuffer Light : register(b0)
{
	float4 direction;
};


float4 main(PS_IN pin):SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float diffuse = dot(pin.normal, -direction.xyz);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	/* �g�D�[�������_�����O(�Z���V�F�[�f�B���O)
	���ۂ�3D�����A��������2D�̂悤�Ɍ����������@�B
	�\�������I�u�W�F�N�g�̉e���A�j���h��̂悤�ɕ\������B(���{�̍�i�ɑ���������\��)
	���܂ł̕\���ł͖��邢����(1)����Â�����(0)�֏��X�ɐ������ω����Ă������A�g�D�[�������_�����O�ł�
	���X�ɕω��������ɁA�i�K�I(2�`4�i�K)�ɂȂ�悤�ɐ������v�Z����
	*/
	
	// �@�s�N�Z���V�F�[�_���Œi�K�I�Ȑ����Ɍv�Z
	// step... �w�肳�ꂽ�������傫����� 1
	//float highLight = step(0.8f, diffuse) * 0.3f;
	//float baseLight = step(0.4f, diffuse) * 0.4f;
	//float shadowLight = step(0.0f, diffuse) * 0.3f;
	// diffuse = highLight + baseLight + shadowLight;
	
	
	
	// �A�摜�t�@�C������i�K�I�ȉe��\��
	// �����v�e�N�X�`���ƌĂ΂��摜���疾�邳���擾����B
	// �����Ɩ@������v�Z�������邳��1�`0�̊Ԃŕ\�����邽�߁A���̒l��UV�̎Q�ƒl�Ɏg����
	// �����v�e�N�X�`�����疾�邳���擾�ł���
	diffuse = rampTex.Sample(samp, float2(diffuse * 0.98 + 0.01f, 0.5f)).r;
	
	color.rgb *= diffuse;
	return color;
}