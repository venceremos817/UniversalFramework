#pragma once
#include <d3d11.h>
#include <XMFLOAT_EX.h>


#pragma region InputLayout
/// <summary>
/// 位置をもつインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[] =
{
	{"SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
};

/// <summary>
/// 位置と色を持つインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// 位置とテクスチャUVを持つインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// 位置・色・テクスチャUVを持つインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// 位置・法線・テクスチャUVをもつインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[] =
{
	{"SV_Position"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD"		, 0, DXGI_FORMAT_R32G32_FLOAT	, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
};

const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureTangentLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// 位置・法線・テクスチャUV・スキニング情報を持つインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDindexes",0, DXGI_FORMAT_R32G32B32A32_UINT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// 位置・法線・テクスチャUV・スキニング情報を持つインプットレイアウト
/// </summary>
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentTextureSkinningLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDindexes",0, DXGI_FORMAT_R32G32B32A32_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
#pragma endregion



#pragma region Vertex
/// <summary>
/// 位置を持つ頂点
/// </summary>
struct VertexPosition
{
	Float3 position;

	VertexPosition() {}
	VertexPosition(float x, float y, float z) :position(x, y, z) {}
	VertexPosition(const Float3& pos) :position(pos) {}
	VertexPosition(DirectX::FXMVECTOR pos)
	{
		DirectX::XMStoreFloat3(&position, pos);
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionLayout); }
};

/// <summary>
/// 位置・色を持つ頂点
/// </summary>
struct VertexPositionColor
{
	Float3 position;
	Float4 color;

	VertexPositionColor() {}
	VertexPositionColor(const Float3& pos, const Float4& col) :position(pos), color(col) {}
	VertexPositionColor(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR col)
	{
		DirectX::XMStoreFloat3(&position, pos);
		DirectX::XMStoreFloat4(&color, col);
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionColorLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionColorLayout); }
};

/// <summary>
/// 位置・テクスチャUVを持つ頂点
/// </summary>
struct VertexPositionTexture
{
	Float3 position;
	Float2 texCoord;

	VertexPositionTexture() {}
	VertexPositionTexture(const Float3& pos, const Float2& uv)
		: position(pos), texCoord(uv)
	{}
	VertexPositionTexture(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&position, pos);
		DirectX::XMStoreFloat2(&texCoord, uv);
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionTextureLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionTextureLayout); }
};

/// <summary>
/// 位置・色・テクスチャUVを持つ頂点
/// </summary>
struct VertexPositionColorTexture
{
	Float3 position;
	Float4 color;
	Float2 texCoord;

	VertexPositionColorTexture() {}
	VertexPositionColorTexture(const Float3& pos, const Float4& col, const Float2& uv)
		: position(pos), color(col), texCoord(uv)
	{}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionColorTextureLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionColorTextureLayout); }
};

/// <summary>
/// 位置・法線・テクスチャUVを持つ頂点
/// </summary>
struct VertexPositionNormalTexture
{
	Float3 position;
	Float3 normal;
	Float2 texCoord;

	VertexPositionNormalTexture() {}
	VertexPositionNormalTexture(const Float3& pos, const Float3& norm, const Float2& uv)
		: position(pos), normal(normal), texCoord(uv)
	{}
	VertexPositionNormalTexture(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR norm, DirectX::FXMVECTOR uv)
	{
		DirectX::XMStoreFloat3(&position, pos);
		DirectX::XMStoreFloat3(&normal, norm);
		DirectX::XMStoreFloat2(&texCoord, uv);
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionNormalTextureLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionNormalTextureLayout); }
};

struct VertexPositionNormalTextureTangent
{
	Float3 position;
	Float3 normal;
	Float2 texCoord;
	Float4 tangent;

	VertexPositionNormalTextureTangent()
	{}
	VertexPositionNormalTextureTangent(const Float3& pos, const Float3& norm, const Float2& uv, const Float4& tan)
		:position(pos), normal(norm), texCoord(uv), tangent(tan)
	{}
	VertexPositionNormalTextureTangent(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR norm, DirectX::FXMVECTOR uv, DirectX::FXMVECTOR tan)
	{
		DirectX::XMStoreFloat3(&position, pos);
		DirectX::XMStoreFloat3(&normal, norm);
		DirectX::XMStoreFloat2(&texCoord, uv);
		DirectX::XMStoreFloat4(&tangent, tan);
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionNormalTextureTangentLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionNormalTextureTangentLayout); }
};

/// <summary>
/// 位置・法線・テクスチャUV・スキニング情報を持つ頂点
/// </summary>
struct VertexPositionNormalTextureSkinning
{
	Float3 position;
	Float3 normal;
	Float2 texCoord;
	uint32_t indexes[4];
	float weights[4];

	VertexPositionNormalTextureSkinning() {}
	VertexPositionNormalTextureSkinning(const Float3& pos, const Float3& norm, const Float2& uv, uint32_t* index, float* weight)
		: position(pos), normal(norm), texCoord(uv)
	{
		for (int i = 0; i < 4; ++i)
		{
			indexes[i] = index[i];
			weights[i] = weight[i];
		}
	}
	VertexPositionNormalTextureSkinning(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR norm, DirectX::FXMVECTOR uv, uint32_t* index, float* weight)
	{
		DirectX::XMStoreFloat3(&position, pos);
		DirectX::XMStoreFloat3(&normal, norm);
		DirectX::XMStoreFloat2(&texCoord, uv);
		for (int i = 0; i < 4; ++i)
		{
			indexes[i] = index[i];
			weights[i] = weight[i];
		}
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionNormalTextureSkinningLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionNormalTextureSkinningLayout); }
};

/// <summary>
/// 位置・法線・タンジェント・テクスチャUV・スキニング情報をもつ頂点
/// </summary>
struct VertexPositionNormalTangentTextureSkinning
{
	Float3 position;
	Float3 normal;
	Float4 tangent;
	Float2 texCoord;
	uint32_t indexes[4];
	float weights[4];

	VertexPositionNormalTangentTextureSkinning() {}
	VertexPositionNormalTangentTextureSkinning(const Float3& pos, const Float3& norm, const Float4& tan, const Float2& uv, uint32_t* index, float* weight)
		: position(pos), normal(norm), tangent(tan), texCoord(uv)
	{
		for (int i = 0; i < 4; ++i)
		{
			indexes[i] = index[i];
			weights[i] = weight[i];
		}
	}

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() { return VertexPositionNormalTangentTextureSkinningLayout; }
	static UINT GetElementsNum() { return ARRAYSIZE(VertexPositionNormalTangentTextureSkinningLayout); }
};

#pragma endregion
