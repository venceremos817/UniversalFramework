// ���_�V�F�[�_����n����Ă���f�[�^
struct PS_IN
{
	float4 pos : SV_POSITION;
};

// �G���g���|�C���g
float4 main(PS_IN pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
