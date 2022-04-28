struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);

// ���C�g�̏��
cbuffer Light : register(b0)
{
	float4 direction;	// 16B��؂�łȂ��Ƃ����Ȃ�
}

float4 main(PS_IN pin) : SV_Target
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	//color = tex.Sample(samp, pin.uv);
	
	// �@���̃f�[�^���������n����Ă��邩�m�F����
	// �m�F���@�Ƃ��ĐF���Ƃ��ďo�͂���(�@���ȊO�̃f�[�^�����̕��@
	//color.rgb = pin.normal;
	// �}�C�i�X�̒l��0�Ɋۂߍ��܂�Ă��܂��̂ŁA�S�č��F�ŕ\������Ă��܂��B
	// �v���X�̏�񂩂�}�C�i�X�����������Ŋm�F�ł���ꍇ�����邪�A
	// ���m�Ɋm�F����̂ł���΃}�C�i�X(-1~1)���v���X(0~1)�ɕϊ����Ă�����
	//color.rgb = (color.rgb + 1.0f) * 0.5f;
	//color = direction;
	//color.a = 1.0f;

	// �����o�[�g�V�F�[�f�B���O
	// �I�u�W�F�N�g�ɉe������v�Z
	// �I�u�W�F�N�g�ɉe�����邽�߂ɂ͑��z(����)��
	// �I�u�W�F�N�g�̖@��(�\�������Ă������)��2���K�v
	// ���z�̌��̌����ƁA�@���̌������ׂĂقړ��������������Ă��镔���ɂ�
	// �e�����A���Ε����������Ă��镔���͉e�����Ȃ��悤�ɂ���B
	float3 N = normalize(pin.normal);
	// �e�̌v�Z�͓�̃x�N�g���̓��ς���v�Z����B���ς̌��ʂ͓�̃x�N�g�������������������Ă����+�A�t�̕����������Ă����-�ɂȂ�B
	// �e���������̖@���ƌ��̓��ς�+�ɂȂ邪�A�e�͐F���Â��Ȃ�=�}�C�i�X�̒l�Ȃ̂ŕ����𔽓]���Ă����B
	float3 L = -direction.xyz;	
	
	// ����
	// �E��̃x�N�g�������������������Ă���... +1
	// �E��̃x�N�g�������Ε����������Ă���... -1
	// �E��̃x�N�g�������s... 0
	// �E�P�ʃx�N�g���Ƃ̌��ʂ́A�P�ʃx�N�g����ł̒�����\��(�x�N�g�������Ƃ��e�̒���)
	
	float diffuse = dot(N, L);
	diffuse = (diffuse + 1.0f) * 0.5f;
	
	color.rgb *= diffuse;
	
	return color;
}
