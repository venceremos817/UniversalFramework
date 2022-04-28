// 頂点シェーダから渡されてくるデータ
struct PS_IN
{
	float4 pos : SV_POSITION;
};

// エントリポイント
float4 main(PS_IN pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
