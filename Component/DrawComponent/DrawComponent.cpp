#include "DrawComponent.h"
#include <GameObject.h>
#include <Stage.h>
#include <XMFLOAT_EX.h>



//=================================================
// DrawComponent
//=================================================
#pragma region DrawComponent

struct DrawComponentBase::Param
{
	BlendState        m_blendState;			// ブレンドステート
	DepthStencilState m_depthStencil;		// デプスステンシルステート
	RasterizerState   m_rasterizerState;	// ラスタライザステート
	SamplerState      m_samplerState;		// サンプラーステート
	Float4x4 m_meshOffsetTransform;			// メッシュとオブジェクトの差を補完する行列

	std::shared_ptr<MeshResource> m_meshResouorce;	// メッシュリソース
	std::vector<std::shared_ptr<TextureResouce>> m_textureResource;	// テクスチャリソース
	//std::shared_ptr<TextureResouce> m_normalMapTextureResource;	// ノーマルマップテクスチャリソース

	Float4 m_emissive;	// エミッシブ
	Float4 m_diffuse;	// ディフューズ
	float m_alpha;		// 透過度
	Float4 m_specular;	// スペキュラー
	Float4 m_ambientLightColor;	// アンビエント

	ID3D11VertexShader*	m_vertexShader;		// 頂点シェーダ
	ID3D11InputLayout*	m_inputLayout;		// インプットレイアウト
	ID3D11PixelShader*	m_pixelShader;		// ピクセルシェーダ

	Float2 m_uvScale;

	bool m_lightingEnable;
	bool m_ownShadowActive;

	Param()
		: m_meshOffsetTransform()
		, m_emissive(1.0f), m_diffuse(1.0f), m_alpha(1.0f), m_specular(1.0), m_ambientLightColor(0.0f)
		, m_vertexShader(nullptr), m_inputLayout(nullptr), m_pixelShader(nullptr)
		, m_uvScale(1.0f)
		, m_lightingEnable(true), m_ownShadowActive(false)
	{}
};

DrawComponentBase::DrawComponentBase(const std::shared_ptr<GameObject>& pGameObject)
	: Component(pGameObject)
	, m_param(new Param())
{
	// 描画有効化
	GetGameObject()->SetDrawActive(true);
}

DrawComponentBase::~DrawComponentBase()
{
}

BlendState DrawComponentBase::GetBlendState() const
{
	return m_param->m_blendState;
}

DepthStencilState DrawComponentBase::GetDepthStencilState() const
{
	return m_param->m_depthStencil;
}

RasterizerState DrawComponentBase::GetRasterizerState() const
{
	return m_param->m_rasterizerState;
}

SamplerState DrawComponentBase::GetSamplerState() const
{
	return m_param->m_samplerState;
}

void DrawComponentBase::SetBlendState(const BlendState & blendState)
{
	m_param->m_blendState = blendState;
}

void DrawComponentBase::SetDepthStencilState(const DepthStencilState & depthStencilState)
{
	m_param->m_depthStencil = depthStencilState;
}

void DrawComponentBase::SetRasterizerState(const RasterizerState & rasterizerState)
{
	m_param->m_rasterizerState = rasterizerState;
}

void DrawComponentBase::SetSamplerState(const SamplerState & samplerState)
{
	m_param->m_samplerState = samplerState;
}

void DrawComponentBase::BindDeviceBlendState()
{
	auto dev = APP->GetDeviceResources();
	auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
	auto pDevRenderState = dev->GetRenderState();
	switch (GetBlendState())
	{
	case BlendState::Opaque:
		pID3D11DeviceContext->OMSetBlendState(pDevRenderState->GetOpaque(), nullptr, 0xffffffff);
		break;
	case BlendState::AlphaBlend:
		pID3D11DeviceContext->OMSetBlendState(pDevRenderState->GetAlphaBlend(), nullptr, 0xffffffff);
		break;
	case BlendState::Additive:
		pID3D11DeviceContext->OMSetBlendState(pDevRenderState->GetAdditive(), nullptr, 0xffffffff);
		break;
	case BlendState::NonPremultiplied:
		pID3D11DeviceContext->OMSetBlendState(pDevRenderState->GetNonPremultiplied(), nullptr, 0xffffffff);
		break;
	}
}

void DrawComponentBase::BindDeviceDepthStencilState()
{
	auto dev = APP->GetDeviceResources();
	auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
	auto pDevRenderState = dev->GetRenderState();
	switch (GetDepthStencilState())
	{
	case DepthStencilState::None:
		pID3D11DeviceContext->OMSetDepthStencilState(pDevRenderState->GetDepthNone(), 0);
		break;
	case DepthStencilState::Default:
		pID3D11DeviceContext->OMSetDepthStencilState(pDevRenderState->GetDepthDefault(), 0);
		break;
	case DepthStencilState::Read:
		pID3D11DeviceContext->OMSetDepthStencilState(pDevRenderState->GetDepthRead(), 0);
		break;
	}
}

void DrawComponentBase::BindDeviceRasterizerState()
{
	auto dev = APP->GetDeviceResources();
	auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
	auto pDevRenderState = dev->GetRenderState();
	switch (GetRasterizerState())
	{
	case RasterizerState::CullNone:
		pID3D11DeviceContext->RSSetState(pDevRenderState->GetCullNone());
		break;
	case RasterizerState::CullFront:
		pID3D11DeviceContext->RSSetState(pDevRenderState->GetCullFront());
		break;
	case RasterizerState::CullBack:
		pID3D11DeviceContext->RSSetState(pDevRenderState->GetCullBack());
		break;
	case RasterizerState::Wireframe:
		pID3D11DeviceContext->RSSetState(pDevRenderState->GetWireframe());
		break;
	}
}

void DrawComponentBase::BindDeviceSamplerState()
{
	auto dev = APP->GetDeviceResources();
	auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
	auto pDevRenderState = dev->GetRenderState();
	ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
	ID3D11SamplerState* pSampler = nullptr;
	switch (GetSamplerState())
	{
	case SamplerState::SamplerNone:
		// サンプラーをクリアして終了
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		return;
		break;
	case SamplerState::PointWrap:
		pSampler = pDevRenderState->GetPointWrap();
		break;
	case SamplerState::PointClamp:
		pSampler = pDevRenderState->GetPointClamp();
		break;
	case SamplerState::LinearWrap:
		pSampler = pDevRenderState->GetLinearWrap();
		break;
	case SamplerState::LinearClamp:
		pSampler = pDevRenderState->GetLinearClamp();
		break;
	case SamplerState::AnisotropicWrap:
		pSampler = pDevRenderState->GetAnisotropicWrap();
		break;
	case SamplerState::AnisotropicClamp:
		pSampler = pDevRenderState->GetAnisotropicClamp();
		break;
	case SamplerState::ComparisonLinear:
		pSampler = pDevRenderState->GetComparisonLinear();
		break;
	}
	// サンプラー適用
	pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
}

const Float4x4 & DrawComponentBase::GetMeshOffsetTransform() const
{
	return m_param->m_meshOffsetTransform;
}

void DrawComponentBase::SetMeshOffsetTransform(const Float4x4 & offsetTransform)
{
	m_param->m_meshOffsetTransform = offsetTransform;
}

std::shared_ptr<MeshResource> DrawComponentBase::GetMeshResource() const
{
	return m_param->m_meshResouorce;
}

void DrawComponentBase::SetMeshResource(const std::shared_ptr<MeshResource>& meshRes)
{
	m_param->m_meshResouorce = meshRes;
}

void DrawComponentBase::SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, UINT slot)
{
	if (m_param->m_textureResource.size() < slot + 1)
		m_param->m_textureResource.resize(slot + 1);
	m_param->m_textureResource[slot] = textureRes;
}

void DrawComponentBase::SetTextureResource(const std::string textureKey, UINT slot)
{
	SetTextureResource(APP->GetResource<TextureResouce>(textureKey), slot);
}

std::shared_ptr<TextureResouce> DrawComponentBase::GetTextureResource(UINT slot) const
{
	if (m_param->m_textureResource.size() < slot + 1)
		return nullptr;
	return m_param->m_textureResource[slot];
}

Float4 DrawComponentBase::GetEmissive() const
{
	return m_param->m_emissive;
}

void DrawComponentBase::SetEmissive(const Float4 & emissive)
{
	m_param->m_emissive = emissive;
}

Float4 DrawComponentBase::GetDiffuse() const
{
	return m_param->m_diffuse;
}

void DrawComponentBase::SetDiffuse(const Float4 & diffuse)
{
	m_param->m_diffuse = diffuse;
}

Float4 DrawComponentBase::GetAmbientLightColor() const
{
	return m_param->m_ambientLightColor;
}

void DrawComponentBase::SetAmbientLightColor(const Float4 & ambientLightColor)
{
	m_param->m_ambientLightColor = ambientLightColor;
}

Float4 DrawComponentBase::GetSpecular() const
{
	return m_param->m_specular;
}

void DrawComponentBase::SetSpecular(const Float4 & specular)
{
	m_param->m_specular = specular;
}

float DrawComponentBase::GetSpecularPower() const
{
	return m_param->m_specular.w;
}

void DrawComponentBase::SetSpecularPower(float power)
{
	m_param->m_specular.w = power;
}

void DrawComponentBase::DisableSpecular()
{
	m_param->m_specular = Float4(0.0f, 0.0f, 0.0f, 1.0f);
}

float DrawComponentBase::GetAlpha() const
{
	return m_param->m_alpha;
}

void DrawComponentBase::SetAlpha(float a)
{
	m_param->m_alpha = a;
}

Float4 DrawComponentBase::GetColorAndAlpha() const
{
	Float4 color = m_param->m_diffuse;
	color.w = m_param->m_alpha;
	return color;
}

void DrawComponentBase::SetColorAlpha(const Float4 & color)
{
	m_param->m_diffuse = color;
	m_param->m_alpha = color.w;
}

void DrawComponentBase::SetVertexShader(ID3D11VertexShader * pVS, ID3D11InputLayout * pIL)
{
	m_param->m_vertexShader = pVS;
	m_param->m_inputLayout = pIL;
}

ID3D11VertexShader * DrawComponentBase::GetVertexShader()
{
	return m_param->m_vertexShader;
}

ID3D11InputLayout * DrawComponentBase::GetInputLayout()
{
	return m_param->m_inputLayout;
}

void DrawComponentBase::SetPixelShader(ID3D11PixelShader * pPS)
{
	m_param->m_pixelShader = pPS;
}

ID3D11PixelShader * DrawComponentBase::GetPixelShader()
{
	return m_param->m_pixelShader;
}

void DrawComponentBase::SetUVScale(const Float2 & scale)
{
	m_param->m_uvScale = scale;
}

const Float2 & DrawComponentBase::GetUVScale()
{
	return m_param->m_uvScale;
}

void DrawComponentBase::SetLightingEnable(bool b)
{
	m_param->m_lightingEnable = b;
}

bool DrawComponentBase::IsLightingEnable()
{
	return m_param->m_lightingEnable;
}

void DrawComponentBase::SetOwnShadowActive(bool b)
{
	m_param->m_ownShadowActive = b;
}

bool DrawComponentBase::IsOwnShadowActive()
{
	return m_param->m_ownShadowActive;
}

#pragma endregion

//=================================================
// MeshResource
//=================================================
#pragma region MeshResource
/// <summary>
/// 頂点とインデックスの向きを反転させる(LH/RH変換)
/// </summary>
/// <param name="indexes">インデックス配列</param>
/// <param name="vertices">頂点配列</param>
void ReverseWinding(std::vector<uint16_t>& indexes, std::vector<VertexPositionNormalTexture>& vertices)
{
	if ((indexes.size() % 3) != 0)
	{
		ThrowBaseException(
			"インデックス数の数が合わない",
			"if((indexes.size() % 3) != 0)",
			"ReverseWinding()"
		);
	}
	for (auto it = indexes.begin(); it != indexes.end(); it += 3)
		std::swap(*it, *(it + 2));

	for (auto it = vertices.begin(); it != vertices.end(); ++it)
		it->texCoord.x = (1.0f - it->texCoord.x);
}

/// <summary>
/// 分割数と分割インデックスからベクトルを取得する (Cylinder・Coneで利用する
/// </summary>
/// <param name="i">分割インデックス</param>
/// <param name="tessellation">分割数</param>
/// <returns></returns>
DirectX::XMVECTOR GetCircleVector(size_t i, size_t tessellation)
{
	float angle = i * DirectX::XM_2PI / tessellation;
	float dx, dz;

	DirectX::XMScalarSinCos(&dx, &dz, angle);

	DirectX::XMVECTORF32 v = { dx, 0, dz, 0 };
	return v;
}

/// <summary>
/// CylinderCapを作成する (Cylinder・Coneで使用する
/// </summary>
/// <param name="vertices">頂点配列</param>
/// <param name="indexes">インデックス配列</param>
/// <param name="tesselation">分割数</param>
/// <param name="height">高さ</param>
/// <param name="radius">半径</param>
/// <param name="isTop">上部かどうか</param>
void CreateCylinderCap(std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes, size_t tesselation, float height, float radius, bool isTop)
{
	for (size_t i = 0; i < tesselation - 2; ++i)
	{
		size_t i1 = (i + 1) % tesselation;
		size_t i2 = (i + 2) % tesselation;

		if (isTop)
			std::swap(i1, i2);

		size_t vbase = vertices.size();
		indexes.push_back((uint16_t)vbase);
		indexes.push_back((uint16_t)(vbase + i1));
		indexes.push_back((uint16_t)(vbase + i2));
	}

	DirectX::XMVECTOR normal = DirectX::g_XMIdentityR1;
	DirectX::XMVECTOR textureScale = DirectX::g_XMNegativeOneHalf;

	if (!isTop)
	{
		normal = normal * -1.0f;
		textureScale = DirectX::XMVectorMultiply(textureScale, DirectX::g_XMNegateX);
	}

	for (size_t i = 0; i < tesselation; ++i)
	{
		DirectX::XMVECTOR circleVector = GetCircleVector(i, tesselation);
		DirectX::XMVECTOR position = (circleVector * radius) + (normal * height);
		DirectX::XMVECTOR textureCoordinate = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, DirectX::g_XMOneHalf);

		vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));
	}
}

/// <summary>
/// 分割数とインデックスからtangentベクトルを取得 (Cylinder・Coneで使用
/// </summary>
/// <param name="i"></param>
/// <param name="tessellation"></param>
/// <returns></returns>
DirectX::XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
{
	float angle = (i * DirectX::XM_2PI / tessellation) + DirectX::XM_PIDIV2;
	float dx, dz;
	DirectX::XMScalarSinCos(&dx, &dz, angle);
	DirectX::XMVECTORF32 v = { dx, 0, dz, 0 };
	return v;
}

MeshResource::~MeshResource()
{
}

const MeshPrimData & MeshResource::GetMeshData() const
{
	return m_meshPrimData;
}

void MeshResource::SetMeshData(const MeshPrimData & meshData)
{
	m_meshPrimData = meshData;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> MeshResource::GetVertexBuffer() const
{
	return m_meshPrimData.m_vertexBuff;
}

UINT MeshResource::GetVerticesNum() const
{
	return m_meshPrimData.m_verticesNum;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> MeshResource::GetIndexBuffer() const
{
	return m_meshPrimData.m_indexBuff;
}

UINT MeshResource::GetindexesNum() const
{
	return m_meshPrimData.m_indexesNum;
}

UINT MeshResource::GetStridesNum() const
{
	return m_meshPrimData.m_strideNum;
}

D3D11_PRIMITIVE_TOPOLOGY MeshResource::GetPrimitiveTopology() const
{
	return m_meshPrimData.m_primitiveTopology;
}

void MeshResource::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	m_meshPrimData.m_primitiveTopology = topology;
}

std::type_index MeshResource::GetTypeIndex() const
{
	return m_meshPrimData.m_meshTypeIndex;
}

const Float4x4 & MeshResource::GetMeshOffsetTransform() const
{
	return m_meshPrimData.m_meshOffsetTransform;
}

void MeshResource::SetMeshOffsetTransform(const Float4x4& offsetTransform)
{
	m_meshPrimData.m_meshOffsetTransform = offsetTransform;
}

bool MeshResource::IsUseMeshOffsetTransform()
{
	return m_meshPrimData.m_useMeshOffsetTransform;
}

void MeshResource::SetUseMeshOffsetTransform(bool useOffsetTransform)
{
	m_meshPrimData.m_useMeshOffsetTransform = useOffsetTransform;
}

void MeshResource::SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource)
{
	m_meshPrimData.m_textureResource = textureResource;
}

void MeshResource::SetTextureResource(const std::string & textureKey)
{
	this->SetTextureResource(APP->GetResource<TextureResouce>(textureKey));
}

std::shared_ptr<TextureResouce> MeshResource::GetTextureResource() const
{
	auto texture = m_meshPrimData.m_textureResource.lock();
	if (texture)
		return texture;
	// テクスチャがなければnull
	return nullptr;
}

const std::vector<Material>& MeshResource::GetMaterialVec() const
{
	return m_meshPrimData.m_materialVec;
}

void MeshResource::CreatePrimitiveBuffer(ID3D11Device * pDX11Device, const D3D11_BUFFER_DESC * pBufferDesc, const D3D11_SUBRESOURCE_DATA * pDataDesc, ID3D11Buffer ** ppBuffer)
{
	if (FAILED(pDX11Device->CreateBuffer(pBufferDesc, pDataDesc, ppBuffer)))
	{
		throw StandardException(
			"バッファ作成失敗",
			"if(FAILED(pDX11Device->CreateBuffer(pBufferDesc, pDataDesc, ppBuffer)))",
			"MeshResource::CreatePrimitiveBuffer()"
		);
	}
}

void MeshResource::CreateSquareVertices(float size,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	float halfSize = size * 0.5f;
	// 頂点
	vertices.push_back(VertexPositionNormalTexture(Float3(-halfSize, halfSize, 0), Float3(0.0f, 0.0f, -1.0f), Float2(0.0f, 0.0f)));
	vertices.push_back(VertexPositionNormalTexture(Float3(halfSize, halfSize, 0), Float3(0.0f, 0.0f, -1.0f), Float2(1.0f, 0.0f)));
	vertices.push_back(VertexPositionNormalTexture(Float3(-halfSize, -halfSize, 0), Float3(0.0f, 0.0f, -1.0f), Float2(0.0f, 1.0f)));
	vertices.push_back(VertexPositionNormalTexture(Float3(halfSize, -halfSize, 0), Float3(0.0f, 0.0f, -1.0f), Float2(1.0f, 1.0f)));
	// インデックス
	indexes.push_back((uint16_t)0);
	indexes.push_back((uint16_t)1);
	indexes.push_back((uint16_t)2);
	indexes.push_back((uint16_t)1);
	indexes.push_back((uint16_t)3);
	indexes.push_back((uint16_t)2);
}

void MeshResource::CreateCubeVertices(float size,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	const int faceNum = 6;
	static const DirectX::XMVECTORF32 faceNormals[faceNum] =
	{
		{ 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f,-1.0f}, { 1.0f, 0.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f}, { 0.0f,-1.0f, 0.0f}
	};
	static const DirectX::XMVECTORF32 textureCoordinates[4] =
	{
		{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}
	};
	size *= 0.5f;
	for (int i = 0; i < faceNum; ++i)
	{
		DirectX::XMVECTOR normal = faceNormals[i];

		DirectX::XMVECTOR basis = (i >= 4) ? DirectX::g_XMIdentityR2 : DirectX::g_XMIdentityR1;
		DirectX::XMVECTOR side1 = DirectX::XMVector3Cross(normal, basis);
		DirectX::XMVECTOR side2 = DirectX::XMVector3Cross(normal, side1);

		// インデックス
		size_t vbase = vertices.size();
		indexes.push_back((uint16_t)vbase + 0);
		indexes.push_back((uint16_t)vbase + 1);
		indexes.push_back((uint16_t)vbase + 2);
		indexes.push_back((uint16_t)vbase + 0);
		indexes.push_back((uint16_t)vbase + 2);
		indexes.push_back((uint16_t)vbase + 3);
		// 頂点
		vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
		vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
		vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
		vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
	}
	// RHからLHに変換
	ReverseWinding(indexes, vertices);
}

void MeshResource::CreateSphereVertices(float diameter, size_t tessellation,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	if (tessellation < 3)
		tessellation = 3;

	float radius = diameter * 0.5f;
	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; ++i)
	{
		float v = 1 - (float)i / verticalSegments;
		float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
		float dy, dxz;
		DirectX::XMScalarSinCos(&dy, &dxz, latitude);

		for (size_t j = 0; j <= horizontalSegments; ++j)
		{
			float u = (float)j / horizontalSegments;
			float longitude = j * DirectX::XM_2PI / horizontalSegments;
			float dx, dz;
			DirectX::XMScalarSinCos(&dx, &dz, longitude);
			dx *= dxz;
			dz *= dxz;

			DirectX::XMVECTOR normal = DirectX::XMVectorSet(dx, dy, dz, 0);
			DirectX::XMVECTOR textureCoordinate = DirectX::XMVectorSet(u, v, 0, 0);
			vertices.push_back(VertexPositionNormalTexture(normal * radius, normal, textureCoordinate));
		}
	}
	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; ++i)
	{
		for (size_t j = 0; j <= horizontalSegments; ++j)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indexes.push_back((uint16_t)(i * stride + j));
			indexes.push_back((uint16_t)(nextI * stride + j));
			indexes.push_back((uint16_t)(i * stride + nextJ));

			indexes.push_back((uint16_t)(i * stride + nextJ));
			indexes.push_back((uint16_t)(nextI * stride + j));
			indexes.push_back((uint16_t)(nextI * stride + nextJ));
		}
	}
	// RHからLHに変換
	ReverseWinding(indexes, vertices);
}

void MeshResource::CreateCapsuleVertices(float diameter, float height, size_t tessellation,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	Float3 pointA(0, -height * 0.5f, 0);
	Float3 pointB(0, height * 0.5f, 0);

	if (tessellation < 3)
		tessellation = 3;

	float radius = diameter * 0.5f;
	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; ++i)
	{
		float v = 1 - (float)i / verticalSegments;
		float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
		float dy, dxz;
		DirectX::XMScalarSinCos(&dy, &dxz, latitude);

		Float3 centerPos = pointA;
		if (i >= (verticalSegments * 0.5f))
			centerPos = pointB;

		float sphereTotalFront = diameter * DirectX::XM_PI * 0.5f;
		float sylinderTotalFront = abs(pointA.y - pointB.y);
		float sphereRate = sphereTotalFront / (sphereTotalFront + sylinderTotalFront) * 0.5f;
		if (v <= 0.5f)
		{
			v *= sphereRate;
		}
		else
		{
			v = 1.0f - ((1.0f - v) * sphereRate);
		}

		for (size_t j = 0; j <= horizontalSegments; ++j)
		{
			float u = (float)j / horizontalSegments;
			float longitude = j * DirectX::XM_2PI / horizontalSegments;
			float dx, dz;
			DirectX::XMScalarSinCos(&dx, &dz, longitude);
			dx *= dxz;
			dz *= dxz;

			DirectX::XMVECTOR normal = DirectX::XMVectorSet(dx, dy, dz, 0);
			DirectX::XMVECTOR textureCoordinate = DirectX::XMVectorSet(u, v, 0, 0);

			vertices.push_back(VertexPositionNormalTexture(normal * radius + (DirectX::XMVECTOR)centerPos, normal, textureCoordinate));
		}
	}

	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; ++i)
	{
		for (size_t j = 0; j <= horizontalSegments; ++j)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indexes.push_back((uint16_t)(i * stride + j));
			indexes.push_back((uint16_t)(nextI * stride + j));
			indexes.push_back((uint16_t)(i * stride + nextJ));

			indexes.push_back((uint16_t)(i * stride + nextJ));
			indexes.push_back((uint16_t)(nextI * stride + j));
			indexes.push_back((uint16_t)(nextI * stride + nextJ));
		}
	}
	//RHからLHに変更
	ReverseWinding(indexes, vertices);
}

void MeshResource::CreateCylinderVertices(float diameter, float height, size_t tessellation,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	if (tessellation < 3)
		tessellation = 3;

	height *= 0.5f;
	DirectX::XMVECTOR topOffset = DirectX::XMVectorScale(DirectX::g_XMIdentityR1, height);
	float radius = diameter * 0.5f;
	size_t stride = tessellation + 1;

	for (size_t i = 0; i <= tessellation; ++i)
	{
		DirectX::XMVECTOR normal = GetCircleVector(i, tessellation);
		DirectX::XMVECTOR sideOffset = normal * radius;

		float u = (float)i / tessellation;
		DirectX::XMVECTOR textureCoordinate = DirectX::XMLoadFloat(&u);

		vertices.push_back(VertexPositionNormalTexture(sideOffset + topOffset, normal, textureCoordinate));
		vertices.push_back(VertexPositionNormalTexture(sideOffset - topOffset, normal, DirectX::XMVectorAdd(textureCoordinate, DirectX::g_XMIdentityR1)));

		indexes.push_back((uint16_t)(i * 2));
		indexes.push_back((uint16_t)((i * 2 + 2) % (stride * 2)));
		indexes.push_back((uint16_t)(i * 2 + 1));

		indexes.push_back((uint16_t)(i * 2 + 1));
		indexes.push_back((uint16_t)((i * 2 + 2) % (stride * 2)));
		indexes.push_back((uint16_t)((i * 2 + 3) % (stride * 2)));
	}
	CreateCylinderCap(vertices, indexes, tessellation, height, radius, true);
	CreateCylinderCap(vertices, indexes, tessellation, height, radius, false);
	ReverseWinding(indexes, vertices);
}

void MeshResource::CreateConeVertices(float diameter, float height, size_t tessellation,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	vertices.clear();
	indexes.clear();

	if (tessellation < 3)
		tessellation = 3;

	height *= 0.5f;
	DirectX::XMVECTOR topOffset = DirectX::XMVectorScale(DirectX::g_XMIdentityR1, height);
	float radius = diameter * 0.5f;
	size_t stride = tessellation + 1;

	for (size_t i = 0; i <= tessellation; ++i)
	{
		DirectX::XMVECTOR circleVec = GetCircleVector(i, tessellation);
		DirectX::XMVECTOR sideOffset = circleVec * radius;

		float u = (float)i / tessellation;
		DirectX::XMVECTOR textureCoordinate = DirectX::XMLoadFloat(&u);

		DirectX::XMVECTOR pt = sideOffset - topOffset;

		DirectX::XMVECTOR normal = DirectX::XMVector3Cross(GetCircleTangent(i, tessellation), topOffset - pt);
		normal = DirectX::XMVector3Normalize(normal);

		vertices.push_back(VertexPositionNormalTexture(topOffset, normal, DirectX::g_XMZero));
		vertices.push_back(VertexPositionNormalTexture(pt, normal, DirectX::XMVectorAdd(textureCoordinate, DirectX::g_XMIdentityR1)));

		indexes.push_back((uint16_t)(i * 2));
		indexes.push_back((uint16_t)((i * 2 + 3) % (stride * 2)));
		indexes.push_back((uint16_t)((i * 2 + 1) % (stride * 2)));
	}
	CreateCylinderCap(vertices, indexes, tessellation, height, radius, false);
	ReverseWinding(indexes, vertices);
}

void MeshResource::CreateTorusVertices(float diameter, float thickness, size_t tessellation,
	std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes)
{
	if (tessellation < 3)
		tessellation = 3;

	size_t stride = tessellation + 1;

	for (size_t i = 0; i <= tessellation; ++i)
	{
		float u = (float)i / tessellation;
		float outerAngle = i * DirectX::XM_2PI / tessellation - DirectX::XM_PIDIV2;
		DirectX::XMMATRIX transform = DirectX::XMMatrixTranslation(diameter / 2, 0, 0) * DirectX::XMMatrixRotationY(outerAngle);

		for (size_t j = 0; j <= tessellation; ++j)
		{
			float v = 1 - (float)j / tessellation;
			float innerAngle = j * DirectX::XM_2PI / tessellation + DirectX::XM_PI;
			float dx, dy;
			DirectX::XMScalarSinCos(&dy, &dx, innerAngle);
			DirectX::XMVECTOR normal = DirectX::XMVectorSet(dx, dy, 0, 0);
			DirectX::XMVECTOR position = normal * thickness * 0.5f;
			DirectX::XMVECTOR textureCoordinate = DirectX::XMVectorSet(u, v, 0, 0);

			position = DirectX::XMVector3Transform(position, transform);
			normal = DirectX::XMVector3TransformNormal(normal, transform);

			vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));

			size_t nextI = (i + 1) % stride;
			size_t nextJ = (j + 1) % stride;

			indexes.push_back((uint16_t)(i * stride + j));
			indexes.push_back((uint16_t)(i * stride + nextJ));
			indexes.push_back((uint16_t)(nextI * stride + j));

			indexes.push_back((uint16_t)(i * stride + nextJ));
			indexes.push_back((uint16_t)(nextI * stride + nextJ));
			indexes.push_back((uint16_t)(nextI * stride + j));
		}
	}
	// RHからLHに変換
	ReverseWinding(indexes, vertices);
}

std::shared_ptr<MeshResource> MeshResource::CreateSquareMeshResource(float size, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;	// 頂点配列
	std::vector<uint16_t> indexes;	// インデックス配列

	// 頂点作成
	CreateSquareVertices(size, vertices, indexes);

	// メッシュリソース作成
	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateCubeMeshResource(float size, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateCubeVertices(size, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateSphereMeshResource(float diameter, size_t tessellation, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateSphereVertices(diameter, tessellation, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateCapsuleMeshResource(float diameter, float height, size_t tessellation, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateCapsuleVertices(diameter, height, tessellation, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateCylinderMeshResource(float diameter, float height, size_t tessellation, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateCylinderVertices(diameter, height, tessellation, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateConeMeshResource(float diameter, float height, size_t tessellation, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateConeVertices(diameter, height, tessellation, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

std::shared_ptr<MeshResource> MeshResource::CreateTorusMeshResource(float diameter, float thickness, size_t tessellation, bool accessWrite)
{
	std::vector<VertexPositionNormalTexture> vertices;
	std::vector<uint16_t> indexes;

	CreateTorusVertices(diameter, thickness, tessellation, vertices, indexes);

	return CreateMeshResource<VertexPositionNormalTexture>(vertices, indexes, accessWrite);
}

void MeshResource::ConvertVertex(const std::vector<VertexPositionNormalTexture>& originVertices, std::vector<VertexPositionColor>& pcVertices, std::vector<VertexPositionTexture>& ptVertices, std::vector<VertexPositionNormalTextureTangent>& pnttgVertices)
{
	pcVertices.clear();
	ptVertices.clear();
	pnttgVertices.clear();
	for (size_t i = 0; i < originVertices.size(); ++i)
	{
		VertexPositionColor pcV;
		VertexPositionTexture ptV;
		VertexPositionNormalTextureTangent pnttgV;

		pcV.position = originVertices[i].position;
		pcV.color = Float4(1.0f);

		ptV.position = originVertices[i].position;
		ptV.texCoord = originVertices[i].texCoord;

		pnttgV.position = originVertices[i].position;
		pnttgV.normal = originVertices[i].normal;
		pnttgV.texCoord = originVertices[i].texCoord;
		Float3 n = Float3::Cross(pnttgV.normal, Float3(0.0f, 1.0f, 0.0f));
		pnttgV.tangent = n;

		pcVertices.push_back(pcV);
		ptVertices.push_back(ptV);
		pnttgVertices.push_back(pnttgV);
	}
}

MeshResource::MeshResource()
	: BaseResource()
{
}

void MeshResource::SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer)
{
	m_meshPrimData.m_vertexBuff = vertexBuffer;
}

void MeshResource::SetVerticiesNum(UINT verticesNum)
{
	m_meshPrimData.m_verticesNum = verticesNum;
}

void MeshResource::SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer)
{
	m_meshPrimData.m_indexBuff = indexBuffer;
}

void MeshResource::SetindexesNum(UINT indexesNum)
{
	m_meshPrimData.m_indexesNum = indexesNum;
}

#pragma endregion


//================================================
// MultiMeshResource
//================================================
#pragma region MultiMeshResource

MultiMeshResource::MultiMeshResource()
	:BaseResource()
{
}

MultiMeshResource::~MultiMeshResource()
{
}

size_t MultiMeshResource::GetMeshVecNum() const
{
	return m_meshPrimDataVec.size();
}

const std::vector<MeshPrimData>& MultiMeshResource::GetMeshVec() const
{
	return m_meshPrimDataVec;
}

size_t MultiMeshResource::AddMesh(const MeshPrimData & data)
{
	auto retCount = m_meshPrimDataVec.size();
	m_meshPrimDataVec.push_back(data);
	for (size_t i = 0; i < m_meshPrimDataVec.size(); ++i)
	{
		m_meshPrimDataVec[i].m_multiMeshIndex = (UINT)i;
	}
	return retCount;
}

size_t MultiMeshResource::AddMesh(const std::shared_ptr<MeshResource>& meshRes)
{
	auto retCount = m_meshPrimDataVec.size();
	m_meshPrimDataVec.push_back(meshRes->GetMeshData());
	for (size_t i = 0; i < m_meshPrimDataVec.size(); ++i)
	{
		m_meshPrimDataVec[i].m_multiMeshIndex = (UINT)i;
	}
	return retCount;
}

size_t MultiMeshResource::AddMesh(const std::string & meshKey)
{
	return AddMesh(APP->GetResource<MeshResource>(meshKey));
}

void MultiMeshResource::RemoveMesh(size_t index)
{
	m_meshPrimDataVec.erase(m_meshPrimDataVec.begin() + index);
	for (size_t i = 0; i < m_meshPrimDataVec.size(); ++i)
	{
		m_meshPrimDataVec[i].m_multiMeshIndex = (UINT)i;
	}
}

void MultiMeshResource::GetMeshData(MeshPrimData & outData, size_t index) const
{
	outData = m_meshPrimDataVec[index];
}

Microsoft::WRL::ComPtr<ID3D11Buffer> MultiMeshResource::GetVertexBuffer(size_t index) const
{
	return m_meshPrimDataVec[index].m_vertexBuff;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> MultiMeshResource::GetIndexBuffer(size_t index) const
{
	return m_meshPrimDataVec[index].m_indexBuff;
}

UINT MultiMeshResource::GetVerticesNum(size_t index) const
{
	return m_meshPrimDataVec[index].m_verticesNum;
}

UINT MultiMeshResource::GetIndexesNum(size_t index) const
{
	return m_meshPrimDataVec[index].m_indexesNum;
}

UINT MultiMeshResource::GetStrideNum(size_t index) const
{
	return m_meshPrimDataVec[index].m_strideNum;
}

D3D11_PRIMITIVE_TOPOLOGY MultiMeshResource::GetPrimitiveTopology(size_t index) const
{
	return m_meshPrimDataVec[index].m_primitiveTopology;
}

void MultiMeshResource::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology, size_t index)
{
	m_meshPrimDataVec[index].m_primitiveTopology = topology;
}

std::type_index MultiMeshResource::GetTypeIndex(size_t index) const
{
	return m_meshPrimDataVec[index].m_meshTypeIndex;
}

const Float4x4 & MultiMeshResource::GetMeshOffsetTransform(size_t index) const
{
	return m_meshPrimDataVec[index].m_meshOffsetTransform;
}

void MultiMeshResource::SetMeshOffsetTransform(const Float4x4 & offsetTransform, size_t index)
{
	m_meshPrimDataVec[index].m_meshOffsetTransform = offsetTransform;
}

bool MultiMeshResource::IsUseMeshOffsetTransform(size_t index) const
{
	return m_meshPrimDataVec[index].m_useMeshOffsetTransform;
}

void MultiMeshResource::SetIsUseMeshOffsetTransform(bool b, size_t index)
{
	m_meshPrimDataVec[index].m_useMeshOffsetTransform = b;
}

void MultiMeshResource::SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, size_t index)
{
	m_meshPrimDataVec[index].m_textureResource = textureRes;
}

void MultiMeshResource::SetTextureResource(const std::string & textureKey, size_t index)
{
	SetTextureResource(APP->GetResource<TextureResouce>(textureKey), index);
}

std::shared_ptr<TextureResouce> MultiMeshResource::GetTextureResource(size_t index) const
{
	auto texture = m_meshPrimDataVec[index].m_textureResource.lock();
	if (texture)
		return texture;
	return nullptr;
}

const std::vector<Material>& MultiMeshResource::GetMaterialVec(size_t index) const
{
	return m_meshPrimDataVec[index].m_materialVec;
}

bool MultiMeshResource::IsSkining(size_t index) const
{
	return m_meshPrimDataVec[index].m_isSkining;
}

UINT MultiMeshResource::GetBoneNum(size_t index) const
{
	return m_meshPrimDataVec[index].m_boneNum;
}

UINT MultiMeshResource::GetSampleNum(size_t index) const
{
	return m_meshPrimDataVec[index].m_sampleNum;
}

const std::vector<Float4x4>& MultiMeshResource::GetSampleMatrixVec(size_t index) const
{
	return m_meshPrimDataVec[index].m_sampleMatrixVec;
}

std::shared_ptr<MultiMeshResource> MultiMeshResource::CreateMultiMeshResource()
{
	return ObjectFactory::Create<MultiMeshResource>();
}

#pragma endregion


//=================================================
// SpriteBase
//=================================================
#pragma region SpriteBase

struct SpriteBase::Param
{
	std::shared_ptr<MeshResource> m_spriteMesh;			// メッシュリソース
	std::weak_ptr<TextureResouce> m_textureResource;	// テクスチャリソース
	Float4 m_emissive;	// エミッシブ
	Float4 m_diffuse;	// ディフューズ

	ID3D11VertexShader*	m_vertexShader;		// 頂点シェーダ
	ID3D11InputLayout*	m_inputLayout;		// インプットレイアウト
	ID3D11PixelShader*	m_pixelShader;		// ピクセルシェーダ

	Param()
		: m_emissive(0, 0, 0, 0)
		, m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
	{}
};

SpriteBase::SpriteBase(const std::shared_ptr<GameObject>& pGameObject)
	: DrawComponentBase(pGameObject)
	, m_param(new Param())
{
	// デフォルトのシェーダー設定
	SetShader<VS_PNTSprite, PS_PNTSprite>();

	// 描画を2Dで行う
	pGameObject->SetSpriteDraw(true);
}

SpriteBase::~SpriteBase()
{
}

void SpriteBase::OnCreate()
{
	GetGameObject()->SetDrawActive(true);
}

void SpriteBase::Draw()
{
	auto meshData = m_param->m_spriteMesh->GetMeshData();
	auto pD3D11DeviceContext = APP->GetDeviceResources()->GetD3DDeviceContext();

	// 頂点シェーダ設定
	pD3D11DeviceContext->VSSetShader(m_param->m_vertexShader, nullptr, 0);
	// インプットレイアウト設定
	pD3D11DeviceContext->IASetInputLayout(m_param->m_inputLayout);
	// ピクセルシェーダ設定
	pD3D11DeviceContext->PSSetShader(m_param->m_pixelShader, nullptr, 0);
	// 定数バッファ設定
	SpriteConstants spCB;
	WriteConstantBuffer(spCB);
	// テクスチャ
	auto texture = GetTextureResource();
	// 定数バッファ更新
	ID3D11Buffer* pConstantBuffer = CBSprite::GetInstance()->GetBuffer();
	pD3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &spCB, 0, 0);
	// 頂点シェーダに渡す
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	// ピクセルシェーダに渡す
	pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	// ストライドとオフセット
	UINT stride = meshData.m_strideNum;
	UINT offset = 0;
	// 描画方法をセット
	pD3D11DeviceContext->IASetPrimitiveTopology(meshData.m_primitiveTopology);
	// 頂点バッファをセット
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, meshData.m_vertexBuff.GetAddressOf(), &stride, &offset);
	// インデックスバッファをセット
	pD3D11DeviceContext->IASetIndexBuffer(meshData.m_indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0);

	//--- レンダリングステート設定 ---
	auto renderState = APP->GetDeviceResources()->GetRenderState();
	// ブレンドステート
	renderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
	// デプスステンシルステート
	renderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
	// テクスチャとサンプラー
	if (texture)
	{
		pD3D11DeviceContext->PSSetShaderResources(0, 1, texture->GetShaderResourceView().GetAddressOf());
		renderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
	}
	else
	{
		// シェーダーリソースクリア
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		// サンプラークリア
		renderState->SetSamplerAllClear(pD3D11DeviceContext);
	}
	// ラスタライザステート
	renderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());

	// 描画
	pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
}

void SpriteBase::SetVertexShader(ID3D11VertexShader * pVS, ID3D11InputLayout * pIL)
{
	m_param->m_vertexShader = pVS;
	m_param->m_inputLayout = pIL;
}

void SpriteBase::SetPixelShader(ID3D11PixelShader * pPS)
{
	m_param->m_pixelShader = pPS;
}

void SpriteBase::WriteConstantBuffer(SpriteConstants & spriteConstant)
{
	auto pTransform = GetGameObject()->GetTransform();
	// 変換行列
	Float4x4 world = pTransform->Get2DWorldMatrix();
	// 射影行列
	Float4x4 projection;
	GetGameObject()->Get2DDrawProjectionMatrix(projection);
	world *= projection;

	spriteConstant.emissive = GetEmissive();
	spriteConstant.diffuse = GetDiffese();
	spriteConstant.world = world;
}

std::shared_ptr<MeshResource> SpriteBase::GetMeshResource() const
{
	return m_param->m_spriteMesh;
}

void SpriteBase::SetMeshResource(const std::shared_ptr<MeshResource>& meshResource)
{
	m_param->m_spriteMesh = meshResource;
}

void SpriteBase::SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource)
{
	m_param->m_textureResource = textureResource;
}

void SpriteBase::SetTextureResource(const std::string & textureKey)
{
	this->SetTextureResource(APP->GetResource<TextureResouce>(textureKey));
}

std::shared_ptr<TextureResouce> SpriteBase::GetTextureResource() const
{
	auto pTex = m_param->m_textureResource.lock();
	if (pTex)
		return pTex;
	return nullptr;
}

Float4 SpriteBase::GetEmissive() const
{
	return m_param->m_emissive;
}

void SpriteBase::SetEmissive(const Float4 & emissive)
{
	m_param->m_emissive = emissive;
}

Float4 SpriteBase::GetDiffese() const
{
	return m_param->m_diffuse;
}

void SpriteBase::SetDiffuse(const Float4 & diffuse)
{
	m_param->m_diffuse = diffuse;
}


#pragma endregion

//=============================================
// Base3DDraw
//=============================================
#pragma region Base3DDraw

SimpleCBDrawComponent::SimpleCBDrawComponent(const std::shared_ptr<GameObject>& pGameObject)
	: DrawComponentBase(pGameObject)
{
	// デフォルト設定
	SetBlendState(BlendState::Opaque);
	SetDepthStencilState(DepthStencilState::Default);
	SetRasterizerState(RasterizerState::CullBack);
	SetSamplerState(SamplerState::LinearClamp);
}

SimpleCBDrawComponent::~SimpleCBDrawComponent()
{
}

void SimpleCBDrawComponent::Draw()
{
	auto pD3D11DeviceContext = APP->GetDeviceResources()->GetD3DDeviceContext();
	auto meshData = GetMeshResource()->GetMeshData();

	//--- シェーダー設定 ---
	pD3D11DeviceContext->VSSetShader(GetVertexShader(), nullptr, 0);
	pD3D11DeviceContext->IASetInputLayout(GetInputLayout());
	pD3D11DeviceContext->PSSetShader(GetPixelShader(), nullptr, 0);

	// 定数バッファ作成
	SimpleConstants SCB;
	WriteConstants(SCB, meshData);
	// テクスチャを使うかどうか
	auto texture = GetTextureResource(0);
	if (texture)
	{
		// テクスチャがある
		SCB.activeFlg.x = 1;
	}
	else
	{
		// コンポーネントにテクスチャが設定されていない
		if (texture = meshData.m_textureResource.lock())
		{
			// メッシュデータにテクスチャがある
			SCB.activeFlg.x = 1;
		}
		else
		{
			SCB.activeFlg.x = 0;
		}
	}

	// 定数バッファ更新
	pD3D11DeviceContext->UpdateSubresource(CBSimple::GetInstance()->GetBuffer(), 0, nullptr, &SCB, 0, 0);
	ID3D11Buffer* pConstantBuffer = CBSimple::GetInstance()->GetBuffer();
	ID3D11Buffer* pNullConstantBuffer = nullptr;
	// 頂点シェーダに渡す
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	// ピクセルシェーダに渡す
	pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
	// ストライドとオフセット
	UINT stride = meshData.m_strideNum;
	UINT offset = 0;
	// 描画方法をセット
	pD3D11DeviceContext->IASetPrimitiveTopology(meshData.m_primitiveTopology);
	// 頂点バッファをセット
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, meshData.m_vertexBuff.GetAddressOf(), &stride, &offset);
	// インデックスバッファをセット
	pD3D11DeviceContext->IASetIndexBuffer(meshData.m_indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0);

	//--- レンダリングステート設定 ---
	auto renderState = APP->GetDeviceResources()->GetRenderState();
	// ブレンドステート
	renderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
	// デプスステンシルステート
	renderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
	// テクスチャとサンプラー
	if (texture)
	{
		pD3D11DeviceContext->PSSetShaderResources(0, 1, texture->GetShaderResourceView().GetAddressOf());
		// サンプラー設定
		renderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
	}
	else
	{
		// シェーダーリソースクリア
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		// サンプラークリア
		renderState->SetSamplerAllClear(pD3D11DeviceContext);
	}
	// サブテクスチャ
	int slot = 1;
	while (true)
	{
		texture = GetTextureResource(slot);
		if (texture)
			pD3D11DeviceContext->PSSetShaderResources(slot, 1, texture->GetShaderResourceView().GetAddressOf());
		else
			break;
		++slot;
	}

	// ラスタライザとステート描画
	if (GetRasterizerState() == RasterizerState::DoubleDraw)
	{
		// 透明処理用
		// ラスタライザステート(裏描画)
		pD3D11DeviceContext->RSSetState(renderState->GetCullFront());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
		// ラスタライザステート(表描画)
		pD3D11DeviceContext->RSSetState(renderState->GetCullBack());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
	}
	else
	{
		// ラスタライザステート設定
		renderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
	}
}



void SimpleCBDrawComponent::WriteConstants(SimpleConstants & SCB, const MeshPrimData & data)
{
	//--- 姿勢行列
	auto gameObject = GetGameObject();
	auto transform = gameObject->GetTransform();
	Float4x4 world;
	if (data.m_useMeshOffsetTransform)
	{
		// メッシュとゲームオブジェクトで姿勢のずれを使う場合
		world = data.m_meshOffsetTransform * GetMeshOffsetTransform();
		world = world * transform->GetWorldMatrix();
	}
	else
	{
		world = GetMeshOffsetTransform() * transform->GetWorldMatrix();
	}
	world = DirectX::XMMatrixTranspose(world);
	SCB.world = world;

	//--- カメラ行列
	auto camera = gameObject->GetDrawCamera();
	Float4x4 view, projection;
	view = DirectX::XMMatrixTranspose(camera->GetViewMatrix());
	projection = DirectX::XMMatrixTranspose(camera->GetProjectionMatrix());
	SCB.view = view;
	SCB.projection = projection;

	//--- 色
	SCB.emissive = GetEmissive();
	SCB.diffuse = GetDiffuse();
	SCB.specular = GetSpecular();

	//--- ライト
	auto light = gameObject->GetDrawLight();
	SCB.lightDir = light.m_direction;
	SCB.lightDir.w = 1.0f;
	SCB.eyePos = camera->GetPos();
	SCB.eyePos.w = 1.0f;

	SCB.uvScale = Float4(GetUVScale());
}

#pragma endregion



#pragma region BasicCBDrawComponent

BasicCBDrawComponent::BasicCBDrawComponent(const std::shared_ptr<GameObject>& pGameObject)
	: DrawComponentBase(pGameObject)
{
	// デフォルト設定
	SetBlendState(BlendState::Opaque);
	SetDepthStencilState(DepthStencilState::Default);
	SetRasterizerState(RasterizerState::CullBack);
	SetSamplerState(SamplerState::LinearClamp);
}

BasicCBDrawComponent::~BasicCBDrawComponent()
{
}

void BasicCBDrawComponent::Draw()
{
	auto dev = APP->GetDeviceResources();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();
	auto renderState = dev->GetRenderState();
	auto meshData = GetMeshResource()->GetMeshData();

	// シェーダー設定
	pD3D11DeviceContext->VSSetShader(GetVertexShader(), nullptr, 0);
	pD3D11DeviceContext->IASetInputLayout(GetInputLayout());
	pD3D11DeviceContext->PSSetShader(GetPixelShader(), nullptr, 0);

	//--- 定数バッファ作成
	BasicConstants BCB;
	WriteConstants(BCB, meshData);
	// テクスチャを使うかどうか
	auto texture = GetTextureResource(0);
	if (texture)
	{
		// テクスチャがある
		BCB.activeFlg.y = 1;
	}
	else
	{
		// コンポーネントにテクスチャが設定されていない
		if (texture = meshData.m_textureResource.lock())
		{
			// メッシュデータにテクスチャがある
			BCB.activeFlg.y = 1;
		}
		else
		{
			BCB.activeFlg.y = 0;
		}
	}
	// 定数バッファを更新
	ID3D11Buffer* pConstantBuffer = CBBasic::GetInstance()->GetBuffer();
	pD3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &BCB, 0, 0);
	// 定数バッファをシェーダに渡す
	pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	// ストライドとオフセット
	UINT stride = meshData.m_strideNum;
	UINT offset = 0;
	//--- 描画方法を設定
	pD3D11DeviceContext->IASetPrimitiveTopology(meshData.m_primitiveTopology);
	// 頂点バッファをセット
	pD3D11DeviceContext->IASetVertexBuffers(0, 1, meshData.m_vertexBuff.GetAddressOf(), &stride, &offset);
	// インデックスバッファをセット
	pD3D11DeviceContext->IASetIndexBuffer(meshData.m_indexBuff.Get(), DXGI_FORMAT_R16_UINT, 0);

	//--- 各レンダリングステートの設定
	// ブレンドステート
	renderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
	// デプスステンシルステート
	renderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
	// テクスチャとサンプラー
	if (texture)
	{
		pD3D11DeviceContext->PSSetShaderResources(0, 1, texture->GetShaderResourceView().GetAddressOf());
		// サンプラー
		renderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
	}
	else
	{
		// シェーダーリソースクリア
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		// サンプラークリア
		renderState->SetSamplerAllClear(pD3D11DeviceContext);
	}
	// サブテクスチャ
	int slot = 1;
	while (true)
	{
		texture = GetTextureResource(slot);
		if (texture)
			pD3D11DeviceContext->PSSetShaderResources(slot, 1, texture->GetShaderResourceView().GetAddressOf());
		else
			break;
		++slot;
	}
	// 影とサンプラー
	if (IsOwnShadowActive())
	{
		// TODO : 
	}
	// ラスタライザとステート描画
	if (GetRasterizerState() == RasterizerState::DoubleDraw)
	{
		// 透明処理用
		// ラスタライザステート(裏面描画)
		pD3D11DeviceContext->RSSetState(renderState->GetCullFront());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
		// ラスタライザステート(表面描画)
		pD3D11DeviceContext->RSSetState(renderState->GetCullBack());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
	}
	else
	{
		// ラスタライザステート設定
		renderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
		// 描画
		pD3D11DeviceContext->DrawIndexed(meshData.m_indexesNum, 0, 0);
	}
}

void BasicCBDrawComponent::WriteConstants(BasicConstants & BCB, const MeshPrimData & data)
{
	auto gameObject = GetGameObject();
	auto transform = gameObject->GetTransform();
	auto camera = gameObject->GetDrawCamera();
	auto light = gameObject->GetDrawLight();

	// フラグをすべて0にしておく
	BCB.activeFlg.x = BCB.activeFlg.y = BCB.activeFlg.z = BCB.activeFlg.w = 0;

	// ワールド行列
	Float4x4 world;
	if (data.m_useMeshOffsetTransform)
	{
		// メッシュとゲームオブジェクトで姿勢のずれを使う場合
		world = data.m_meshOffsetTransform * GetMeshOffsetTransform();
		world = world * transform->GetWorldMatrix();
	}
	else
	{
		world = GetMeshOffsetTransform() * transform->GetWorldMatrix();
	}

	// ビュー行列
	Float4x4 view = camera->GetViewMatrix();
	// 射影行列
	Float4x4 projection = camera->GetProjectionMatrix();
	Float4x4 worldView = world * view;
	BCB.worldViewProj = DirectX::XMMatrixTranspose(DirectX::XMMatrixMultiply(worldView, projection));

	if (IsLightingEnable())
	{
		BCB.activeFlg.x = 1;

		BCB.lightDirection = light.m_direction;
		BCB.lightDiffuse = light.m_diffuse;
		BCB.lightSpecular = light.m_specular;

		BCB.world = DirectX::XMMatrixTranspose(world);

		DirectX::XMMATRIX worldInverse = DirectX::XMMatrixTranspose(world);
		BCB.worldInversTranspose[0] = worldInverse.r[0];
		BCB.worldInversTranspose[1] = worldInverse.r[1];
		BCB.worldInversTranspose[2] = worldInverse.r[2];

		DirectX::XMMATRIX viewInverse = DirectX::XMMatrixInverse(nullptr, view);
		BCB.eyePosition = viewInverse.r[3];
	}

	DirectX::XMVECTOR diffuse = GetDiffuse();
	DirectX::XMVECTOR alphaV = DirectX::XMVectorReplicate(GetAlpha());
	DirectX::XMVECTOR emissive = GetEmissive();
	DirectX::XMVECTOR ambientLightColor = GetAmbientLightColor();
	if (IsLightingEnable())
	{
		BCB.emissive = (emissive + ambientLightColor * diffuse) * alphaV;
		BCB.specular = GetSpecular();
	}
	else
	{
		BCB.activeFlg.x = 0;
		diffuse = diffuse + emissive;
		BCB.specular = GetSpecular();
	}
	BCB.diffuse = DirectX::XMVectorSelect(alphaV, diffuse * alphaV, DirectX::g_XMSelect1110);

	if (IsOwnShadowActive())
	{
		Float3 calcLightDir(light.m_direction * -1.0f);
		Float3 lightAt(camera->GetLook());
		Float3 lightEye(calcLightDir);
		lightEye *= 20.0f;
		lightEye = lightAt + lightEye;
		Float4 lightEye4(lightEye);
		lightEye4.w = 1.0f;
		BCB.lightPos = lightEye4;
		Float4 eyePos4(camera->GetPos());
		eyePos4.w = 1.0f;
		BCB.eyePosition = eyePos4;
		Float4x4 lightView, lightProj;
		lightView = DirectX::XMMatrixLookAtLH(lightEye, lightAt, Float3(0.0f, 1.0f, 0.0f));
		lightProj = DirectX::XMMatrixOrthographicLH(32.0f, 32.0f, 1.0f, 200.0f);
		BCB.lightView = DirectX::XMMatrixTranspose(lightView);
		BCB.lightProjection = DirectX::XMMatrixTranspose(lightProj);
	}

	BCB.uvScale = Float4(GetUVScale());
}


#pragma endregion
